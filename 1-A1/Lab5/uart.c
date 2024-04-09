/*
*
*   uart.c
*
*
*
*   @author
*   @date
*/

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "uart.h"
#include "cyBot_uart.h"
#include <string.h>
#include <stdlib.h>

void uart_init(void){
  unsigned char mask = 0x02;
	//TODO
  //enable clock to GPIO port B
  SYSCTL_RCGCGPIO_R |= mask;

  //enable clock to UART1
  SYSCTL_RCGCUART_R |= mask;

  //wait for GPIOB and UART1 peripherals to be ready
  while ((SYSCTL_PRGPIO_R & mask) == 0) {};
  while ((SYSCTL_PRUART_R & mask) == 0) {};

  //enable alternate functions on port B pins
  GPIO_PORTB_AFSEL_R |= 0x03;

  //enable digital functionality on port B pins
  GPIO_PORTB_DEN_R |= 0x03;

  //enable UART1 Rx and Tx on port B pins
  GPIO_PORTB_PCTL_R |= 0x00000011;  //***************************************************************** ???????????????????

  //calculate baud rate
  double BRD = 16000000.0 / (16.0 * 115200);  //***************************************************************** ???????????????????
  uint16_t iBRD = (uint16_t) BRD; //use equations
  uint16_t fBRD = (uint16_t) ((BRD - iBRD) * 64 + .5); //use equations

  //turn off UART1 while setting it up
  UART1_CTL_R &= 0x00;

  //set baud rate
  //note: to take effect, there must be a write to LCRH after these assignments
  UART1_IBRD_R = iBRD;
  UART1_FBRD_R = fBRD;

  //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
  //note: this write to LCRH must be after the BRD assignments
  UART1_LCRH_R = 0x60; //***************************************************************** ???????????????????

  //use system clock as source
  //note from the datasheet UARTCCC register description:
  //field is 0 (system clock) by default on reset
  //Good to be explicit in your code
  UART1_CC_R = 0x0;

  //re-enable UART1 and also enable RX, TX (three bits)
  //note from the datasheet UARTCTL register description:
  //RX and TX are enabled by default on reset
  //Good to be explicit in your code
  //Be careful to not clear RX and TX enable bits
  //(either preserve if already set or set them)
  UART1_CTL_R |= 0x00000011;  //***************************************************************** ???????????????????

  cyBot_uart_init_last_half();  // Complete the UART device configuration

}

void uart_sendChar(char data){
    while(UART1_FR_R & 0x20) {}
    UART1_DR_R = data;
}

char uart_receive(void){
    while(UART1_FR_R & 0x10) {}
    return (char)UART1_DR_R;
}

char uart_receive_nonblocking(void){
    if(UART1_FR_R & 0x10 == 0) {
        return (char)UART1_DR_R;
    } else {
        return '\0';
    }
}

void uart_sendStr(char *data){
    int size = 21;
    int i = 0;
    for(; i < size - 1; i++) {
        char received = uart_receive();
        if(received == '\r') {
            *(data + i) = '\0';
            break;
        }
        *(data + i) = received;
    }
    *(data + i) = '\0';
}
