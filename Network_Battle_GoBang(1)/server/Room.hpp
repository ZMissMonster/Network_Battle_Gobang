#pragma once
#include <iostream>
#include <pthread.h>
#include <string>
using namespace std;

#define SIZE 5//棋盘大小
#define BLACK 'X'
#define WHITE 'O'//定义黑白子

class Room
{
public:
        Room()
        {}
        Room(uint32_t &id1, uint32_t &id2)
        {
                piece[0]='X';//默认先进来的是用户一，用黑子
                piece[1]='O';
                memset(board,' ',sizeof(board));//初始化棋盘为空格
                result = 'N';//默认继续
                pthread_mutex_init(&lock,NULL);
        }
        void Board(string &_board)
        {
                for(auto i = 0 ; i < SIZE ;i++)
                {
                        for(auto j = 0 ; j < SIZE;j++)
                        {
                                _board.push_back(board[i][j]);
                        }
                }
        }

        char Piece(uint32_t &id)
        {
                int pos = (id == one? 0:1);
                return piece[pos];
        }

        ~Room()
        {
                pthread_mutex_destroy(&lock);
        }
private:
        uint32_t one;//用户1拿 'X'
        uint32_t two;//用户2拿 'O'
        char piece[2];//棋子有2种,0下标对应one用户，1下标对应two用户
        uint32_t current;//当前该谁走
        char board[SIZE][SIZE];//定义二维数组表示棋盘
        char result;//X--黑子赢 O--白子赢 E--平局 N--继续
        pthread_mutex_t lock;
};
