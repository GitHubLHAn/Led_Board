#include "scan_led.h"
#include <string.h>
/*
NOTE: Code nay su dung cho mach 23 led
	
*/
/**********************************************************************************************************************************/

/*Extern*/
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

extern UART_HandleTypeDef huart1;

/*Declare struct variable*/
flag_irq_t vFlag;

uint8_t cnt_led_runningMode = 24;

static bool d = true;

uint8_t vBattery = 0;
uint8_t vTempMode = 0;

/*Function*/
/*_____________________________________________________________________*/
	/*Delay micro second*/
	void delay_us(uint16_t us){
		__HAL_TIM_SetCounter(&htim1,0);
		while(__HAL_TIM_GetCounter(&htim1) < us);
	}
/*_____________________________________________________________________*/
	/*Delay mili second*/
	void delay_ms(uint16_t ms){
		for(uint16_t i = 0; i<ms; i++){
			delay_us(1000);
		}
	}
/*_____________________________________________________________________*/
	/*Delay second*/
	void delay_s(uint16_t s){
			for(uint16_t i = 0; i < s; i++){
			delay_ms(1000);
		}
	}
/*_____________________________________________________________________*/
	/*On Led*/
	void led1(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	}
	
	void led2(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
	}
	
	void led3(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
	}
	
	void led4(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
	}
	
	void led5(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_SET);
	}

	void led6(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, GPIO_PIN_SET);
	}
	
	void led7(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED7_GPIO_Port, LED7_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED7_GPIO_Port, LED7_Pin, GPIO_PIN_SET);
	}
	
	void led8(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED8_GPIO_Port, LED8_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED8_GPIO_Port, LED8_Pin, GPIO_PIN_SET);
	}
	
	void led9(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED9_GPIO_Port, LED9_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED9_GPIO_Port, LED9_Pin, GPIO_PIN_SET);
	}
	
	void led10(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED10_GPIO_Port, LED10_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED10_GPIO_Port, LED10_Pin, GPIO_PIN_SET);
	}
	
	void led11(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED11_GPIO_Port, LED11_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED11_GPIO_Port, LED11_Pin, GPIO_PIN_SET);
	}
	
	void led12(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED12_GPIO_Port, LED12_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED12_GPIO_Port, LED12_Pin, GPIO_PIN_SET);
	}
	
	void led13(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED13_GPIO_Port, LED13_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED13_GPIO_Port, LED13_Pin, GPIO_PIN_SET);
	}
	
	void led14(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED14_GPIO_Port, LED14_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED14_GPIO_Port, LED14_Pin, GPIO_PIN_SET);
	}
	
	void led15(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED15_GPIO_Port, LED15_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED15_GPIO_Port, LED15_Pin, GPIO_PIN_SET);
	}
	
	void led16(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED16_GPIO_Port, LED16_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED16_GPIO_Port, LED16_Pin, GPIO_PIN_SET);
	}
	
	void led17(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED17_GPIO_Port, LED17_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED17_GPIO_Port, LED17_Pin, GPIO_PIN_SET);
	}
	
	void led18(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED18_GPIO_Port, LED18_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED18_GPIO_Port, LED18_Pin, GPIO_PIN_SET);
	}
	
	void led19(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED19_GPIO_Port, LED19_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED19_GPIO_Port, LED19_Pin, GPIO_PIN_SET);
	}
		
	void led20(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED20_GPIO_Port, LED20_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED20_GPIO_Port, LED20_Pin, GPIO_PIN_SET);
	}
		
	void led21(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED21_GPIO_Port, LED21_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED21_GPIO_Port, LED21_Pin, GPIO_PIN_SET);
	}
		
	void led22(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED22_GPIO_Port, LED22_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED22_GPIO_Port, LED22_Pin, GPIO_PIN_SET);
	}
		
	void led23(Status_e status){
		if(status == on)
			HAL_GPIO_WritePin(LED23_GPIO_Port, LED23_Pin, GPIO_PIN_RESET);
		else
			HAL_GPIO_WritePin(LED23_GPIO_Port, LED23_Pin, GPIO_PIN_SET);
	}
	
/*_____________________________________________________________________*/
	/*On Full Led*/
	void Full_LED(Status_e status){	
		led1(status);
		led2(status);
		led3(status);
		led4(status);
		led5(status);
		led6(status);
		led7(status);
		led8(status);
		led9(status);
		led10(status);
		led11(status);
		led12(status);
		led13(status);
		led14(status);
		led15(status);
		led16(status);
		led17(status);
		led18(status);
		led19(status);
		led20(status);
		led21(status);
		led22(status);
		led23(status);
	}
/*_____________________________________________________________________*/	
	void choose_led(uint8_t led, Status_e status){
		switch (led){
			case 1:
				led1(status);
				break;
			case 2:
				led2(status);
				break;
			case 3:
				led3(status);
				break;
			case 4:
				led4(status);
				break;
			case 5:
				led5(status);
				break;
			case 6:
				led6(status);
				break;
			case 7:
				led7(status);
				break;
			case 8:
				led8(status);
				break;
			case 9:
				led9(status);
				break;
			case 10:
				led10(status);
				break;
			case 11:
				led11(status);
				break;
			case 12:
				led12(status);
				break;
			case 13:
				led13(status);
				break;
			case 14:
				led14(status);
				break;
			case 15:
				led15(status);
				break;
			case 16:
				led16(status);
				break;
			case 17:
				led17(status);
				break;
			case 18:
				led18(status);
				break;	
			case 19:
				led19(status);
				break;
			case 20:
				led20(status);
				break;
			case 21:
				led21(status);
				break;
			case 22:
				led22(status);
				break;
			case 23:
				led23(status);
				break;			
		}
	}
