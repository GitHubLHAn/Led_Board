/*
 * USER.h
 * Created on: 21-Mar-2024
 * Author: Le Huu An
 */

#ifndef SCAN_LED_H_
#define SCAN_LED_H_

#include<main.h>
/*Include the type of stm32*/
#include <stdbool.h>


/*DEFINE*/
#define LENGTH_RX 16

#define FULL_OFF 0x01
#define FULL_ON 0x02
#define NHAY_LED 0x03
#define XEN_KE 0x04
#define QUET_LED 0x05
#define HAI_BEN 0x06
#define LOADING 0x07
#define SNAKE 0x08
#define RUNNING 0x09


/************************************************************************************/
/*DECLARE STRUCT*/
typedef enum{
	on, off
}Status_e;


typedef struct
{
	volatile uint8_t MODE;
	volatile uint8_t cur_led_snake, cur_led_loading, cur_led_haiben;
	volatile bool dir;
	
	volatile uint16_t quetLed;
	volatile uint16_t haiBen, cnt_haiBen;
	volatile uint16_t nhapNhay;
	volatile uint16_t loading;
	volatile uint16_t running;
	volatile uint16_t snake, cnt_snake;
}flag_irq_t;


	
/*DECLARE FUNCTION*/
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);
void delay_s(uint16_t s);

void Full_LED(Status_e statu);

void nhayled(void);
void quetled(void);
void xenke(void);
void hai_ben(flag_irq_t *pFL);
void loading(flag_irq_t *pFL);
void snake(flag_irq_t *pFL);


void FLAG_Init(flag_irq_t *pFL);

void HANDLE_MODE(flag_irq_t *pFL);

/************************************************************************************/
#endif /* SCAN_LED_H */

