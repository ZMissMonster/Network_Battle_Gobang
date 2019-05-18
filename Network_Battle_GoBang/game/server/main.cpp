#include <rpc_server.h>
#include "Hall.hpp"
using namespace rest_rpc;
using namespace rpc_service;
#include <fstream>
using namespace std;

Hall GameHall;

uint32_t RpcRegister(connection* conn,string name,string passwd)
{
       // std::cout<<"获得一个注册请求 昵称:"<< name <<" "<<"密码:"<< passwd <<std::endl;
        return GameHall.Register(name,passwd);
}

uint32_t RpcLogin(connection* conn,uint32_t id,string passwd)
{
        return GameHall.Login(id,passwd);
}

bool RpcMatchAndWait(connection* conn,uint32_t id)
{
        return GameHall.PushIdInMatchPool(id);
}
int RpcPlayerReady(connection* conn,uint32_t id)
{
        return GameHall.IsPlayerReady(id);
}
string RpcBoard(connection* conn,uint32_t room_id)
{
        return GameHall.GetPlayerBoard(room_id);
}
uint32_t RpcPlayerRoomId(connection* conn,uint32_t id)
{
        return GameHall.GetPlayerRoomId(id);
}
char RpcPlayerPiece(connection* conn,uint32_t room_id,uint32_t id)
{
        return GameHall.GetPlayerPiece(room_id,id);
}
bool RpcIsMyTurn(connection* conn,uint32_t room_id,uint32_t id)
{
        return GameHall.IsMyTurn(room_id,id);
}
void RpcStep(connection* conn,uint32_t room_id,uint32_t id,int x,int y)
{
        return GameHall.Step(room_id,id,x,y);
}
char RpcJudge(connection* conn,uint32_t room_id,uint32_t id)
{
       return GameHall.Judge(room_id,id);
}
bool RpcPopMatchPool(connection* conn,uint32_t id)
{
        return GameHall.PopIdMatchPool(id);
}
int main() {
	rpc_server server(9000, 4);//创建一个rpc服务器
    LOG(INFO,"初始化服务器成功...");

    server.register_handler("RpcRegister",RpcRegister);//注册一个注册方法
    server.register_handler("RpcLogin",RpcLogin);//注册一个登录方法
    server.register_handler("RpcMatchAndWait",RpcMatchAndWait);//注册一个匹配并且等待方法
    server.register_handler("RpcPlayerReady",RpcPlayerReady);//注册一个用户准备方法
    server.register_handler("RpcPlayerRoomId",RpcPlayerRoomId);//注册一个房间号方法
    server.register_handler("RpcPlayerPiece",RpcPlayerPiece);//注册一个棋子方法
    server.register_handler("RpcBoard",RpcBoard);//注册一个棋盘方法
    server.register_handler("RpcIsMyTurn",RpcIsMyTurn);
    server.register_handler("RpcStep",RpcStep);
    server.register_handler("RpcJudge",RpcJudge);
    server.register_handler("RpcPopMatchPool",RpcPopMatchPool);

    LOG(INFO,"方法注册完毕...");
    LOG(INFO,"服务器已启动...");
    GameHall.InitHall();//初始化大厅

	server.run();//服务器跑起来

	std::string str;
	std::cin >> str;
}
