/**
  ******************************************************************************
  * @file    main.c
  * @author  Weili An
  * @version V1.0
  * @date    Oct 24, 2022
  * @brief   ECE 362 Lab 7 template
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include <stdint.h>

// Global data structure
char* login          = "xyz"; // Replace with your login.
char disp[9]         = "Hello...";
uint8_t col          = 0;
uint8_t mode         = 'A';
uint8_t thrust       = 0;
int16_t fuel         = 800;
int16_t alt          = 4500;
int16_t velo         = 0;

// Make them visible to autotest.o
extern char* login;
// Keymap is in `font.S` to match up what autotester expected
extern char keymap;
extern char disp[9];
extern uint8_t col;
extern uint8_t mode;
extern uint8_t thrust;
extern int16_t fuel;
extern int16_t alt;
extern int16_t velo;

char* keymap_arr = &keymap;

// Font array in assembly file
// as I am too lazy to convert it into C array
extern uint8_t font[];

// The functions we should implement
void enable_ports();
void setup_tim6();
void show_char(int n, char c);
void drive_column(int c);
int read_rows();
char rows_to_key(int rows);
void handle_key(char key);
void setup_tim7();
void write_display();
void update_variables();
void setup_tim14();

// Autotest functions
extern void check_wiring();
extern void autotest();
extern void fill_alpha();

int main(void) {
    // check_wiring();
    // autotest();
    // fill_alpha();
    enable_ports();
    setup_tim6();
    setup_tim7();
    setup_tim14();

    for(;;) {
        asm("wfi");
    }
}

/**
 * @brief Enable the ports and configure pins as described
 *        in lab handout
 * 
 */
void enable_ports(){
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER &= ~(GPIO_MODER_MODER10 |      //Turn off port B pins 0-10
                      GPIO_MODER_MODER9 |
                      GPIO_MODER_MODER8 |
                      GPIO_MODER_MODER7 |
                      GPIO_MODER_MODER6 |
                      GPIO_MODER_MODER5 |
                      GPIO_MODER_MODER4 |
                      GPIO_MODER_MODER3 |
                      GPIO_MODER_MODER2 |
                      GPIO_MODER_MODER1 |
                      GPIO_MODER_MODER0);
    GPIOB->MODER |= (GPIO_Mode_OUT << (0 << 1) |  //Make port B pins 0-10 outputs
                     GPIO_Mode_OUT << (1 << 1) |
                     GPIO_Mode_OUT << (2 << 1) |
                     GPIO_Mode_OUT << (3 << 1) |
                     GPIO_Mode_OUT << (4 << 1) |
                     GPIO_Mode_OUT << (5 << 1) |
                     GPIO_Mode_OUT << (6 << 1) |
                     GPIO_Mode_OUT << (7 << 1) |
                     GPIO_Mode_OUT << (8 << 1) |
                     GPIO_Mode_OUT << (9 << 1) |
                     GPIO_Mode_OUT << (10 << 1));

    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER &= ~(GPIO_MODER_MODER8 |      //Turn off port C pins 0-8
                      GPIO_MODER_MODER7 |
                      GPIO_MODER_MODER6 |
                      GPIO_MODER_MODER5 |
                      GPIO_MODER_MODER4 |
                      GPIO_MODER_MODER3 |
                      GPIO_MODER_MODER2 |
                      GPIO_MODER_MODER1 |
                      GPIO_MODER_MODER0);
    GPIOC->PUPDR &= ~(GPIO_MODER_MODER3 |      //Turn off port C pupdr pins 0-3
                      GPIO_MODER_MODER2 |
                      GPIO_MODER_MODER1 |
                      GPIO_MODER_MODER0);
    GPIOC->MODER |= (GPIO_Mode_IN << (0 << 1) |     //Turn port C pins 0-3 inputs and 4-8 outputs
                     GPIO_Mode_IN << (1 << 1) |
                     GPIO_Mode_IN << (2 << 1) |
                     GPIO_Mode_IN << (3 << 1) |
                     GPIO_Mode_OUT << (4 << 1) |
                     GPIO_Mode_OUT << (5 << 1) |
                     GPIO_Mode_OUT << (6 << 1) |
                     GPIO_Mode_OUT << (7 << 1) |
                     GPIO_Mode_OUT << (8 << 1));
    GPIOC->PUPDR |= (GPIO_PuPd_DOWN << (0 << 1) |      //Make port C pupdr pins 0-3 pull low
                     GPIO_PuPd_DOWN << (1 << 1) |
                     GPIO_PuPd_DOWN << (2 << 1) |
                     GPIO_PuPd_DOWN << (3 << 1));
}

