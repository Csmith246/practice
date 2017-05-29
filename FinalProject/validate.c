/*	$Id: validate.c,v 0.1 2009/02/26 15:41:04 cberardi Exp $	*/ 

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

#include <ctype.h>
#include <curses.h>
#include <stdlib.h>
#include "checkers.h"

/* 
 * VALIDATE USER INPUT 
 * Not only must the row and column be within the confines 
 * of the game board, but it must also not occur on a dark
 * square, which are not capable of hold a game piece
 */
int 
validate_input(char old_row, char old_col, char new_row, char new_col)
{
	char invalid_entry, toquit;
	int is_valid = true;
	int old_valid = true;
	int new_valid = true;

	/* if the user enters a 'q' for the first row, the game quits */
	if (tolower(old_row) == 'q') {
		wmove(stdscr, 21, 3);
		printw("Do you really want to QUIT? ");
		toquit = getch();
		if (tolower(toquit) == 'y') {
			endwin();
			system("clear");
			exit(0);
		} else 
			return false;
	}

	/* is the current row within the confines of the game board? */
	if (!isalpha(old_row) || (tolower(old_row) - 'a') >= 8) {
		wmove(stdscr, 21, 3);
		deleteln();
		printw("Current ROW is invalid");
		invalid_entry = getch();
		is_valid = false;
		old_valid =false;
	}
	
	/* is the current column within the confines of the game board? */
	if (!isdigit(old_col) || old_col == '0' || old_col == '9') {
		wmove(stdscr, 21, 3);
		deleteln();
		printw("Current COLUMN is invalid");
		invalid_entry = getch();
		is_valid = false;
		old_valid = false;
	}

	/* is the current row and column on a valid non-dark square? */
	if (old_valid && (tolower(old_row) - 'a') % 2 != (old_col - '0') % 2) { 
		wmove(stdscr, 21, 3);
		deleteln();
		printw("The current SQUARE is invalid - non-dark");
		invalid_entry = getch();
		is_valid = false;
	}


	/* is the new row within the confines of the game board? */
	if (!isalpha(new_row) || (tolower(new_row) - 'a') >= 8) {
		wmove(stdscr, 21, 3);
		deleteln();
		printw("New ROW is invalid");
		invalid_entry = getch();
		is_valid = false;
		new_valid = false;
	}

	/* is the new column within the confines of the game board? */
	if (!isdigit(new_col) || new_col == '0' || new_col == '9') {
		wmove(stdscr, 21, 3);
		deleteln();
		printw("New COLUMN is invalid");
		invalid_entry = getch();
		is_valid = false;
		new_valid = false;
	}

	/* is the new row and column on a valid non-dark square? */
	if (new_valid && (tolower(new_row) - 'a') % 2 != (new_col - '0') % 2) { 
		wmove(stdscr, 21, 3);
		deleteln();
		printw("The new SQUARE is invalid - non-dark");
		invalid_entry = getch();
		is_valid = false;
	}


	return is_valid;
}


/* 
 * VALIDATE GAME BOARD SQUARES
 * This function will take the array of game squares and will populate 
 * the valid_square data field with the boolean value of whether that
 * square may be occupied at any given time
 */
void
validate_squares(struct square (*gameboard)[8])
{
	int row, col;

	for (row = 0; row < 8; row++) {
		for (col = 0; col < 8; col++) {
			if (dark_square(row, col)) {
				gameboard[row][col].valid_square = false;
			} else {
				gameboard[row][col].valid_square = true;
			}
		}
	}

	return;

}


/*
 * IS DARK SQUARE
 * This function takes a position and tells whether the square is dark or not
 */
int
dark_square(int row, int col)
{
	if ((row % 2) == (col % 2))
		return true;
	else
		return false;
}


/*
 * ARE THERE ANY PIECES LEFT
 * This function will scan the board to determine if player has any more pieces
 * that are still alive, the results of which will determine if the game has
 * been won by player's opponent
 */
