#include <stdio.h>
#include <unistd.h>

#include "bmp.h"

int main()
{
	int ret = -1;
	printf("这是我的第一个工程《图片播放器》\n");

	debug("test for  DEBUG ret = %d\n", ret);

	ret = fb_open();
	if(ret < 0)
	{
		printf("fb_open error.\n");
		return -1;
	}

	fb_draw_back(WIDTH, HEIGHT, RED);
/*	fb_draw_line(RED);
	sleep(1);
	fb_draw_picture_equal();
	sleep(1);
	fb_draw_picture_small();
	fb_draw_picture(510, 300);
	sleep(1);
	fb_draw_back(WIDTH, HEIGHT, BLUE);
	fb_draw_picture_1((1024-510)/2, (600-300)/2);
	sleep(1);
	fb_draw_back(WIDTH, HEIGHT, GREEN);
*/
	ret = bmp_analyze("123.bmp");

	debug("test for  DEBUG ret = %d\n", ret);


	fb_close();

	debug("test for  DEBUG ret = %d\n", ret);
	return 0;
}


