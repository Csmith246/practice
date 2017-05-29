/*	$Id: populate.c,v 0.1 2009/02/26 15:41:04 cberardi Exp $	*/ 

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
 * POPULATE GAME BOARD SQUARES
 * This function will set the board up for the initial game state.
 */
void
populate_squares(struct square (*gameboard)[8])
{
	int row, col;

	for (row = 0; row < 8; row++) {
		for (col = 0; col < 8; col++) {
			if (gameboard[row][col].valid_square && row <= 2) {
				gameboard[row][col].occupied = true;
				gameboard[row][col].occupier.player = 2; 
				gameboard[row][col].occupier.symbol = 'O'; 
				gameboard[row][col].occupier.alive = true;
				gameboard[row][col].occupier.king = false; 
			}
			if (gameboard[row][col].valid_square && row >= 5) {
				gameboard[row][col].occupied = true;
				gameboard[row][col].occupier.player = 1; 
				gameboard[row][col].occupier.symbol = 'X'; 
				gameboard[row][col].occupier.alive = true;
				gameboard[row][col].occupier.king = false; 
			}
			if (gameboard[row][col].valid_square 
			    && row > 2 && row < 5) {
				gameboard[row][col].occupied = false;
			}
		}
	}

	return;

}
