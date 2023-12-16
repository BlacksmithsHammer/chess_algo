import random
import copy

desk = [["--" for _ in range(8)] for _ in range(8)]

desk[0] = ["er", "ek", "eb", "--", "eg", "eb", "ek", "er"]
desk[1] = ["ep" for _ in range(8)]
desk[6] = ["mp" for _ in range(8)]
desk[7] = ["mr", "mk", "mb", "--", "mg", "mb", "mk", "mr"]

prices = {
    "--": 0,
    "ep": 1,
    "ek": 3,
    "eb": 4,
    "er": 5,
    "eg": 1000
}


# "--" - первый символ - союзник (m) или враг (e), второй - фигура:
# p - pawn - пешка, k - knight - конь, b - bishop - слон, r - rook - ладья, g - король, -- - пустая клетка
# сверху враг, снизу мы

def show_desk(d):
    for e in range(len(d)):
        print(*d[e], e)
    for i in range(len(d)):
        print(i, end="  ")
    print('\n=========================')


def rotate_180(d):
    d = [line[::-1] for line in reversed(d)]
    for i in range(8):
        for j in range(8):
            if d[i][j][0] == 'e':
                d[i][j] = 'm' + d[i][j][1]
            elif d[i][j][0] == 'm':
                d[i][j] = 'e' + d[i][j][1]
    return d


def in_border(coord):
    if 0 <= coord <= 7:
        return True


def where_can_move(d, x, y):
    res = []
    if d[x][y] == "mp":
        if in_border(x - 1) and d[x - 1][y] == "--":
            res.append([x - 1, y])
        if in_border(y + 1) and in_border(x - 1) and d[x - 1][y + 1][0] == "e":
            res.append([x - 1, y + 1])
        if in_border(y - 1) and in_border(x - 1) and d[x - 1][y - 1][0] == "e":
            res.append([x - 1, y - 1])

    if d[x][y] == "mk":
        for mk_x in [-2, -1, 1, 2]:
            for mk_y in [-2, -1, 1, 2]:
                if abs(mk_x) != abs(mk_y) and in_border(x + mk_x) and in_border(y + mk_y) and \
                        (d[x + mk_x][y + mk_y][0] == "e" or d[x + mk_x][y + mk_y][0] == "-"):
                    res.append([x + mk_x, y + mk_y])

    def moves_on_line(res, add_x, add_y):
        r_x = x + add_x
        r_y = y + add_y
        while in_border(r_x) and in_border(r_y) and d[r_x][r_y] == "--":
            res.append([r_x, r_y])
            r_x += add_x
            r_y += add_y
        if in_border(r_x) and in_border(r_y) and d[r_x][r_y][0] == "e":
            res.append([r_x, r_y])

    if d[x][y] == "mr":
        moves_on_line(res, 1, 0)
        moves_on_line(res, -1, 0)
        moves_on_line(res, 0, 1)
        moves_on_line(res, 0, -1)

    if d[x][y] == "mb":
        moves_on_line(res, 1, 1)
        moves_on_line(res, 1, -1)
        moves_on_line(res, -1, 1)
        moves_on_line(res, -1, -1)

    if d[x][y] == "mg":
        for add_x in range(-1, 2):
            for add_y in range(-1, 2):
                if abs(add_x) + abs(add_y) > 0 and in_border(x + add_x) and in_border(y + add_y) and (
                        d[x + add_x][y + add_y][0] == "-" or d[x + add_x][y + add_y][0] == "e"):
                    res.append([x + add_x, y + add_y])
    return res


def generate_desk(d, long):
    def _xy():
        return [random.randint(0, 7), random.randint(0, 7)]

    for i in range(long):
        cell = _xy()
        while (len(where_can_move(d, cell[0], cell[1]))) == 0: cell = _xy()

        choosen_move = where_can_move(d, cell[0], cell[1])[
            random.randint(0, len(where_can_move(d, cell[0], cell[1])) - 1)]
        d[choosen_move[0]][choosen_move[1]] = d[cell[0]][cell[1]]
        d[cell[0]][cell[1]] = "--"
    return d.copy()


for i in range(30):
    generate_desk(desk, 1)
    desk = rotate_180(desk)

show_desk(desk)


def get_my_positions(d):
    tmp_arr = []
    for i in range(8):
        for j in range(8):
            if d[i][j][0] == "m":
                tmp_arr.append([i, j])

    return tmp_arr


def brute_moves(desk, myPositions):
    desks = []

    for e_1 in myPositions:
        for mv_1 in where_can_move(desk, e_1[0], e_1[1]):
            desk_1 = copy.deepcopy(desk)
            desk_1[mv_1[0]][mv_1[1]] = desk_1[e_1[0]][e_1[1]]
            desk_1[e_1[0]][e_1[1]] = "--"
            desks.append(desk_1)
    return desks


def calc_score(deskstart, deskstop):
    score = 0

    for i in range(8):
        for j in range(8):
            if deskstart[i][j][0] == "e" and deskstop[i][j][0] != "e":
                score += prices[deskstart[i][j]]
    return score


res1 = []
res2 = []
res3 = []

for tmp in brute_moves(desk, get_my_positions(desk)):
    res1.append(tmp)

for desk_1 in res1:
    for tmp in brute_moves(desk_1, get_my_positions(desk_1)):
        res2.append(tmp)

for desk_2 in res2:
    for tmp in brute_moves(desk_2, get_my_positions(desk_2)):
        res3.append(tmp)

score1 = [calc_score(desk, d) for d in res1]
score2 = [calc_score(desk, d) for d in res2]
score3 = [calc_score(desk, d) for d in res3]


def answ(mx, desks):
    for e in desks:
        if calc_score(desk, e) == mx:

            for i in range(8):
                for j in range(8):
                    if desk[i][j][0] == "e" and e[i][j][0] == "-":
                        e[i][j] = "☒☒"
                        print("Got ", i, j, "; ", end=" ")
                    if desk[i][j][0] == "e" and e[i][j][0] == "m":
                        e[i][j] = "☑☑"
                        print("Got ", i, j, "; ", end=" ")
                    if desk[i][j][0] == "m" and e[i][j][0] == "-":
                        e[i][j] = "☐☐"
            print()
            show_desk(e)


if max(score1) >= 1000:
    print("================================================\n===================best - 1 moves===============\n================================================")
    answ(max(score1), res1)
elif max(score2) >= 1000:
    print("================================================\n===================best - 2 moves===============\n================================================")
    answ(max(score2), res2)
else:
    print("================================================\n===================best - 3 moves===============\n================================================")
    answ(max(score3), res3)
