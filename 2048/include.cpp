/*************************************************************************
	> File Name: include.cpp
	> Author: 
	> Mail: 
	> Created Time: 2020年04月30日 星期四 14时57分28秒
 ************************************************************************/

#include "include.h"

char *itoc(int num) {
    char str[4];
    int ind = 0;
    char *temp;
    temp = (char *)calloc(4, 1);
    if (num == 0) {
        temp[0] = '0';
        return temp;
    }
    while (num != 0) {
        str[ind] = '0' + num % 10;
        num /= 10;
        ind++;
    }
    for (int i = ind - 1, j = 0; i >= 0; i--) {
        temp[j] = str[i];
        j++;
    }
    return temp;
}

void change_col() {
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    return ;
}

void op_off_col(int num, bool opt) {
    int flag;
    switch (num) {
        case 0 : flag = 5; break;
        case 2 :
        case 256 : flag = 1; break;
        case 4 :
        case 512 : flag = 2; break;
        case 8 :
        case 1024 : flag = 3; break;
        case 16 :
        case 2048 : flag = 4; break;
        case 64 : flag = 6; break;
        case 32 :
        case 128 : flag = 7; break;
    }
    if (opt) attron(COLOR_PAIR(flag));
    else attroff(COLOR_PAIR(flag));
    return ;
}

Game::Game(int UP, int LEFT, int HIGH, int LEN) {
    up = UP;
    left = LEFT;
    high = HIGH;
    len = LEN;
    score = 0;
    regret_cnt = 3;
    regret_flag = false;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            data[i][j] = 0;
            pre_data[i][j] = 0;
            em_ind.push_back(pair<int, int> (i, j));
        }
    }
}

void Game::mk_map() {
    initscr();
    clear();
    cbreak();
    noecho();
    curs_set(0);
    change_col();
    for (int i = 0; i < 5; i++) {
        int x = up + 4 * i;
        for (int j = 0; j < len; j++) {
            move(x, left + j);
            addch('-');
        }
    }
    for (int i = 0; i < 4; i++) {
        int temp = up + i * 4 + 1;
        for (int k = 0; k < 3; k++) {
            int x = temp + k;
            for (int j = 1; j <= 5; j++) {
                int y = left + (j - 1) * 7;
                move(x, y);
                addch('|');
            }
        }
    }
    mvprintw(up + high + 4, left, "score : %s", itoc(score));
    mvprintw(up + high + 5, left, "back to previous times : %s", itoc(regret_cnt));
    mvprintw(up + high + 8, left, "prompt : w, a, s, d -> control the game");
    mvprintw(up + high + 9, left, "prompt : enter 'h' back to previous step");
    mvprintw(up + high + 10, left, "prompt : enter 'r' play the game again");
    mvprintw(up + high + 11, left, "prompt : enter 'q' exit the game");
    FILE *fp = fopen("his_max_score.txt", "r");
    char *str = (char *)calloc(25, 1);
    fgets(str, 23, fp);
    int max_score = atoi(str);
    mvprintw(up + high + 6, left, "highest score in history : %s", itoc(max_score));
    fclose(fp);
    free(str);
    refresh();
    return ;
}

void Game::show() {
    for (int i = 0; i < 4; i++) {
        int x = up + i * 4 + 2;
        for (int j = 0; j < 4; j++) {
            int y = left + j * 7 + 3;
            char *str = itoc(data[i][j]);
            mvprintw(x, y, "    ");
            op_off_col(data[i][j], true);
            mvprintw(x, y, "%s", str);
            op_off_col(data[i][j], false);
            free(str);
        }
    }
    mvprintw(up + high + 4, left, "score : %s", itoc(score));
    mvprintw(up + high + 5, left, "back to previous times : %s", itoc(regret_cnt));
    refresh();
    return ;
}

void Game::rand_ind() {
    if (!em_ind.size()) return ;
    pair<int, int> p;
    srand(time(0));
    p = em_ind[rand() % em_ind.size()];
    data[p.first][p.second] = rand() % 100 < 20 ? 4 : 2;
}

bool Game::empty() {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (data[x][y] == 0) return false;
        }
    }
    return true;
}

void Game::re_empty() {
    em_ind.clear();
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (data[x][y] == 0) {
                em_ind.push_back(pair<int, int> (x, y));
            }
        }
    }
    return ;
}

void Game::regret() {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            pre_data[x][y] = data[x][y];
        }
    }
    return ;
}

