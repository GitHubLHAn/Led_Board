/*
 * uart.h 
 * Created on: 25-June-2024
 * Author: Le Huu An
 */

#ifndef UART_H_
#define UART_H_

#include "main.h"

/*DEFINE*/
	#define USART_RX_END 2
	#define USART_READING 1
	#define USART_WAIT 0
	

	#define USART_OK		 1
	#define USART_WRONG	 0

	#define UART_NUM_BYTE 12


	#define CMD_CHECK_CODE_TEST	0x50
	#define CMD_CHECK_CODE_MAIN	0x70
	
	#define BOARD_BOOST					0x01
	#define BOARD_CENTER				0x02
	#define BOARD_LED						0x03
	#define BOARD_AP						0x04
	#define BOARD_SENSOR				0x05
	#define BOARD_CHARGER				0x06
	#define BOARD_DOC_CHARGER		0x07


//#define FLAG_DEBUG
			

/************************************************************************************/
/*DECLARE STRUCT*/
	typedef struct{
		uint8_t rxBuff[UART_NUM_BYTE];
		uint8_t txBuff[UART_NUM_BYTE];
		uint8_t RxFlag, TxFlag;
		uint8_t STATE;
		uint8_t rxByte, rxPointer;
		uint8_t header_High, header_Low;						
		uint8_t	CS_byte;											// checksum byte
		uint16_t cnt_mess_sent;									
		uint16_t cnt_mess_received;			
		
		uint8_t	cmd_type;
		
		UART_HandleTypeDef *huart;
		
	}uart_t;

	
	
/*EXTERN VARIABLE*/	
	extern uart_t vUART3_JIG;					
	

/*DECLARE FUNCTION*/
	uint8_t UART_Read(uart_t *pRx);
	
	void UART_Init(uart_t *pUART, UART_HandleTypeDef *huart);
	
	uint8_t Handle_CMD_From_Jig(uart_t *pUART);
	
	uint8_t Cal_CheckSum(uint8_t *Buff, uint8_t length);

	


/************************************************************************************/
#endif /* UART_H_*/

