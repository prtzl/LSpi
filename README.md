# LSPI low level spi library for STM32

This library works with LL driver for SPI for ST32. It supports spi timeouts with `expTime` library (https://github.com/SausageCode/ExpTime).

Example project is included in `lSpi_example`.

## How to use this library

First initialize SPI peripheral in CubeMX depending on the device you're using.
Include this library at least in your `main.c` to call `lSpi_init()` after MX init functions, like under section `USER CODE BEGIN 2`.

Library can use timeout for sending and receiving data in ll functions. This function is provided by `expTime` library. You can enable and configure timeout in `lSpi_Config.h`.

Library only interfaces with SPI peripheral, therefore chip select line control has to be implemented by user as it's device specific (some devices also have data control lines to be toggled on data transfer). Sample functions for reading and writing device registers are implemented in example's `main.c`.

Any fixes, additions and suggestions are welcome.
