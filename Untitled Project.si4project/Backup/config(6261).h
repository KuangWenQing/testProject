#ifndef __CONFIG_H__
#define __CONFIG_H__

//最大 分辨率
#define MAX_RESOLUTION		(1080*1920)
#define MAX_BUF_SIZE		(MAX_RESOLUTION*3)


//#define DEBUG			// 打开调试信息输出的开关

// debug宏的定义
#ifdef DEBUG
#define debug(...)                                                      \
        {                                                               \
            fprintf(stderr, "[debug][%s : %s : %d] ",                     \
                    __FILE__, __FUNCTION__, __LINE__);                  \
            fprintf(stderr, __VA_ARGS__);                               \
        }
#else
#define debug(...)  
#endif



typedef struct picture_info
{
	char *pathname;			//图片文件路径	
	int width;				//宽，水平方向的像素
	int height;				//高，垂直方向的像素
	int bpp;				//每个像素所需的位数
	unsigned char *pData;	//指向图片数据存储区
}picture_info;


int display_bmp(const int x0, const int y0, const char *pathname);
int display_jpg(const int x0, const int y0, const char *pathname);

#endif