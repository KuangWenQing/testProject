#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>		//lstat
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include <fcntl.h>
#include <linux/input.h>



#include "config.h"



#define PATHNAME_LEN		256


typedef enum image_type
{
	IMAGE_TYPE_BMP,
	IMAGE_TYPE_JPG,
	IMAGE_TYPE_PNG,
	IMAGE_TYPE_UNKNOWN,
}image_type_e;

//用来封装一张图片的信息
typedef struct image_info
{
	char pathname[PATHNAME_LEN];	//图片名字
	image_type_e type;				//图片格式
}image_info_t;





