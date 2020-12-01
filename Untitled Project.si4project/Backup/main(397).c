#include <stdio.h>
#include <unistd.h>

#include "bmp.h"

int main()
{
	int ret = -1;
	
	printf("这是我的第一个工程《图片播放器》\n");

	ret = fb_open();
	if(ret < 0)
	{
		printf("fb_open error.\n");
		return -1;
	}
	debug("ret = %d\n", ret);

	fb_draw_back(WIDTH, HEIGHT, BLUE);
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

//	picture.pathname = "123.bmp";
//	ret = bmp_analyze(100,0, &picture);
	
	ret = display_bmp(100, 0, "123.bmp");
	sleep(3);
	
	ret = display_jpg(0, 0, "rainbow.jpg");
	debug("ret = %d\n", ret);
	sleep(3);

	ret = display_png(0, 0, "huangshengyi.png");
	
	
	fb_close();
	
	
	return 0;
}


