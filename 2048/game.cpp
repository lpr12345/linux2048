/*************************************************************************
	> File Name: game.cpp
	> Author: 
	> Mail: 
	> Created Time: 2020年04月30日 星期四 14时43分30秒
 ************************************************************************/

#include "include.h"

int main() {
    signal(SIGINT, SIG_IGN);
    Game game(10, 25, 17, 29);
    while (1) {
        game.mk_map();
        game.show();
        game.rand_ind();
        game.re_empty();
        game.rand_ind();
        game.re_empty();
        game.show();
        char op;
        while ((op = getch()) != 'q' && op != 'r') {
            if (!game.operate(op)) continue;
            game.re_empty();
            if (op != 'h') game.rand_ind();
            game.show();
            if (game.empty()) {
                if (game.is_over()) {
                    if (!game.gameover()) {
                        game.record();
                        game.re_play();
                        break;
                    } else {
                        game.record();
                        game.exit_game();
                        return 0;
                    }
                }
            }
        }
        if (op == 'r') {
            game.record();
            game.re_play();
            continue;
        } else if (op == 'q') {
            game.record();
            game.exit_game();
            break;
        }
    }
    return 0;
}
