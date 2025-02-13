/*
 * rs485.c 
 * Created on: 19-Sep-2024
 * Author: Le Huu An
 * Used for: Led Board for AGV 30Kg
 */

#include "rs485.h"
#include <string.h>
#include "scan_led.h"

/*
NOTE: 
	+ Code nay cho robot AGV Sorting 30Kg, giong voi loai 5Kg
	
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

/*========================================================================================*\
 *																			INIT RS485	  																		*
\*========================================================================================*/

	void RS485_Init(rs485_t *pRS, uint8_t ID)
	{
		pRS->RxFlag = 0; 
		pRS->TxFlag = 0;
		pRS->rxByte = 0; 
		pRS->rxPointer = 0;
		pRS->STATE = USART_WAIT;
		pRS->cmd_type = 0x00;
		pRS->cnt_mess_received = 0;
		pRS->cnt_mess_sent = 0;
		pRS->cnt_byte_sent = 0;
		
		pRS->header = 0xFE;
		pRS->ender = 0xFF;
		pRS->ID_board = ID;
		memset(pRS->rxBuff, 0x00, SIZE_RS485);
		memset(pRS->txBuff, 0x00, SIZE_RS485);
		
		HAL_GPIO_WritePin(DE_485_GPIO_Port, DE_485_Pin, GPIO_PIN_RESET);
		HAL_UART_Receive_IT(&huart1, &vRS485.rxByte, 1);
	}
	
/*========================================================================================*\
 *																			RESET RS485	  																		*
\*========================================================================================*/
	
	void RS485_Reset(rs485_t *pRS)
	{
		pRS->RxFlag = 0; 
		pRS->rxPointer = 0;
		pRS->STATE = USART_WAIT;
		
		HAL_GPIO_WritePin(DE_485_GPIO_Port, DE_485_Pin, GPIO_PIN_RESET);
		HAL_UART_Receive_IT(&huart1, &vRS485.rxByte, 1);
	}
	
/*========================================================================================*\
 *																			READ RS485	  																		*
\*========================================================================================*/
	
	uint8_t RS485_Read(rs485_t *pRx)
	{			
		uint8_t result = RS485_OK;
		
	 //Process receive data 
		switch(pRx->STATE) 
		{
			/*---------------------------------------------------------------------*/
			//Wait for start of new message  
			case  USART_WAIT:                   
			{
				//check header
				if(pRx->rxByte == pRx->header)
				{       
					pRx->rxBuff[0] = pRx->rxByte;
					pRx->STATE = USART_READING;      //switch to Reading state    
					pRx->rxPointer = 1;
				}   
			break;
			}  
			/*---------------------------------------------------------------------*/
			case USART_READING:  
			{
				//Read data 
				pRx->rxBuff[pRx->rxPointer] = pRx->rxByte; 	

				//Check the last byte
				if(pRx->rxPointer == SIZE_RS485-1)              
				{    
					//Success -> Turn on the Flag and process data
					if(pRx->rxBuff[pRx->rxPointer] == pRx->ender && pRx->rxBuff[2] == pRx->ID_board)    			  
					{	
						pRx->RxFlag = 1;  
						pRx->cnt_mess_received++;		
						pRx->cmd_type = pRx->rxBuff[3];
						HAL_GPIO_WritePin(DE_485_GPIO_Port, DE_485_Pin, GPIO_PIN_SET);						
					}
					else{ 										//Fail -> Reset buffer, pointer and move to WAIT state                 
						result = RS485_WRONG;					
					}  
				}
				else{
					pRx->rxPointer = pRx->rxPointer + 1;  //Increase the pointer    
					if(pRx->rxPointer > SIZE_RS485-1){           //Fail
						result = RS485_WRONG;
					}   
				}   
			 break;  
			} 
		}
		
		if(result == RS485_WRONG)
			RS485_Reset(pRx);
		else
			HAL_UART_Receive_IT(&huart1, &vRS485.rxByte, 1);
		
		return result;
	}


