/**
@author Richard Chomjak
@file 	main.c
@brief  Main source of project.
@date 	11/11/2012
*/

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "main.h"
#include "block.h"
#include "tetris.h"

block_t element;


  
unsigned char NEW_BLOCK = 0;

unsigned char matrix[MATRIX_R_SIZE][MATRIX_C_SIZE] = {0};
unsigned char column = 0;
unsigned char matrix_element = 0;
unsigned int actual_row = 0;
unsigned char GAME_OVER = FALSE;

int elements_type[NUMBER_OF_BLOCKS]={
	 0, 2, 3, 5, 7, 11, 15
};


//extern unsigned char block_board[BLOCK_ORDI][BLOCK_SIZE_RANGE];

/**
\fn set_simplex_port_A
\brief Set ports to OUT mode 
 PTA1(TFT_RGB_EN}  
*/

void set_simplex_port_A(void){
     
  /* Set ports to OUT mode */
    
  /* PTA1(TFT_RGB_EN}  */
    PTADD_PTADD1 = 1;
    return;
}
        
/**
\fn set_simplex_port_B
\brief Set ports to OUT mode 
 PTB0DD .. PTB7DD
*/
void set_simplex_port_B(void){
 
  /* PTB0DD .. PTB7DD  */
  PTBDD = 0xff;
  return;
}

/**
\fn set_simplex_port_C
\brief Set ports to OUT mode for KEYBOARD [LEDs] 
 PTC2DD .. PTC6DD
*/
void
set_simplex_port_C(void){
  /* Set simplex of keys*/
	PTCDD = PTCDD | 0b01111000;

}

/**
\fn set_simplex_port_D
\brief Set ports to OUT mode 
 PTDD3..PTDD7 (Y+,Y-,X+,X-)   
*/
void 
set_simplex_port_D(void){
  
  /* Set ports to OUT mode */
  PTDDD = PTDDD | 0b01111000;
 
  return;
}

/**
\fn set_simplex_port_E
\brief Set ports to OUT mode 
 PTEDD2(TFT_HSYNC )
 PTEDD3(TFT_VSYNC)   
*/
void 
set_simplex_port_E(void){
  
  /* Set ports to OUT mode */
  // TFT_HSYNC 
  PTEDD_PTEDD2 = 0b1;
  // TFT_VSYNC
  PTEDD_PTEDD3 = 0b1;
   
  return;
}

/**
\fn set_simplex_port_F
\brief Set ports to OUT mode. 
 PTFDD0(TFT_DOTCLK).   
*/
void 
set_simplex_port_F(void){
  
  // TFT_DOTCLK
  PTFDD_PTFDD0 = 0b1;
   
  return;
}

/**
\fn set_simplex
\brief Set ALL ports to OUT mode. 
 PTFDD0(TFT_DOTCLK).   
*/
void 
set_simplex(void){
  
  set_simplex_port_A(); 
  set_simplex_port_B();
  set_simplex_port_D();
  set_simplex_port_E();
  set_simplex_port_F();
  
  return;
}

/**
\fn row_map
\brief Row maps. 
*/
void 
row_map( int row ){
  
  if( row >= 0 && row <= MATRIX_R_SIZE -1) { 
      	
	// D0,2,4,6
    // 1 row 
    PTBD_PTBD0 = 0b1;
    // 2 row
    PTBD_PTBD2 = 0b1;
    // 3 row
    PTBD_PTBD4 = 0b1;
    // 4 row
    PTBD_PTBD6 = 0b1;  
    //RGB_EN  5 row
    PTAD_PTAD1=0b1;
    //hsync   6 row
    PTED_PTED2 = 0b1;
    //  Y+    7 row 
    PTDD_PTDD5 = 0b1;
    // Y- 	  8 row
    PTDD_PTDD3 = 0b1;
  
  } 
  else{

    return;
  }
 
  if( row == 0){
    // 1 row 
    PTBD_PTBD0 = 0b0;
    return;
  }
  else  if( row == 1){
    // 2 row
    PTBD_PTBD2 = 0b0;
    return; 
  } 
  else if( row == 2){
    
    // 3 row
    PTBD_PTBD4 = 0b0;
    return; 
  }
   else if( row == 3){
    
    // 4 row
    PTBD_PTBD6 = 0b0;
    return; 
  }
   else if( row == 4){
    
    //RGB_EN  5 row
    PTAD_PTAD1=0b0; 
    return; 
  }
   else if( row == 5){
    
    //hsync      6 row
    PTED_PTED2 = 0b0;
    return;
  }
   else if( row == 6){
    
    // Y-,   7 row 
    PTDD_PTDD5 = 0b0;
    return;
      
  } else if( row == 7){

    // Y+ 8 row
    PTDD_PTDD3 = 0b0;
    return;
  }
   
   return;
}
/**
\fn column_matrix_2_reg
\brief Map ALL columns from matrix to register. 
*/
void 
column_matrix_2_reg(){
      
  __RESET_WATCHDOG(); 
	for( column = 0; column < MATRIX_C_SIZE; column++){

    column_map(matrix[actual_row][column]);
    }

    return;
}