bool Game::operate(char op) {
    bool flag = false;
    if (op != 'h') regret();
    switch (op) {
        case 'w' : {
            regret_flag = true;
            for (int y = 0; y < 4; y++) {
                int temp[4] = {0}, ind = 0;
                for (int x = 0; x < 4; x++) {
                    if (data[x][y] != 0) {
                        temp[ind] = data[x][y];
                        if (ind != x) flag = true;
                        ind++;
                    }
                }
                for (int i = 0; i < ind; i++) {
                    if (i < 3 && temp[i] == temp[i + 1]) {
                        score += temp[i];
                        temp[i] *= 2;
                        temp[i + 1] = 0;
                        flag = true;
                    }
                }
                for (int x = 0; x < 4; x++) {
                    data[x][y] = 0;
                }
                int k = 0;
                for (int i = 0; i < ind; i++) {
                    if (temp[i] != 0) {
                        data[k][y] = temp[i];
                        k++;
                    }
                }
            }
        } break;
        case 's' : {
            regret_flag = true;
            for (int y = 0; y < 4; y++) {
                int temp[4] = {0}, ind = 0;
                for (int x = 3; x >= 0; x--) {
                    if (data[x][y] != 0) {
                        temp[ind] = data[x][y];
                        if (ind != 3 - x) flag = true;
                        ind++;
                    }
                }
                for (int i = 0; i < ind; i++) {
                    if (i < 3 && temp[i] == temp[i + 1]) {
                        score += temp[i];
                        temp[i] *= 2;
                        temp[i + 1] = 0;
                        flag = true;
                    }
                }
                for (int x = 0; x < 4; x++) {
                    data[x][y] = 0;
                }
                int k = 3;
                for (int i = 0; i < ind; i++) {
                    if (temp[i] != 0) {
                        data[k][y] = temp[i];
                        k--;
                    }
                }
            }
        } break;
        case 'a' : {
            regret_flag = true;
            for (int x = 0; x < 4; x++) {
                int temp[4] = {0}, ind = 0;
                for (int y = 0; y < 4; y++) {
                    if (data[x][y] != 0) {
                        temp[ind] = data[x][y];
                        if (ind != y) flag = true;
                        ind++;
                    }
                }
                for (int i = 0; i < ind; i++) {
                    if (i < 3 && temp[i] == temp[i + 1]) {
                        score += temp[i];
                        temp[i] *= 2;
                        temp[i + 1] = 0;
                        flag = true;
                    }
                }
                for (int y = 0; y < 4; y++) {
                    data[x][y] = 0;
                }
                int k = 0;
                for (int i = 0; i < ind; i++) {
                    if (temp[i] != 0) {
                        data[x][k] = temp[i];
                        k++;
                    }
                }
            }
        } break;
        case 'd' : {
            regret_flag = true;
            for (int x = 0; x < 4; x++) {
                int temp[4] = {0}, ind = 0;
                for (int y = 3; y >= 0; y--) {
                    if (data[x][y] != 0) {
                        temp[ind] = data[x][y];
                        if (ind != 3 - y) flag = true;
                        ind++;
                    }
                }
                for (int i = 0; i < ind; i++) {
                    if (i < 3 && temp[i] == temp[i + 1]) {
                        score += temp[i];
                        temp[i] *= 2;
                        temp[i + 1] = 0;
                        flag = true;
                    }
                }
                for (int y = 0; y < 4; y++) {
                    data[x][y] = 0;
                }
                int k = 3;
                for (int i = 0; i < ind; i++) {
                    if (temp[i] != 0) {
                        data[x][k] = temp[i];
                        k--;
                    }
                }
            }
        } break;
        case 'h' : {
            if (regret_flag == false || regret_cnt == 0) break;
            regret_flag = false;
            regret_cnt--;
            flag = true;
            for (int x = 0; x < 4; x++) {
                for (int y = 0; y < 4; y++) {
                    data[x][y] = pre_data[x][y];
                }
            }        
        } break;
    } 
    return flag;
}

bool Game::is_over() {
    bool flag = true;
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            if (x != 0 && data[x][y] == data[x - 1][y]) flag = false;
            if (x != 3 && data[x][y] == data[x + 1][y]) flag = false;
            if (y != 0 && data[x][y] == data[x][y - 1]) flag = false;
            if (y != 3 && data[x][y] == data[x][y + 1]) flag = false;
        }
    }
    return flag;
}

bool Game::gameover() {
    sleep(1);
    clear();
    mvprintw(up + high / 2 - 4, left + len / 3, "GAME    OVER!");
    mvprintw(up + high / 2 - 2, left + len / 3 - 7, "the score this time is %s!", itoc(score));
    mvprintw(up + high / 2 + 4, left + len / 3 - 4, "whether to play again?");
    mvprintw(up + high / 2 + 6, left + len / 3 - 1, "enter 'y' continue");
    mvprintw(up + high / 2 + 7, left + len / 3 - 1, "enter 'n' exit");
    refresh();
    char op;
    while ((op = getch()) != 'y' && op != 'n');
    if (op == 'y') return false;
    else return true;
}

void Game::exit_game() {
    clear();
    mvprintw(up + high / 2, left + len / 3, "EXIT THE GAME!");
    refresh();
    sleep(2);
    clear();
    endwin();
    return ;
    
}

void Game::record() {
    FILE *fp = fopen("his_max_score.txt", "r");
    char *buff = (char *)calloc(25, 1);
    fread(buff, 1, 23, fp);
    int num = atoi(buff);
    fclose(fp);
    if (score > num) {
        FILE *fq = fopen("his_max_score.txt", "w");
        fprintf(fq, "%d", score);
        fclose(fq);
    }
    free(buff);
    return ;
}

void Game::re_play() {
    score = 0;
    regret_cnt = 3;
    regret_flag = false;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            data[i][j] = 0;
            em_ind.clear();
            em_ind.push_back(pair<int, int> (i, j));
        }
    }
    clear();
    mvprintw(up + high / 2, left + len / 3 - 3, "PLAY THE GAME AGAIN!");
    refresh();
    sleep(2);
    return ;
}
