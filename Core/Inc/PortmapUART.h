/*
 * PortmapUART.h
 *
 *  Created on: Apr 29, 2021
 *      Author: hydra
 */

#ifndef PORTMAPUART_H_
#define PORTMAPUART_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "stm32f7xx_hal.h"

#ifdef __cplusplus
}
#endif


class PortmapUART
{
	public:
		PortmapUART();
		PortmapUART(UART_HandleTypeDef h);
		void setHandler(UART_HandleTypeDef h);
		UART_HandleTypeDef getHandler();
	private:
		UART_HandleTypeDef _handler;
};

#endif /* PORTMAPUART_H_ */
