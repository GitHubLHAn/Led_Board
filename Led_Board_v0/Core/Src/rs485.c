#include <rs485.h>
#include <string.h>
#include "scan_led.h"

/*
NOTE: 

		+ Put in CODE BEGIN 2
			HAL_UART_Receive_IT(&huart3, &vRS485.rxByte, 1);
			
		+ Chu y: Ham HAL_UART_RxCpltCallback chi callback 1 lan
		
		
FRAME MAIN READY: 
				FE 02 A0 00 00 00 00 00 00 00 00 00 00 FF
				
FRAME MAIN ERROR:
				FE 02 E0 00 00 00 00 00 00 00 00 00 00 FF
			
	
*/
/**********************************************************************************************************************************/

/******EXTERN*******/
extern UART_HandleTypeDef huart1;
extern flag_irq_t vFlag;


/**********************************************************************************************************************************/
/*******DECLARE VARIABLE********/
rs485_t vRS485;		// variable RS485


/**********************************************************************************************************************************/
/*******FUNCTION********/

/*______________________________________________________________________________*/
	void RS485_Read(rs485_t *pRx){			 //Read Data from RS485
	 //Process receive data 
		switch(pRx->STATE) 
		{
			case  USART_WAIT:                  //Wait for start of new message   
			{
				if(pRx->rxByte == pRx->header){       //check header
					pRx->rxBuff[0] = pRx->rxByte;
					pRx->STATE = USART_READING;      //switch to Reading state    
					pRx->rxPointer = 1;
				}   
			break;
			}  
			//-------------------------------------------------------------------------------------------
			case USART_READING:  
			{
				pRx->rxBuff[pRx->rxPointer] = pRx->rxByte; 			//Read data   
				if(pRx->rxPointer == SIZE_RS485-1)              //Check the last byte
				{    
					if(pRx->rxBuff[pRx->rxPointer] == pRx->ender && pRx->rxBuff[2] == pRx->ID_board){    //Success -> Turn on the Flag and process data			  
						pRx->RxFlag = 1;  
						pRx->cnt_mess_received++;						
					}
					else{ 										//Fail -> Reset buffer, pointer and move to WAIT state                 
						pRx->rxPointer = 0;     
						pRx->RxFlag = 0;
						memset(pRx->rxBuff, 0x00, SIZE_RS485);    
					}  
				}
				else{
					pRx->rxPointer = pRx->rxPointer + 1;  //Increse the pointer    
					if(pRx->rxPointer > SIZE_RS485-1){           //Fail
						pRx->rxPointer = 0;
						pRx->RxFlag = 0;     
						memset(pRx->rxBuff, 0x00, SIZE_RS485);
						pRx->STATE = USART_WAIT;
					}   
				}   
			 break;  
			} 
		}
	}

/*______________________________________________________________________________*/
	void RS485_Init(rs485_t *pRS, uint8_t ID){
		pRS->RxFlag = 0; 
		pRS->rxByte = 0; 
		pRS->rxPointer = 0;
		pRS->STATE = USART_WAIT;
		pRS->cmd_type = 0x00;
		pRS->cnt_mess_received = 0;
		pRS->cnt_mess_send = 0;
		pRS->header = 0xFE;
		pRS->ender = 0xFF;
		pRS->ID_board = ID;
		memset(pRS->rxBuff, 0x00, SIZE_RS485);
		memset(pRS->txBuff, 0x00, SIZE_RS485);
	}
	
/*______________________________________________________________________________*/

void handle_dataRx(rs485_t *pRS, flag_irq_t *pFL){
	if(pRS->rxBuff[3] == MAIN_OK){
		FLAG_Init(pFL);
		pFL->MODE = SNAKE;
		
		//back response
		for(uint8_t index = 0; index < SIZE_RS485; index++)
			pRS->txBuff[index] = pRS->rxBuff[index];
		
		HAL_Delay(1);
		
		HAL_UART_Transmit(&huart1, pRS->txBuff, SIZE_RS485, HAL_MAX_DELAY);
	}
	if(pRS->rxBuff[3] == MAIN_ERROR){
		FLAG_Init(pFL);
		pFL->MODE = NHAY_LED;
		
		//back response
		for(uint8_t index = 0; index < SIZE_RS485; index++)
			pRS->txBuff[index] = pRS->rxBuff[index];
		
		HAL_Delay(1);
		
		HAL_UART_Transmit(&huart1, pRS->txBuff, SIZE_RS485, HAL_MAX_DELAY);
	}

}


/*______________________________________________________________________________*/
/*Ham nay dat trong Main While(1)*/
	
//	if(vRS485.RxFlag == 1){   
//		HAL_GPIO_WritePin(DE_RS485_GPIO_Port, DE_RS485_Pin, GPIO_PIN_SET);
//		vRS485.RxFlag = 0;   
//		
//		/*Xu ly data nhan duoc o day*/
//		

//		//for(uint16_t index=0; index<1000; index++);			//delay with for 200us
//		
//		HAL_UART_Transmit(&huart1, vRS485.txBuff, SIZE_RS485, HAL_MAX_DELAY);
//		HAL_GPIO_WritePin(DE_RS485_GPIO_Port, DE_RS485_Pin, GPIO_PIN_RESET);  
//		vRS485.STATE = USART_WAIT;
//  }

/*______________________________________________________________________________*/
/*Code test, flag_sent dat trong ngat voi chu ky 5ms*/
	
//if(flag_sent >= 5){
//	HAL_GPIO_WritePin(DE_RS485_GPIO_Port, DE_RS485_Pin, GPIO_PIN_SET);
//	flag_sent = 0;
//	vRS485.txBuff[0] = vRS485.header;
//	
//	vRS485.txBuff[1] = 0xAA; 
//	
//	tx_buffer[SIZE_RS485-1] = vRS485.ender;
//	for(uint8_t i = 2; i<SIZE_RS485-3; i++){
//		vRS485.txBuff[i] = i;
//	}
//	vRS485.txBuff[SIZE_RS485-2] = rand() % 0xFF;
//	vRS485.txBuff[SIZE_RS485-3] = rand() % 0xFF;
//	
//	HAL_UART_Transmit(&huart3, vRS485.txBuff, SIZE_RS485, HAL_MAX_DELAY);
//	
//	HAL_GPIO_WritePin(DE_RS485_GPIO_Port, DE_RS485_Pin, GPIO_PIN_RESET);
//	vRS485.STATE = USART_WAIT;
//}




