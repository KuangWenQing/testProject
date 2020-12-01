#ifndef __CONFIG_H__
#define __CONFIG_H__

//#define DEBUG			// 打开调试信息输出的开关

// debug宏的定义
#ifdef DEBUG
#define debug(...)                                                      \
        {                                                               \
            fprintf(stderr, "[debug][%s:%s:%d] ",                     \
                    __FILE__, __FUNCTION__, __LINE__);                  \
            fprintf(stderr, __VA_ARGS__);                               \
        }
#else
#define debug(...)  
#endif


#endif