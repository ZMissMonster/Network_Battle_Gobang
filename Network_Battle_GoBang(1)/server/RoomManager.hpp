#pragma once

#include "Room.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

#define ROOM_ID 1024

class RoomManager
{
private:
        unordered_map<uint32_t,Room> room_set;//通过房间号快速找到房间
        uint32_t assign_id;//每次有人匹配好分配房间时，房间号从这里分配，每次++
public:
        RoomManager()
                :assign_id(ROOM_ID)
        {
        }

        uint32_t CreateRoom(uint32_t &one,uint32_t &two)
        {
                Room r(one,two);//创建一个房间，不new是因为容器底层开好了空间，只需要插入
                uint32_t id = assign_id++;
                room_set.insert({id,r});//插入一个房间
                return id;
        }

        void GetBoard(uint32_t &room_id,string &_board)
        {
                room_set[room_id].Board(_board);//根据room_id拿到对应的棋盘
        }



        char GetPlayerPiece(uint32_t &room_id,uint32_t &id)
        {
                return room_set[room_id].Piece(id);
        }

        ~RoomManager()
        {}
};
