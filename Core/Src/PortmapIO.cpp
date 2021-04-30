/*
 * PortmapIO.cpp
 *
 *  Created on: Apr 29, 2021
 *      Author: hydra
 */

#include "PortmapIO.h"

PortmapIO::PortmapIO()
{

}
PortmapIO::PortmapIO (GPIO_TypeDef * port, uint16_t pin) {
	_port = port;
	_pin=pin;
	_status=0;
}

void PortmapIO::TogglePin()
{

	HAL_GPIO_TogglePin(_port,_pin);
	_status=!_status;
}
void PortmapIO::setLow()
{
	HAL_GPIO_WritePin(_port,_pin,GPIO_PIN_RESET);
	_status=0;
}
void PortmapIO::setHigh()
{
	HAL_GPIO_WritePin(_port,_pin,GPIO_PIN_SET);
	_status=1;
}
uint8_t PortmapIO::getStatus()
{
	return _status;
}