int
pieces_left(struct square (*gameboard)[8], int player)
{
	int row, col;
	int pieces = 0;

	for (row = 0; row < 8; row++) {
		for (col = 0; col < 8; col++) {
			if (gameboard[row][col].valid_square && 
			    gameboard[row][col].occupied &&
			    gameboard[row][col].occupier.player == player) {
			/*	if (gameboard[row][col].occupier.king) {
					pieces += 2;
				} else {
					pieces++;
				}*/
				pieces++;
			}
		}
	}

	return pieces;

}


/*
 * IS THERE A MOVE AVAILABLE 
 * This function tests to see if there is a single space move available for
 * the player.
 */
int
move_single_space(struct square (*gameboard)[8], int player, int row, int col)
{
	if (((!gameboard[row - 1][col - 1].occupied 
		&& !((row - 1) < 0) && !((col - 1) < 0)) 
		&& valid_direction(gameboard, row, col, 
		    (row - 1), (col - 1), player)) ||
	    ((!gameboard[row - 1][col + 1].occupied 
		&& !((row - 1) < 0) && !((col + 1) > 7)) 
		&& valid_direction(gameboard, row, col, 
		    (row - 1), (col + 1), player)) ||
	    ((!gameboard[row + 1][col - 1].occupied 
		&& !((row + 1) > 7) && !((col - 1) < 0)) 
		&& valid_direction(gameboard, row, col, 
		    (row + 1), (col - 1), player)) ||
	    ((!gameboard[row + 1][col + 1].occupied
		&& !((row + 1) > 7) && !((col + 1) > 7))
		&& valid_direction(gameboard, row, col, 
		    (row + 1), (col + 1), player))) 
		return true;
	else
		return false;
}


/*
 * IS THERE A JUMP AVAILABLE
 * This function tests to see if a piece has a jump available for the player.
 * It also takes three extra variable that will be used in the future to help
 * implement multiple jumps.
 */
int
jump_available(struct square (*gameboard)[8], int player, int row, int col, int flag, int old_row, int old_col)
{
	if (((!gameboard[row - 2][col - 2].occupied 
		&& !((row - 2) < 0) && !((col - 2) < 0)) 
		&& valid_direction(gameboard, row, col, 
		    (row - 2), (col - 2), player) 
		&& can_jump(gameboard, row, col,
		    (row - 2), (col - 2), player)) ||
	    ((!gameboard[row - 2][col + 2].occupied 
		&& !((row - 2) < 0) && !((col + 2) > 7)) 
		&& valid_direction(gameboard, row, col, 
		    (row - 2), (col + 2), player) 
		&& can_jump(gameboard, row, col,
		    (row - 2), (col + 2), player)) ||
	    ((!gameboard[row + 2][col - 2].occupied 
		&& !((row + 2) > 7) && !((col - 2) < 0)) 
		&& valid_direction(gameboard, row, col, 
		    (row + 2), (col - 2), player) 
		&& can_jump(gameboard, row, col,
		    (row + 2), (col - 2), player)) ||
	    ((!gameboard[row + 2][col + 2].occupied
		&& !((row + 2) > 7) && !((col + 2) > 7))
		&& valid_direction(gameboard, row, col, 
		    (row + 2), (col + 2), player) 
		&& can_jump(gameboard, row, col,
		    (row + 2), (col + 2), player))) 
		return true;
	else
		return false;
}


/*
 * HAS THE GAME ENDED
 * This function checks to see if the game is over. If there are no more pieces
 * left or if there are no moves or jumps availabe, the game is over.
 */
int
game_ended(struct square (*gameboard)[8], int player)
{
	int row, col;
	int moves = 0;

	for (row = 0; row < 8; row++) {
		for (col = 0; col < 8; col++) {
			if (gameboard[row][col].valid_square && 
			    gameboard[row][col].occupied &&
			    gameboard[row][col].occupier.player == player &&
			    (move_single_space(gameboard, player, 
				row, col) ||
			    jump_available(gameboard, player, 
				row, col, 0, 0, 0))) {
				moves++;
			}
		}
	}

	if (!pieces_left(gameboard, player))
		return true;
	else if (!moves)
		return true;
	else
		return false;
}
