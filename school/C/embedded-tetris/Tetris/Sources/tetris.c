/**
@author Richard Chomjak
@file 	tetris.c
@brief  Main source of project.
@date 	11/11/2012
*/

#include "main.h"
#include "block.h"
#include "tetris.h"
extern unsigned char block_board[BLOCK_ORDI][BLOCK_SIZE_RANGE];
extern unsigned char matrix[MATRIX_R_SIZE][MATRIX_C_SIZE];

/**
\fn remove_block
\brief Remove block(element) from matrix. 
*/
void
remove_block(block_t elementin, char  x, char y){
						   
	char hx = 0;
	char hy = 0;
	
	char xhelp = x;
        char remhelp = 0;
		for( hy = 0; hy < ELEMENT_SIZE ; hy++){
			x = xhelp;
			for ( hx = 0; hx < ELEMENT_SIZE ; hx++){
				
					if(block_board[(elementin.type_of_element + elementin.current_rotation)][(hx+hy*ELEMENT_SIZE)] != 0 && matrix[y][x] != 0 ){

					matrix[y][x] = 0;
					remhelp++;
					if( remhelp == ELEMENT_SIZE){
						return;
					}
				}

				x++;
			}

			y++;
		}
	return;
}

/**
\fn is_move_ok
\brief Check, if is move possible. 
*/
int 
is_move_ok(block_t elementin, char x, char y){

	char hx = 0;
	char hy = 0;
	char xhelp = x;
	
		for( hy = 0; hy < ELEMENT_SIZE ; hy++){
			x = xhelp;
			for ( hx = 0; hx < ELEMENT_SIZE ; hx++){
		
				if(block_board[(elementin.type_of_element + elementin.current_rotation)][(hx+hy*ELEMENT_SIZE)] != 0 && (y > MATRIX_R_SIZE -1))
					return FALSE;
				if(block_board[(elementin.type_of_element + elementin.current_rotation)][(hx+hy*ELEMENT_SIZE)] != 0 &&( (x > MATRIX_C_SIZE -1) || x < 0 ))
					return FALSE;	
				if(block_board[(elementin.type_of_element + elementin.current_rotation)][(hx+hy*ELEMENT_SIZE)] != 0 && matrix[y][x] != 0 ){

					return FALSE;
				}


				x++;
			}



			y++;
		}

	return TRUE;

}

/**
\fn print_block
\brief Print block into matrix[display]. 
*/
void
print_block(block_t elementin, char x, char y){
	char hx = 0;
	char hy = 0;
	
	char xhelp = x;
        char printhelp = 0;
        
		for( hy = 0; hy < ELEMENT_SIZE ; hy++){
			x = xhelp;
			for ( hx = 0; hx < ELEMENT_SIZE ; hx++){

			
				if(block_board[(elementin.type_of_element + elementin.current_rotation)][hx+hy*ELEMENT_SIZE] != 0 ){

					matrix[y][x] = 1;
				        printhelp++;
				      if( printhelp == ELEMENT_SIZE){
						return;
					} 
				}
				x++;
			}

			y++;
		}
	return;
}

/**
\fn moves_lines
\brief Move previous block in matrix to new position. 
*/
void
move_lines(){

	int i =0;
	int j = 0;
	int x = 0;
	

	for( i = MATRIX_R_SIZE ; i > 1; i--){
	
		for( j = 0; j < MATRIX_C_SIZE; j++){
			
			if(matrix[i][j] ==0){
				matrix[i][j]    = matrix[i -1][j];
				matrix[i-1][j] = 0;
				
			}
		}
		       remove_row();

		
	}

}

/**
\fn remove_row
\brief Main function to check state of rows. 
*/
void
remove_row(){
	
	char i =0;
	for( i = MATRIX_R_SIZE -1; i >0; i--){
		if( is_row_full(i)){
		   clear_lines(i);
		   move_lines();
		   
		}
	}

}

/**
\fn is_full_row
\brief Check if all colums in row are in "1". 
*/
int 
is_row_full(char row){

	int i = 0;
	for( i = 0; i < MATRIX_C_SIZE; i++){
		if(matrix[row][i] == 0){
			return FALSE;
		}
	}
	return TRUE;		       			
}

/**
\fn clear_lines
\brief Set colums in row to "0". 
*/
void 
clear_lines(char r){
	char i = 0;	
	for(i = 0; i < MATRIX_C_SIZE; i++){
		matrix[r][i]=0;
	}
}











