#pragma once

#include <vector>
#include <string>
#include <pthread.h>
#include <iostream>
#include "RoomManager.hpp"
using namespace std;
#include "PlayerManager.hpp"

#define MATCH_LEVEL 101

class Hall
{
private:
        PlayerManager pm;
        RoomManager rm;
        vector<vector<uint32_t> > match_pool;//匹配池：相同胜率的在同一胜率级别vector中，一共101个vector胜率级别，由一个vector管理
        int match_num;
        pthread_mutex_t match_lock;//匹配锁
        pthread_cond_t match_cond;//条件变量
public:
        Hall()
          :match_pool(MATCH_LEVEL)//默认匹配池有[0,100]一共101个数据
          ,match_num(0)
        {
        }

        int MatchNum()
        {
                return match_num;
        }

        void IncMatchNum()
        {
                match_num++;
        }

        void ResetMatchNum()
        {
                match_num = 0;
        }

        void GetAllMatchId(vector<uint32_t> &id_list)//获得所有匹配的用户ID放进id_list
        {
                for(auto i = MATCH_LEVEL-1;i>=0;i--)
                {
                        auto &v = match_pool[i];//从胜率100的开始倒着放
                        if(v.empty())//如果这一级别的为空，则去下一个级别
                        {
                                continue;
                        }
                        for(auto it = v.begin();it != v.end();it++)
                        {
                                id_list.push_back(*it);//把用户放进去
                        }
                }
        }

        void LockMatchPool()
        {
                pthread_mutex_lock(&match_lock);
        }

        void UnlockMatchPool()
        {
                pthread_mutex_unlock(&match_lock);
        }

        void ServiceWait()//线程等待
        {
                pthread_cond_wait(&match_cond,&match_lock);//在cond条件变量下等
        }

        void ServiceWakeup()//唤醒
        {
                pthread_cond_signal(&match_cond);//唤醒
        }

        uint32_t Register(string &name, string &passwd)
        {
                return pm.InsertPlayer(name,passwd);//调用pm中的插入方法，在大厅中管理玩家
        }

        uint32_t Login(uint32_t &id,string &passwd)
        {
                return pm.SearchPlayer(id,passwd);//调用pm中的查找方法，验证用户是否已经注册
        }

        bool PushIdInMatchPool(uint32_t &id)//把用户放进匹配池
        {

                LOG(INFO,"把用户放进匹配池....");
                pm.SetMatching(id);//把用户设置为匹配状态
                int rate = pm.GetRate(id);//获取用户胜率，以便放入vector进行管理
                LockMatchPool();
                auto &v = match_pool[rate];//拿到同胜率级别的vector
                auto it = v.begin();
                for(; it != v.end(); it++)
                {
                        if(*it == id)//该用户已存在匹配池中
                        {
                                return false;
                        }
                }
                v.push_back(id);//否则放入匹配池中
                IncMatchNum();
                UnlockMatchPool();
                ServiceWakeup();//唤醒
                return true;
               // return pm.PlayerWait(id);//特定用户等待匹配
        }

        void MatchPoolClear()//匹配池清空
        {

                LOG(INFO,"匹配池被清空....");
                for(auto i = MATCH_LEVEL-1;i>=0;i--)
                {
                        auto &v = match_pool[i];
                        if(v.empty())
                        {continue;}
                        vector<uint32_t>().swap(v);//定义一个临时空对象，交换后就清空了每一个胜率级别的用户
                }
                ResetMatchNum();//把里面的用户数据也要清空
        }

        int IsPlayerReady(uint32_t &id)//用户状态是否是ready
        {
                return pm.Ready(id);
        }

        void GamePrepare(uint32_t &one,uint32_t &two)//游戏准备
        {

                pm.SetPlayStatus(one,two);//把玩家状态设置为游戏状态
                uint32_t room_id = rm.CreateRoom(one,two);//匹配好的两个人进入创建房间
                pm.SetPlayRoom(room_id,one,two);//将房间号设置进玩家信息
               // pm.SignalPlayer(one,two);//一切准备就绪唤醒玩家告知可以游戏
        }
        static void *MatchService(void* arg)//线程的匹配服务
        {
                pthread_detach(pthread_self());//线程分离

                Hall *hp = (Hall*)arg;//static无法访问成员函数或者成员变量，所以定义该指针来访问
            
                while(1)
                {
                hp->LockMatchPool();
                while(hp->MatchNum() < 2)//匹配的用户只有一个人或者没有,防止误唤醒，用while循环
                {
                        LOG(INFO,"服务线程开始等待....");
                        //wait
                       hp->ServiceWait();
                }
                LOG(INFO,"服务线程已被唤醒...");
                vector<uint32_t> id_list;//创建一个vector，用户ID列表，按胜率依次放入，以便提高匹配效率
                hp->GetAllMatchId(id_list);//把用户放进id_list

                int num = id_list.size();//一共有多少人

                num &= (~1);//1按位取反后与num进行按位与运算，将num的31位保留，最后一位置为0，这样num就是偶数了，只匹配偶数个玩家，单独的下次进入匹配池匹配
                for(auto i = 0;i<=num;i+=2)//一次匹配两个人
                {
                        uint32_t play_one = id_list[i];
                        uint32_t play_two = id_list[i+1];
                        hp->GamePrepare(play_one,play_two);//游戏准备
                }
                //匹配完毕后将所有用户清空
               hp->MatchPoolClear();
               hp->UnlockMatchPool();
                }
        }

        void InitHall()//初始化大厅
        {
                pthread_mutex_init(&match_lock,NULL);
                pthread_cond_init(&match_cond,NULL);//初始化锁和条件变量

                pthread_t tid;
                pthread_create(&tid,NULL,MatchService,this);//创建线程
        }

        string GetPlayerBoard(uint32_t &room_id)
        {
                string board;
               // uint32_t room_id = pm.GetPlayerRoomId(id);
                rm.GetBoard(room_id,board);
                return board;
        }

        uint32_t GetPlayerRoomId(uint32_t &id)
        {
                return pm.GetPlayerRoomId(id);
        }

        char GetPlayerPiece(uint32_t &room_id,uint32_t &id)
        {
                return rm.GetPlayerPiece(room_id,id);
        }
        bool IsMyTurn(uint32_t &room_id,uint32_t &id)
        {
                return rm.IsMyTurn(room_id,id);
        }

        void Step(uint32_t &room_id,uint32_t &id,int x,int y)
        {
                return rm.Step(room_id,id,x,y);
        }

        char Judge(uint32_t &room_id,uint32_t &id)
        {
                return rm.Judge(room_id,id);
        }

        ~Hall()
        {
                pthread_mutex_destroy(&match_lock);//销毁锁和条件变量
                pthread_cond_destroy(&match_cond);
        }
};
