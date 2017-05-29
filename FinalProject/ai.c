/*	$Id: ai.c,v 0.1 2009/02/26 15:41:04 cberardi Exp $	*/ 

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

#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include "checkers.h"

#include <stdio.h>


/*
 * NEGAMAX
 * This function is the heart of the AI for this checkers game. It uses the
 * Negamax algorithm (big shocker, huh?) to recursively build a game tree
 * that will be evaluated and return the best move.
 */
int
negamax(struct square (*gameboard)[8], int player, int maxdepth, int depth,
    int alpha, int beta) 
{
	int bestvalue = -1000;
	int value = -1000;
	int element = 0;
	int row, col;
	int localalpha = alpha;

	if (game_ended(gameboard, player) || depth == maxdepth) 
		return evaluate(gameboard, player);

	struct move movelist[100];
	struct square newboard[8][8];

	generate_moves(gameboard, movelist, player);

	while(moves_left(movelist, element)) {
		for (row = 0; row < 8; row++) {
			for (col = 0; col < 8; col++) {
				newboard[row][col] = gameboard[row][col];
			}
		}
		do_move(newboard, movelist[element], player);
		
		/* recurse negamax */
		value = -negamax(newboard, 3 - player, 5,  depth + 1, 
				-beta, -localalpha);

		if (value > bestvalue) {
			bestvalue = value;
		}
		if (bestvalue >= beta) {
			break;
		}
		if (bestvalue > localalpha) {
			localalpha = bestvalue;
		}
		element++;
	}
	return bestvalue;
}


/*
 * AIMAX
 * This is a helper function for the negamax function that will serve to 
 * get the proper moves only at the top of the tree.
 */
struct move
aimax(struct square (*gameboard)[8], int player, int maxdepth)
{
	int bestvalue = -1000;
	int element = 0;
	int row, col;
	struct move bestmove;

	if (game_ended(gameboard, player)) 
		return bestmove;

	struct move movelist[100];
	struct square newboard[8][8];

	generate_moves(gameboard, movelist, player);

	while(moves_left(movelist, element)) {
		for (row = 0; row < 8; row++) {
			for (col = 0; col < 8; col++) {
				newboard[row][col] = gameboard[row][col];
			}
		}

		do_move(newboard, movelist[element], player);
		movelist[element].value = -negamax(newboard, 3 - player, 
						maxdepth, 0, -1000, 1000);
		element++;
	}

	element = 0;
	srand(time(NULL));
	while(moves_left(movelist, element)) {
		if (movelist[element].value == bestvalue) { 
			/* randomize the outcome of equal values */ 
			/* jumps should be preferred over moves */
			row = rand();
			col = rand();
			if (row > col && 
			    (!(movelist[element].jump && !bestmove.jump) ||
			    !(!movelist[element].jump && bestmove.jump))) {
				row = rand();
				col = rand();
				if (row > col) {
					bestvalue = bestvalue;
				} else {
					bestmove = movelist[element];
					bestvalue = movelist[element].value;
				}
			} else if (movelist[element].jump && !bestmove.jump) {  
				bestmove = movelist[element];
				bestvalue = movelist[element].value;
			} 
		} else if (movelist[element].value > bestvalue) {
			bestmove = movelist[element];
			bestvalue = movelist[element].value;
		}
		element++;
	}
	return bestmove;
}


/*
 * GENERATE A MOVE LIST
 * This function will generate and populate an array of all possible moves
 * on a given board for a particular player.
 */