/*========================================================================================*\
 *													HANDLE DATA RECEIVED FROM RS485	 															*
\*========================================================================================*/

void Handle_Data_RS485_Rx(rs485_t *pRS, flag_irq_t *pFL)
{
	if(pRS->RxFlag == 1)
	{
		/*--------------------------------------------------------------------*/
		if(pRS->cmd_type == MAIN_OK)
		{
			FLAG_Init(pFL);
			pFL->MODE = SNAKE;
			
			//back response
			for(uint8_t index = 0; index < SIZE_RS485; index++)
				pRS->txBuff[index] = pRS->rxBuff[index];
			
			delay_ms(1);
			
			pRS->TxFlag = 1;
		}
		/*--------------------------------------------------------------------*/
		if(pRS->cmd_type == MAIN_ERROR)
		{
			FLAG_Init(pFL);
			pFL->MODE = BLINK_LED;
			
			//back response
			for(uint8_t index = 0; index < SIZE_RS485; index++)
				pRS->txBuff[index] = pRS->rxBuff[index];
			
			delay_ms(1);
			
			pRS->TxFlag = 1;
		}
		/*--------------------------------------------------------------------*/
		if(pRS->cmd_type == MAIN_SLEEP)
		{
			FLAG_Init(pFL);
			pFL->MODE = FULL_OFF;
		
			//back response
			for(uint8_t index = 0; index < SIZE_RS485; index++)
				pRS->txBuff[index] = pRS->rxBuff[index];
			
			delay_ms(1);

			pRS->TxFlag = 1;
		}
		
		/*--------------------------------------------------------------------*/
		if(pRS->cmd_type == MAIN_BATTERY)
		{
			Full_LED(off);
			vTempMode = pFL->MODE;
			pFL->MODE = BATTERY;
			
			if(pRS->rxBuff[4] > 100)
				vBattery  = 27;
			else
				vBattery = pRS->rxBuff[4]*27/100;
		
			delay_ms(1);

			pRS->TxFlag = 1;
		}
		
		
		/*--------------------------------------------------------------------*/
		if(pRS->cmd_type == TEST_JIG)
		{
			//back response
			for(uint8_t index = 0; index < SIZE_RS485; index++)
				pRS->txBuff[index] = pRS->rxBuff[index];
			
			delay_ms(1);

			pRS->TxFlag = 1;
		}
		
		pRS->RxFlag = 0;
	}

}

/*========================================================================================*\
 *														TRANSMIT EACH BYTE VIA RS485 																*
\*========================================================================================*/

/*Transmit each byte via USART*/
	static uint8_t isSending = 0;
	uint8_t TC = 0;
	
	void RS485_SendData(rs485_t *pRS, USART_TypeDef * USARTx, uint16_t timeout)
	{
		if(pRS->TxFlag == 1)
		{
			uint8_t cnt_clear_TC = 10;
			TC = USARTx->SR & USART_SR_TC;
			
			if((isSending == 1) && ((USARTx->SR & USART_SR_TC) != 0)){
				if(++pRS->cnt_byte_sent == SIZE_RS485){
					pRS->TxFlag = 0;
					pRS->cnt_mess_sent++;
					pRS->cnt_byte_sent = 0;
					HAL_GPIO_WritePin(DE_485_GPIO_Port, DE_485_Pin, GPIO_PIN_RESET);
					RS485_Reset(pRS);
				}
				__HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_TC);	
				isSending = 0;
			}
			
			if(isSending == 0 && pRS->TxFlag == 1){
				__HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_TC);		
				
				// Confirm TC is off
				while(!(USARTx->SR & USART_SR_TXE)){		
					if(--cnt_clear_TC == 0) break;
				}
				
				// transmit byte
				USARTx->DR = pRS->txBuff[pRS->cnt_byte_sent];
				
				isSending = 1;
			}	
		}
	}

/**********************************************************************************************************************************/
	



