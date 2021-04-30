/*
 * ILI9488.cpp
 *
 *  Created on: Apr 29, 2021
 *      Author: hydra
 */

#include "ILI9488.h"


/*ILI9488::ILI9488(PortmapIO CS, PortmapIO DC, PortmapIO RST,PortmapSPI spi,PortmapUART uart)
{
	_cs=CS;
	_dc=DC;
	_rst = RST;
	_spi=spi;
	_uart=uart;

}*/
ILI9488::ILI9488(PortmapIO *CS, PortmapIO *DC, PortmapIO *RST, PortmapSPI *spi)
{
	_cs=CS;
	_dc=DC;
	_rst = RST;
	_spi=spi;

}
void ILI9488::begin(void)
{
	_rst->setLow();
	HAL_Delay(10);
	_rst ->setHigh();
	_width=ILI9488_TFTWIDTH;
	_height=ILI9488_TFTHEIGHT;

	writecommand(0xE0);
	writedata(0x00);
	writedata(0x03);
	writedata(0x09);
	writedata(0x08);
	writedata(0x16);
	writedata(0x0A);
	writedata(0x3F);
	writedata(0x78);
	writedata(0x4C);
	writedata(0x09);
	writedata(0x0A);
	writedata(0x08);
	writedata(0x16);
	writedata(0x1A);
	writedata(0x0F);

	writecommand(0XE1);
	writedata(0x00);
	writedata(0x16);
	writedata(0x19);
	writedata(0x03);
	writedata(0x0F);
	writedata(0x05);
	writedata(0x32);
	writedata(0x45);
	writedata(0x46);
	writedata(0x04);
	writedata(0x0E);
	writedata(0x0D);
	writedata(0x35);
	writedata(0x37);
	writedata(0x0F);

	writecommand(0XC0);      //Power Control 1
	writedata(0x17);    //Vreg1out
	writedata(0x15);    //Verg2out

	writecommand(0xC1);      //Power Control 2
	writedata(0x41);    //VGH,VGL

	writecommand(0xC5);      //Power Control 3
	writedata(0x00);
	writedata(0x12);    //Vcom
	writedata(0x80);

	writecommand(0x36);      //Memory Access
	writedata(0x48);

	writecommand(0x3A);      // Interface Pixel Format
	writedata(0x66); 	  //18 bit

	writecommand(0XB0);      // Interface Mode Control
	writedata(0x80);     			 //SDO NOT USE

	writecommand(0xB1);      //Frame rate
	writedata(0xA0);    //60Hz

	writecommand(0xB4);      //Display Inversion Control
	writedata(0x02);    //2-dot

	writecommand(0XB6); //Display Function Control  RGB/MCU Interface Control

	writedata(0x02);    //MCU
	writedata(0x02);    //Source,Gate scan dieection

	writecommand(0XE9);      // Set Image Functio
	writedata(0x00);    // Disable 24 bit data

	writecommand(0xF7);      // Adjust Control
	writedata(0xA9);
	writedata(0x51);
	writedata(0x2C);
	writedata(0x82);    // D7 stream, loose

	writecommand(ILI9488_SLPOUT);    //Exit Sleep

	HAL_Delay(120);

	writecommand(ILI9488_DISPON);    //Display on


}

