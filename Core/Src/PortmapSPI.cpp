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
	HAL_SPI_Transmit(_handler, d,1,100);
}

