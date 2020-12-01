#include <stdio.h>
#include <fb.h>

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

	fb_draw_back(WIDTH, HEIGHT, GREEN);
	fb_draw_line(RED);
	fb_draw_picture();
	fb_close();
	return 0;
}


