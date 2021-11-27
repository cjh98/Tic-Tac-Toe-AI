#pragma once

#include <stdint.h>
#include <stdbool.h>

#define WINS 8

enum Player
{
    x = 0,
    o = 1,
    draw = 2
};

const uint16_t WIN_CONDITIONS[] =
    {
        0b000000111, // top row
        0b000111000, // middle row
        0b111000000, // bottom row
        0b001001001, // left column
        0b010010010, // middle column
        0b100100100, // right column
        0b100010001, // top left bottom right diagonal
        0b001010100  // top right bottom left diagonal
    };

void display_board();
void make_move(enum Player, uint16_t);
void unmake_move(enum Player, uint16_t);
bool check_for_winner(enum Player *);
bool is_valid_move(enum Player, uint16_t);
uint16_t get_full_board();
uint16_t get_random_move();
uint16_t get_best_move();

int eval_position(uint16_t);
int minimax(uint16_t, int, bool);

int my_max(int, int);
int my_min(int, int);