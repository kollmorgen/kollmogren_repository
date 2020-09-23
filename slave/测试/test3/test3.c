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

 //读
 if((fp = fopen("test.txt","r")) == NULL)
 {
 perror("fail to read");
 exit (1) ;
 }
 while(fgets(buf,MAX_LINE,fp) != NULL)
 {
 len = strlen(buf);
 buf[len-1] = '\0';  /*去掉换行符*/
 printf("%s %d \n",buf,len - 1);
 }
//写
 if((fp=fopen("write_txt.txt","wb"))==NULL)
	{
		printf("\nopen file Fail,close!");
		getchar();
		exit(1);
	}
	fputs("Hello world123",fp);
	fclose(fp);	
  return 0;
}
