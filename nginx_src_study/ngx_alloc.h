//
//  ngx_alloc.h
//  nginx_src_study
//
//  Created by zhangyoulun on 2018/3/22.
//  Copyright © 2018年 zhangyoulun. All rights reserved.
//

#ifndef ngx_alloc_h
#define ngx_alloc_h

#include <stdio.h>
#include <stdlib.h>

#define ngx_free          free
#define ngx_alloc         malloc

void * ngx_memalign(size_t alignment, size_t size);
void * ngx_alloc(size_t size);

#endif /* ngx_alloc_h */
