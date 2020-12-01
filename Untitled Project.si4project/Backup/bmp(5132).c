//本文件用来解析bmp格式的图片，并显示到framebuffer中
#include <stdio.h>
#include <stdlib.h>		//malloc
#include <unistd.h>		//read()
#include <sys/types.h>
#include <sys/stat.h>	//open()
#include <fcntl.h>

#include "bmp.h"


//path : 要解析的bmp图片的pathname
int bmp_analyze(int x0, int y0, struct picture_info *pPicture)
{
	bmp_head Head;
	bmp_info info;
	ssize_t ret = 0;
	unsigned long len;
	int fd = -1;
	fd = open(pPicture->pathname, O_RDONLY);
	if (fd < 0)
	{
		fprintf(stderr, "open %s error. fd = %d\n", pPicture->pathname, fd);
		return -1;
	}
	
	
	ret = read(fd, &Head, sizeof(Head));
	//判断是不是BMP格式的图片
	if (Head.type[0] != 'B' || Head.type[1] != 'M') {
		fprintf(stderr, "the file %s is not a BMP picture.\n", pPicture->pathname);		
		ret = -1;
		goto close;
	}
	else {
		debug("The file %s is a BMP picture and size = %d bytes\n", pPicture->pathname, Head.file_size);
		ret = 0;
	}

	ret = read(fd, &info, sizeof(info));
	debug("picture resolution: %d x %d\n\n", info.width, info.height);
	debug("picture BPP: %d\n\n", info.count);		

	pPicture->width = info.width;
	pPicture->height = info.height;
	pPicture->bpp	=  info.count;
	
	lseek(fd, Head.offset, SEEK_SET);		//将文件指针移动到第55个字节
	len = info.width * info.height * info.count / 3;
	if (len > BMP_BUF_SIZE)	{
		fprintf(stderr, "图片分辨率大于 1920*1080，无法显示\n");
		ret = -1;
		goto close;
	}
	pPicture->pData = (unsigned char *)malloc(len);
	read(fd, pPicture->pData, len);

	fb_draw_BMP_picture(x0,y0, pPicture);
	
close:
	close(fd);
	return ret;
}






