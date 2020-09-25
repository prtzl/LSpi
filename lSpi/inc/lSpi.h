/* This library only interfaces with SPIx peripheral with LL driver.
 * You still have to manually drive CS LOW before calling the function
 * and HIGH after. Sample of device implementation
 *
 * Do not forget to initialize SPIx peripheral in CubeMx. This library only
 * starts the peripheral.
 *
 * written by Matej Blagsic (@SausageCode)
 *
 */
#ifndef LL_SPI_H
#define LL_SPI_H

#include "main.h"
#include "lSpi_Config.h"

void lSpi_init(SPI_TypeDef *SPIx);
ErrorStatus lSpi_master_sendByte(SPI_TypeDef *SPIx, uint8_t byte);
ErrorStatus lSpi_master_writeReg(SPI_TypeDef *SPIx, uint8_t reg_address, uint8_t reg_data);
ErrorStatus lSpi_master_readReg(SPI_TypeDef *SPIx, uint8_t reg_address, uint8_t *data);
ErrorStatus lSpi_master_sendData(SPI_TypeDef *SPIx, const uint8_t *byte, const uint16_t size);

#endif
