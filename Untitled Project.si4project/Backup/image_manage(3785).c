#include "image_manage.h"



image_info_t images[100];		//最多支持100张图片
unsigned int image_cnt = 0;			//记录有多少张图片


//在本函数中递归检索path文件夹，将其中的图片检索到image
int readFileList(char *basePath)
{
	DIR *dir;
	struct dirent *ptr;
	char base[PATHNAME_LEN];	//当前（basePath）路径 + 图片名
	
	if ((dir=opendir(basePath)) == NULL) {
		perror("Open dir error...");
		exit(1);
	}

	//每次调用readdir()都会返回basePath目录下的一个文件
	while ((ptr=readdir(dir)) != NULL)
	{
		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)//current dir OR parrent dir
			continue;
		debug("d_name = %s \n", ptr->d_name);
		debug("d_type = %d, DT_REG=%d, DT_DIR=%d, DT_UNKNOWN=%d\n",
			ptr->d_type, DT_REG, DT_DIR, DT_UNKNOWN);
		
		switch (ptr->d_type)
		{
			case DT_REG:		//file
				printf("d_name:%s/%s\n",basePath,ptr->d_name);
				break;

			case DT_DIR:		//dir
				memset(base,'\0',sizeof(base));
				strcpy(base,basePath);
				strcat(base,"/");
				strcat(base,ptr->d_name);
				readFileList(base);
				break;

			case DT_UNKNOWN:
				printf("未知文件\n");
				break;

			default:
				break;

		}
	}

	closedir(dir);

	return 1;
}


//本函数和上面的 readFileList() 目的一样，且支持nfs挂载文件系统
int scan_image(const char *basePath)
{
	DIR *dir;
	struct dirent *ptr;
	char base[PATHNAME_LEN];	//当前（basePath）路径 + 图片名
	struct stat sta;

	if ((dir=opendir(basePath)) == NULL) {
		perror("Open dir error...");
		exit(1);
	}

	//每次调用readdir()都会返回basePath目录下的一个文件
	while ((ptr=readdir(dir)) != NULL)
	{
		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)//current dir OR parrent dir
			continue;
		
			memset(base,'\0',sizeof(base));
			strcpy(base,basePath);
			strcat(base,"/");
			strcat(base,ptr->d_name);
		
			debug("pathname [ %s ]\n", base);
			
			lstat(base, &sta);
		
		if (S_ISREG(sta.st_mode)) {
			if (0 == is_bmp(base)) {
				strcpy(images[image_cnt].pathname, base);
				images[image_cnt].type = IMAGE_TYPE_BMP;
			}
			if (0 == is_jpg(base)) {
				strcpy(images[image_cnt].pathname, base);
				images[image_cnt].type = IMAGE_TYPE_JPG;
			}
			if (0 == is_png(base)) {
				strcpy(images[image_cnt].pathname, base);
				images[image_cnt].type = IMAGE_TYPE_PNG;
			}
			
			image_cnt++;		
		}
		if (S_ISDIR(sta.st_mode)) {
			printf("directory, [ %s ]\n", base);
			scan_image(base);
		}	
	}

	closedir(dir);
	return 0;
}

void print_images(void)
{
	int i;
	printf("image_cnt = %d\n", image_cnt);
	for(i = 0; i<image_cnt; i++)
		printf("images[i].pathname = %s  :  type = %d\n", images[i].pathname, images[i].type);

}

void show_images(void)					//幻灯片
{
	int i;
	printf("image_cnt = %d\n", image_cnt);
	for(i = 0; i<image_cnt; i++) {
		switch (images[i].type) {
			case IMAGE_TYPE_BMP:
				display_bmp(0, 0, images[i].pathname);	break;
			case IMAGE_TYPE_JPG:
				display_jpg(0, 0, images[i].pathname);	break;
			case IMAGE_TYPE_PNG:
				display_png(100, 0, images[i].pathname);	break;
		}	
		sleep(2);
	}
}



static void show_image(int i)
{
	switch (images[i].type) {
		case IMAGE_TYPE_BMP:
			display_bmp(0, 0, images[i].pathname);	break;
		case IMAGE_TYPE_JPG:
			display_jpg(0, 0, images[i].pathname);	break;
		case IMAGE_TYPE_PNG:
			display_png(100, 0, images[i].pathname);	break;	
	}
}

int ts_updown()
{	
	int fd = -1, ret = -1;
	int i = 0;				//用来记录显示的是第几幅图片
	struct input_event ev;
	
	// 第1步：打开设备文件
	fd = open(DEVICE_TOUCHSCREEN, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return -1;
	}
	
	while(1)
	{
		// 第2步：读取一个event事件包
		memset(&ev, 0, sizeof(struct input_event));
		ret = read(fd, &ev, sizeof(struct input_event));	//阻塞
		if (ret != sizeof(struct input_event))
		{
			perror("read");
			close(fd);
			return -1;
		}
/*		
		// 第3步：解析event包，才知道发生了什么样的输入事件
		printf("-------------------------\n");
		printf("type: %hd\n", ev.type);
		printf("code: %hd\n", ev.code);
		printf("value: %d\n", ev.value);
		printf("\n");
*/
		
		//触摸屏事件       &&    x坐标数据
		if ((ev.type==EV_ABS) && (ev.code==ABS_X))
		{
			if(ev.value < TOUCH_left)		//上翻页
				if (--i<0)
					i = image_cnt - 1;
			
			if(ev.value > TOUCH_right)		//下翻页
				if (++i>=image_cnt)
					i = 0;	
			show_image(i);
		}
	}
	// 第4步：关闭设备
	close(fd);
	return 0;
}

