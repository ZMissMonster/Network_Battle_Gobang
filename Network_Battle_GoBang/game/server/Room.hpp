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
                :one(id1)
                ,two(id2)
        {
                piece[0]='X';//默认先进来的是用户一，用黑子
                piece[1]='O';
                memset(board,' ',sizeof(board));//初始化棋盘为空格
                result = 'N';//默认继续
                current = one;//默认当前one先走
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

        bool IsMyTurn(uint32_t &id)
        {
                return id == current?true:false;
        }

        void Step(uint32_t &id,int &x,int &y)
        {
                if(current == id)
                {
                   int pos = (id ==one ? 0:1);//拿到对应的颜色下标piece[pos]
                   board[x][y]=piece[pos];
                   current = (id == one ?two:one);//切换用户走
                   result = Judge();
                }
        }

        char GameResult(uint32_t &id)
        {
                return result;
        }

        char Judge()
        {
                int row = SIZE;//设置按行列检测
                int col = SIZE;
                for(auto i = 0;i<row;i++)//一行五个是否相等
                {
                        if( board[i][0] != ' '&& \
                            board[i][0] == board[i][1] && \
                            board[i][1] == board[i][2] && \
                            board[i][2] == board[i][3] && \
                            board[i][3] == board[i][4])
                          {
                                  return board[i][0];
                          }
                }
                for(auto i = 0;i<col;i++)//一列五个是否相等
                {
                        if( board[0][i] != ' '&& \
                            board[0][i] == board[1][i] && \
                            board[1][i] == board[2][i] && \
                            board[2][i] == board[3][i] && \
                            board[3][i] == board[4][i])
                          {
                                  return board[0][i];
                          }
                }
                //检查对角线是否相等
                if( board[0][0] != ' '&& \
                    board[0][0] == board[1][1] && \
                    board[1][1] == board[2][2] && \
                    board[2][2] == board[3][3] && \
                    board[3][3] == board[4][4])
                   {
                           return board[0][0];
                   }

                if( board[0][4] != ' '&& \
                    board[0][4] == board[1][3] && \
                    board[1][3] == board[2][2] && \
                    board[2][2] == board[3][1] && \
                    board[3][1] == board[4][0])
                   {
                           return board[0][4];
                   }
                //检查是否满盘
                for(auto i =0;i < row;i++)
                {
                        for(auto j =0; j < col;j++)
                        {
                                if(board[i][j] == ' ')//没有人赢
                                {
                                        return 'N';//继续走
                                }
                        }
                }
                return 'E';
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
