/*
 * PortmapUART.cpp
 *
 *  Created on: Apr 29, 2021
 *      Author: hydra
 */

#include "PortmapUART.h"

PortmapUART::PortmapUART()
{

}
PortmapUART::PortmapUART(UART_HandleTypeDef h)
{
	_handler=h;
}
void PortmapUART::setHandler(UART_HandleTypeDef h)
{
	_handler=h;
}
UART_HandleTypeDef PortmapUART::getHandler()
{
	return _handler;
}
