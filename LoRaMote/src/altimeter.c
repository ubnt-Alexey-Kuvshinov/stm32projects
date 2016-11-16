/*
 * altimeter.c
 *
 *  Created on: Nov 14, 2016
 *      Author: Alexey
 */


#include "stm32l152x_board.h"



void i2cWriteMPL3115A(uint8_t a, uint8_t d)
{
    char cmd[2];

    cmd[0] = a;
    cmd[1] = d;

    i2cwrite(MPL3115A_I2C_ADDRESS, cmd, 2);
}

uint8_t i2cReadMPL3115A(uint8_t a)
{
    char cmd[2];

    cmd[0] = a;
    i2cwrite(MPL3115A_I2C_ADDRESS, cmd, 1);

    if (m_i2c.read(MPL3115A_I2C_ADDRESS, cmd, 1))
        printf("MPL read-fail\n");

    if (a == CTRL_REG4)
        ctrl_reg4 = cmd[0];

    return cmd[0];
}
