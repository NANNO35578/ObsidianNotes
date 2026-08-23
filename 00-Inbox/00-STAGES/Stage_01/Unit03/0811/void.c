#include<stdio.h>
//返回值位置的void
//参数位置
void f1( void )//没有参数 大部分编译器void参数可以省略
{
	return  ; //此时的return可以省略 如果要写 则不加值
}
//f1 的void代表不需要返回值。  f2的void* 必须要返回一个指针 
void* f3(void* a )
{
	return NULL;
}
int f2()
{
	return 0;//如果返回值不是void return必须写对应类型的值。可能报编译错误
}

int main()
{
	//void :无类型/不确定类型  起到类型名的占位作用
	//应用位置： 
	//明确区分：void与void*在返回类型位置的区别。
	return 0;
}