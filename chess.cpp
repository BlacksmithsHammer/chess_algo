#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <map>
#include <ctime>
#include <windows.h>

std::vector<std::vector<std::string>> desk(8, std::vector<std::string>(8, "--"));


std::map<std::string, int> prices = {
    {"--", 0},
    {"ep", 1},
    {"ek", 3},
    {"eb", 4},
    {"er", 5},
    {"eg", 1000}
};

void show_desk(std::vector<std::vector<std::string>>& d) {
    for (int e = 0; e < d.size(); e++) {
        for (int i = 0; i < d[e].size(); i++) {
            std::cout << d[e][i] << " ";
        }
        std::cout << e << std::endl;
    }
    for (int i = 0; i < d.size(); i++) {
        std::cout << i << "  ";
    }
    std::cout << std::endl << "=========================" << std::endl;
}

std::vector<std::vector<std::string>> rotate_180(std::vector<std::vector<std::string>>& d) {
    std::reverse(d.begin(), d.end());
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (d[i][j][0] == 'e') {
                d[i][j] = "m" + d[i][j].substr(1);
            }
            else if (d[i][j][0] == 'm') {
                d[i][j] = "e" + d[i][j].substr(1);
            }
        }
    }
    return d;
}

bool in_border(int coord) {
    return (coord >= 0 && coord <= 7);
}

std::vector<std::vector<int>> get_possible_moves(std::vector<std::vector<std::string>>& d, int x, int y) {
    std::vector<std::vector<int>> res;
    if (d[x][y] == "mp") {
        if (in_border(x - 1) && d[x - 1][y] == "--") {
            res.push_back({x - 1, y});
        }
        if (in_border(y + 1) && in_border(x - 1) && d[x - 1][y + 1][0] == 'e') {
            res.push_back({x - 1, y + 1});
        }
        if (in_border(y - 1) && in_border(x - 1) && d[x - 1][y - 1][0] == 'e') {
            res.push_back({x - 1, y - 1});
        }
    }
    if (d[x][y] == "mk") {
        for (int mk_x : {-2, -1, 1, 2}) {
            for (int mk_y : {-2, -1, 1, 2}) {
                if (abs(mk_x) != abs(mk_y) && in_border(x + mk_x) && in_border(y + mk_y) &&
                    (d[x + mk_x][y + mk_y][0] == 'e' || d[x + mk_x][y + mk_y][0] == '-')) {
                    res.push_back({x + mk_x, y + mk_y});
                }
            }
        }
    }
    auto moves_on_line = [&](std::vector<std::vector<int>>& res, int add_x, int add_y) {
        int r_x = x + add_x;
        int r_y = y + add_y;
        while (in_border(r_x) && in_border(r_y) && d[r_x][r_y] == "--") {
            res.push_back({r_x, r_y});
            r_x += add_x;
            r_y += add_y;
        }
        if (in_border(r_x) && in_border(r_y) && d[r_x][r_y][0] == 'e') {
            res.push_back({r_x, r_y});
        }
    };
    if (d[x][y] == "mr") {
        moves_on_line(res, 1, 0);
        moves_on_line(res, -1, 0);
        moves_on_line(res, 0, 1);
        moves_on_line(res, 0, -1);
    }
    if (d[x][y] == "mb") {
        moves_on_line(res, 1, 1);
        moves_on_line(res, 1, -1);
        moves_on_line(res, -1, 1);
        moves_on_line(res, -1, -1);
    }
    if (d[x][y] == "mg") {
        for (int add_x = -1; add_x <= 1; add_x++) {
            for (int add_y = -1; add_y <= 1; add_y++) {
                if (abs(add_x) + abs(add_y) > 0 && in_border(x + add_x) && in_border(y + add_y) &&
                    (d[x + add_x][y + add_y][0] == '-' || d[x + add_x][y + add_y][0] == 'e')) {
                    res.push_back({x + add_x, y + add_y});
                }
            }
        }
    }
    return res;
}

std::vector<std::vector<std::string>> generate_desk(std::vector<std::vector<std::string>>& d, int num_moves) {
    auto _xy = [&]() {
        return std::vector<int>{std::rand() % 8, std::rand() % 8};
    };
    for (int i = 0; i < num_moves; i++) {
        std::vector<int> cell = _xy();
        while (get_possible_moves(d, cell[0], cell[1]).size() == 0) {

            cell = _xy();
        }
        std::vector<int> choosen_move = get_possible_moves(d, cell[0], cell[1])[std::rand() % get_possible_moves(d, cell[0], cell[1]).size()];
        d[choosen_move[0]][choosen_move[1]] = d[cell[0]][cell[1]];
        d[cell[0]][cell[1]] = "--";
    }
    return d;
}

