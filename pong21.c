#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25
#define WIN_SCORE 21
#define RACKET_SIZE 3
#define BALL_INIT_X 40
#define BALL_INIT_Y 12
#define RACKET_INIT 11

void render_score(int score1, int score2) {
    int row;
    for (row = 0; row < HEIGHT; row++) {
        putchar('\n');
    }
    printf("%45s", "SCORE:\n");
    printf("%20sPlayer 1: %d | Player 2: %d\n", "", score1, score2);
}
void render_row(int row, int ball_x, int ball_y, int racket1, int racket2) {
    int col;
    for (col = 0; col < WIDTH; col++) {
        if (row == 0 || row == HEIGHT - 1) {
            putchar('-');
        } else if (col == 0 || col == WIDTH - 1) {
            putchar('|');
        } else if (col == ball_x && row == ball_y) {
            putchar('0');
        } else if (col == WIDTH / 2) {
            putchar('|');
        } else if (col == 1 && row >= racket1 && row < racket1 + RACKET_SIZE) {
            putchar('|');
        } else if (col == WIDTH - 2 && row >= racket2 && row < racket2 + RACKET_SIZE) {
            putchar('|');
        } else {
            putchar(' ');
        }
    }
    putchar('\n');
}
void render_field(int score1, int score2, int ball_x, int ball_y, int racket1, int racket2) {
    int row;
    render_score(score1, score2);
    for (row = 0; row < HEIGHT; row++) {
        render_row(row, ball_x, ball_y, racket1, racket2);
    }
    printf(
        " Player 1: A - up, Z - down"
        " | Player 2: K - up, M - down\n");
    printf("%46s", "[ENTER]:\n");
}
char read_input(void) {
    char ch, extra;
    char result;
    if (scanf("%c", &ch) != 1) {
        printf("Invalid input\n");
        result = '\0';
    } else if (ch == '\n') {
        result = ' ';
    } else if (scanf("%c", &extra) != 1) {
        result = ch;
    } else if (extra != '\n') {
        while (extra != '\n' && scanf("%c", &extra) == 1) {
        }
        printf("Invalid input\n");
        result = '\0';
    } else {
        result = ch;
    }
    return result;
}
int is_valid_cmd(char cmd) {
    int valid;
    if (cmd == 'a' || cmd == 'A' || cmd == 'z' || cmd == 'Z' || cmd == 'k' || cmd == 'K' || cmd == 'm' ||
        cmd == 'M' || cmd == ' ') {
        valid = 1;
    } else {
        valid = 0;
    }
    return valid;
}
int move_racket1(int racket1, char cmd) {
    if (cmd == 'a' || cmd == 'A') {
        if (racket1 > 1) {
            racket1 -= 1;
        }
    } else if (cmd == 'z' || cmd == 'Z') {
        if (racket1 + RACKET_SIZE < HEIGHT - 1) {
            racket1 += 1;
        }
    }
    return racket1;
}
int move_racket2(int racket2, char cmd) {
    if (cmd == 'k' || cmd == 'K') {
        if (racket2 > 1) {
            racket2 -= 1;
        }
    } else if (cmd == 'm' || cmd == 'M') {
        if (racket2 + RACKET_SIZE < HEIGHT - 1) {
            racket2 += 1;
        }
    }
    return racket2;
}

int move_ball_y(int ball_y, int dy) {
    int new_y = ball_y + dy;
    int new_dy = dy;
    if (new_y >= HEIGHT - 2) {
        new_y = HEIGHT - 2;
        new_dy = -1;
    }
    if (new_y <= 1) {
        new_y = 1;
        new_dy = 1;
    }
    return new_y * 10 + (new_dy == 1 ? 1 : 2);
}

int move_ball_x(int ball_x, int ball_y, int dx, int racket1, int racket2, int score1, int score2) {
    int new_dx = dx;
    int new_bx = ball_x + dx;
    int new_s1 = score1;
    int new_s2 = score2;

    if (new_bx <= 1) {
        if (ball_y >= racket1 && ball_y < racket1 + RACKET_SIZE) {
            new_bx = 1;
            new_dx = 1;
        } else {
            new_s2 = score2 + 1;
            new_bx = BALL_INIT_X;
            new_dx = 1;
        }
    }

    if (new_bx >= WIDTH - 2) {
        if (ball_y >= racket2 && ball_y < racket2 + RACKET_SIZE) {
            new_bx = WIDTH - 2;
            new_dx = -1;
        } else {
            new_s1 = score1 + 1;
            new_bx = BALL_INIT_X;
            new_dx = -1;
        }
    }
    return ((new_s1 * 100 + new_s2) * 100 + new_bx) * 10 + (new_dx == 1 ? 1 : 2);
}
void print_result(int score1, int score2) {
    if (score1 >= WIN_SCORE) {
        printf("\n*** Player 1 wins %d:%d! Congratulations! ***\n", score1, score2);
    } else {
        printf("\n*** Player 2 wins %d:%d! Congratulations! ***\n", score2, score1);
    }
}
int main(void) {
    int score1 = 0, score2 = 0;
    int ball_x = BALL_INIT_X, ball_y = BALL_INIT_Y;
    int dx = 1, dy = 1;
    int racket1 = RACKET_INIT, racket2 = RACKET_INIT;

    while (score1 < WIN_SCORE && score2 < WIN_SCORE) {
        int prev_s1 = score1;
        int prev_s2 = score2;

        render_field(score1, score2, ball_x, ball_y, racket1, racket2);
        {
            char cmd = '\0';
            while (!is_valid_cmd(cmd)) {
                cmd = read_input();
                if (cmd != '\0' && !is_valid_cmd(cmd)) {
                    printf("Invalid input\n");
                }
            }
            racket1 = move_racket1(racket1, cmd);
            racket2 = move_racket2(racket2, cmd);
        }
        {
            int packed = move_ball_y(ball_y, dy);
            ball_y = packed / 10;
            dy = (packed % 10 == 1) ? 1 : -1;
        }
        {
            int packed = move_ball_x(ball_x, ball_y, dx, racket1, racket2, score1, score2);
            dx = (packed % 10 == 1) ? 1 : -1;
            ball_x = (packed / 10) % 100;
            score2 = (packed / 1000) % 100;
            score1 = (packed / 100000) % 100;
        }

        if (score1 != prev_s1 || score2 != prev_s2) {
            ball_x = BALL_INIT_X;
            ball_y = BALL_INIT_Y;
            dy = 1;
        }
    }
    print_result(score1, score2);
    return 0;
}
