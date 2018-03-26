//
//  ngx_palloc.h
//  nginx_src_study
//
//  Created by zhangyoulun on 2018/3/22.
//  Copyright © 2018年 zhangyoulun. All rights reserved.
//

#ifndef ngx_palloc_h
#define ngx_palloc_h

#include "ngx_core.h"
#include "ngx_alloc.h"
#include "ngx_string.h"


#define NGX_POOL_ALIGNMENT       16
#define NGX_MAX_ALLOC_FROM_POOL  (4096 - 1)

typedef struct ngx_pool_s ngx_pool_t;
typedef struct ngx_pool_large_s ngx_pool_large_t;



//这部分必须放在ngx_pool_s结构的定义之前
typedef struct{
    u_char *last;       //当前内存分配结束位置，即下一段可分配内存的起始位置
    u_char *end;        //内存池的结束位置
    ngx_pool_t *next;   //链接到下一个内存池
} ngx_pool_data_t;

struct ngx_pool_large_s{
    ngx_pool_large_t *next;//下一个大块内存，链表结构
    void *alloc;//nginx分配的大块内存
};

struct ngx_pool_s{
    ngx_pool_data_t d;//数据块
    size_t max;//数据块的大小，即小块内存的最大值
    ngx_pool_t *current;//保存当前内存值
    ngx_pool_large_t *large;//分配大块内存使用，即超过max的内存请求
};


//创建pool
ngx_pool_t *ngx_create_pool(size_t size);
//删除pool
void ngx_destroy_pool(ngx_pool_t *pool);
//重置pool
void ngx_reset_pool(ngx_pool_t *pool);

void *ngx_palloc(ngx_pool_t *pool, size_t size);
void *ngx_pnalloc(ngx_pool_t *pool, size_t size);
void *ngx_pcalloc(ngx_pool_t *pool, size_t size);
void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);

//打印内存池中的数据
void ngx_print_pool(ngx_pool_t *pool);

//----------------------------------
//以下为内部函数，更合理的方式是放在.c文件中
//----------------------------------
//在pool上申请大块内存
void *ngx_palloc_large(ngx_pool_t *pool, size_t size);
//在pool上申请小块内存
void *ngx_palloc_small(ngx_pool_t *pool, size_t size);
//申请一个新的pool
void *ngx_palloc_block(ngx_pool_t *pool, size_t size);

#endif /* ngx_palloc_h */
