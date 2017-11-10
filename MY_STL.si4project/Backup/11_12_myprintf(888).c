//#include<stdio.h>
//#include<stdlib.h>
//#include<stdarg.h>
//#include<string.h>
// 
////自定义输出函数，按照str的字符顺序输出  字符，字符串，数字
//void my_printf(const char *str, int len, ...)
//{	
//	int i = 0; 
//    va_list arg; 
//    va_start(arg, len); 
//
//    for (i = 0; i < len; i++,*str++)
//    {
//        char a = 0;
//        char *a1 = NULL;
//        int a2 = 0;
//        //多分支语句，分情况讨论
//        switch (*str)
//        {
//        case 'c':   //参数是字符型，char类型
//            a = va_arg(arg, char);
//            putchar(a);
//            break;
//        case 's':   //参数是字符串时，char*类型
//            a1 = va_arg(arg,char*);
//            while (*a1)
//            {
//                putchar(*a1);
//                *a1++;
//            }
//            break;
//        case 'd':   //参数是整形，int类型
//             a2 = va_arg(arg, int);
//            putchar(a2+'0');
//            break;
//        default:   //参数是其他，按照本来形式输出
//            putchar(*str);
//            break;
//        }
//    }
//    va_end(arg);
//}
// 
//int main()
//{
//    char *str = "dc\ts\ndc\ndc\ts!";
//    int len = strlen(str);
//    my_printf(str, len,1, 'b',"zhangweina",2, 'i', 3,'t', "welcome to you");
//    system("pause");
//    return 0;
//}

#include，》
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

static int my_putint( int a )
{
    int prev = a / 10;
    if( 0 != prev )
    {
        my_putint(prev);
    }
    putchar( a%10 + '0');
}

int my_printf(const char *format,...)
{
    va_list arg_list;
    va_start(arg_list, format);
    const char *start = format;
    while( *start == '\0')
    {
       if( *start == '%')
       {
           start++;
           switch(*start)
           {
               case 'd':
               int _int = va_arg(arg_list,int);
               my_putint(_int);
               break;

               case 's':
               char *_string = va_arg(arg_list, char *);
               while( *_string )
               {
                   putchar( *_string++ );
               }
               break;
               case 'c':
               char _char = va_arg(arg_;ist, char);
               putchar(_char);
           }
       }
       else
       {
           putchar(*start);
       }
    }
    va_end(arg_list);
    return 0;
}

int main()
{
    int a = 10;
    const char *msg = "hello bit";
    char c = 'c';
    my_printf("number : %d\nstring : %s\nCHAR :%C\N",a, msg, C);
    return 0;
}
