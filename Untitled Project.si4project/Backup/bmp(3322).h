#ifndef __BMP_H__
#define __BMP_H__

#pragma pack (4)  /*指定按4字节对齐*/

typedef struct bmp_file_head		//BMP文件头结构
{
//	char type[2];		//位图文件的类型，必须为字母B、M
	unsigned int size;	//位图文件的大小，以字节为单位
	short rd1;			// 位图文件保留字，必须为0
	short rd2;			// 位图文件保留字，必须为0
	unsigned int offset;// 位图数据的起始位置，以相对于位图
}bmp_head;
#pragma pack ()  /*恢复默认字节对齐*/



#define BMP_MAX_RESOLUTION		(1080*1920)
#define BMP_BUF_SIZE			(BMP_MAX_RESOLUTION*3)

//unsigned char *bmp_buf = NULL;

//path : 要解析的bmp图片的pathname
int is_bmp(const  char *path);
int bmp_analyze(const   char *path);



#endif