void ILI9488::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	writecommand(ILI9488_CASET); // Column addr set
	writedata(x0 >> 8);
	writedata(x0 & 0xFF);     // XSTART
	writedata(x1 >> 8);
	writedata(x1 & 0xFF);     // XEND
	writecommand(ILI9488_PASET); // Row addr set
	writedata(y0 >> 8);
	writedata(y0 & 0xff);     // YSTART
	writedata(y1 >> 8);
	writedata(y1 & 0xff);     // YEND
	writecommand(ILI9488_RAMWR); // write to RAM
}
void ILI9488::setScrollArea(uint16_t topFixedArea, uint16_t bottomFixedArea)
{
	writecommand(0x33); // Vertical scroll definition
	writedata(topFixedArea >> 8);
	writedata(topFixedArea);
	writedata((_height - topFixedArea - bottomFixedArea) >> 8);
	writedata(_height - topFixedArea - bottomFixedArea);
	writedata(bottomFixedArea >> 8);
	writedata(bottomFixedArea);
}
void ILI9488::scroll(uint16_t pixels)
{
	writecommand(0x37); // Vertical scrolling start address
	writedata(pixels >> 8);
	writedata(pixels);
}
void  ILI9488::pushColor(uint16_t color)
{
	_dc->setHigh();
	_cs->setLow();
	write16BitColor(color);
	_cs->setHigh();
}
void ILI9488::pushColors(uint16_t *data, uint8_t len, uint8_t first)
{
	uint16_t color;
	uint8_t buff[len * 3 + 1];
	uint16_t count = 0;
	uint8_t lencount = len;
	_cs->setLow();

	if (first == 1) {
		_dc->setHigh();
	}
	while (lencount--)
	{
		color = *data++;
		buff[count] = (((color & 0xF800) >> 11) * 255) / 31;
		count++;
		buff[count] = (((color & 0x07E0) >> 5) * 255) / 63;
		count++;
		buff[count] = ((color & 0x001F) * 255) / 31;
		count++;
	}
	HAL_SPI_Transmit(_spi->getHandler(), buff, len * 3, 100);
	_cs->setHigh();

}
void ILI9488::drawImage(const uint8_t* img, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{

	if ((x >= _width) || (y >= _height))
		return;
	if ((x + w - 1) >= _width)
		w = _width - x;
	if ((y + h - 1) >= _height)
		h = _height - y;
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	_dc->setHigh();
	_cs->setLow();

	uint8_t linebuff[w * 3 + 1];
	uint32_t count = 0;
	for (uint16_t i = 0; i < h; i++) {
		uint16_t pixcount = 0;
		for (uint16_t o = 0; o < w; o++) {
			uint8_t b1 = img[count];
			count++;
			uint8_t b2 = img[count];
			count++;
			uint16_t color = b1 << 8 | b2;
			linebuff[pixcount] = (((color & 0xF800) >> 11) * 255)
					/ 31;
			pixcount++;
			linebuff[pixcount] = (((color & 0x07E0) >> 5) * 255)
					/ 63;
			pixcount++;
			linebuff[pixcount] = ((color & 0x001F) * 255) / 31;
			pixcount++;
		}
		HAL_SPI_Transmit(_spi->getHandler(), linebuff, w * 3, 100);

	}

	_cs->setHigh();
}
void ILI9488::fillScreen(uint16_t color)
{
	fillRect(0, 0,  _width, _height, color);
}
void ILI9488::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
		return;

	setAddrWindow(x, y, x + 1, y + 1);
	_dc->setHigh();
	_cs->setLow();
	write16BitColor(color);
	_cs->setHigh();

}
void ILI9488::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{

	if ((x >= _width) || (y >= _height))
		return;

	if ((y + h - 1) >= _height)
		h = _height - y;

	setAddrWindow(x, y, x, y + h - 1);
	_dc->setHigh();
	_cs->setLow();

	while (h--) {
		write16BitColor(color);
	}
	_cs->setHigh();

}
void ILI9488::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{

	if ((x >= _width) || (y >= _height))
		return;
	if ((x + w - 1) >= _width)
		w = _width - x;
	setAddrWindow(x, y, x + w - 1, y);
	_dc->setHigh();
	_cs->setLow();
	while (w--)
	{
		write16BitColor(color);
	}
	_cs->setHigh();
}
void ILI9488:: drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color)
{
	 if (x0 == x1) {
	    if (y0 > y1)
	      _swap_int16_t(y0, y1);
	    drawFastVLine(x0, y0, y1 - y0 + 1, color);
	  } else if (y0 == y1) {
	    if (x0 > x1)
	      _swap_int16_t(x0, x1);
	    drawFastHLine(x0, y0, x1 - x0 + 1, color);
	  } else {

	    writeLine(x0, y0, x1, y1, color);

	  }

}
void ILI9488::writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color) {

  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, color);
    } else {
      drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}
