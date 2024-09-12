/*
  *i2c_slave.h
  * Created on:sep 11, 2024
  *   Author : Sarthak Chaudhary

*/

#include "main.h"
#include "i2c_slave.h"

extern I2C_HandleTypeDef hi2c1;

#define RxSize 6
uint8_t RxData[RxSize];
int rxcount = 0;   //for 
int countAddr = 0;  //debugging
int countrxcplt = 0;   //purpose
int counterror = 0;    //only


//callback function to enter into slave mode for slave mcu
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c){
    HAL_I2C_EnableListen_IT(hi2c);
}

//callback function when the address of the slave matches initiated by the master
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode){
    if (TransferDirection == I2C_DIRECTION_TRANSMIT) //if master wants to transmit data
    {  
        countAddr++;
        HAL_I2C_Slave_Sequential_Receive_IT(hi2c,RxData+rxcount,1 , I2C_FIRST_FRAME);
    }
    else  //if master wants to receive data from slave
    {
        Error_Handler();  
    }

}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c){
    rxcount++;
    if (rxcount>= RxSize) rxcount=0;
    HAL_I2C_Slave_Sequential_Receive_IT(hi2c,RxData+rxcount,1,I2C_NEXT_FRAME);
    countrxcplt++;
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    counterror++;
    HAL_I2C_EnableListen_IT(hi2c);

}