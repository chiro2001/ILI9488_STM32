/*
 * PortmapSPI.cpp
 *
 *  Created on: Apr 29, 2021
 *      Author: hydra
 */

#include "PortmapSPI.h"

PortmapSPI::PortmapSPI()
{

}
PortmapSPI::PortmapSPI(SPI_HandleTypeDef *h)
{
	_handler=h;
}
void PortmapSPI::setHandler(SPI_HandleTypeDef *h)
{
	_handler = h;
}
SPI_HandleTypeDef *PortmapSPI::getHandler()
{
	return _handler;
}
void PortmapSPI::send(uint8_t *d)
{
	HAL_SPI_Transmit(_handler, d,1,1);
	//HAL_SPI_Transmit_DMA(_handler, d, 1);
	/*uint8_t tmp = *d;
	for (uint8_t bit = 0x80; bit; bit >>= 1)
	{
		if (tmp & bit)
		{

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
		}

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	}*/
}

