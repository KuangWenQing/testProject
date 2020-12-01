/*
*	本文件用来解码jpg图片，并调用framebuffer.c中的接口显示到LCD上
*/

#include "fb.h"
#include <string.h>		//memcpy
#include <stdlib.h>		//malloc


#include "jpeglib.h"
#include "jerror.h"

#include <setjmp.h>

/*
*函数功能：	判断是不是jpg图片
*函数参数： pathname是图片文件的 路径文件名
*返回值：	  如果是jpg就返回0，不是返回1，错误返回-1
*/
int is_jpg(const char *pathname)
{
	FILE *file = NULL;
	char buf[2] = {0};

	//打开文件
	if ((file = fopen(pathname, "rb")) == NULL) {
		fprintf(stderr, " open %s error\n", pathname);
		return -1;
	}
	
	// 读出开始 2个字节
	fread(buf, 1, 2, file);
	if((buf[0] == 0xff) && (buf[1] == 0xd8)) {
		fseek(file, -2, SEEK_END);	// 文件指针移动到倒数第二
		fread(buf, 1, 2, file);		// 读出末尾 2个字节
		if((buf[0] == 0xff) && (buf[1] == 0xd9)) {
			fclose(file);
			return 0;	//是jpg图片
		}
	}
	fclose(file);
	return 1;		//不是jpg图片
}


struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

//  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;



METHODDEF(void)my_error_exit (j_common_ptr cinfo)
{
	//my_error_ptr myerr = (my_error_ptr) cinfo->err;
//	(*cinfo->err->output_message) (cinfo);
	fprintf(stderr, "my_error_exit\n");
//	longjmp(myerr->setjmp_buffer, 1);
}



/*
*函数功能：解码jpg图片，
*成功解码则返回0
*/

static int jpg_analyze(struct picture_info *pPicture)
{
	struct jpeg_decompress_struct cinfo;	//读出的 图片文件流 将与之关联
	struct my_error_mgr jerr;				//错误处理 结构体

	FILE * infile;				//图片文件流
//	JSAMPARRAY buffer = NULL;		//每一行像素数据的数组
	unsigned char *buffer = NULL;	//每一行像素数据的数组
	int row_stride;				//解码出来的一行数据的个数
	int len;					//整个图片的数据 个数
	int ret = 0;
	
	if ((infile = fopen(pPicture->pathname, "rb")) == NULL) {
		fprintf(stderr, "can't open %s\n", pPicture->pathname);
		return -1;
	}

	//第 1 步：错误处理
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	jpeg_create_decompress(&cinfo);		//给解码器做必要的内存分配和数据结构的初始化


	//第 2 步：fopen打开的源jpg图片和解码器相关联
	jpeg_stdio_src(&cinfo, infile);
	

	//第 3 步， 读取jpg文件头
	jpeg_read_header(&cinfo, TRUE);

	
	//第 5 步，启动解码器
	jpeg_start_decompress(&cinfo);
	debug("image resolution: %d * %d, bpp / 8 = %d\n", cinfo.output_width, 
						cinfo.output_height, cinfo.output_components);
	pPicture->width  = cinfo.output_width;
	pPicture->height = cinfo.output_height;
	pPicture->bpp	 = cinfo.output_components * 8;	
	
	// 解码一行数据
	row_stride = cinfo.output_width * cinfo.output_components;
	//buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
	buffer = (unsigned char *)malloc(row_stride);
	if(NULL == buffer) {
		fprintf(stderr, "malloc buffer error.\n");
		ret = -1;
		goto close;
	}

	len = cinfo.output_width * cinfo.output_height * cinfo.output_components;
	if (len > MAX_BUF_SIZE)	{
		fprintf(stderr, "resolution morethan 1920*1080\n");
		ret = -1;
		goto close;
	}
	pPicture->pData = (unsigned char *)malloc(len);

	//第 6 步 ，逐行解码，并将解出来的数据放到pData
	while (cinfo.output_scanline < cinfo.output_height) {
  		jpeg_read_scanlines(&cinfo, &buffer, 1);
		memcpy(pPicture->pData + cinfo.output_scanline * row_stride, buffer, row_stride);
	}

	// 第 7 步， 清理工作
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
		
close:
	fclose(infile);
	return ret;

}




/*
*功能 ： 任意位置显示jpg图片
*参数 ： 起始坐标 x, y； 路径文件名pathname
*返回值：正确返回0，出错返回-1
*/
int display_jpg(const int x0, const int y0, const char *pathname)
{
	int ret = -1;
	struct picture_info picture;
	
	ret = is_jpg(pathname);
	if(ret) 
		return -1;
	
	picture.pathname = pathname;
	ret = jpg_analyze(&picture);
		
	//显示图片
	fb_draw_JPEG_picture(x0, y0, &picture);

	return ret;
}