/**
\fn tpm1ovf
\brief Interrupt for draw "matrix" on display. 
*/
interrupt VectorNumber_Vtpm1ovf void tpm1ovf(void) {

	//set bit "interrupt"
   	TPM1SC_TOF = 0b0;

   	//which row
  	actual_row = actual_row % MATRIX_C_SIZE;
  	//every column 
	column_matrix_2_reg();
  	//set row_map
  	row_map(actual_row);
  	actual_row++; 
}

/**
\fn delay_var
\brief Variable delay. 
*/
void 
delay_var(unsigned int delay_opt){

 	unsigned int j = 0;
 	for( j = 0; j < delay_opt; j++){

    __RESET_WATCHDOG();
  	}
	return;
}



/**
\fn column_map
\brief Map colum from matrix to register. 
*/
void 
column_map(unsigned char matrix_element ){
 
 	//columns
 	if ( column == 0 ){
    if ( matrix_element != 0 ){

        PTBD_PTBD1 = 0b1;
        return;
        } 
        else {      
       	 	
       	PTBD_PTBD1 = 0b0;
        return;
       }
  }
     
  if ( column == 1) {		
		if ( matrix_element != 0 ){
		    // 1 col
       	PTBD_PTBD3 = 0b1;
       	return;
        } 
        else  {
          PTBD_PTBD3 = 0b0;
          return;
       }   					  
  }

  if ( column == 2) {
		if ( matrix_element != 0 ){
		    // 2 col
        PTBD_PTBD5 = 0b1;
        return;
        } 
        else  {
          PTBD_PTBD5 = 0b0;
          return;
       }   
  }

  if ( column == 3) {
		if ( matrix_element != 0 ){
		    // 3 col
        PTBD_PTBD7 = 0b1;
        return;
        } 
        else  {
         	PTBD_PTBD7 = 0b0;
         	return;
       }   
  }

  if ( column == 4) {
		if ( matrix_element != 0 ){
		//DOT_CLK  5 col
		PTFD_PTFD0 = 0b1;
        return;
        } 
        else  {
        	PTFD_PTFD0 = 0b0;
         	return;
       }   
  }
    
  if ( column == 5) {
		if ( matrix_element != 0 ){
			//VSYNC 6 col
			PTED_PTED3 = 0b1;
      return;
        
    } 
    else  {
      PTED_PTED3 = 0b0;
      return;
     }   
  }
      
  if ( column == 6) {
		if ( matrix_element != 0 ){
			//X+ 7 col
		 	PTDD_PTDD4 = 0b1;
      return;
        } 
        else  {
           	PTDD_PTDD4 = 0b0;
           	return;
       }   
  }
  if ( column == 7) {
		if ( matrix_element != 0 ){
		//X- 8 col
		PTDD_PTDD6 = 0b1;
        return;
        } 
        else  {

          PTDD_PTDD6 = 0b0;
          return;
       }   
  }          

	return; 
}

/**
\fn	tmp1ovf_settings
\brief Set registers for tmp1ovf interrupt.
*/
void 
tmp1ovf_settings(void){

 	/* Timer interrupt settings */  
 	TPM1SC_TOF     = 0b0;
	TPM1SC_TOIE    = 0b1;
	//BUS RATE CLOCK
	TPM1SC_CLKSA = 0b1;
	TPM1SC_CLKSB = 0b0;
	TPM1SC_PS0   = 0b0;
	TPM1MODH = 110;	         
	TPM1MODL = 0;

	return;
	
}

