#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 1024
int main()
{
  //读
 char buf[MAX_LINE];  /*缓冲区*/
 FILE *fp;            /*文件指针*/
 int len;             /*行字符个数*/
 int d[7];
  d[1]=9;
d[2]=19;
d[3]=29;
d[4]=39;
d[5]=49;
d[6]=59;
d[0]=1;
 // 写
 if((fp=fopen("write2.txt","wb"))==NULL)
	{
		printf("\nopen file Fail,close!");
		getchar();
		exit(1);
	}

	fputs("Hello world123\n",fp);
        fputs("位置信息:",fp);
fprintf(fp,"%d,%d,%d,%d,%d,%d\n",d[1],d[2],d[3],d[4],d[5],d[6]);
fputs("位置信息:",fp);
fclose(fp);

 // 写2
 if((fp=fopen("write2.txt","ab"))==NULL)
	{
		printf("\nopen file Fail,close!");
		getchar();
		exit(1);
	}

	fputs("Hello world123\n",fp);
        fputs("位置信息23:",fp);
fprintf(fp,"%d,%d,%d,%d,%d,%d\n",d[1],d[2],d[3],d[4],d[5],d[6]);
fputs("位置信息23:",fp);
fclose(fp);
//fp = fopen("write2.txt","w");

	//fclose(fp);	
  return 0;
}
