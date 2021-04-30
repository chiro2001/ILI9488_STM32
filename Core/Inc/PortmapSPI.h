/*
 * PortmapSPI.h
 *
 *  Created on: Apr 29, 2021
 *      Author: hydra
 */

#ifndef PORTMAPSPI_H_
#define PORTMAPSPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "stm32f7xx_hal.h"

#ifdef __cplusplus
}
#endif


class PortmapSPI
{
	public:
		PortmapSPI();
		PortmapSPI(SPI_HandleTypeDef *h);
		void setHandler(SPI_HandleTypeDef *h);
		void send(uint8_t *d);
		SPI_HandleTypeDef *getHandler();
	private:
		SPI_HandleTypeDef *_handler;

};

#endif /* PORTMAPSPI_H_ */
