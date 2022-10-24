#include <stdio.h>

unsigned char buff[5] = {1,2,3,4,5};
unsigned char *p1;
unsigned char *p2;

int main()
{
    /*************1指针变量************/
	// unsigned char a;
    // unsigned char *p;
    // a = 10;
    // p = &a;
    // printf("变量a地址");
    // printf("&a=0x%x\r\n",&a);
    // printf("p=0x%x\r\n",p);
    // printf("&p=0x%x\r\n",&p);
    // printf("\r\n");

    // printf("变量a的值");
    // printf("a=%d\r\n",a);
    // printf("*p=%d\r\n",*p);
    // printf("\r\n");

    // printf("通过指针变量改变变量a的值\r\n");
    // *p = 11;
    // printf("a=%d\r\n",a);
    // printf("*p=%d\r\n",*p);
    // printf("\r\n");

    // printf("通过内存地址改变变量a的值\r\n");
    // *(unsigned int *)0x61fe1f = 12;
    // printf("a=%d\r\n",a);
    // printf("*p=%d\r\n",*p);
    /*************1指针变量************/

    /*************2指针数组************/
    // p1 = buff;
    // p2 = &buff[0];

    // printf("buff=0x%x\r\n",buff);
    // printf("&buff=0x%x\r\n",&buff[0]);
    // printf("p1_addr=0x%x\r\n",p1);
    // printf("p2_Addr=0x%x\r\n",p2);
    // printf("\r\n");
    // printf("buff[0]=%d\r\n",buff[0]);
    // printf("*p1=%d\r\n",*p1);
    // printf("*p2=%d\r\n",*p2);
    /*************2指针数组************/

    /*************3指针数组运算************/
    p1 = &buff[0];

    printf("&buff[0]=0x%x   buff[0]=%d\r\n");
    /*************3指针数组运算************/

	return 0;
}