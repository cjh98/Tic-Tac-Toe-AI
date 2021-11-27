#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

#include "tictactoe.h"

// LSB is top left
uint16_t X_BOARD = 0b000000000;
uint16_t O_BOARD = 0b000000000;

int main()
{
    bool won = false;

    enum Player currentPlayer = x;
    enum Player winner;

    puts("Valid inputs are 1, 2, 4, 8, 16, 32, 64, 128, and 256\n");
    
    puts("1\t2\t4\n");
    puts("8\t16\t32\n");
    puts("64\t128\t256\n");

    // main loop
    while (!won)
    {
        uint16_t move;

        // ensure move is valid
        printf("Current Player: %s\n", currentPlayer == x ? "x" : "o");

        if (currentPlayer == x)
        {
            printf("Enter move: ");
            scanf("%hu", &move);
        }
        else
        {
            move = get_best_move();
            printf("AI's MOVE IS: %hu", move);
        }

        if (is_valid_move(currentPlayer, move))
        {
            make_move(currentPlayer, move);
            display_board();

            won = check_for_winner(&winner);

            // change player
            if (currentPlayer == x)
                currentPlayer = o;
            else if (currentPlayer == o)
                currentPlayer = x;
        }
        else
        {
            puts("Invalid move");
        }
    }

    if (winner == x)
        puts("X is the winner!");
    else if (winner == o)
        puts("O is the winner!");
    else
        puts("Draw!");
}

void display_board()
{
    system("cls"); // clear screen

    uint16_t mask = 1;
    for (uint16_t i = 0; i < 9; i++)
    {
        if (i != 0 && i % 3 == 0)
            puts("\n");

        if (X_BOARD & mask)
            printf("x\t");
        else if (O_BOARD & mask)
            printf("o\t");
        else
            printf("-\t");

        mask <<= 1;
    }

    puts("\n");
}

void make_move(enum Player player, uint16_t move)
{
    if (player == x)
        X_BOARD += move;

    else if (player == o)
        O_BOARD += move;
}

void unmake_move(enum Player player, uint16_t move)
{
    if (player == x)
        X_BOARD -= move;

    else if (player == o)
        O_BOARD -= move;
}

bool check_for_winner(enum Player *winner)
{

    for (int i = 0; i < WINS; i++)
    {
        if ((X_BOARD & WIN_CONDITIONS[i]) == WIN_CONDITIONS[i])
        {
            if (winner)
               *winner = x;
            return true;
        }
    }

    for (int i = 0; i < WINS; i++)
    {
        if ((O_BOARD & WIN_CONDITIONS[i]) == WIN_CONDITIONS[i])
        {
            if (winner)
               *winner = o;
            return true;
        }
    }

    if (get_full_board() == 0b111111111)
    {
        return true;
    }

    if (winner)
        *winner = draw;

    return false;
}

bool is_valid_move(enum Player currentPlayer, uint16_t move)
{
    uint16_t allPieces = get_full_board();

    // ensure move is not zero, is only a power of two less than 512, and is not in a place where there is already an x or o
    if (currentPlayer == x)
        return ((allPieces & move) == 0) && ((move != 0) && ((move & (move - 1)) == 0) && (move < 512));
    else if (currentPlayer == o)
        return ((allPieces & move) == 0) && ((move != 0) && ((move & (move - 1)) == 0) && (move < 512));
    else
        return false;
}

uint16_t get_full_board()
{
    return X_BOARD | O_BOARD;
}

uint16_t get_random_move()
{
    srand(time(NULL));

    uint16_t choices[9] = {1, 2, 4, 8, 16, 32, 64, 128, 256};

    uint16_t move = 0;

    do 
    {
        move = choices[rand() % 9];
    } while (!is_valid_move(o, move));

    return move;
}

uint16_t get_best_move()
{
    uint16_t board = get_full_board();

    int best = INT32_MIN;
    uint16_t move;

    uint16_t mask = 1;
    for (int i = 0; i < 9; i++)
    {
        if (!(board & mask))
        {
            make_move(o, mask);
            int score = minimax(board, 0, false);
            unmake_move(o, mask);

            if (score > best)
            {
                best = score;
                move = mask;
            }
        }

        mask <<= 1;
    }

    return move;
}

int eval_position(uint16_t position)
{

    for (int i = 0; i < WINS; i++)
    {
        if ((X_BOARD & WIN_CONDITIONS[i]) == WIN_CONDITIONS[i])
        {
            return -10;
        }
    }
    for (int i = 0; i < WINS; i++)
    {
        if ((O_BOARD & WIN_CONDITIONS[i]) == WIN_CONDITIONS[i])
        {
            return 10;
        }
    }

    return 0;
}

int minimax(uint16_t position, int depth, bool isMaximizing)
{
    if (check_for_winner(NULL))
    {
        return eval_position(position);
    }

    uint16_t board = get_full_board();

    if (isMaximizing)
    {
        int best = INT32_MIN;
        uint16_t mask = 1;
        for (int i = 0; i < 9; i++)
        {
            // if there is no piece there
            if (!(board & mask)) 
            {
                make_move(o, mask);
                int score = minimax(board, depth + 1, false);
                unmake_move(o, mask);

                best = my_max(score, best);
            }


            mask <<= 1;
        }

        return best;
    }
    else
    {
        int best = INT32_MAX;
        uint16_t mask = 1;
        for (int i = 0; i < 9; i++)
        {
            // if there is no piece there
            if (!(board & mask)) 
            {
                make_move(x, mask);
                int score = minimax(board, depth + 1, true);
                unmake_move(x, mask);

                best = my_min(score, best);
            }


            mask <<= 1;
        }

        return best;
    }
}

int my_max(int a, int b)
{
    return (((a)>(b))?(a):(b));
}

int my_min(int a, int b)
{
    return (((a)<(b))?(a):(b));
}