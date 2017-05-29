/*	$Id: checkers.h,v 0.1 2009/02/26 15:41:04 cberardi Exp $	*/ 

/*
 * Copyright (c) 2009 Christopher J. Berardi
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _CHECKERS_H_
#define _CHECKERS_H_

/* allows to use ints to emulate bool */
#define true 1
#define false 0


/* 
 * these values define where the (x,y) coordinates of the game board start
 * and the offsets from one squares (x,y) to anothers
 */
#define X_BASE 2
#define Y_BASE 1
#define X_OFFSET 4
#define Y_OFFSET 2


/* struct for the game pieces */
struct piece {
	int player;
	int alive;
	int king;
	char symbol;
};


/* struct for the game board squares */
struct square {
	int valid_square;
	int occupied;
	struct piece occupier;
};


/* struct for generating a move list */
struct move {
	int old_row; 
	int old_col;
	int new_row;
	int new_col;
	int move;
	int jump;
	int value;
};


/* function prototypes */
void draw(WINDOW*, struct square (*gameboard)[]);
void populate_squares(struct square(*gameboard)[]);
void validate_squares(struct square(*gameboard)[]);
void move_piece(struct square (*gameboard)[], int, int, int, int);
void jump_piece(struct square (*gameboard)[], int, int, int, int);
void king_me(struct square (*gameboard)[], int, int, int);
int validate_input(char, char, char, char);
int occupy_square(struct square(*gameboard)[], int, int, int);
int square_occupied(struct square(*gameboard)[], int, int);
int dark_square(int, int);
int pieces_left(struct square (*gameboard)[], int);
int valid_direction(struct square (*gameboard)[], int, int, int, int, int);
int can_jump(struct square (*gameboard)[], int, int, int, int, int);
int move_single_space(struct square (*gameboard)[], int, int, int);
int jump_available(struct square (*gameboard)[], int, int, int, int, int, int);
int game_ended(struct square (*gameboard)[], int);

/* These are the AI functions */
int negamax(struct square (*gameboard)[], int, int, int, int, int);
void generate_moves(struct square (*gameboard)[], struct move movelist[], int);
int moves_left(struct move movelist[], int);
void do_move(struct square (*gameboard)[], struct move, int);
int evaluate(struct square (*gameboard)[], int);
void aiturn(struct square (*gameboard)[], int, int);
struct move aimax(struct square (*gameboard)[], int, int);


#endif /* !_CHECKERS_H_ */