std::vector<std::vector<int>> get_my_positions(std::vector<std::vector<std::string>>& d) {
    std::vector<std::vector<int>> tmp_arr;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (d[i][j][0] == 'm') {
                tmp_arr.push_back({i, j});
            }
        }
    }
    return tmp_arr;
}

std::vector<std::vector<std::vector<std::string>>> brute_moves(std::vector<std::vector<std::string>>& desk, std::vector<std::vector<int>>& myPositions) {
    std::vector<std::vector<std::vector<std::string>>> desks;
    for (auto e_1 : myPositions) {
        for (auto mv_1 : get_possible_moves(desk, e_1[0], e_1[1])) {
            std::vector<std::vector<std::string>> desk_1 = desk;
            desk_1[mv_1[0]][mv_1[1]] = desk_1[e_1[0]][e_1[1]];
            desk_1[e_1[0]][e_1[1]] = "--";
            desks.push_back(desk_1);
        }
    }
    return desks;
}

int calc_score(std::vector<std::vector<std::string>>& deskstart, std::vector<std::vector<std::string>>& deskstop) {
    int score = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (deskstart[i][j][0] == 'e' && deskstop[i][j][0] != 'e') {
                score += prices[deskstart[i][j]];
            }
        }
    }
    return score;
}

void answ(int mx, std::vector<std::vector<std::string>>& desk, std::vector<std::vector<std::vector<std::string>>>& desks) {
    for (auto e: desks){
        if (calc_score(desk, e) == mx){
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    if (desk[i][j][0] == 'e' && e[i][j][0] == '-') {
                        e[i][j] = "☒☒";
                    }
                    if (desk[i][j][0] == 'e' && e[i][j][0] == 'm') {
                        e[i][j] = "☑☑";
                    }
                    if (desk[i][j][0] == 'm' && e[i][j][0] == '-') {
                        e[i][j] = "☐☐";
                    }
                }
            }
            show_desk(e);
        }
    }

}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    desk[0] = {"er", "ek", "eb", "--", "eg", "eb", "ek", "er"};
    desk[1] = {"ep", "ep", "ep", "ep", "ep", "ep", "ep", "ep"};
    desk[6] = {"mp", "mp", "mp", "mp", "mp", "mp", "mp", "mp"};
    desk[7] = {"mr", "mk", "mb", "--", "mg", "mb", "mk", "mr"};

    std::srand(std::time(0));
    for (int i = 0; i < 20; i++) {
        generate_desk(desk, 1);
        rotate_180(desk);
    }
    show_desk(desk);

    std::vector<std::vector<int>> myPositions = get_my_positions(desk);
    std::vector<std::vector<std::vector<std::string>>> res1 = brute_moves(desk, myPositions);
    std::vector<std::vector<std::vector<std::string>>> res2;
    std::vector<std::vector<std::vector<std::string>>> res3;
    for (auto desk_1 : res1) {
        std::vector<std::vector<int>> myPositions_1 = get_my_positions(desk_1);
        std::vector<std::vector<std::vector<std::string>>> tmp = brute_moves(desk_1, myPositions_1);
        res2.insert(res2.end(), tmp.begin(), tmp.end());
    }
    for (auto desk_2 : res2) {
        std::vector<std::vector<int>> myPositions_2 = get_my_positions(desk_2);
        std::vector<std::vector<std::vector<std::string>>> tmp = brute_moves(desk_2, myPositions_2);
        res3.insert(res3.end(), tmp.begin(), tmp.end());
    }
    std::vector<int> score1;
    std::vector<int> score2;
    std::vector<int> score3;
    for (auto d : res1) {
        score1.push_back(calc_score(desk, d));
    }
    for (auto d : res2) {
        score2.push_back(calc_score(desk, d));
    }
    for (auto d : res3) {
        score3.push_back(calc_score(desk, d));
    }
    if (*std::max_element(score1.begin(), score1.end()) >= 1000) {
        std::cout << "================================================\n===================best - 1 moves===============\n================================================" << std::endl;
        answ(*std::max_element(score1.begin(), score1.end()), desk, res1);
    }
    else if (*std::max_element(score2.begin(), score2.end()) >= 1000) {
        std::cout << "================================================\n===================best - 2 moves===============\n================================================" << std::endl;
        answ(*std::max_element(score2.begin(), score2.end()), desk, res2);
    }
    else {
        std::cout << "================================================\n===================best - 3 moves===============\n================================================" << std::endl;
        answ(*std::max_element(score3.begin(), score3.end()), desk, res3);
    }
    return 0;
}
