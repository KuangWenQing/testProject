#ifndef __BMP_H__
#define __BMP_H__
#include "fb.h"


#pragma pack (2)  /*指定按 2 字节对齐*/
//这样bmp_head的大小才是 14 Bytes，
//不然就是20字节（内存对齐），但read函数不会考虑内存对齐
typedef struct bmp_file_head		//BMP文件头
{
	char type[2];		//位图文件的类型，必须为字母B、M
	unsigned int file_size;	//位图文件的大小，以字节为单位
	short rd1;			// 位图文件保留字，必须为0
	short rd2;			// 位图文件保留字，必须为0
	unsigned int offset;// 位图数据的起始位置在第55个字节，所以offset=54
}bmp_head;


typedef struct bmp_file_info   //图像信息区
{
unsigned int bsize;			//本结构体所占用字节数,即40个字节(当然也要2字节对齐)
int width;					// 位图的宽度，以像素为单位，像素数量是4字节对齐的
int height;					// 位图的高度，以像素为单位
unsigned short planes;		// 目标设备的级别，必须为1
unsigned short count;		// 每个像素所需的位数，必须是1(双色） 4(16色)，8(256色)或24(真彩色)之一
unsigned int compression;	// 位图压缩类型，必须是 0(不压缩), 1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一
unsigned int sizeimage;		// 位图的大小，以字节为单位
unsigned int xmeter;		// 位图水平分辨率，每米像素数
unsigned int ymeter;		// 位图垂直分辨率，每米像素数
unsigned int cused;			// 位图实际使用的颜色表中的颜色数
unsigned int cimportant;	// 位图显示过程中重要的颜色数

}bmp_info;

#pragma pack ()  /*恢复默认字节对齐*/


#define BMP_MAX_RESOLUTION		(1080*1920)
#define BMP_BUF_SIZE			(BMP_MAX_RESOLUTION*3)


int bmp_analyze(int x0, int y0, struct picture_info *Picture);




#endif
