/*	$Id: checkers.c,v 0.1 2009/02/26 15:41:04 cberardi Exp $	*/

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
#include <stdio.h>
#include <stdlib.h>
#include "checkers.h"

int
main()
{
	int player = 1;
	int player1type, player2type, player1depth, player2depth;
	int old_row, old_col, new_row, new_col;
	char row1, row2, col1, col2;
	char spreco;
	struct move aimove;

	/* get initial information on players and difficulty */
	system("clear");
	printf("\n0 = HUMAN -- 1 = COMPUTER\n");
	printf("\nWhat type is PLAYER 1? ");
	scanf("%d", &player1type);
	if (player1type == 1) {	
		printf("\nHow difficult should this player be? ");
		scanf("%d", &player1depth);
	}
	printf("\nWhat type is PLAYER 2? ");
	scanf("%d", &player2type);
	if (player2type == 1) {	
		printf("\nHow difficult should this player be? ");
		scanf("%d", &player2depth);
	}
	spreco = getchar();
	
	/* This will initialize the curses interface and game board */
	WINDOW *gamefield;
	initscr();
	gamefield = subwin(stdscr, 18, 34, 2, 3);


	/* This will create the game board and set up the valid squares */
	struct square gameboard[8][8];
	validate_squares(gameboard); 
	populate_squares(gameboard);


	while(1) {

		draw(gamefield, gameboard);
		wmove(stdscr, 20, 3);
		deleteln();
		deleteln();

		/* is it the AI's turn? */
		if (player == 1 && player1type == 1) {
			aiturn(gameboard, player, player1depth);
			player = (player % 2) + 1;
			continue;
		}
		if (player == 2 && player2type == 1) {
			aiturn(gameboard, player, player2depth);
			player = (player % 2) + 1;
			continue;
		}

		printw("Player %d enter your move: ", (player));

		/* get user input */
		wscanw(stdscr, "%c%c %c%c", &row1, &col1, 
		    &row2, &col2); 

		if (validate_input(row1, col1, row2, col2)) {
			/* 
			 * this converts the character input into the 
			 * integer values that can be used to locate
			 * and move the pieces on the game board
			 */
			old_row = tolower(row1) - 'a';
			old_col = col1 - '0' - 1;
			new_row = tolower(row2) - 'a';
			new_col = col2 - '0' - 1;

			/* does the player have a piece in the current square */
			if (!occupy_square(gameboard, player, old_row, 
			    old_col)) {
				wmove(stdscr, 21, 3);
				deleteln();
				printw("You don't occupy the CURRENT square");
				spreco = getch();
				continue;
			}

			/* does the player have a piece in the current square */
			if (square_occupied(gameboard, new_row, new_col)) { 
				wmove(stdscr, 21, 3);
				deleteln();
				printw("You can't move to an OCCUPIED square");
				spreco = getch();
				continue;
			}

			/* is the player going in the right direction? */
			if (!valid_direction(gameboard, old_row, old_col,
			    new_row, new_col, player)) {
				wmove(stdscr, 21, 3);
				deleteln();
				printw("That DIRECTION is an invalid move");
				spreco = getch();
				continue;
			}

			/* if the move is a single space, move it */
			if (abs(old_row - new_row) == 1) {
				move_piece(gameboard, old_row, old_col,
				    new_row, new_col);
			}

			/*
			 * if the move is a jump, make sure it can jump and
			 * then jump the piece
			 */
			if (abs(old_row - new_row) == 2) {
				if (!can_jump(gameboard, old_row, old_col, 
				    new_row, new_col, player)) { 
					wmove(stdscr, 21, 3);
					deleteln();
					printw("That JUMP is an invalid move");
					spreco = getch();
					continue;
				} else {
					jump_piece(gameboard, old_row, old_col,
					    new_row, new_col);
				}
			}

			/* if the piece has reached the other end, king it */
			if ((player == 1 && new_row == 0) || 
			    (player == 2 && new_row == 7)) {
				king_me(gameboard, new_row, new_col, player);
			}



			player = (player % 2) + 1;

			/* has the game been won? */
			if (game_ended(gameboard, player)) {
				wmove(stdscr, 21, 3);
				deleteln();
				printw("PLAYER %d HAS WON THE GAME!", 
				    (player % 2) + 1);
				spreco = getch();
				endwin();
				system("clear");
				exit(0);
			}
		}

	}
}
