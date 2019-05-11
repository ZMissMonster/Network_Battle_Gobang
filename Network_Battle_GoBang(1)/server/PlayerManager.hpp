#pragma once
#include <iostream>
#include"Player.hpp"
#include <string>
#include <utility>
#include "Log.hpp"
#define DEFAULT_ID 10000
#include <unordered_map>

using namespace std;


class PlayerManager
{
private:
        unordered_map<uint32_t,Player> Player_set;//<key,value>用户集 <id,player>密码在player里
        uint32_t assign_id;//每次有人注册，assign_id++后返回；id从这里分配
public:
        PlayerManager()
                :assign_id(DEFAULT_ID)
        {}

        uint32_t InsertPlayer(string &name, string &passwd)//插入一个玩家
        {
                uint32_t _id = assign_id++;
                Player p(name,passwd,_id);
               // player_set.insert(std::make_pair<uint32_t,Player>(_id,p));//键值插入
                Player_set.insert({_id,p});
                LOG(INFO,"插入一个玩家成功....");
                return _id;
        }
        uint32_t SearchPlayer(uint32_t &id,string &passwd)//登录用户并验证一个玩家是否注册过--在用户集中查找
        {
                //1.迭代器遍历2.find
                auto iter = Player_set.find(id);
                if(iter == Player_set.end())//到尾了，没找到
                {
                        LOG(WARNING,"查找用户失败....");
                        return 1;//返回错误码1，表示该用户不存在
                }
                Player &p = iter->second;
                if(p.Passwd()!=passwd)
                {
                        LOG(WARNING,"密码错误....");
                        return 2;//返回错误码2，表示认证失败
                }
                //表示登陆成功，让用户上线
                p.Online();
                LOG(INFO,"登陆成功！");
                return id;//成功返回id
        }

//        void SignalPlayer(uint32_t &one,uint32_t &two)
//        {
//                Player_set[one].Signal();
//                Player_set[two].Signal();
//        }

        void SetPlayStatus(uint32_t &one,uint32_t &two)//把玩家状态设置为在线
        {
                Player_set[one].Playing();
                Player_set[two].Playing();
        }

        void SetPlayRoom(uint32_t &room_id,uint32_t &one,uint32_t &two)
        {
                Player_set[one].SetRoom(room_id);
                Player_set[two].SetRoom(room_id);
        }

        void SetMatching(uint32_t &id)
        {
                Player_set[id].Matching();//将用户状态设置为matching
        }

        uint32_t GetPlayerRoomId(uint32_t &id)
        {
                Player_set[id].Room();
        }

        int GetRate(uint32_t &id)//获得胜率
        {
                return Player_set[id].Rate();
        }

        int Ready(uint32_t &id)
        {
                return Player_set[id].Status();//直接把状态返回给用户
        }

        ~PlayerManager()
        {}
};
