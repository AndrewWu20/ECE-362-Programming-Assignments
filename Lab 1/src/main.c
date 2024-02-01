/**
  ******************************************************************************
  * @file    main.c
  * @author  Weili An
  * @version V1.0
  * @date    Oct 17, 2022
  * @brief   ECE 362 Lab 6 Student template
  ******************************************************************************
*/

#include "stm32f0xx.h"
#include <stdint.h>

void initc();
void initb();
void togglexn(GPIO_TypeDef *port, int n);
void init_exti();
void set_col(int col);
void SysTick_Handler();
void init_systick();
void adjust_priorities();

extern void nano_wait(int);

volatile int current_col = 1;

int main(void) {
    // Uncomment when most things are working
    //autotest();
    
    initb();
    initc();
    init_exti();
    init_systick();
    adjust_priorities();

    // Slowly blinking
    for(;;) {
        togglexn(GPIOC, 9);
        nano_wait(500000000);
    }
}

/**
 * @brief Init GPIO port C
 *        PC0-PC3 as input pins with the pull down resistor enabled
 *        PC4-PC9 as output pins
 * 
 */
void initc() {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER &= ~(GPIO_MODER_MODER9 |       //Turn off pins 0-9
                      GPIO_MODER_MODER8 |
                      GPIO_MODER_MODER7 |
                      GPIO_MODER_MODER6 |
                      GPIO_MODER_MODER5 |
                      GPIO_MODER_MODER4 |
                      GPIO_MODER_MODER3 |
                      GPIO_MODER_MODER2 |
                      GPIO_MODER_MODER1 |
                      GPIO_MODER_MODER0);

    GPIOC->MODER |= GPIO_Mode_IN << 1;          //Make pin 0-3 inputs
    GPIOC->MODER |= GPIO_Mode_IN << (1 << 1);
    GPIOC->MODER |= GPIO_Mode_IN << (2 << 1);
    GPIOC->MODER |= GPIO_Mode_IN << (3 << 1);
    GPIOC->MODER |= GPIO_Mode_OUT << (4 << 1);  //Make pin 4-9 outputs
    GPIOC->MODER |= GPIO_Mode_OUT << (5 << 1);
    GPIOC->MODER |= GPIO_Mode_OUT << (6 << 1);
    GPIOC->MODER |= GPIO_Mode_OUT << (7 << 1);
    GPIOC->MODER |= GPIO_Mode_OUT << (8 << 1);
    GPIOC->MODER |= GPIO_Mode_OUT << (9 << 1);

    GPIOC->PUPDR &= ~0x000000ff;                //Turn off pins 0-3 in PUPDR
    GPIOC->PUPDR |= 0x000000aa;                 //Set pins 0-3 to use PUPDR
}

/**
 * @brief Init GPIO port B
 *        PB0, PB2, PB3, PB4 as input pins
 *          enable pull down resistor on PB2 and PB3
 *        PB8-PB11 as output pins
 * 
 */
void initb() {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER &= ~(GPIO_MODER_MODER11 |          //Turn off pins 0, 2-4, 8-11
                      GPIO_MODER_MODER10 |
                      GPIO_MODER_MODER9 |
                      GPIO_MODER_MODER8 |
                      GPIO_MODER_MODER4 |
                      GPIO_MODER_MODER3 |
                      GPIO_MODER_MODER2 |
                      GPIO_MODER_MODER0);

    GPIOB->MODER |= GPIO_Mode_IN << 1;              //Make pin 0, 2-4 inputs
    GPIOB->MODER |= GPIO_Mode_IN << (2 << 1);
    GPIOB->MODER |= GPIO_Mode_IN << (3 << 1);
    GPIOB->MODER |= GPIO_Mode_IN << (4 << 1);
    GPIOB->MODER |= GPIO_Mode_OUT << (8 << 1);      //Make pin 8-11 outputs
    GPIOB->MODER |= GPIO_Mode_OUT << (9 << 1);
    GPIOB->MODER |= GPIO_Mode_OUT << (10 << 1);
    GPIOB->MODER |= GPIO_Mode_OUT << (11 << 1);

    GPIOB->PUPDR &= ~0x000000f0;                    //Turn off pins 2 and 3 in PUPDR
    GPIOB->PUPDR |= 0x000000a0;                     //Set pins 2 and 3 in PUPDR to use
}

/**
 * @brief Change the ODR value from 0 to 1 or 1 to 0 for a specified 
 *        pin of a port.
 * 
 * @param port : The passed in GPIO Port
 * @param n    : The pin number
 */
void togglexn(GPIO_TypeDef *port, int n) {
    int32_t val = (port->ODR) & (1<<n);             //Obtain initial toggle value
    if(val == 0){                                   //If the value is 0, set bit to 1
        port->BSRR |= 1 << n;
    }
    else{                                           //If the value is 1, set bit to 0
        port->BRR |= 1 << n;
    }
}

//==========================================================
// Write the EXTI interrupt handler for pins 0 and 1 below.
// Copy the name from startup/startup_stm32.s, create a label
// of that name below, declare it to be global, and declare
// it to be a function.
// It acknowledge the pending bit for pin 0, and it should
// call togglexn(GPIOB, 8).
void EXTI0_1_IRQHandler(){
    EXTI->PR = EXTI_PR_PR0;                         //Acknowledge pin 0 pending bit (PR = pending register, PR0 = bit 0)
    togglexn(GPIOB,8);                              //Toggle port b pin 8
}

