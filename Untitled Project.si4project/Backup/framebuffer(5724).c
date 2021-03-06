/*****************************************************
*	framebuffer的基础代码，包括fb的打开、ioctl获取信息
*	测试fb显示的基本代码
********************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <fb.h>
#include "1024600.h"


// 全局变量
unsigned int *pfb = NULL;	//指向framebuffer的首地址
int fd = -1;				//打开 FBDEVICE 得到的文件描述符

int fb_open(void)
{
	int ret = -1;
	
	
	struct fb_fix_screeninfo finfo = {0};
	struct fb_var_screeninfo vinfo = {0};
	
	// 第1步：打开设备
	fd = open(FBDEVICE, O_RDWR);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}
	printf("open %s success.\n", FBDEVICE);
	
	// 第2步：获取设备的硬件信息
	ret = ioctl(fd, FBIOGET_FSCREENINFO, &finfo);
	if (ret < 0)
	{
		perror("ioctl");
		return -1;
	}
	printf("smem_start = 0x%lx, smem_len = %u.\n", finfo.smem_start, finfo.smem_len);
	
	ret = ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);
	if (ret < 0)
	{
		perror("ioctl");
		return -1;
	}
	printf("xres = %u, yres = %u.\n", vinfo.xres, vinfo.yres);
	printf("xres_virtual = %u, yres_virtual = %u.\n", vinfo.xres_virtual, vinfo.yres_virtual);
	printf("bpp = %u.\n", vinfo.bits_per_pixel);
	
	
	// 第3步：进行mmap
	unsigned long len = vinfo.xres_virtual * vinfo.yres_virtual * vinfo.bits_per_pixel / 8;
	printf("len = %ld\n", len);
	pfb = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (NULL == pfb)
	{
		perror("mmap");
		return -1;
	}
	printf("pfb = %p.\n", pfb);
	
	return 0;
}

void fb_close(void)
{
	close(fd);
}


void fb_draw_back(unsigned int width, unsigned int height, unsigned int color)
{
	unsigned int x, y;
	
	for (y=0; y<height; y++)
	{
		for (x=0; x<width; x++)
		{
			*(pfb + y * WIDTH + x) = color;
		}
	}
}

void fb_draw_line(unsigned int color)
{
	unsigned int x;
	
	for (x=50; x<600; x++)
	{
		*(pfb + 200 * WIDTH + x) = color;
	}
}




void fb_draw_picture(void)
{
	const unsigned char *pData = gImage_1024600;
	unsigned int y, x, cnt;
	for (y = 0; y < HEIGHT; y++)
	{
		for (x = 0; x < WIDTH; x++){
			cnt = WIDTH * y + x;		//当前像素点在屏幕的位置			
			*(pfb + cnt) = ((pData[3*cnt+0]<<16) | (pData[3*cnt+1]<<8) | (pData[3*cnt+2]<<0));
		}

	}
}


















