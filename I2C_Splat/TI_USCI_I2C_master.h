#ifndef USCI_LIB
#define USCI_LIB
/*
#define SDA_PIN 0x02                                  // msp430x261x UCB0SDA pin
#define SCL_PIN 0x04                                  // msp430x261x UCB0SCL pin
*/
#define SDA_PIN 0x80                                  	// msp430g2553 UCB0SDA pin (Pin 7)1000 0000
#define SCL_PIN 0x40									// msp430g2553 UCB0SCL pin (Pin 6)

void TI_USCI_I2C_receiveinit(unsigned char slave_address, unsigned char prescale);
void TI_USCI_I2C_transmitinit(unsigned char slave_address, unsigned char prescale);


void TI_USCI_I2C_receive(unsigned char byteCount, unsigned char *field);
void TI_USCI_I2C_transmit(unsigned char byteCount, unsigned char *field);


unsigned char TI_USCI_I2C_slave_present(unsigned char slave_address);
unsigned char TI_USCI_I2C_notready();


#endif
