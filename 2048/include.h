/*************************************************************************
	> File Name: include.h
	> Author: 
	> Mail: 
	> Created Time: 2020年04月30日 星期四 14时44分13秒
 ************************************************************************/

#include <iostream>
#include <algorithm>
#include <cstdio>
#include <curses.h>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <signal.h>
using namespace std;


#ifndef _INCLUDE_H
#define _INCLUDE_H

char *itoc(int num);

void change_col();
void open_col(int num);
void off_col(int num);

class Game {
public:
    Game (int UP, int LEFT, int HIGH, int LEN);
    void mk_map();
    void show();
    void rand_ind(); 
    bool empty();
    void re_empty();
    bool operate(char op);
    bool is_over();
    bool gameover();
    void exit_game();
    void record();
    void re_play();
    void regret();
private:
    bool regret_flag;
    int up, left, high, len;
    int score, regret_cnt;
    int data[4][4], pre_data[4][4];
    vector<pair<int, int> > em_ind;
};

#endif