//-------------------------------
// Timer 6 ISR goes here
//-------------------------------

void TIM6_DAC_IRQHandler(){
  TIM6 -> SR = ~TIM_SR_UIF;                             //Acknowledge interrupt
  int32_t val = (GPIOC -> ODR) & (1 << 8);              //Read value of GPIOC ODR bit 8
  if(val == 0){                                         //If PC8 is not set, turn it on
    GPIOC -> BSRR |= 1 << 8;
  }
  else{                                                 //Else turn it off
    GPIOC -> BRR |= 1 << 8;
  }
}

/**
 * @brief Set up timer 6 as described in handout
 * 
 */
void setup_tim6() {
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;                   //Enable RCC clock for timer 6
  TIM6->PSC = 48000 - 1;                                //Prescale system clock by 48000
  TIM6->ARR = 500 - 1;                                  //Set Auto-Reload Register to have a 500 counting period
  TIM6->DIER |= TIM_DIER_UIE;                           //Configure timer 6 DMA/IER to enable UIE flag
  TIM6->CR1 |= TIM_CR1_CEN;                             //Let timer 6 start counting by setting the CEN bit in the timer 6 control register 1
  NVIC->ISER[0] = 1 << TIM6_DAC_IRQn;                   //Enable timer 6 interrupt in NVIC ISER
}

/**
 * @brief Show a character `c` on column `n`
 *        of the segment LED display
 * 
 * @param n 
 * @param c 
 */
void show_char(int n, char c) {
    if((n <= 7) && (n >= 0)){                           //If within specified range, write pattern for character and shift enable pin by 8 for the column
        GPIOB->ODR = font[c];
        GPIOB->ODR |= (n << 8);
    }
}

/**
 * @brief Drive the column pins of the keypad
 *        First clear the keypad column output
 *        Then drive the column represented by `c`
 * 
 * @param c 
 */
void drive_column(int c) {
    c &= 0x3;                                           //Read at least two bits
    GPIOC->BSRR |= 0x00F00000;                          //Clear bits 4-7 of GPIOC
    GPIOC->BSRR |= 1 << (c + 4);                        //Set bits to column c
}

/**
 * @brief Read the rows value of the keypad
 * 
 * @return int 
 */
int read_rows() {
    int32_t idr = 0xf & (GPIOC->IDR);                   //Read idr of GPIOC and return the value
    return(idr);
}

/**
 * @brief Convert the pressed key to character
 *        Use the rows value and the current `col`
 *        being scanning to compute an offset into
 *        the character map array
 * 
 * @param rows 
 * @return char 
 */
char rows_to_key(int rows) {
    int32_t row;
    int32_t offset;
    char c;
    if(rows == 2){              //Set row number based on rows input in binary
        row = 1;
    }
    else if(rows == 4){
        row = 2;
    }
    else if(rows == 8){
        row = 3;
    }
    else{
        row = 0;
    }
    offset = row + (4 * col);   //Calculate offset for keymap
    c = keymap_arr[offset];     //Return the character based off of offset index in keymap
    return(c);
}

/**
 * @brief Handle key pressed in the game
 * 
 * @param key 
 */
void handle_key(char key) {
    if(key == 'A'){         //If key is a character, set mode equal to key, else if key is a number, set thrust equal to key
        mode = 'A';
    }
    else if(key == 'B'){
        mode = 'B';
    }
    else if(key == 'D'){
        mode = 'D';
    }
    else{
        switch(key){
        case('0'):
            thrust = 0;
            break;
        case('1'):
            thrust = 1;
            break;
        case('2'):
            thrust = 2;
            break;
        case('3'):
            thrust = 3;
            break;
        case('4'):
            thrust = 4;
            break;
        case('5'):
            thrust = 5;
            break;
        case('6'):
            thrust = 6;
            break;
        case('7'):
            thrust = 7;
            break;
        case('8'):
            thrust = 8;
            break;
        case('9'):
            thrust = 9;
            break;
        }
    }
}

