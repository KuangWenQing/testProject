//本文件用来解析bmp格式的图片，并显示到framebuffer中
#include <stdio.h>
#include <stdlib.h>		//malloc
#include <unistd.h>		//read()
#include <sys/types.h>
#include <sys/stat.h>	//open()
#include <fcntl.h>



#define BMP_MAX_RESOLUTION		(1080*1920)
#define BMP_BUF_SIZE			(BMP_MAX_RESOLUTION*3)

unsigned char *bmp_buf = NULL;


//path : 要解析的bmp图片的pathname
int bmp_analyze(unsigned char *path)
{
	unsigned char buf[54] = {0};
	ssize_t ret = 0;
	int fd = -1;
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		fprintf(stderr, "open %s error. fd = %d\n", path, fd);
		return -1;
	}
	
//	bmp_buf = (unsigned char *)malloc(BMP_BUF_SIZE)
	

	//判断是不是BMP格式的图片
	ret = read(fd, buf, 54);
	if (ret != 54){
		fprintf(stderr, "read file header error.\n");
		return -1;
	}

	if (buf[0] != 'B' || buf[1] != 'M') {
		fprintf(stderr, "the file %s is not a BMP picture.\n", path);
		return -1;
	}
	printf("file %s is a BMP picture\n", path);

	close(fd);
	return 0;
}






