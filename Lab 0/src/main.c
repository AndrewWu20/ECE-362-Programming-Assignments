/**
  ******************************************************************************
  * @file    main.c
  * @author  Andrew Wu
  * @version V1.0
  * @date    Aug 30, 2023
  * @brief   ECE 362 Lab 1 template
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include <stdint.h>

void initb();
void initc();
void setn(int32_t pin_num, int32_t val);
int32_t readpin(int32_t pin_num);
void buttons(void);
void keypad(void);

void mysleep(void) {
    for(int n = 0; n < 1000; n++);
}

int main(void) {
    // Uncomment when most things are working
    autotest();
    
    initb();
    initc();

    // uncomment one of the loops, below, when ready
    // while(1) {
    //   buttons();
    // }

    // while(1) {
    //   keypad();
    // }

    for(;;);
}

/**
 * @brief Init GPIO port B
 *        Pin 0: input
 *        Pin 4: input
 *        Pin 8-11: output
 *
 */
void initb() {                          //Step 4.1***
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;  //Enables rcc clock on port two
    GPIOB->MODER &= ~(GPIO_MODER_MODER11 | //Turn off ports 0. 4. 8-11
                      GPIO_MODER_MODER10 |
                      GPIO_MODER_MODER9 |
                      GPIO_MODER_MODER8 |
                      GPIO_MODER_MODER4 |
                      GPIO_MODER_MODER0);

    GPIOB->MODER |= GPIO_Mode_IN << 1;          //Make pin 0 and 4 input
    GPIOB->MODER |= GPIO_Mode_IN << (4 << 1);
    GPIOB->MODER |= GPIO_Mode_OUT << (8 << 1);  //Make pin 8-11 outputs
    GPIOB->MODER |= GPIO_Mode_OUT << (9 << 1);
    GPIOB->MODER |= GPIO_Mode_OUT << (10 << 1);
    GPIOB->MODER |= GPIO_Mode_OUT << (11 << 1);
}

/**
 * @brief Init GPIO port C
 *        Pin 0-3: inputs with internal pull down resistors
 *        Pin 4-7: outputs
 *
 */
void initc() {                          //Step 4.2***
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  //Enables rcc clock on port three
    GPIOC->MODER &= ~(GPIO_MODER_MODER7 |       //Turn off pins 0-7
                      GPIO_MODER_MODER6 |
                      GPIO_MODER_MODER5 |
                      GPIO_MODER_MODER4 |
                      GPIO_MODER_MODER3 |
                      GPIO_MODER_MODER2 |
                      GPIO_MODER_MODER1 |
                      GPIO_MODER_MODER0);
    GPIOC->MODER |= 0x00005500;               //Make pins 4-7 outputs
    GPIOC->PUPDR &= ~0x00000055;                 //Make pins 0-3 in PUPDR off
    GPIOC->PUPDR |= 0x000000aa;                 //Set pins 0-3 to "10" for PUPDR port
}

/**
 * @brief Set GPIO port B pin to some value
 *
 * @param pin_num: Pin number in GPIO B
 * @param val    : Pin value, if 0 then the
 *                 pin is set low, else set high
 */
void setn(int32_t pin_num, int32_t val) {   //Step 4.3***
    if(val == 0) {                          //If value is 0, set pin to low
        GPIOB->BRR |= 1 << pin_num;
    }
    else {                                  //Else, set pin to high
        GPIOB->BSRR |= 1 << pin_num;
    }
}

/**
 * @brief Read GPIO port B pin values
 *
 * @param pin_num   : Pin number in GPIO B to be read
 * @return int32_t  : 1: the pin is high; 0: the pin is low
 */
int32_t readpin(int32_t pin_num) {          //Step 4.4***
    int32_t value = GPIOB->IDR & (1 << pin_num);    //Reads value at pin number in GPIO port B
    return(value);
}

/**
 * @brief Control LEDs with buttons
 *        Use PB0 value for PB8
 *        Use PB4 value for PB9
 *
 */
void buttons(void) {            //Step 4.6***
    setn(8, readpin(0));        //Read pin 0 and set it to pin 8
    setn(9, readpin(4));        //Read oin 4 and set it to pin 9
}

/**
 * @brief Control LEDs with keypad
 *
 */
void keypad(void) {             //Step 4.7***
    for(int i = 0; i <= 4; i++){
        GPIOC->ODR = (1 << (7-i));      //Set ith column to 1 using GPIOC->ODR
        int32_t row_in = 0x55555555 & (GPIOC -> IDR);   //Read row inputs
        mysleep();
        if(i == 3){                     //Check column 4
            int32_t row_i = row_in & (1<<0);    //Check 1st row
            setn(11, row_i);            //Set pin 11 to 1st row
        }
        if(i == 2){                     //Check column 3
            int32_t row_i = row_in & (1<<1);    //Check 2nd row
            setn(10, row_i);            //Set pin 10 to 2nd row
        }
        if(i == 1){                     //Check column 2
            int32_t row_i = row_in & (1<<2);    //Check 3rd row
            setn(9, row_i);             //Set pin 9 to 3rd row
        }
        if(i == 0){                     //Check column 1
            int32_t row_i = row_in & (1<<3);    //Check 4th row
            setn(8, row_i);                //Set pin 8 to 4th row
        }
    }
}