void ILI9488::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	if ((x >= _width) || (y >= _height))
		return;
	if ((x + w - 1) >= _width)
		w = _width - x;
	if ((y + h - 1) >= _height)
		h = _height - y;

	setAddrWindow(x, y, x + w - 1, y + h - 1);

	_dc->setHigh();
	_cs->setLow();

	for (y = h; y > 0; y--) {
		for (x = w; x > 0; x--) {

			write16BitColor(color);
		}
	}

	_cs->setHigh();

}
void ILI9488::setRotation(uint8_t r)
{

	writecommand(ILI9488_MADCTL);
	rotation = r % 4; // can't be higher than 3
	switch (rotation) {
	case 0:
		writedata(MADCTL_MX | MADCTL_BGR);
		_width = ILI9488_TFTWIDTH;
		_height = ILI9488_TFTHEIGHT;
		break;
	case 1:
		writedata(MADCTL_MV | MADCTL_BGR);
		_width = ILI9488_TFTHEIGHT;
		_height = ILI9488_TFTWIDTH;
		break;
	case 2:
		writedata(MADCTL_MY | MADCTL_BGR);
		_width = ILI9488_TFTWIDTH;
		_height = ILI9488_TFTHEIGHT;
		break;
	case 3:
		writedata(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
		_width = ILI9488_TFTHEIGHT;
		_height = ILI9488_TFTWIDTH;
		break;
	}

}
void ILI9488::invertDisplay(uint8_t i)
{

	  writecommand(i ? ILI9488_INVON : ILI9488_INVOFF);

}
uint16_t ILI9488::color565(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
void ILI9488::spiwrite(uint8_t data)
{
	uint8_t d=data;
	_spi->send(&d);

}
void ILI9488::writecommand(uint8_t c)
{
	uint8_t d=c;
	_dc->setLow();

	//  digitalWrite(_sclk, LOW);
	_cs->setLow();

	_spi->send(&d);
	//HAL_SPI_Transmit(_spi.getHandler(), &d,1,100);

	_cs->setHigh();

}
void ILI9488::write16BitColor(uint16_t color)
{

	  uint8_t r = (color & 0xF800) >> 11;
	  uint8_t g = (color & 0x07E0) >> 5;
	  uint8_t b = color & 0x001F;

	  r = (r * 255) / 31;
	  g = (g * 255) / 63;
	  b = (b * 255) / 31;

	  _spi->send(&r);
	  _spi->send(&g);
	  _spi->send(&b);

}
void ILI9488::writedata(uint8_t d)
{

	uint8_t tmp=d;
	_dc->setHigh();
	_cs->setLow();
	_spi->send(&tmp);
	//HAL_SPI_Transmit(_spi.getHandler(), &tmp,1,100);
	_cs->setHigh();

}
void ILI9488::testLines(uint8_t color)
{

	unsigned long start, t;
	int x1, y1, x2, y2, w = _width, h = _height;
	fillScreen(ILI9488_BLACK);

	x1 = y1 = 0;
	y2 = h - 1;

	for (x2 = 0; x2 < w; x2 += 6)
		drawLine(x1, y1, x2, y2, color);
	x2 = w - 1;
	for (y2 = 0; y2 < h; y2 += 6)
		drawLine(x1, y1, x2, y2, color);
	fillScreen(ILI9488_BLACK);

	x1 = w - 1;
	y1 = 0;
	y2 = h - 1;

	for (x2 = 0; x2 < w; x2 += 6)
		drawLine(x1, y1, x2, y2, color);
	x2 = 0;
	for (y2 = 0; y2 < h; y2 += 6)
		drawLine(x1, y1, x2, y2, color);

	fillScreen(ILI9488_BLACK);

	x1 = 0;
	y1 = h - 1;
	y2 = 0;

	for (x2 = 0; x2 < w; x2 += 6)
		drawLine(x1, y1, x2, y2, color);
	x2 = w - 1;
	for (y2 = 0; y2 < h; y2 += 6)
		drawLine(x1, y1, x2, y2, color);

	fillScreen(ILI9488_BLACK);

	x1 = w - 1;
	y1 = h - 1;
	y2 = 0;

	for (x2 = 0; x2 < w; x2 += 6)
		drawLine(x1, y1, x2, y2, color);
	x2 = 0;
	for (y2 = 0; y2 < h; y2 += 6)
		drawLine(x1, y1, x2, y2, color);
}
/*void ILI9488::commandList(uint8_t *addr)
 {
	uint8_t numCommands, numArgs;
	uint16_t ms;

	numCommands = pgm_read_byte(addr++);
	while (numCommands--) {
		writecommand(pgm_read_byte(addr++));
		numArgs = pgm_read_byte(addr++);
		ms = numArgs & 0x80;
		numArgs &= ~0x80;
		while (numArgs--) {
			writedata(pgm_read_byte(addr++));
		}

		if (ms) {
			ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
			if (ms == 255)
				ms = 500;     // If 255, delay for 500 ms
			delay(ms);
		}
	}

}*/


