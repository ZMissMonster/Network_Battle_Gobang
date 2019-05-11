#pragma once

#include <iostream>
#include <string>
#include <rpc_client.hpp>
#include <chrono>
#include <stdlib.h>
#include <fstream>
#include "codec.h"
using namespace std::chrono_literals;
using namespace std;
using namespace rest_rpc;
using namespace rest_rpc::rpc_service;

bool PushMatchPool(string &ip,int &port,uint32_t &id)//把用户放进匹配池
{
	try {
		rpc_client client(ip, port);//短链接：操作一次连一次然后断开，client调用时开辟，用完就断开
		bool r = client.connect();
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 3;
		}

    	return client.call<bool>("RpcMatchAndWait",id);//调用服务器的方法
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
    }
}
int CheckReady(string &ip,int &port,uint32_t &id)
{
	try {
		rpc_client client(ip, port);//短链接：操作一次连一次然后断开，client调用时开辟，用完就断开
		bool r = client.connect();
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 3;
		}
    	return client.call<int>("RpcPlayerReady",id);//调用服务器的方法
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
    }
}
bool Match(string &ip,int &port,uint32_t &id)//匹配
{
        PushMatchPool(ip,port,id);
        int count = 20;
        while(1)//周期性地检测用户状态
        {
                int result = CheckReady(ip,port,id);
                if(result == 3)//匹配成功
                {
                        return true;
                }
                else if(result == 1)//匹配失败
                {
                        cout<<"匹配失败！"<<endl;
                        return false;
                }
                else//继续匹配，系统时间为20秒，超过则自动超时
                {
                        printf("请等待：%2d\r",count--);
                        fflush(stdout);
                        if(count<0)
                        {
                                cout<<endl;
                                cout<<"匹配超时！"<<endl;
                              //  PopMatchPool(ip,port,id);
                                break;
                        }
                        sleep(1);
                }
        }
}
int GetBoard(string &ip,int &port,uint32_t &room_id,string &board)
{
	try {
		rpc_client client(ip, port);//短链接：操作一次连一次然后断开，client调用时开辟，用完就断开
		bool r = client.connect();
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 3;
		}
    	board = client.call<string>("RpcBoard",room_id);//调用服务器的方法
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
    }

}

void ShowBoard(string &board)
{
        for(auto i = 0; i < board.size();i++)
        {
                cout <<'|'<< board[i]<<'|'<< ' '<<endl;
        }
}

uint32_t GetMyRoomId(string &ip,int &port,uint32_t &id)
{
	try {
		rpc_client client(ip, port);//短链接：操作一次连一次然后断开，client调用时开辟，用完就断开
		bool r = client.connect();
		if (!r) {
			std::cout << "连接超时！" << std::endl;
			return 3;
		}
    	return client.call<uint32_t>("RpcPlayerRoomId",id);//调用服务器的方法
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
    }
}
char GetMyPiece(string &ip,int &port,uint32_t &room_id,uint32_t &id)
{
	try {
		rpc_client client(ip, port);//短链接：操作一次连一次然后断开，client调用时开辟，用完就断开
		bool r = client.connect();
		if (!r) {
			std::cout << "连接超时！" << std::endl;
			return 3;
		}
    	return client.call<char>("RpcPlayerPiece",room_id,id);//调用服务器的方法
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
    }
}
bool IsMyTurn()
{
        return false;
}
bool PosIsRight(int x,int y)
{
        return false;
}
void Step(uint32_t &id, int x, int y)
{

}
char Judge(uint32_t id)
{
        return 'N';
}
void PlayGame(string &ip,int &port,uint32_t &id)
{
        cout<<"匹配成功，开始游戏！"<<endl;
        int x,y;
        char result = 'N';//继续游戏
        string board;
        uint32_t room_id = GetMyRoomId(ip,port,id);
        if(room_id < 1024)
        {
                return;
        }
        cout << "房间号: " <<room_id << endl;
        char piece = GetMyPiece(ip,port,room_id,id);
        cout << "你的棋子: " << piece << endl;
        for( ; ;)
        {
               // system("clear");
                GetBoard(ip,port,room_id,board);//获得棋盘
                ShowBoard(board);//给用户展示棋盘
                if(!IsMyTurn())//不该我走
                {
                        sleep(1);//每隔一秒检测一次是否该我走
                        continue;
                }
                //该我走
                 cout << "请输入你的落子位置->" << endl;
                 cin >> x >> y;

                 //检测坐标是否合法
                 if(x >= 1 && x <=5 && y>=1 && y<=5)
                 {
                        if(!PosIsRight(x,y))
                        {
                                 cout << "已占用！请重新输入！" << endl;
                         }
                        else
                         {
                                Step(id,x,y);
                                result = Judge(id);//判断输赢
                                if(result !='N')//此时结果已出，不再继续
                                {
                                        break;
                                }
                        }
                }
                else
                {
                        cout << "你输入的位置有误，请重新输入！" << endl;
                }
        }
        switch(result)
        {
                case 'X':

                        break;
                case 'O':
                        break;
                case 'E':
                        cout << "平局！恭喜你们！" << endl;
                        break;
                default:
                        break;
        }
}

void Game(string &ip,int &port,uint32_t &id)
{
        int select = 0;
        volatile bool quit = false;
        while(!quit)
        {
         cout<<"***************************************"<<endl;
         cout<<"***   1.匹配           2.退出     ****"<<endl;
         cout<<"***************************************"<<endl;
         cout<<"请选择：";
         cin>>select;
         switch(select)
         {
                 case 1:
                        {
                                if(Match(ip,port,id))
                                {
                                        PlayGame(ip,port,id);
                                }
                                else
                                {
                                        cout<<"匹配失败！请重试！"<<endl;
                                }
                        }
                         break;
                 case 2:
                         quit = true;
                         break;
                 default:
                         cout<<"输入有误，请重新输入！"<<endl;
                         break;
         }
        }
}

uint32_t Login(const string &ip,const int &port)
{
        uint32_t id;
        string passwd;
        cout <<"请输入你的登录ID：";
        cin >> id;
        cout<<"请输入登录密码：";
        cin >>passwd;
        uint32_t result = 0;
	try {
		rpc_client client(ip, port);//短链接：操作一次连一次然后断开，client调用时开辟，用完就断开
		bool r = client.connect();
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 3;
		}

    	result = client.call<uint32_t>("RpcLogin",id,passwd);//调用服务器的方法
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
    }
		return result;
}
//注册
uint32_t Register(const string &ip,const int &port,string &name,string &passwd,uint32_t &id)//id从这里返回

{
        cout << "请输入昵称:";
        cin >> name;
        cout << "请输入密码:";
        cin >> passwd;
        string passwd_;
        cout << "请确认密码:";
        cin >> passwd_;
        if(passwd != passwd_)
        {
                cout<<"密码输入不一致，请重新输入:";
                return 1;//密码错误返回错误码，正确返回分配的id
        }
	try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r) {
			std::cout << "connect timeout" << std::endl;
			return 2;//异常返回
		}

    	id = client.call<uint32_t>("RpcRegister",name,passwd);//调用服务器的方法
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
    }
		return 0;//注册成功返回0
}
