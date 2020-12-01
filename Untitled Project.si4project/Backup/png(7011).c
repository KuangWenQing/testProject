/*
* 本文件用来解码png图片，并调用framebuffer.c中的显示接口将图片显示到LCD上
*
*/
#include "fb.h"

#include <pngstruct.h>
#include <png.h>		//libpng.so 中的头文件


#define PNG_BYTES_TO_CHECK		8


static int is_png(const char *pathname)
{
	FILE *fp = NULL;
	char buf[PNG_BYTES_TO_CHECK];
	
	   /* Open the prospective PNG file. */
	   if ((fp = fopen(pathname, "rb")) == NULL)
		  return -1;
	
	   /* Read in some of the signature bytes */
	   if (fread(buf, 1, PNG_BYTES_TO_CHECK, fp) != PNG_BYTES_TO_CHECK)
		  return -1;
	
	   //是 png 图片就返回0，否则返回非0
	   return(png_sig_cmp(buf, (png_size_t)0, PNG_BYTES_TO_CHECK));
}



static int png_analyze(int x0, int y0, struct picture_info *pPicture)
{
	png_structp png_ptr;
	png_infop info_ptr;
	int color_type;

	FILE *fp;
	if ((fp = fopen(pPicture->pathname, "rb")) == NULL)
		return -1;

	// 1、关数据实例化
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,0, 0, 0);	
	if (png_ptr == NULL)
	{
		fclose(fp);
		return -1;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return -1;
	}

	// 2、错误处理函数
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		/* Free all of the memory associated with the png_ptr and info_ptr */
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		/* If we get here, we had a problem reading the file */
		return -1;
	}

	// 3、解码的图片的文件指针和png解码器绑定
	png_init_io(png_ptr, fp);


	// 4、读取png图片信息
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_STRIP_ARPHA, 0);

	// 5、打印相关信息
	color_type = info_ptr->color_type;
	debug("color_type = %d\n", color_type);

	pPicture->width = info_ptr->width;
	pPicture->height = info_ptr->height;
	pPicture->bpp = info_ptr->pixel_depth;
	debug("width = %d, height = %d, bpp = %d\n", pPicture->width, pPicture->height, pPicture->bpp);

	return 0;
}




/*
*功能 ： 任意位置显示 bmp 图片
*参数 ： 起始坐标 x, y； 路径文件名pathname
*返回值：正确返回0，出错返回-1
*/
int display_png(const int x0, const int y0, const char *pathname)
{
	int ret = -1;
	struct picture_info picture;
	
	ret = is_png(pathname);
	
	if(ret ==0 ){
		debug("This file %s is png.\n", pathname);
	}
	else {
		debug("%s 不是 png.\n", pathname);
		return -1;
	}
	
	return 0;
	
	picture.pathname = pathname;
	ret = bmp_analyze(x0, y0, &picture);

	return ret;

}


