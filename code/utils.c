#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "utils.h"

//combines 2 separate uint8_t type interger to one uint16_t type integer
uint16_t toTwoByte(uint8_t byte1, uint8_t byte2){
    return (byte1 << 8) | byte2;
}
