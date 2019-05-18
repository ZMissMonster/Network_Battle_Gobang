#include "game.hpp"

static void Usage(string proc)
{
        cout << "Usage : " << proc << "IP地址 端口号" << endl;//参数输入错误，提示用户如何连接服务器
}
static void LoginMenu()
{
        cout << "*******************************" << endl;
        cout << "*** 1. 登录         2. 注册 ***" << endl;
        cout << "***                 3. 退出 ***" << endl;
        cout << "*******************************" << endl;
        cout <<"请选择-> ";
}
int main(int argc,char *argv[])//启动服务器时，传参只需两个，IP地址和端口号
{
        if(argc != 3)
        {
                Usage(argv[0]);
                return 1;
        }
        string ip = argv[1];
        int port = atoi(argv[2]);
        string name;
        string passwd;
        uint32_t id;
        int select = 0;
        while(1) 
        {
                LoginMenu();
                cin >> select;
                switch(select)
                {
                        case 1:
                             {
                                 uint32_t result= Login(ip,port);
                                 if(result == id)
                                {
                                         cout<<"登陆成功！请开始匹配游戏！"<<endl;
                                         Game(ip,port,id);
                                 }
                               else
                               {
                                       cout<<"登陆失败，退出码："<<result<<endl;
                               }
                                }
                                break;
                        case 2:
                                Register(ip,port,name,passwd,id);
                                cout<<"注册成功！"<<endl;
                                cout<<"请记住你的登录ID：["<< id <<"]"<<endl;
                                break;
                        case 3:
                                cout << "退出成功！"<< endl;
                                exit(2);
                        default:
                                cout << "输入错误，请重新输入-> "<< endl;
                                break;
                }
        }

}
