/*
 * Trimpot.c
 *
 *  Created on: 11 de abr de 2018
 *      Author: samuelpereira
 */

#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"

/* FreeRTOS.org includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo includes. */
#include "basic_io.h"

#include "Trimpot.h"

static callback_t tx_callback;

void Trimpot_task( void *pvParameters );

void Trimpot_init(void)
{
	PINSEL_CFG_Type PinCfg;

	/*
	 * Init ADC pin connect
	 * AD0.0 on P0.23
	 */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 23;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);

	/* Configuration for ADC :
	 * 	Frequency at 1Mhz
	 *  ADC channel 0, no Interrupt
	 */
	ADC_Init(LPC_ADC, 1000000);
	ADC_IntConfig(LPC_ADC,ADC_CHANNEL_0,DISABLE);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_0,ENABLE);

	xTaskCreate( Trimpot_task, "Trimpot", 192, NULL, 1, NULL );
}

void Trimpot_setCallback(callback_t c)
{
	if (c != NULL)
	{
		tx_callback = c;
	}
}

uint16_t Trimpot_read(void)
{
	uint16_t trim;

	ADC_StartCmd(LPC_ADC,ADC_START_NOW);
	//Wait conversion complete
	while (!(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_0,ADC_DATA_DONE)));
	trim = ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_0);

	return trim;
}

void Trimpot_task( void *pvParameters )
{
	message_t msg;

	while(1)
	{
		memset(&msg, 0x00, sizeof(msg));
		msg.source = TRIM;

		vPortEnterCritical();
		msg.payload[0] = (int16_t)Trimpot_read();
		vPortExitCritical();

		if (tx_callback != NULL)
		{
			tx_callback(msg);
		}

		vTaskDelay( 800 / portTICK_RATE_MS );
	}
}


