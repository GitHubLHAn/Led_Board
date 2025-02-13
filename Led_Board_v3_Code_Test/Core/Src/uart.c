/*
 * uart.h 
 * Created on: 15-July-2024
 * Author: Le Huu An
 * Used for: Led Board
 */


#include <uart.h>

#include <string.h>
#include <stdio.h>

/*
BRIEF NOTE: 
	
*/
/*===============================================================================================================================*/
/******EXTERN*******/
	extern UART_HandleTypeDef huart3;

/*===============================================================================================================================*/
/*******DECLARE VARIABLE********/

	uart_t vUART3_JIG;						// communicate to CT Board

/*===============================================================================================================================*/
/*******FUNCTION********/
	

/*______________________________________________________________________________*/
/*Calculating checksum*/	
	uint8_t Cal_CheckSum(uint8_t *Buff, uint8_t length){
		uint8_t result = 0x00;
		
		for(uint8_t i=0; i<length-1; i++){
			result += Buff[i];
		}
		return result;
	}

/*______________________________________________________________________________*/
/*Initiating for UART variable*/
	void UART_Init(uart_t *pUART, UART_HandleTypeDef *huart){
		pUART->huart = huart;
		
		pUART->RxFlag = 0;	pUART->TxFlag = 0;	
		pUART->rxByte = 0; 
		pUART->rxPointer = 0;
		pUART->STATE = USART_WAIT;
		pUART->cnt_mess_received = 0;
		
		pUART->cnt_mess_sent = 0;
		pUART->header_High = 0xAB;
		pUART->header_Low = 0xCD;
		pUART->CS_byte = 0x00;
		
		pUART->cmd_type = 0;
		
		memset(pUART->rxBuff, 0x00, UART_NUM_BYTE);
		memset(pUART->txBuff, 0x00, UART_NUM_BYTE);
		
		HAL_UART_Receive_IT(pUART->huart, &pUART->rxByte, 1);
	}
/*______________________________________________________________________________*/
/*Reset UART*/
	void UART_Reset(uart_t *pUART){
		pUART->RxFlag = 0;
		pUART->rxByte = 0; 
		pUART->rxPointer = 0;
		pUART->STATE = USART_WAIT;
		
		HAL_UART_Receive_IT(pUART->huart, &pUART->rxByte, 1);
	}	

/*______________________________________________________________________________*/
/*Read Data from UART*/
	uint8_t UART_Read(uart_t *pRx){		
		uint8_t result = USART_OK;
	 //Process receive data 
		switch(pRx->STATE) 
		{
			/*Wait for start of new message*/
			case  USART_WAIT:                     
			{
				/*Check header high*/
				if(pRx->rxByte == pRx->header_High){       
					pRx->rxBuff[0] = pRx->rxByte;
					
					/*Switch to Reading state  */
					pRx->STATE = USART_READING;    
					pRx->rxPointer = 1;
				}   
			break;
			}  
			//-------------------------------------------------------------
			case USART_READING:  
			{
				/*Read data each byte*/
				pRx->rxBuff[pRx->rxPointer] = pRx->rxByte; 			
				
				/*If Reading enough byte*/
				if(pRx->rxPointer == UART_NUM_BYTE-1)              
				{    
					/*Check Header Low*/
					if(pRx->rxBuff[1] == pRx->header_Low && pRx->rxBuff[2] == CMD_CHECK_CODE_TEST)
					{	
						/*Cal checksum of received mess*/
						pRx->CS_byte = Cal_CheckSum(pRx->rxBuff, UART_NUM_BYTE);
						if(pRx->rxBuff[UART_NUM_BYTE-1] == pRx->CS_byte){
							pRx->RxFlag = 1;  
							pRx->cnt_mess_received++;
						}
						else result = USART_WRONG;		//wrong checksum byte
					}
					else result = USART_WRONG;		// wrong header low				
				}
				else
				{
					/*Increase the Pointer*/
					pRx->rxPointer = pRx->rxPointer + 1; 
					
					/*Fail*/
					if(pRx->rxPointer > UART_NUM_BYTE-1)
						result = USART_WRONG;
				}   
				break;  
			} 
		}
		
		if(result == USART_WRONG)	
			UART_Reset(pRx);
		else
			/*Set uart receive interrupt again*/
			HAL_UART_Receive_IT(pRx->huart, &pRx->rxByte, 1);
		
		return result;
	}
	

	
/********************************************************************************/ 
/********************************************************************************/
/*Handle the command mess from JIG*/
	uint8_t Handle_CMD_From_Jig(uart_t *pUART)
	{
		if(pUART->RxFlag == 1)
		{
			for(uint8_t j=0; j<UART_NUM_BYTE; j++)
				pUART->txBuff[j] = pUART->rxBuff[j];
		
			HAL_UART_Transmit(&huart3, pUART->txBuff, UART_NUM_BYTE, HAL_MAX_DELAY);
			UART_Reset(pUART);
		}
	}	
	
	
	
	
/*______________________________________________________________________________*/
	
	
	
	


/*______________________________________________________________________________*/