//==========================================================
// Write the EXTI interrupt handler for pins 2-3 below.
// It should acknowledge the pending bit for pin2, and it
// should call togglexn(GPIOB, 9).
void EXTI2_3_IRQHandler(){
    EXTI->PR = EXTI_PR_PR2;                         //Acknowledge pin 2 pending bit (PR = pending register, PR2 = bit 2)
    togglexn(GPIOB,9);                              //Toggle port b pin 9
}

//==========================================================
// Write the EXTI interrupt handler for pins 4-15 below.
// It should acknowledge the pending bit for pin4, and it
// should call togglxn(GPIOB, 10).
void EXTI4_15_IRQHandler(){
    EXTI->PR = EXTI_PR_PR4;                         //Acknowledge pin 4 pending bit (PR = pending register, PR2 = bit 4)
    togglexn(GPIOB,10);                             //Toggle port b pin 10
}

/**
 * @brief Follow lab manual section 4.4 to initialize EXTI
 *        (1-2) Enable the SYSCFG subsystem, and select Port B for
 *            pins 0, 2, 3, and 4.
 *        (3) Configure the EXTI_RTSR register so that an EXTI
 *            interrupt is generated on the rising edge of
 *            pins 0, 2, 3, and 4.
 *        (4) Configure the EXTI_IMR register so that the EXTI
 *            interrupts are unmasked for pins 2, 3, and 4.
 *        (5) Enable the three interupts for EXTI pins 0-1, 2-3 and
 *            4-15. Don't enable any other interrupts.
 */
void init_exti() {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;       //Enable Syscfg subsystem

    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB;   //Select port b for pins 0, 2, 3, 4
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PB;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PB;
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PB;

    EXTI->RTSR |= EXTI_RTSR_TR0;                    //Interrupt is generated on rising edge of pins 0, 2, 3, 4
    EXTI->RTSR |= EXTI_RTSR_TR2;
    EXTI->RTSR |= EXTI_RTSR_TR3;
    EXTI->RTSR |= EXTI_RTSR_TR4;

    //EXTI->IMR |= EXTI_IMR_MR0;
    EXTI->IMR |= EXTI_IMR_MR2;                      //Interrupts are unmasked for pins 2, 3, 4
    EXTI->IMR |= EXTI_IMR_MR3;
    EXTI->IMR |= EXTI_IMR_MR4;

    NVIC->ISER[0] = 1 << EXTI0_1_IRQn;              //Enable interrupts for EXTI pins 0-1, 2-3, 4-15
    NVIC->ISER[0] = 1 << EXTI2_3_IRQn;
    NVIC->ISER[0] = 1 << EXTI4_15_IRQn;
}

/**
 * @brief For the keypad pins, 
 *        Set the specified column level to logic "high.
 *        Set the other three three columns to logic "low".
 * 
 * @param col 
 */
void set_col(int col) {
    GPIOC->ODR &= ~0x000000f0;                      //Set port c pin 4-7 to be 0
    if(col == 4){                                   //Based on specified column, set the specified column logic to high
        GPIOC->ODR = 0x00000010;
    }
    else if(col == 3){
        GPIOC->ODR = 0x00000020;
    }
    else if(col == 2){
        GPIOC->ODR = 0x00000040;
    }
    else{
        GPIOC->ODR = 0x00000080;
    }
}

/**
 * @brief The ISR for the SysTick interrupt.
 * 
 */
void SysTick_Handler() {
    if((current_col == 4) && ((GPIOC->IDR & 0b0001) == 1)){      //If column is 4 and input is 1, toggle pb11
        togglexn(GPIOB, 11);
    }
    if((current_col == 3) && ((GPIOC->IDR & 0b0010) == 2)){      //If column is 3 and input is 2, toggle pb10
        togglexn(GPIOB, 10);
    }
    if((current_col == 2) && ((GPIOC->IDR & 0b0100) == 4)){      //If column is 2 and input is 4, toggle pb9
        togglexn(GPIOB, 9);
    }
    if((current_col == 1) && ((GPIOC->IDR & 0b1000) == 8)){      //If column is 1 and input is 8, toggle pb8
        togglexn(GPIOB, 8);
    }
    current_col += 1;
    if(current_col > 4){                            //Set column back to 1 if current_col is > 4
        current_col = 1;
    }
    set_col(current_col);                           //Set column to 1 and the other columns to 0
}

/**
 * @brief Enable the SysTick interrupt to occur every 1/16 seconds.
 * 
 */
void init_systick() {
    SysTick->LOAD = 375000 - 1;                     //Set reset value to 1/16th seconds
    SysTick->CTRL = SysTick_CLKSource_HCLK_Div8;    //Enable clock division of 8
}

/**
 * @brief Bonus question
 *        Set the priority for EXTI pins 2-3 interrupt to 192.
 *        Set the priority for EXTI pins 4-15 interrupt to 128.
 *        Do not adjust the priority for any other interrupts.
 * 
 */
void adjust_priorities() {
    NVIC->IP[1] &= 0xc0ffffff;
    NVIC->IP[1] &= 0xff80ffff;
}
