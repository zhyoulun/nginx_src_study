//
//  ngx_string.h
//  nginx_src_study
//
//  Created by zhangyoulun on 2018/3/23.
//  Copyright © 2018年 zhangyoulun. All rights reserved.
//

#ifndef ngx_string_h
#define ngx_string_h

#include "ngx_core.h"

//在内存中设置字符0
#define ngx_memzero(buf, n)       (void) memset(buf, 0, n)
//在内存中设置字符c
#define ngx_memset(buf, c, n)     (void) memset(buf, c, n)

#endif /* ngx_string_h */
