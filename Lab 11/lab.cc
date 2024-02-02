#include "lab.h"
#include <stdint.h>
#include <stdio.h>

void setControl(uint32_t instBits, ControlSignals& outputSignals) {
  uint8_t opcode = instBits & 0x7F;
  switch(opcode) {                        //Depending on instruction type, set control signals associated with instruction type
    case 0b0110011:                       //If R type instruction
      outputSignals = {Zero, Zero, Zero, 0b10, Zero, Zero, One};
      break;
    case 0b0010011:                       //If I type instruction
      outputSignals = {Zero, Zero, Zero, 0b10, Zero, One, One};
      break;
    case 0b0100011:                       //If S type instruction
      outputSignals = {Zero, Zero, DC, 0b00, One, One, Zero};
      break;
    case 0b1100011:                       //If SB type instruction
      outputSignals = {One, Zero, DC, 0b01, Zero, Zero, Zero};
      break;
    case 0b0000011:                       //If load word instruction
      outputSignals = {Zero, One, One, 0b00, Zero, One, One};
      break;
  }
}

// Assume that the lower 32-bits of instBits contain the instruction.
uint32_t getExtendedBits(uint32_t instBits) {
  uint32_t returnVal = 0;
  uint8_t opcode = instBits & 0x7F;
  bool bits_12 = true;
  switch(opcode) {                                      //Check for I, S, SB type instructions
    case 0b0010011:                                     //If instruction is addi, ori, andi (I type)
      returnVal = (instBits & 0xFFF00000)>>20;          //Get bits 20-31 (immediate value)
      break;
    case 0b0000011:                                     //If instruction is load word (I type)
      returnVal = (instBits & 0xFFF00000)>>20;          //Get bits 20-31 (immediate value)
      break;
    case 0b0100011:                                     //If instruction is save word (S type)
      returnVal = (instBits & 0xFE000000)>>20;          //Get bits 25-31
      returnVal |= ((instBits & 0xF80)>>7)<<5;          //Get bits 7-11
      bits_12 = false;
      break;
    case 0b1100011:                                     //If instruction is beq (SB type)
      returnVal = (instBits & 0x80000000)>>31;          // Get bit 31
      returnVal |= ((instBits & 0x7E000000)>>20)>>25;   // Get bits 25-30
      returnVal |= ((instBits & 0xF80)>>7);             // Get bits 7-11
      returnVal |= ((instBits & 0x80)<<4)>>8;           // Get bit 8
      bits_12 = false;
      break;
  }
  if((bits_12) && (returnVal & 0x800)) {                //If MSB is 12 bit, sign extend 12 bit immediate value
    returnVal |= 0xFFFFF000;
  }
  else if(returnVal & 0x1000) {                         //If MSB is 13 bit, sign extend 13 bit immediate value
    returnVal |= 0xFFFFE000;
  }
  return returnVal;
}
