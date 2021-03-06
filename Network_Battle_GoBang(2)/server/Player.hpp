#pragma once
#include <iostream>
#include <string>
#include <pthread.h>
using namespace std;
#define TIMEOUT 20

//用户状态
typedef enum
{
        OFFLINE=0,
        ONLINE,
        MATCHING,
        PLAYING,
}status_t;

class Player
{
private:
        string name;
        string passwd;
        uint32_t id;

        int win;//赢局数
        int lose;//输局数
        int tie;//平局数

        status_t st;//当前用户状态
        uint32_t room_id;

      // pthread_mutex_t lock;
       // pthread_cond_t cond;
public:
        //构造函数
        Player()//无参的构造函数
        {}
        Player(string &_name,string &_passwd,uint32_t &_id)
                :name(_name)
                ,passwd(_passwd)
                ,id(_id)
        {
                win = 0;
                lose = 0;
                tie = 0;
                st = OFFLINE;
              //  pthread_mutex_init(&lock,NULL);
              //  pthread_cond_init(&cond,NULL);
        }

        const string& Passwd()//获取密码
        {
                return passwd;//为了在PlayerManager中验证密码是否正确，暴露接口保证封装完整性
        }
        void Online()//让用户上线
        {
                st = ONLINE;
        }
        void Matching()
        {
                st = MATCHING;
        }
        void Playing()
        {
                st = PLAYING;
        }
        int Status()
        {
                return st;
        }
        //用户的胜率：不算平局，只算输赢局
        int Rate()
        {
                int total = win+lose;
                if(total==0)
                {
                        return 0;
                }
                return win*100/total;//取整数
        }

        uint32_t Room()
        {
                return room_id;
        }
        void SetRoom(uint32_t &room_id_)
        {
                room_id = room_id_;
        }
//        int Wait()//等待
//        {
//                struct timespec ts;//获得一个时间
//                clock_gettime(CLOCK_REALTIME,&ts);//获得当前时间
//                ts.tv_sec += TIMEOUT;//加若干时间
//                return pthread_cond_timedwait(&cond,&lock,&ts);
//        }
 //       void Signal()//唤醒
 //       {
//                pthread_cond_signal(&cond);
//        }

        ~Player()//析构函数：一个用户一旦注册，就一直保存，所以不写
        {
//                pthread_mutex_destroy(&lock);
//                pthread_cond_destroy(&cond);
        }
};
