//
//  ngx_alloc.c
//  nginx_src_study
//
//  Created by zhangyoulun on 2018/3/22.
//  Copyright © 2018年 zhangyoulun. All rights reserved.
//

#include "ngx_alloc.h"

//申请一块内存，大小为size，按alignment对齐
void * ngx_memalign(size_t alignment, size_t size)
{
    void *p;
    int err;
    
    err = posix_memalign(&p,alignment,size);
    if(err){
        p = NULL;
    }
    return p;
}
