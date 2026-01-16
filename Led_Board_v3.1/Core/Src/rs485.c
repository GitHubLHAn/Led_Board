#include <rs485.h>
#include <string.h>
#include "scan_led.h"

/*
NOTE: 

		+ Put in CODE BEGIN 2
			HAL_UART_Receive_IT(&huart3, &vRS485.rxByte, 1);
			
		+ Chu y: Ham HAL_UART_RxCpltCallback chi callback 1 lan
		
			
	
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
 *																			CHECK CRC16  																		*
\*========================================================================================*/

uint16_t Modbus_CRC16(uint8_t *buf, uint16_t len)
{
    uint16_t crc = 0xFFFF;
    
    for (uint16_t pos = 0; pos < len; pos++) {
        crc ^= buf[pos];  // XOR byte vao LSB cua CRC

        for (uint8_t i = 0; i < 8; i++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;  // Polynomial reversed
            } else {
                crc >>= 1;
            }
        }
    }

    return crc; // CRC dung little endian: LSB truoc
}
/*========================================================================================*\
 *																			READ RS485	  																		*
\*========================================================================================*/
	uint16_t nhan_fe = 0;
uint16_t co_led = 0;
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
					nhan_fe++;
					pRx->rxBuff[0] = pRx->rxByte;
					pRx->STATE = USART_CHECK_LED;      //switch to Reading state    
					pRx->rxPointer = 1;
				}   
			break;
			}  
			/*---------------------------------------------------------------------*/
			//check ma lenh mach led
			case  USART_CHECK_LED:                   
			{
				if(pRx->rxByte == pRx->ID_board)
				{     
					co_led++;
					pRx->rxBuff[1] = pRx->rxByte;
					pRx->STATE = USART_READING;      //switch to Reading state    
					pRx->rxPointer = 2;
				}   
				else{
					RS485_Reset(pRx);
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
					uint16_t check_crc16 = Modbus_CRC16(pRx->rxBuff, SIZE_RS485-2);			  
					
					if(pRx->rxBuff[pRx->rxPointer] == ((check_crc16 >> 8) & 0xFF) && 	// them check crc16
																			pRx->rxBuff[pRx->rxPointer-1] == (check_crc16 & 0xFF))		
					{	
							pRx->cnt_mess_received++;	
							pRx->RxFlag = 1;
							pRx->cmd_type = pRx->rxBuff[2];			// dich sang 1 byte
							HAL_GPIO_WritePin(DE_485_GPIO_Port, DE_485_Pin, GPIO_PIN_SET);		
							Handle_Data_RS485_Rx(&vRS485, &vFlag);								
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
		else{
			
			HAL_UART_Receive_IT(&huart1, &vRS485.rxByte, 1);
		}
		
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
		if(pRS->cmd_type == MAIN_DEBUG_L)
		{
			FLAG_Init(pFL);
			pFL->MODE = DB_L;
			
			//back response
			for(uint8_t index = 0; index < SIZE_RS485; index++)
				pRS->txBuff[index] = pRS->rxBuff[index];
			
			delay_ms(1);
			
			pRS->TxFlag = 1;
		}
		if(pRS->cmd_type == MAIN_DEBUG_R)
		{
			FLAG_Init(pFL);
			pFL->MODE = DB_R;
			
			//back response
			for(uint8_t index = 0; index < SIZE_RS485; index++)
				pRS->txBuff[index] = pRS->rxBuff[index];
			
			delay_ms(1);
			
			pRS->TxFlag = 1;
		}
		if(pRS->cmd_type == MAIN_DEBUG_LR)
		{
			FLAG_Init(pFL);
			pFL->MODE = DB_LR;
			
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
			Full_LED(off);
			
			if(pRS->rxBuff[4] > 100)
				vBattery  = 23;
			else
				vBattery = pRS->rxBuff[3]*23/100;
		
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
	



