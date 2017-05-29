/*	$Id: move.c,v 0.1 2009/02/26 15:41:04 cberardi Exp $	*/ 

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
#include "checkers.h"


/*
 * DO YOU OCCUPY THE SQUARE
 * This function serves to tell whether the player occupies the square in 
 * question, useful to tell if the square being moved from has a piece owned
 * by the player
 */
int
occupy_square(struct square (*gameboard)[8], int player, int row, int col)
{
	if (gameboard[row][col].occupier.player == player && 
	    gameboard[row][col].occupier.alive)
		return true;
	else
		return false;
}


/*
 * IS THE SQUARE MOVED TO OCCUPIED
 * This function tests whether the square to be moved to is occupied or not
 */
int
square_occupied(struct square (*gameboard)[8], int row, int col)
{
	if (gameboard[row][col].occupied)
		return true;
	else
		return false;
}


/*
 * WHAT DIRECTION CAN THE PIECE MOVE
 * This function determines if the piece can only move forward or both
 * forward and backwards (king) - this function also checks the horizontal
 * movement to verify that it is within bounds
 */
int
valid_direction(struct square (*gameboard)[8], int old_row, int old_col, 
    int new_row, int new_col, int player)
{
	/* check vertical movement */
	if (!gameboard[old_row][old_col].occupier.king) {
		if (player == 1) {
			if (!(old_row > new_row && (old_row - new_row) <= 2)) {
				return false;
			}
		} else if (player == 2) {
			if (!(old_row < new_row && (new_row - old_row) <= 2)) {
				return false;
			}
		}
	} else if (gameboard[old_row][old_col].occupier.king) {
		if (abs(old_row - new_row) > 2) {
			return false;
		}
	}

	/* check horizontal movement */
	if (abs(old_row - new_row) != abs(old_col - new_col)) {
		return false;
	}

	return true;
			
}


/*
 * MOVE PIECE ONE SPACE
 * This funciton will move a piece one space, up one row and over one column.
 */
void
move_piece(struct square (*gameboard)[8], int old_row, int old_col, 
    int new_row, int new_col)
{
	/* copy piece to new square */
	gameboard[new_row][new_col].occupied = true;
	gameboard[new_row][new_col].occupier = 
	    gameboard[old_row][old_col].occupier;

	/* delete piece from old square */
	gameboard[old_row][old_col].occupied = false;
	gameboard[old_row][old_col].occupier.alive = false;
}


/*
 * CAN THE PIECE JUMP
 * This function determines whether the piece can make the jump in question;
 * if there isn't a piece in the way, then the jump is illegal
 */
int 
can_jump(struct square (*gameboard)[8], int old_row, int old_col, 
    int new_row, int new_col, int player)
{
	if (old_row > new_row) {
		if (old_col > new_col) {
			if (!gameboard[old_row - 1][old_col - 1].occupied ||
			    gameboard[old_row - 1][old_col - 1].occupier.player
			    == player) { 
				return false;
			}
		} else if (old_col < new_col) {
			if (!gameboard[old_row - 1][old_col + 1].occupied || 
			    gameboard[old_row - 1][old_col + 1].occupier.player
			    == player) { 
				return false;
			}
		}
	} else if (old_row < new_row) {
		if (old_col > new_col) {
			if (!gameboard[old_row + 1][old_col - 1].occupied || 
			    gameboard[old_row + 1][old_col - 1].occupier.player
			    == player) { 
				return false;
			}
		} else if (old_col < new_col) {
			if (!gameboard[old_row + 1][old_col + 1].occupied || 
			    gameboard[old_row + 1][old_col + 1].occupier.player
			    == player) { 
				return false;
			}
		}
	}

	return true;

}


/*
 * JUMP THE PIECE
 * This function will allow the piece to jump. It will move the piece to the 
 * new location, delete it from the old location, and delete the piece that 
 * has been jumped.
 */
void
jump_piece(struct square (*gameboard)[8], int old_row, int old_col,
    int new_row, int new_col)
{
	int jump_row, jump_col;

	/* copy piece to new square */
	gameboard[new_row][new_col].occupied = true;
	gameboard[new_row][new_col].occupier = 
	    gameboard[old_row][old_col].occupier;

	/* delete piece from old square */
	gameboard[old_row][old_col].occupied = false;
	gameboard[old_row][old_col].occupier.alive = false;

	/* delete piece that has been jumped */
	if (old_row > new_row) {
		jump_row = old_row - 1;
		if (old_col > new_col) {
			jump_col = old_col - 1;
		} else if (old_col < new_col) {
			jump_col = old_col + 1;
		}
	} else if (old_row < new_row) {
		jump_row = old_row + 1;
		if (old_col > new_col) {
			jump_col = old_col - 1;
		} else if (old_col < new_col) {
			jump_col = old_col + 1;
		}
	}
	gameboard[jump_row][jump_col].occupied = false; 
	gameboard[jump_row][jump_col].occupier.alive = false; 
}


/*
 * KING ME
 * This function will change the piece to reflect the fact that the piece
 * has been kinged.
 */
void
king_me(struct square (*gameboard)[8], int row, int col, int player)
{
	gameboard[row][col].occupier.king = true;
	if (player == 1) {
		gameboard[row][col].occupier.symbol = 'K';
	} else if (player == 2) {
		gameboard[row][col].occupier.symbol = 'C';
	}

	return;
}