void
generate_moves(struct square (*gameboard)[8], struct move movelist[], 
	int player)
{
	int row, col;
	int element = 0;

	for (row = 0; row < 8; row++) {
		for (col = 0; col < 8; col++) {
			if (gameboard[row][col].valid_square && 
			    gameboard[row][col].occupied &&
			    gameboard[row][col].occupier.player == player &&
			    move_single_space(gameboard, player, row, col)) { 
				if ((!gameboard[row - 1][col - 1].occupied 
				    && !((row - 1) < 0) && !((col - 1) < 0)) 
				    && valid_direction(gameboard, row, col, 
				    (row - 1), (col - 1), player)) {
					movelist[element].old_row = row;
					movelist[element].old_col = col;
					movelist[element].new_row = row - 1;
					movelist[element].new_col = col - 1;
					movelist[element].move = true;
					movelist[element].jump = false;
					element++;
				}

				if ((!gameboard[row - 1][col + 1].occupied 
				    && !((row - 1) < 0) && !((col + 1) > 7))
				    && valid_direction(gameboard, row, col, 
				    (row - 1), (col + 1), player)) {
					movelist[element].old_row = row;
					movelist[element].old_col = col;
					movelist[element].new_row = row - 1;
					movelist[element].new_col = col + 1;
					movelist[element].move = true;
					movelist[element].jump = false;
					element++;
				}

				if ((!gameboard[row + 1][col - 1].occupied 
				    && !((row + 1) > 7) && !((col - 1) < 0))
				    && valid_direction(gameboard, row, col, 
				    (row + 1), (col - 1), player)) {
					movelist[element].old_row = row;
					movelist[element].old_col = col;
					movelist[element].new_row = row + 1;
					movelist[element].new_col = col - 1;
					movelist[element].move = true;
					movelist[element].jump = false;
					element++;
				}

				if ((!gameboard[row + 1][col + 1].occupied
				    && !((row + 1) > 7) && !((col + 1) > 7))
			 	    && valid_direction(gameboard, row, col,
				    (row + 1), (col + 1), player)) {
					movelist[element].old_row = row;
					movelist[element].old_col = col;
					movelist[element].new_row = row + 1;
					movelist[element].new_col = col + 1;
					movelist[element].move = true;
					movelist[element].jump = false;
					element++;
				}
			}

			if (gameboard[row][col].valid_square && 
			    gameboard[row][col].occupied &&
			    gameboard[row][col].occupier.player == player &&
			    jump_available(gameboard, player, row, col, 0,0,0)){
				if ((!gameboard[row - 2][col - 2].occupied 
				    && !((row - 2) < 0) && !((col - 2) < 0)) 
				    && valid_direction(gameboard, row, col, 
				    (row - 2), (col - 2), player) 
				    && can_jump(gameboard, row, col,
				    (row - 2), (col - 2), player)) {
					movelist[element].old_row = row;
					movelist[element].old_col = col;
					movelist[element].new_row = row - 2;
					movelist[element].new_col = col - 2;
					movelist[element].move = false;
					movelist[element].jump = true;
					element++;
				}

				if ((!gameboard[row - 2][col + 2].occupied 
				    && !((row - 2) < 0) && !((col + 2) > 7)) 
				    && valid_direction(gameboard, row, col, 
				    (row - 2), (col + 2), player) 
				    && can_jump(gameboard, row, col,
				    (row - 2), (col + 2), player)) {
					movelist[element].old_row = row;
					movelist[element].old_col = col;
					movelist[element].new_row = row - 2;
					movelist[element].new_col = col + 2;
					movelist[element].move = false;
					movelist[element].jump = true;
					element++;
				}
	
				if ((!gameboard[row + 2][col - 2].occupied 
				    && !((row + 2) > 7) && !((col - 2) < 0)) 
				    && valid_direction(gameboard, row, col, 
				    (row + 2), (col - 2), player) 
				    && can_jump(gameboard, row, col,
				    (row + 2), (col - 2), player)) {
					movelist[element].old_row = row;
					movelist[element].old_col = col;
					movelist[element].new_row = row + 2;
					movelist[element].new_col = col - 2;
					movelist[element].move = false;
					movelist[element].jump = true;
					element++;
				}

				if ((!gameboard[row + 2][col + 2].occupied
				    && !((row + 2) > 7) && !((col + 2) > 7))
				    && valid_direction(gameboard, row, col, 
				    (row + 2), (col + 2), player) 
				    && can_jump(gameboard, row, col,
				    (row + 2), (col + 2), player)) {
					movelist[element].old_row = row;
					movelist[element].old_col = col;
					movelist[element].new_row = row + 2;
					movelist[element].new_col = col + 2;
					movelist[element].move = false;
					movelist[element].jump = true;
					element++;
				}
			}
		}
	}
	movelist[element].old_row = -1;
}


/*
 * ANY MOVES LEFT
 * This function checks to see if there are any moves left in the movelist 
 * array. 
 */
int
moves_left(struct move movelist[], int element)
{
	if (movelist[element].old_row == -1)
		return false;
	else
		return true;
}


/*
 * MAKE MOVE
 * This function makes the move being analyzed.
 */
void
do_move(struct square (*gameboard)[8], struct move moves, int player)
{
	if (moves.move) {
		move_piece(gameboard, moves.old_row, moves.old_col,
		    moves.new_row, moves.new_col);
	} else if (moves.jump) {
		jump_piece(gameboard, moves.old_row, moves.old_col,
		    moves.new_row, moves.new_col);
	}
	/* if the piece has reached the other end, king it */
	if ((player == 1 && moves.new_row == 0) || 
	    (player == 2 && moves.new_row == 7)) {
		king_me(gameboard, moves.new_row, moves.new_col, player);
	}
}


/*
 * EVALUATE BOARD
 * This function uses a heuristic to evaluate a board. It checks to see how
 * many pieces each side has and subtracts the two. The higer the value,
 * the better the board.
 */
int
evaluate(struct square (*gameboard)[8], int player)
{
	return (pieces_left(gameboard, player) 
	    - pieces_left(gameboard, (3 - player)));
}


/*
 * AI'S TURN TO MOVE
 * This function aids the main game to make the ai's turn. 
 */
void
aiturn(struct square (*gameboard)[8], int player, int depth)
{
	int old_row, old_col, new_row, new_col;
	char spreco;
	struct move aimove;

	aimove = aimax(gameboard, player, depth);

	/* if the move is a single space, move it */
	if (aimove.move) {
		move_piece(gameboard, aimove.old_row, aimove.old_col, 
		    aimove.new_row, aimove.new_col);
	}

	/* if the move is a jump, jump the piece */
	if (aimove.jump) {
		jump_piece(gameboard, aimove.old_row, aimove.old_col, 
		    aimove.new_row, aimove.new_col);
	}

	/* if the piece has reached the other end, king it */
	if ((player == 1 && aimove.new_row == 0) || 
	    (player == 2 && aimove.new_row == 7)) {
		king_me(gameboard, aimove.new_row, aimove.new_col, player);
	}

	/* Let the user know what the move is */
	wmove(stdscr, 20, 3);
	deleteln();
	printw("Player %d has moved: %d%d -> %d%d", player, 
	    aimove.old_row, aimove.old_col, aimove.new_row, aimove.new_col);
	spreco = getch();

	/* allow a way to quit even when it is computer vs. computer */
	if (tolower(spreco) == 'q') {
		wmove(stdscr, 21, 3);
		printw("Do you really want to QUIT? ");
		spreco = getch();
		if (tolower(spreco) == 'y') {
			endwin();
			system("clear");
			exit(0);
		} 
	}

	player = (player % 2) + 1;

	/* has the game been won? */
	if (game_ended(gameboard, player)) {
		wmove(stdscr, 21, 3);
		deleteln();
		printw("PLAYER %d HAS WON THE GAME!", (player % 2) + 1);
		spreco = getch();
		endwin();
		system("clear");
		exit(0);
	}
}
