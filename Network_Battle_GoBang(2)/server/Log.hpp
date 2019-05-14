#pragma once
#include <iostream>
#include <string>
#include<vector>
using namespace std;

#define INFO 0 //没有危害，打出来看看
#define WARNING 1 //警告
#define FATAL 2 //致命错误

vector<string> g_error_level={"INFO","WARNING","FATAL",};//像数组一样使用vector，最后一项可以加逗号，也可以不加

void Log(int level,string message,string file,int line)//日志级别，错误信息，哪个文件，哪一行
{
        cout<<"["<<g_error_level[level]<<"]["<<message<<"]"<<file<<":"<<line<<endl;//打印日志
}
//定义宏代替函数只用传两个参数
#define LOG(level,message) Log(level,message,__FILE__,__LINE__)
