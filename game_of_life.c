#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define HEIGHT 25
#define WIDTH 80

void input_field(int field[HEIGHT][WIDTH]);
void display_field(int field[HEIGHT][WIDTH]);
void check_input(int *speed, int *flag, char c);
int count_live_neighbours(int field[HEIGHT][WIDTH], int y, int x);
int make_decision(int field[HEIGHT][WIDTH], int y, int x);
int make_decision(int field[HEIGHT][WIDTH], int y, int x);
void next_gen(int field[HEIGHT][WIDTH]);
void clear_input_buffer();

void input_field(int field[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; ++i)
        for (int j = 0; j < WIDTH; ++j) {
            scanf("%d", &field[i][j]);
        }
}

void display_field(int field[HEIGHT][WIDTH]) {
    printw("+");
    for (int i = 0; i < WIDTH; ++i) printw("-");
    printw("+");
    printw("\n");
    for (int i = 0; i < HEIGHT; i++) {
        printw("|");
        for (int j = 0; j < WIDTH; j++) {
            if (field[i][j] == 1) {
                printw("*");
            } else {
                printw(" ");
            }
        }
        printw("|");
        printw("\n");
    }
    printw("+");
    for (int i = 0; i < WIDTH; ++i) printw("-");
    printw("+");
}

void check_input(int *speed, int *flag, char c) {
    if (c == 'q')
        *flag = 0;
    else if (c == '1')
        *speed = 2;
    else if (c == '2')
        *speed = 7;
    else if (c == '3')
        *speed = 12;
}

int count_live_neighbours(int field[HEIGHT][WIDTH], int y, int x) {
    int res = 0;
    int next_y = (y + 1) % HEIGHT;
    int next_x = (x + 1) % WIDTH;
    int prev_y = (y - 1 + HEIGHT) % HEIGHT;
    int prev_x = (x - 1 + WIDTH) % WIDTH;

    res += field[prev_y][prev_x];
    res += field[prev_y][x];
    res += field[prev_y][next_x];
    res += field[y][prev_x];
    res += field[y][next_x];
    res += field[next_y][prev_x];
    res += field[next_y][x];
    res += field[next_y][next_x];

    return res;
}

int make_decision(int field[HEIGHT][WIDTH], int y, int x) {
    int ans = field[y][x];
    int count = count_live_neighbours(field, y, x);
    if (field[y][x] == 0 && count == 3) ans = 1;
    if (field[y][x] == 1 && (count < 2 || count > 3)) ans = 0;
    return ans;
}

void next_gen(int field[HEIGHT][WIDTH]) {
    int new_field[HEIGHT][WIDTH];

    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            new_field[i][j] = make_decision(field, i, j);
        }
    }
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            field[i][j] = new_field[i][j];
        }
    }
}

void clear_input_buffer() { while (getch() != ERR); }

int main() {
    int field[HEIGHT][WIDTH];
    int speed = 2;
    int flag = 1;

    input_field(field);

    if (freopen("/dev/tty", "r", stdin)) {
        initscr();
        nodelay(stdscr, true);
        cbreak();
        noecho();
        curs_set(0);
    }

    while (flag) {
        char c = getch();
        check_input(&speed, &flag, c);
        clear_input_buffer();
        clear();
        display_field(field);
        refresh();
        next_gen(field);
        usleep(1000000 / speed);
    }
    endwin();
    return 0;
}