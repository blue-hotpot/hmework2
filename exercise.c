#include "exercise.h"
#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>

//pa2--usart1_tx
//pa3--usart1_rx

uint32_t number=0;

void usart_config(uint32_t rate)
{
	rcu_periph_clock_enable(RCU_GPIOA);//USART_TX引脚设置
	rcu_periph_clock_enable(RCU_GPIOA);//USART_RX引脚设置
	
	gpio_af_set(GPIOA,GPIO_AF_7,GPIO_PIN_9);
	gpio_af_set(GPIOA,GPIO_AF_7,GPIO_PIN_10);
	
	gpio_mode_set(GPIOA,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_9);
	gpio_mode_set(GPIOA,GPIO_MODE_AF,GPIO_PUPD_PULLUP,GPIO_PIN_10);
	
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_9);
	gpio_output_options_set(GPIOA,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_10);
	
	rcu_periph_clock_enable(RCU_USART0);
	usart_deinit(USART0);
	usart_baudrate_set(USART0,rate);
	//波特率设置
	usart_parity_config(USART0,USART_PM_NONE);
	//奇偶位设置
	usart_word_length_set(USART0,USART_WL_8BIT);
	//数据位设置
	usart_stop_bit_set(USART0,USART_STB_1BIT);
	//停止位设置
	usart_enable(USART0);
	
	usart_transmit_config(USART0,USART_TRANSMIT_ENABLE);
//	usart_receive_config(USART1,USART_TRANSMIT_ENABLE);
//	
//	nvic_irq_enable(USART1_IRQn,2,2);
//	usart_interrupt_enable(USART1,USART_INT_TBE);
}

//void USART1_IRQHandler(void)
//{
//	if(usart_flag_get(USART1,USART_FLAG_TBE)==SET)
//	{
//		printf("the data transmit sccess/r/n");
//	}
//}

int fputc (int data,FILE *f)
{
	usart_data_transmit(USART0,(uint32_t)data);
	while(usart_flag_get(USART0,USART_FLAG_TBE)==RESET)
	{
	}
	return data;
}

void bsp_led_config(led_type ledx)
{
	rcu_periph_clock_enable(LED_RCU[ledx]);
	gpio_mode_set(LED_PORT[ledx],GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,LED_PIN[ledx]);
	gpio_output_options_set(LED_PORT[ledx],GPIO_OTYPE_PP,GPIO_OSPEED_25MHZ,LED_PIN[ledx]);
}

void key_interrupt(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_SYSCFG);
	
	syscfg_exti_line_config(EXTI_SOURCE_GPIOA,EXTI_SOURCE_PIN0);
	
	nvic_irq_enable(EXTI0_IRQn,3,3);
	
	exti_init(EXTI_0,EXTI_INTERRUPT,EXTI_TRIG_BOTH);
	exti_interrupt_enable(EXTI_0);
	exti_flag_clear(EXTI_0);
}


void EXTI0_IRQHandler(void)
{
	if(exti_interrupt_flag_get(EXTI_0)==SET)
	{
		if(gpio_input_bit_get(GPIOA,GPIO_PIN_0)==SET)
		{
			gpio_bit_toggle(LED_PORT[number],LED_PIN[number]);
			number=(number+1)%4;
			printf("asakie+5153245A\r\n");
		}
		while(gpio_input_bit_get(GPIOA,GPIO_PIN_0)==SET)
		{
		}
		exti_interrupt_flag_clear(EXTI_0);
	}
}

