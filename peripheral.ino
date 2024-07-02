/*
 * Peripheral Code for the Optical Modem
 * 
 * Only works with ATmega microcontrollers
 * will not work with Arduino Due 
 * works with Arduino Uno
 * 
 * SPI has 3 registers: SPCR (control register) SPSR (status register) SPDR(data register)
 * Reference: https://web.engr.oregonstate.edu/~traylor/ece473/lectures/spi.pdf
 * Note: uses the outdated SPI terms 
 */

#include <SPI.h>

volatile boolean recieved;
volatile byte recievedData;

ISR(SPI_STC_vect){
  recievedData = SPDR; //save byte from data register
  recieved = true;
}

void setup(){
  Serial.begin(115200);
  pinMode(MISO, OUTPUT); //set as peripheral
  SPCR |= _BV(SPE); //enables the SPI interface 
  recieved = false;
  SPI.attachInterrupt();
}

void loop(){

  //TODO: detect if packet recieved & determine all parts of it
  //TODO: bit error rate detection
  //TODO: send own packets 

  //very simple print out of the data recieved
  if(recieved){
    SPDR = recievedData; //sending back 
    Serial.println(recievedData,HEX);
    recieved = false;
  }
}
