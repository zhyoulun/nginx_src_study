//
//  ngx_palloc.c
//  nginx_src_study
//
//  Created by zhangyoulun on 2018/3/22.
//  Copyright © 2018年 zhangyoulun. All rights reserved.
//

#include "ngx_palloc.h"


ngx_pool_t *ngx_create_pool(size_t size)
{
    ngx_pool_t *p;
    
    p = (ngx_pool_t *)ngx_memalign(NGX_POOL_ALIGNMENT,size);
    if (p==NULL){
        return NULL;
    }
    
    p->d.last = (u_char *)p+sizeof(ngx_pool_t);//ngx_pool_t结构本身需要占用一部分空间
    p->d.end = (u_char *)p+size;//最远可分配的位置
    p->d.next = NULL;//指向下一个ngx_pool_t
    
    //减掉ngx_pool_t结构本身的大小，赋给max，代表可以使用内存的最大值
    size = size-sizeof(ngx_pool_t);
    p->max = (size<NGX_MAX_ALLOC_FROM_POOL)?size:NGX_MAX_ALLOC_FROM_POOL;
    
    p->current = p;
    p->large = NULL;
    
    return p;
}


void ngx_print_pool(ngx_pool_t *pool)
{
    if(pool==NULL){
        printf("pool is empty\n");
        return;
    }
    
    printf("print pool info: \n");
    printf("max: %zu\n", pool->max);
    printf("pool   addr: %p\n", pool);
    printf("d.last addr: %p\n", pool->d.last);
    printf("d.end  addr: %p\n", pool->d.end);
}


void * ngx_palloc(ngx_pool_t *pool, size_t size)
{
    if(size<=pool->max){
        return ngx_palloc_small(pool,size);
    }
    
    return ngx_palloc_large(pool,size);
}

void * ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
    if(size<=pool->max){
        return ngx_palloc_small(pool,size);
    }
    
    return ngx_palloc_large(pool,size);
}

void *ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    void *p;
    ngx_pool_large_t *large;
    
    //申请一个size大小的内存
    p = ngx_alloc(size);
    if(p==NULL){
        return NULL;
    }
    
    //看看大内存池链表有没有空闲的位置
    for(large=pool->large;large;large=large->next){
        if(large->alloc==NULL){
            large->alloc = p;
            return p;
        }
    }
    
    //在小内存池上为ngx_pool_large_t申请一个块空间
    large = ngx_palloc_small(pool,sizeof(ngx_pool_large_t));
    if(large==NULL){
        ngx_free(p);
        return NULL;
    }
    
    large->alloc = p;
    //插入到大内存池的链表中
    large->next = pool->large;
    pool->large = large;
    
    return p;
}

//检查小块内存链表是否有足够存放size大小的位置，如果有，则开辟；否则添加一个新的结点到链表
void * ngx_palloc_small(ngx_pool_t *pool, size_t size)
{
    u_char *m;
    ngx_pool_t *p;
    
    p = pool->current;
    
    do{
        m = p->d.last;
        if((size_t)(p->d.end-m)>=size){
            p->d.last = m+size;
            return m;
        }
        p = p->d.next;
    }while(p);
    
    return ngx_palloc_block(pool,size);
}

//申请一个新的ngx_pool_t,用于存放size大小的数据，将这部分数据存放在小块数据
//返回申请的内存的空间首地址
void * ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char *m;
    size_t psize;
    ngx_pool_t *p,*new;
    
    //当前pool的大小，新的pool预计也要申请这么大
    psize = (size_t)(pool->d.end - (u_char *)pool);
    
    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize);
    if(m==NULL){
        return NULL;
    }
    
    new = (ngx_pool_t *)m;
    new->d.end = m+psize;
    new->d.next = NULL;
    
    //size的空间，放在ngx_pool_data_t中
    m += sizeof(ngx_pool_data_t);
    new->d.last = m+size;
    
    for(p=pool->current;p->d.next;p=p->d.next){}
    
    p->d.next = new;
    
    return m;
}


void *ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    void *p;
    p = ngx_palloc(pool,size);
    if (p){
        ngx_memzero(pool, size);
    }
    return p;
}
