//#include <msp430.h>
#include <msp430g2553.h>
#include "TI_USCI_I2C_master.h"

unsigned char timercounter;

void main(void){
	unsigned char array[40] = { 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb };
	unsigned char store[10] = { 88, 88, 88, 88, 88, 88, 88, 88, 88, 88};

	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
	P1DIR |= 0x01; // Set P1.0 to output direction
	P1REN |= BIT3; // Pull up for P1.3 switch
	P1IE |= BIT3; // P1.3 interrupt enabled
	//P1IES |= BIT3; // P1.3 Hi/lo edge
	P1IFG &= ~BIT3; // P1.3 IFG cleared
	//0x 0000 1000
	P1OUT = 0x08; 	// Initialize the LED to be off.

	while ( TI_USCI_I2C_notready() );
	TI_USCI_I2C_receiveinit(0x50,0x12);   		// init receiving with USCI
	while (1){
		while ( TI_USCI_I2C_notready() );       // wait for bus to be free
		TI_USCI_I2C_receive(4,store);
		if (store[0] != 88){
			P1OUT ^= 0x01; 						// P1.0 = toggle
		}
	}
	_BIS_SR(LPM4_bits + GIE); // Enter LPM4 w/interrupt
}