/*_____________________________________________________________________*/		
	void nhayled(void){ 	// nhay led
		static bool sts = true;
		if(sts){
			Full_LED(on);
			sts = false;
		}
		else{
			Full_LED(off);
			sts = true;
		}

	}
/*_____________________________________________________________________*/
	void quetled(void){ 	// nhay led
		static uint8_t current_led = 1;
		Full_LED(off);
		
		choose_led(current_led, on);
		current_led++;
		if(current_led == 18)
			current_led = 1;
		//delay_ms(delay);
		
	}
/*_____________________________________________________________________*/	
	void hai_ben(flag_irq_t *pFL){ 	
		Full_LED(off);
		
		if(pFL->dir){
			pFL->cur_led_haiben++;
			if(pFL->cur_led_haiben == 13)
				pFL->dir = false;
		}
		
		if(!pFL->dir){
			pFL->cur_led_haiben--;
			if(pFL->cur_led_haiben == 0)
				pFL->dir = true;
		}
		
		choose_led(pFL->cur_led_haiben, on);
		choose_led(24 - pFL->cur_led_haiben, on);
		
		//delay_ms(delay);
	}
/*_____________________________________________________________________*/	
	void loading(flag_irq_t *pFL){
		Full_LED(off);
	
		for(uint8_t cnt = 23; cnt >= pFL->cur_led_loading; cnt--){
			choose_led(cnt, on);
		}
	
		pFL->cur_led_loading--;
		if(pFL->cur_led_loading == 0)
			pFL->cur_led_loading = 24;
	
	}
/*_____________________________________________________________________*/
	void snake(flag_irq_t *pFL){
		Full_LED(off);
		
		choose_led(pFL->cur_led_snake, on);
		choose_led(pFL->cur_led_snake - 1, on);
		choose_led(pFL->cur_led_snake - 2, on);
		choose_led(pFL->cur_led_snake - 3, on);
		choose_led(pFL->cur_led_snake - 4, on);
		
		pFL->cur_led_snake--;
		if(pFL->cur_led_snake == 0)
			pFL->cur_led_snake = 27;
	}
/*_____________________________________________________________________*/
	void FLAG_Init(flag_irq_t *pFL){
		pFL->MODE = TWO_SIDE;
		pFL->cur_led_haiben = 0;
		pFL->cur_led_loading = 23;
		pFL->cur_led_snake = 27;
		pFL->dir = true;
		
		pFL->running = 0;
		pFL->haiBen = 0;
		pFL->nhapNhay = 0;
		pFL->quetLed = 0;
		pFL->loading = 0;
		pFL->snake = 0;
	
		pFL->cnt_haiBen = 0;
		pFL->cnt_snake = 0;
		
		pFL->battery = 0;
	}

	
/*_____________________________________________________________________*/
	void HANDLE_MODE(flag_irq_t *pFL)
	{
		//--------------------------------------------------------
		switch(pFL->MODE)
		{
			//-------------
			case TWO_SIDE:
			{
				if(++pFL->haiBen >= 20){
					pFL->haiBen = 0;
					hai_ben(pFL);
				if(++pFL->cnt_haiBen >= 74){			// thuc hien 3 lan
					pFL->cnt_haiBen = 0;
					pFL->MODE = LOADING;
				}
			}
				break;
			}
			//-------------
			case SCAN_LED:
			{
				if(++pFL->quetLed >= 20){
					pFL->quetLed = 0;
					quetled();
				}
				break;
			}
			//-------------
			case LOADING:
			{
				if(++pFL->loading >= 50){
					pFL->loading = 0;
					loading(pFL);
				}
				break;
			}
			//-------------
			case SNAKE:
			{
				if(++pFL->snake >= 20){
					pFL->snake = 0;
					snake(pFL);
					if(++pFL->cnt_snake >= 81){			// thuc hien 3 lan
						pFL->cnt_snake = 0;
						Full_LED(off);
						pFL->MODE = RUNNING;
					}
				}
				break;
			}
			//-------------
			case BLINK_LED:
			{
				if(++pFL->nhapNhay >= 200){
					pFL->nhapNhay = 0;
					nhayled();
				}
				break;
			}
			//-------------
			case BATTERY:
			{
				for(uint8_t i=1; i< vBattery + 1; i++)
					choose_led(i,on);
				if(++pFL->battery >= 1000){
					pFL->battery = 0;
					Full_LED(off);
					pFL->MODE = vTempMode;
				}
				break;
			}
			//-------------
			case FULL_ON:
			{
				Full_LED(on);
				break;
			}
			//-------------
			case FULL_OFF:
			{
				Full_LED(off);
				break;
			}
			//-------------
			case RUNNING:
			{
				if(d){
						
					if(++pFL->running >= 30){
						Full_LED(off);
						d = false;
						pFL->running = 0;
					}
				}
				else{
					if(++pFL->running >= 3000){
						pFL->running = 0;
						
						choose_led(10, on);
						choose_led(11, on);
						choose_led(12, on);
						choose_led(13, on);
						choose_led(14, on);
						d = true;		
					}			
				}
				break;
			}	
		}
	}
/*_____________________________________________________________________*/
