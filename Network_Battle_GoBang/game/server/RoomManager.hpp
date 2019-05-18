#pragma once

#include "Room.hpp"
#include <iostream>
#include <string>
#include <pthread.h>
#include <unordered_map>
using namespace std;

#define ROOM_ID 1024

class RoomManager
{
private:
        unordered_map<uint32_t,Room> room_set;//通过房间号快速找到房间
        uint32_t assign_id;//每次有人匹配好分配房间时，房间号从这里分配，每次++
        pthread_mutex_t lock;
public:
        RoomManager()
                :assign_id(ROOM_ID)
        {
                pthread_mutex_init(&lock,NULL);
        }
        void Lock()
        {
                pthread_mutex_lock(&lock);
        }
        void Unlock()
        {
                pthread_mutex_unlock(&lock);
        }
        uint32_t CreateRoom(uint32_t &one,uint32_t &two)
        {
                Room r(one,two);//创建一个房间，不new是因为容器底层开好了空间，只需要插入
                Lock();
                uint32_t id = assign_id++;
                room_set.insert({id,r});//插入一个房间
                Unlock();
                return id;
        }

        void GetBoard(uint32_t &room_id,string &_board)
        {
                Lock();
                room_set[room_id].Board(_board);//根据room_id拿到对应的棋盘
                Unlock();
        }

        char GetPlayerPiece(uint32_t &room_id,uint32_t &id)
        {
                Lock();
                char st = room_set[room_id].Piece(id);
                Unlock();
                return st;
        }

        bool IsMyTurn(uint32_t &room_id,uint32_t &id)
        {
                Lock();
                bool is_my_turn = room_set[room_id].IsMyTurn(id);
                Unlock();
                return is_my_turn;
        }

        void Step(uint32_t &room_id,uint32_t &id,int &x,int &y)
        {
                Lock();
                room_set[room_id].Step(id,x,y);
                Unlock();
        }

        char Judge(uint32_t &room_id,uint32_t &id)
        {
                Lock();
                char res = room_set[room_id].GameResult(id);
                Unlock();
                return res;
        }


        ~RoomManager()
        {
                pthread_mutex_destroy(&lock);
        }
};
