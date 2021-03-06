#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
/*因为4字节对齐的代码一加上去，输出的图片会有问题，所以只能完美处理宽度是4的倍数的图片*/ 
typedef struct BITMAPFILEHEADER{
	WORD bfType;/* 2byte 位图类别，根据不同的操作系统而不同，在Windows中，此字段的值总为‘BM’*/
	DWORD bfSize;/* 4byte BMP的大小 */
	WORD bfReserved1;/* 2byte 总为0*/
	WORD bfReserved2;/*2byte 总为0*/
	DWORD bfOffBits; /*图像数据的偏移地址，既从文件bfOffBits之后的数据就是主要的图像数据了*/
	
};/*定义头文件结构*/ 

typedef struct BITMAPINFOHEADER{
	DWORD biSize;/*4byte 本结构的大小*/ 
	LONG biWidth; /* 4byte BMP图像的宽度*/ 
	LONG biHeight;/* 4byte BMP图像的高度*/ 
	WORD biPlanes;/* 2byte 总为0*/ 
	WORD biBitCount; /* 2byte BMP图像的色深，即一个像素用多少位表示，常见有1、4、8、16、24和32，分别对应单色、16色、 256色、16位高彩色、24位真彩色和32位增强型真彩色*/
	DWORD biCompression;/* 4byte 压缩方式，0表示不压缩，1表示RLE8压缩，2表示RLE4压缩，3表示每个像素值由指定的掩码决定*/
	DWORD biSizeImage;/* 4byte BMP图像数据大小，必须是4的倍数，图像数据大小不是4的倍数时用0填充补足*/
	LONG biXPelsPerMeter;/* 4byte 水平分辨率*/
	LONG biYPelsPerMeter;/* 4byte 垂直分辨率*/
	DWORD biClrUsed;/* 4byte BMP图像使用的颜色，0表示使用全部颜色*/
	DWORD biClrImportant;/* 4byte 重要的颜色数，此值为0时所有颜色都重要，对于使用调色板的BMP图像来说，当显卡不能够显示所有颜色时，此值将辅助驱动程序显示颜色*/
};	/*定义信息头结构 */

typedef struct RGB	{
	BYTE Blue;
    BYTE Green;
	BYTE Red;
}RGB;/*定义颜色结构 */

typedef struct bmp{
	BITMAPFILEHEADER file;
	BITMAPINFOHEADER info;
}bmp;/*定义BMP文件结构 */

BITMAPFILEHEADER ReadHead();
BITMAPINFOHEADER ReadInfo();



int main(){
   printf("因为4字节对齐的代码一加上去，输出的图片有问题，所以只能处理宽度是4的倍数的图片.\n");	
   printf("所以请修改要处理的图片的宽度为4的倍数。\n然后将图片重命名为source.bmp，移动到D盘根目录。\n");
   printf("完成以上步骤后"); 
   system("pause");
   bmp m;
   m.file=ReadHead();
   m.info=ReadInfo();
   int width=m.info.biWidth;/*读取图片宽度 */ 
   int height=m.info.biHeight;/*读取图片高度 */ 
   int Allpoint=width*height;/*计算总像素 */ 
   int BmpSize=m.file.bfSize;/*读取图片大小 */ 
   printf("图片信息\n");
   printf("图片大小：%dKB\n",BmpSize);
   printf("高度：%d\t宽度：%d\t总像素：%d\n",height,width,Allpoint);/*显示一些信息 */ 
   
   RGB bmrgb[height][width];
   /*创建一个二维数组存储每个像素的RGB值 */ 
   int i;
   int j;
   FILE *fpin=NULL;
   fpin=fopen("D:\\source.bmp","rb");
   FILE *fpout=NULL;
   fpout=fopen("D:\\out.bmp","wb");
   fwrite(&m.file,sizeof(BITMAPFILEHEADER),1,fpout);/*输出文件头*/ 
   fwrite(&m.info,sizeof(BITMAPINFOHEADER),1,fpout);/*输出信息头 */ 
   
   if(fpin==NULL){
   	printf("文件不存在\n");
   }else{
 	 fseek(fpin,54L,0);
 	 for(i=0;i<height;i++){
 	 	for(j=0;j<width;j++){
	 	 	fread(&bmrgb[i][j],sizeof(RGB),1,fpin);
	 	 }
       fseek(fpin,(3*width)%4,SEEK_CUR); /*四字节对齐 */ 
 	 }/*读取位图数据 */ 
 	 
 	 for(i=1;i<height-1;i++){
 	 	for(j=1;j<width-1;j++){
	 	 	bmrgb[i][j].Blue=(bmrgb[i][j].Blue+bmrgb[i+1][j].Blue+bmrgb[i+1][j+1].Blue+bmrgb[i][j+1].Blue+bmrgb[i-1][j+1].Blue+bmrgb[i-1][j].Blue+bmrgb[i-1][j-1].Blue+bmrgb[i][j-1].Blue+bmrgb[i+1][j-1].Blue)/9;
			bmrgb[i][j].Green=(bmrgb[i][j].Green+bmrgb[i+1][j].Green+bmrgb[i+1][j+1].Green+bmrgb[i][j+1].Green+bmrgb[i-1][j+1].Green+bmrgb[i-1][j].Green+bmrgb[i-1][j-1].Green+bmrgb[i][j-1].Green+bmrgb[i+1][j-1].Green)/9;
			bmrgb[i][j].Red=(bmrgb[i][j].Red+bmrgb[i+1][j].Red+bmrgb[i+1][j+1].Red+bmrgb[i][j+1].Red+bmrgb[i-1][j+1].Red+bmrgb[i-1][j].Red+bmrgb[i-1][j-1].Red+bmrgb[i][j-1].Red+bmrgb[i+1][j-1].Red)/9;
			
	 	 }
 	 }/*均值滤波计算 ，忽略上下左右边*/ 
 	 
 	 for(i=0;i<height;i++){
 	 	for(j=0;j<width;j++){
	 	 	fwrite(&bmrgb[i][j],sizeof(RGB),1,fpout);
	 	 }
	 	 fseek(fpout,(3*width)%4,SEEK_CUR);/*四字节对齐 */ 
 	 }/*输出位图数据 */  
   }

   fclose(fpin);
   fclose(fpout);
   printf("完成\n处理后的图片在D盘根目录，图片名为out.bmp\n");
}




BITMAPFILEHEADER ReadHead(){
	bmp p;
	FILE *fp=NULL;
	fp=fopen("D:\\source.bmp","rb");
	if(fp==NULL){
		printf("文件不存在\n");
	}else{
		fread(&p.file,sizeof(BITMAPFILEHEADER),1,fp);
	
	}
	return p.file;
}/*读取头文件并返回*/ 

BITMAPINFOHEADER ReadInfo(){
	bmp q;
	FILE *fp=NULL;
	fp=fopen("D:\\source.bmp","rb");
	if(fp==NULL){
		printf("文件不存在\n");
	}else{
		fseek(fp,sizeof(BITMAPFILEHEADER),0);
		fread(&q.info,sizeof(BITMAPINFOHEADER),1,fp);
	}
	return q.info;
}/*读取信息头*/

