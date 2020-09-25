#include "lSpi.h"
#ifdef LSPI_USE_TIMEOUT
#include "expTime.h"
#endif

/**
* @brief  Start SPI peripheral
* @param  SPIx spi peripheral
**/
void lSpi_init(SPI_TypeDef *SPIx)
{
	LL_SPI_Enable(SPIx);
}

/**
* @brief  Write byte to SPIx device
* @param  SPIx spi peripheral typedef pointer
* @param  byte byte to send
**/
ErrorStatus lSpi_master_sendByte(SPI_TypeDef *SPIx, uint8_t byte)
{
#ifdef LSPI_USE_TIMEOUT
	expTime_timeout_start(LSPI_TIMEOUT);
	while(!LL_SPI_IsActiveFlag_TXE(SPIx))
	{
		// Check for timeout, 10ms
		if(expTime_timeout_check())
		{
			return ERROR;
		}
	}
	// Send byte
  LL_SPI_TransmitData8(SPIx, byte);

  // wait for communication to finish
  expTime_timeout_start(LSPI_TIMEOUT);
  while(LL_SPI_IsActiveFlag_BSY(SPIx))
  {
		// Check for timeout, 10ms
		if(expTime_timeout_check())
		{
			return ERROR;
		}
  }
#else
  while(!LL_SPI_IsActiveFlag_TXE(SPIx));
  LL_SPI_TransmitData8(SPIx, byte);
  while(LL_SPI_IsActiveFlag_BSY(SPIx));
#endif
  return SUCCESS;
}

/**
* @brief  Write two bytes one after another to SPIx device
* @param  SPIx spi peripheral typedef pointer
* @param  byte1 first byte to send(device register address)
* @param  byte2 second byte to send(data for device register)
**/
ErrorStatus lSpi_master_writeReg(SPI_TypeDef *SPIx, uint8_t reg_address, uint8_t reg_data)
{
#ifdef LSPI_USE_TIMEOUT
	// Send first byte
	expTime_timeout_start(LSPI_TIMEOUT);
	while(!LL_SPI_IsActiveFlag_TXE(SPIx))
	{
		// Check for timeout, 10ms
		if(expTime_timeout_check())
		{
			return ERROR;
		}
	}
  LL_SPI_TransmitData8(SPIx, reg_address);

  // Send second byte
  expTime_timeout_start(LSPI_TIMEOUT);
	while( !LL_SPI_IsActiveFlag_TXE(SPIx) )
	{
		// Check for timeout, 10ms
		if(expTime_timeout_check())
		{
			return ERROR;
		}
	}
  LL_SPI_TransmitData8(SPIx, reg_data);

  // wait for communication to finish
  expTime_timeout_start(LSPI_TIMEOUT);
  while(LL_SPI_IsActiveFlag_BSY(SPIx))
  {
		// Check for timeout, 10ms
		if(expTime_timeout_check())
		{
			return ERROR;
		}
  }
#else
  // Send first byte
  while(!LL_SPI_IsActiveFlag_TXE(SPIx));
  LL_SPI_TransmitData8(SPIx, reg_address);

	// Send second byte
	while( !LL_SPI_IsActiveFlag_TXE(SPIx) );
	LL_SPI_TransmitData8(SPIx, reg_data);

	// wait for communication to finish
	while(LL_SPI_IsActiveFlag_BSY(SPIx));
#endif
  return SUCCESS;
}

/**
* @brief  Write two bytes one after another to SPIx device
* @param  SPIx spi peripheral typedef pointer
* @param  byte1 first byte to send(device register address)
* @param  byte2 second byte to send(data for device register)
**/
ErrorStatus lSpi_master_readReg(SPI_TypeDef *SPIx, uint8_t reg_address, uint8_t *data)
{
#ifdef LSPI_USE_TIMEOUT
	// Send reg address to read from
	expTime_timeout_start(LSPI_TIMEOUT);
	while(!LL_SPI_IsActiveFlag_TXE(SPIx))
	{
		// Check for timeout, 10ms
		if(expTime_timeout_check())
		{
			return ERROR;
		}
	}
	LL_SPI_TransmitData8(SPIx, reg_address|0x80); // 0x80: read command

	// Empty input register
	expTime_timeout_start(LSPI_TIMEOUT);
	while(!LL_SPI_IsActiveFlag_RXNE(SPIx))
	{
		// Check for timeout, 10ms
		if(expTime_timeout_check())
		{
			return ERROR;
		}
	}
	LL_SPI_ReceiveData8(SPIx);

	// Send dummy data to get 16bit pulses
	expTime_timeout_start(LSPI_TIMEOUT);
	while(!LL_SPI_IsActiveFlag_TXE(SPIx))
	{
		// Check for timeout, 10ms
		if(expTime_timeout_check())
		{
			return ERROR;
		}
	}
	LL_SPI_TransmitData8(SPIx, 0x00);

	// Get returned data (2nd received byte)
	expTime_timeout_start(LSPI_TIMEOUT);
	while(!LL_SPI_IsActiveFlag_RXNE(SPIx))
	{
		// Check for timeout, 10ms
		if(expTime_timeout_check())
		{
			return ERROR;
		}
	}
	*data = LL_SPI_ReceiveData8(SPIx);
#else
	// Send reg address to read from
	while(!LL_SPI_IsActiveFlag_TXE(SPIx));
	LL_SPI_TransmitData8(SPIx, reg_address|0x80);

	// Empty input register
	while(!LL_SPI_IsActiveFlag_RXNE(SPIx));
	LL_SPI_ReceiveData8(SPIx);

	// Send dummy data to get 16bit pulses
	while(!LL_SPI_IsActiveFlag_TXE(SPIx));
	LL_SPI_TransmitData8(SPIx, 0x00);

	// Get returned data (2nd received byte)
	while(!LL_SPI_IsActiveFlag_RXNE(SPIx));
	*data = LL_SPI_ReceiveData8(SPIx);
#endif
	return SUCCESS;
}

/**
* @brief  Write multiple bytes to SPIx device
* @param  SPIx spi peripheral typedef pointer
* @param  bytes pointer to byte array
* @param  size number of bytes to send
**/
ErrorStatus lSpi_master_sendData(SPI_TypeDef *SPIx, const uint8_t *bytes, const uint16_t size)
{
	uint16_t i;
#ifdef LSPI_USE_TIMEOUT
	// Send <size> bytes of data
	for( i = 0; i < size; i++ )
	{
		expTime_timeout_start(LSPI_TIMEOUT);
		while(!LL_SPI_IsActiveFlag_TXE(SPIx))
		{
			// Check for timeout, 10ms
			if(expTime_timeout_check())
			{
				return ERROR;
			}
		}
		LL_SPI_TransmitData8(SPIx, bytes[i]);
	}

	// wait for communication to finish
	expTime_timeout_start(LSPI_TIMEOUT);
  while(LL_SPI_IsActiveFlag_BSY(SPIx))
  {
		// Check for timeout, 10ms
		if(expTime_timeout_check())
		{
			return ERROR;
		}
  }
#else
	for(i = 0; i < size; i++)
	{
		while(!LL_SPI_IsActiveFlag_TXE(SPIx));
		LL_SPI_TransmitData8(SPIx, bytes[i]);
	}

	// wait for communication to finish
	while(LL_SPI_IsActiveFlag_BSY(SPIx));
#endif
  return SUCCESS;
}

