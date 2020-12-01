#ifndef __FB_H__
#define __FB_H__


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

#include "config.h"
//#include "1024600.h"
//#include "510300.h"


// 宏定义
#define FBDEVICE	"/dev/fb0"

// 旧开发板
//#define WIDTH		800	
//#define HEIGHT	480
// 新开发板
#define WIDTH		1024	
#define HEIGHT		600

#define WHITE		0xffffffff			// test ok
#define BLACK		0x00000000
#define RED			0xffff0000
#define GREEN		0xff00ff00			// test ok
#define BLUE		0xff0000ff			


// 函数声明
int fb_open(void);
void fb_close(void);
void fb_draw_back(unsigned int width, unsigned int height, unsigned int color);
void fb_draw_line(unsigned int color);
/*
void fb_draw_picture_equal(void);		//图片和屏幕大小相等
void fb_draw_picture_small(void);		//图片小于屏幕
void fb_draw_picture(unsigned int x0, unsigned int y0);
void fb_draw_picture_1(unsigned int x0, unsigned int y0);
*/

void fb_draw_BMP_picture(int x0, int y0, struct picture_info *pBMP);



#endif