//-------------------------------
// Timer 7 ISR goes here
//-------------------------------
// TODO
void TIM7_IRQHandler(){
    TIM7->SR = ~TIM_SR_UIF;                 //Acknowledge interrupt
    int32_t row_val = read_rows();          //Read row value
    if(row_val != 0){                       //If row is not 0, read the character and handle it
        char key = rows_to_key(row_val);
        handle_key(key);
    }
    show_char(col, disp[col]);              //Display the character at column col
    col += 1;
    if(col >= 8){                           //Wrap the column if it is greater than 7
        col = 0;
    }
    drive_column(col);                      //Drive the next column
}
/**
 * @brief Setup timer 7 as described in lab handout
 * 
 */
void setup_tim7() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;         //Enable RCC Clock for timer 7
    TIM7->PSC = 4800 - 1;                       //Prescale system clock by 4800
    TIM7->ARR = 10 - 1;                         //Set Auto-Reload Register to have a 10 counting period
    TIM7->DIER |= TIM_DIER_UIE;                 //Configure timer 7 DMA/IER to enable UIE flag
    TIM7->CR1 |= TIM_CR1_CEN;                   //Let timer 7 start counting by setting the CEN bit in the timer 7 control register 1
    NVIC->ISER[0] = 1 << TIM7_IRQn;             //Enable timer 7 interrupt in NVIC ISER
}

/**
 * @brief Write the display based on game's mode
 * 
 */
void write_display() {
    if(mode == 'C'){                                        //If mode c, display that the spacecraft has crashed
        snprintf(disp, sizeof(disp), "Crashed");
    }
    if(mode == 'L'){                                        //If mode l, display that the spacecraft has landed
        snprintf(disp, sizeof(disp), "Landed ");
    }
    if(mode == 'A'){                                        //If mode a, display the altitude of the spacecraft
        snprintf(disp, sizeof(disp), "ALt%5d", alt);
    }
    if(mode == 'B'){                                        //If mode b, display the fuel of the spacecraft
        snprintf(disp, sizeof(disp), "FUEL %3d", fuel);
    }
    if(mode == 'D'){                                        //If mode d, display the velocity of the spacecraft
        snprintf(disp, sizeof(disp), "Spd %4d", velo);
    }
}

/**
 * @brief Game logic
 * 
 */
void update_variables() {
    fuel -= thrust;                                         //Fuel is decremented by thrust
    if(fuel <= 0){                                          //If fuel is zero or negative, set thrust and fuel to zero
        thrust = 0;
        fuel = 0;
    }
    alt += velo;                                            //Altitude is incremented by velocity
    if(alt <= 0){                                           //If altitude is zero or negative, and the velocity is greater than -10, set mode to 'L'
        if((velo * -1) < 10){
            mode = 'L';
        }
        else{                                               //Else, set mode to 'C'
            mode = 'C';
        }
        return;
    }
    velo += (thrust - 5);                                   //Increment velocity by thrust - 5
}

//-------------------------------
// Timer 14 ISR goes here
//-------------------------------
// TODO
void TIM14_IRQHandler(){
    TIM14->SR = ~TIM_SR_UIF;                                //Acknowledge interrupt
    update_variables();                                     //Call update_variables
    write_display();                                        //Call write_display
}
/**
 * @brief Setup timer 14 as described in lab
 *        handout
 * 
 */
void setup_tim14() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;                    //Enable RCC Clock for timer 14
    TIM14->PSC = 48000 - 1;                                 //Prescale system clock by 48000
    TIM14->ARR = 500 - 1;                                   //Set Auto-Reload Register to have a 500 counting period
    TIM14->DIER |= TIM_DIER_UIE;                            //Configure timer 14 DMA/IER to enable UIE flag
    TIM14->CR1 |= TIM_CR1_CEN;                              //Let timer 14 start counting by setting the CEN bit in the timer 14 control register 1
    NVIC->ISER[0] = 1 << TIM14_IRQn;                        //Enable timer 14 interrupt in NVIC ISER
}
