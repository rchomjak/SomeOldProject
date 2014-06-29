/**
@author Richard Chomjak
@file 	tetris.h
@brief  Tetris functions.
@date 	11/11/2012
*/

#ifndef _TETRIS_H
#define _TETRIS_H

void
remove_block(block_t, char , char );
int 
is_move_ok(block_t elementin, char , char );
void
print_block(block_t elementin, char , char );

int 
is_row_full(char);

void
remove_row();

void 
clear_lines(char );

void
move_lines();



#endif 
