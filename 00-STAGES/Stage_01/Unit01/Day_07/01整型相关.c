/*
  知识点列表：
  1 类型关键字: 全称 + 简写**
  2 命名规范：匈牙利命名法   驼峰式**
  3 字面值后缀：L  LL   u ull  表达存储类型**
  4 字节大小 sizeof** 与数据范围
  5 溢出现象
  6 进制转换
  7 内存存储原理：原码 补码 反码
  8 左值 右值
  9   0八进制 0x十六进制  0b二进制  默认 十进制 字面值**
  10 打印格式
*/
#include<stdio.h>
int main ()
{
	//signed  int iValue = 9;
	//printf("signed  int 字节数: %d %d %d\n", sizeof(int),sizeof(iValue) ,sizeof(9));
	//unsigned int uiValue=9u;
	//printf("unsigned int 字节数: %d %d %d\n", sizeof(unsigned), sizeof(uiValue), sizeof(9u));


	// 
	//signed long int lValue = 9L;
	//unsigned long int ulValue=9UL;
	//printf("signed long int 字节数: %d %d %d\n", sizeof( long ), sizeof(lValue), sizeof(9L));

	//signed long long int llValue = 9LL;
	//unsigned long long int ullValue = 95ull;
	//printf("signed long long int 字节数: %d %d %d\n", sizeof( long long ), sizeof(llValue), sizeof(9LL));


	//signed short int sValue= 9 ;//小于int 按int
	//unsigned short int usValue=9u;
	//printf("signed short int 字节数: %d %d %d\n", sizeof( short ), sizeof(usValue), sizeof(9));

	// signed char cValue = 'A';
	//unsigned char ucValue='B';
	//printf("signed char 字节数: %d %d %d\n", sizeof( char), sizeof(cValue), sizeof('A'));

	// 字面值  · 常量
	//整型字面值常量：   进制前缀 数值 存储类型后缀
	2024;//默认后缀 int 默认前缀：十进制数
	0b1011011;//0b 0B 二进制前缀
	0356;//0开头：八进制数
	0x2fb;//0x 0X 开头：十六进制数

	int age = 1000;
	printf("十进制角度：%d 八进制角度：%o 十六进制角度：%X \n",age,age,age
		);


	0xaabbccddull;

	return 0;
}