//本文件用来解析bmp格式的图片，并显示到framebuffer中
#include <stdio.h>
#include <stdlib.h>		//malloc
#include <unistd.h>		//read()
#include <sys/types.h>
#include <sys/stat.h>	//open()
#include <fcntl.h>

#include "bmp.h"


//path : 要解析的bmp图片的pathname
int bmp_analyze(const   char *path)
{
	bmp_head Head;
	bmp_info info;
	ssize_t ret = 0;
	unsigned long len;
	int fd = -1;
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		fprintf(stderr, "open %s error. fd = %d\n", path, fd);
		return -1;
	}
	
	
	ret = read(fd, &Head, sizeof(Head));
	//判断是不是BMP格式的图片
	if (Head.type[0] != 'B' || Head.type[1] != 'M') {
		fprintf(stderr, "the file %s is not a BMP picture.\n", path);		
		ret = -1;
		goto close;
	}
	else {
		printf("file %s is a BMP picture\n", path);
		printf("The size of %s = %d bytes\n", path, (unsigned int)Head.file_size);
		ret = 0;
	}

	ret = read(fd, &info, sizeof(info));
	printf("picture resolution: %d x %d\n\n", info.width, info.height);
	printf("picture BPP: %d\n\n", info.count);				
	lseek(fd, Head.offset, SEEK_SET);		//将文件指针移动到第55个字节
	len = info.width * info.height * info.count / 3;
	BMP_BUF = (unsigned char *)malloc(BMP_BUF_SIZE);
	read(fd, BMP_BUF, len);

	fb_draw_BMP_picture(100,0, BMP_BUF);
	debug("test for  DEBUG ret = %d\n", ret);
close:
	close(fd);
	return ret;
}