/**
\fn	keyboard_settings
\brief Set registers for keyboard interrupt.
*/
void
keyboard_settings(void){

	KBISC_KBIE = 1;
	KBIPE     |= 0b11000111;
  KBIES      = 0;
}

/**
\fn btn_state
\brief  Check button function.
This function  and macros were hijacked from IMP lab3 2012
*/
unsigned int btn_state() {

  unsigned char btn_state = 0;

  if(~BTN_PORT1D & BTN_UP) btn_state |= BTN_UP;
  if(~BTN_PORT1D & BTN_DOWN) btn_state |= BTN_DOWN;
  if(~BTN_PORT1D & BTN_LEFT) btn_state |= BTN_LEFT;
  if(~BTN_PORT1D & BTN_RIGHT) btn_state |= BTN_RIGHT;
  if(~BTN_PORT2D & 4) btn_state |= BTN_OK;

  return(btn_state);
}

interrupt VectorNumber_Vkeyboard void kbi_isr(void){
  __RESET_WATCHDOG();
  if( GAME_OVER == 0 ){
       	
	  block_t elementhelp;
		unsigned int tmp_i= btn_state();        
	
	  	  
	  delay_var(5000);
	  if(tmp_i != btn_state()) tmp_i=0;
	  
     elementhelp = element;
	 
		if(tmp_i & BTN_LEFT) {
		 	PTCD_PTCD4 = 1;
      remove_block(element, element.x, element.y);
		  if( is_move_ok( element, element.x-1, element.y)){
	   	element.x -= 1;
	   	print_block(element, element.x, element.y);
	    
	 	 }
      else{
	 		  print_block(element, element.x, element.y);
		  }  	      
		}
		if(tmp_i & BTN_RIGHT) { 
		  PTCD_PTCD5 = 1;
      remove_block(element, element.x, element.y);
		  if( is_move_ok( element, element.x+1, element.y)){
	  		element.x += 1;
	   		print_block(element, element.x, element.y);
	  	}
      else {
	 			print_block(element, element.x, element.y);	     	
		 	}  		 
		}
		if(tmp_i & BTN_OK){
	    PTCD_PTCD6 = 1;
      remove_block(element, element.x, element.y);   	
   		element.current_rotation++;  
		  
		 	if((element.current_rotation >= (elements_type[element.element_chose + 1] - element.type_of_element))
		 	||  element.current_rotation + element.type_of_element >= BLOCK_ORDI) 
			element.current_rotation = 0;
		  if( is_move_ok( element, element.x, element.y)){
	    	print_block(element, element.x, element.y); 
	 		  }
      else{
	 		element = elementhelp;
	 		print_block(element, element.x, element.y);	
	 		}
		}

  }
  PTCD = 0;

	KBISC_KBACK = 1;                                             
}


/**
\fn init_element
\brief Create NEW element.
*/
void
init_element(){

	
  // START POSITION
  element.x =( MATRIX_C_SIZE -1)/2;
  element.y = 0;
  element.element_chose = (unsigned char)(element.type_of_element ) % (NUMBER_OF_BLOCKS) ;
  element.type_of_element =(unsigned char) elements_type[element.element_chose];;
  element.current_rotation = 0 ;
  if( is_move_ok(element, element.x, element.y)){
  	
 	print_block(element, element.x, element.y);
 }
 else{
 	 	GAME_OVER = TRUE;
  }
  
}


void 
main(void) {

 tmp1ovf_settings();
     
  EnableInterrupts;

/* Keyboard inerrupt settings 	*/  
  keyboard_settings();
  //keyboard simplex ports -> out
  set_simplex_port_C();
  
  /* Init simplex -> out  */
 	set_simplex();
  init_element();		

 
  for(;;) {

	__RESET_WATCHDOG(); /* feeds the dog */
 
 if(GAME_OVER == 0){
	
  if ( NEW_BLOCK ){
      init_element();
      if( GAME_OVER){
      	continue;
      }
      NEW_BLOCK = 0;

  }
 }else{
 	continue;
 }
  delay_var(60000);
  delay_var(60000);
    
  remove_block(element, element.x, element.y);
  if( is_move_ok( element, element.x, element.y+1)){
    
    element.y += 1;
    print_block(element, element.x, element.y);
  }
  else{  

    print_block(element, element.x, element.y);
    remove_row();
    NEW_BLOCK = 1;
  }      
     
 
      
  }
} 

