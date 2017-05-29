/*	$Id: draw.c,v 0.1 2009/02/26 15:41:04 cberardi Exp $	*/ 

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
#include "checkers.h"

void
draw(WINDOW *gamefield, struct square (*gameboard)[8])
{
	int rows, columns, dark;
	int offset = 0;
	char col_counter = '1';
	char row_counter = 'A';

	werase(gamefield);

	/* this will draw the game board's cells and fill in the dark squares */
	for (rows = 0; rows <= 16; rows++) {
		for (columns = 0; columns <= 32; columns++) {
			if ((rows % 2 == 0) || (rows == 0)) {
				wmove(gamefield, rows, columns);
				waddch(gamefield, '-');
			} else if ((columns % 4 == 0) || (columns == 0)) {
				wmove(gamefield, rows, columns);
				waddch(gamefield, '|');
				if ((columns % 8 == 0) && (columns < 28)) {
					for (dark = 0; dark < 3; dark++) {
						wmove(gamefield, rows,
						    (columns + dark + (!offset)
						    + (offset * 5)));
						waddch(gamefield, '#');
					}
				}
				offset = ++offset % 2;
			}
		}
	}

	/* this will print the row and column headings for the game board */
	for (columns = 5; columns < 36; columns += 4) {
		wmove(stdscr, 1, columns);
		waddch(stdscr, col_counter);
		col_counter++;
	}

	for (rows = 3; rows < 18; rows +=2) {
		wmove(stdscr, rows, 1);
		waddch(stdscr, row_counter);
		row_counter++;
	}

	/* this will print a small legend */
	wmove(stdscr, 7, 40);
	printw("Player 1 KING = K");
	wmove(stdscr, 9, 40);
	printw("Player 1 = X");
	wmove(stdscr, 11, 40);
	printw("Player 2 = O");
	wmove(stdscr, 13, 40);
	printw("Player 2 KING = C");

	/* this will go through the gameboard and print the pieces */
	for (rows = 0; rows < 8; rows++) {
		for (columns = 0; columns < 8; columns++) {
			if (gameboard[rows][columns].valid_square 
			    && gameboard[rows][columns].occupied) {
				wmove(gamefield, (Y_BASE + (rows * Y_OFFSET)), 
				    (X_BASE + (columns * X_OFFSET)));
				waddch(gamefield, 
				    gameboard[rows][columns].occupier.symbol);
			}
		}
	}
				

	refresh();
	wrefresh(gamefield);

	return;
}
