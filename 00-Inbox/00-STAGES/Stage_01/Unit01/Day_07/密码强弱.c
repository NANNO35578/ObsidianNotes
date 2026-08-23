#include<stdio.h>
int main()
{
	//面向过程
	//输入的字符数量
	char pwd;
	//连续输入 以 换行 作为结束
	int count = 0;
	int hasNumber = 0;//是否有数字： 0没有 1有
	int hasUpper = 0;//是否有大写字母： 0没有 1有
	int hasLower = 0;//是否有小写字母： 0没有 1有
	int hasOthers = 0;//是否有其它字符： 0没有 1有
	do 
	{
		//scanf("%c",&pwd);
		scanf_s("%c", &pwd,1);//可用字节数为1 
		if (pwd!='\n')
		{
			count++;
			if ( pwd>='0' && pwd<='9')
			{
				hasNumber = 1;
			}
			else if (pwd >= 'A' && pwd <= 'Z')
			{
				hasUpper = 1;
			}
			else if (pwd >= 'a' && pwd <= 'z')
			{
				hasLower = 1;
			}
			else
			{
				hasOthers = 1;
			}

		}
		
	} while (pwd != '\n');
	//printf("%d\n",count);
	//密码强弱的 判断策略
	if (count<=6)
	{
		printf("弱密码\n");
	}
	else
	{
		switch ( hasNumber+hasUpper+hasLower+hasOthers )
		{
		case 1:
		case 2:
			printf("弱密码\n");
			break;
		case 3:
			printf("中密码\n");
			break;
		case 4:
			printf("强密码\n");
			break;
		}
	}
	return 0;
}