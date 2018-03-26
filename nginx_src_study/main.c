//
//  main.c
//  nginx_src_study
//
//  Created by zhangyoulun on 2018/3/22.
//  Copyright © 2018年 zhangyoulun. All rights reserved.
//

#include <stdio.h>
#include "ngx_palloc.h"

int main(int argc, const char * argv[]) {
    ngx_pool_t *pool = ngx_create_pool(1000);
    ngx_print_pool(pool);
    
    char * buf = (char *)ngx_palloc_small(pool, 10);
    printf("%p\n", buf);
    ngx_print_pool(pool);
    
    printf("haha\n");
    return 0;
}
