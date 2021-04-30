/*
 * PortmapIO.h
 *
 *  Created on: Apr 29, 2021
 *      Author: hydra
 */

#ifndef PORTMAPIO_H_
#define PORTMAPIO_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "stm32f7xx_hal.h"

#ifdef __cplusplus
}
#endif


class PortmapIO
{
	public:
		PortmapIO ();
		PortmapIO (GPIO_TypeDef *port, uint16_t pin);
//		virtual ~Portmap ();
		void TogglePin();
		void setLow();
		void setHigh();
		uint8_t getStatus();
	private:
		GPIO_TypeDef * _port;
		uint16_t _pin;
		uint8_t _status;
};

#endif /* PORTMAPIO_H_ */
