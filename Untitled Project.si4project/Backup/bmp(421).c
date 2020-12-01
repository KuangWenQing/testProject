//本文件用来解析bmp格式的图片，并显示到framebuffer中
#include <stdio.h>
#include <stdlib.h>		//malloc
#include <unistd.h>		//read()
#include <sys/types.h>
#include <sys/stat.h>	//open()
#include <fcntl.h>

#include <bmp.h>



//path : 要解析的bmp图片的pathname
int is_bmp(const  char *path)
{
	unsigned char buf[2] = {0};
	ssize_t ret = 0;
	int fd = -1;
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		fprintf(stderr, "open %s error. fd = %d\n", path, fd);
		return -1;
	}
	

	//判断是不是BMP格式的图片
	ret = read(fd, buf, 2);
	if (ret != 2){
		fprintf(stderr, "read file header error.\n");
		ret = -1;
		goto close;
	}

	if (buf[0] != 'B' || buf[1] != 'M') {
		fprintf(stderr, "the file %s is not a BMP picture.\n", path);		
		ret = -1;
		goto close;
	}
	else {
		printf("file %s is a BMP picture\n", path);
		ret = 0;
	}

close:
	close(fd);
	return ret;
}


//path : 要解析的bmp图片的pathname
int bmp_analyze(const   char *path)
{
	bmp_head fHead;
	char tmp[2];
	ssize_t ret = 0;
	int fd = -1;
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		fprintf(stderr, "open %s error. fd = %d\n", path, fd);
		return -1;
	}

	
	ret = read(fd, tmp, sizeof(tmp));
	
	ret = read(fd, &fHead, sizeof(fHead));
	//判断是不是BMP格式的图片
/*	if (fHead.type[0] != 'B' || fHead.type[1] != 'M') {
		fprintf(stderr, "the file %s is not a BMP picture.\n", path);		
		ret = -1;
		goto close;
	}
	else {
		printf("file %s is a BMP picture\n", path);
		ret = 0;
	}*/
	printf("%d\n", sizeof(fHead.size));
	printf("%d\n", fHead.offset);
	printf("The size of %s = %d bytes\n", path, (unsigned int)fHead.size);

close:
	close(fd);
	return ret;
}






