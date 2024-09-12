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
int is_first_recvd = 0;
int rxcount = 0;   //for 
int countAddr = 0;  //debugging
int countrxcplt = 0;   //purpose
int counterror = 0;    //only

void process_data(void){
    //memcpy(mainbuff,RxData+1,rxcount-1);
   
}


//callback function to enter into slave mode for slave mcu
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c){
    HAL_I2C_EnableListen_IT(hi2c);
}

//callback function when the address of the slave matches initiated by the master
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode){
    if (TransferDirection == I2C_DIRECTION_TRANSMIT) //if master wants to transmit data
    {  
        if(is_first_recvd == 0)
        {
            rxcount = 0;
            countAddr++;
            HAL_I2C_Slave_Sequential_Receive_IT(hi2c,RxData+rxcount,1 , I2C_FIRST_FRAME);

        }
        
    }
    else  //if master wants to receive data from slave
    {
        Error_Handler();  
    }

}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(is_first_recvd == 0)
    {
        rxcount++;
        is_first_recvd=1;
        HAL_I2C_Slave_Seq_Receive_IT(hi2c,RxData+rxcount,RxData[0],I2C_LAST_FRAME);
    }
    else
    {
        rxcount = rxcount+RxData[0];
        is_first_recvd=0;
        process_data();

    }
    
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    counterror++;
    uint32_t error_code = HAL_I2C_GetError(hi2c);
    if (error_code == 4)  //AF error
    {
        process_data();
    }
    HAL_I2C_EnableListen_IT(hi2c);

}