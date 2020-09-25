// I recommend using timeout in case SPI fails (device does not responds, bad SPI configuration)
// On F4@168MHz it can push 231fps at 10MHz SPI clock
#define LSPI_USE_TIMEOUT
#define LSPI_TIMEOUT		5 // timeout in ms
