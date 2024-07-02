#include <SPI.h>

//CS pins on the ARDUINO DUE are 10, 4, 52

uint8_t data[1504];
const uint16_t seed = 0xFF & 0x1FF; //MUST BE MASKED TO 8-BITS FOR SEQUENCE SENT IN PACKET TO MATCH


void setup()
{
  Serial.begin(115200);
  
  digitalWrite(10, HIGH);  // ensure SS stays high for now

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into controller mode and turn SPI on
  SPI.begin (10); 
  SPI.setClockDivider(10, 21); //21 is 16Mhz
  SPI.setBitOrder(10, LSBFIRST);
  
  genPN(seed, data, 1504);
}


void loop (void)
{
  for(int i = 0; i < sizeof(data); i++){
    SPI.transfer(10, data[i]);
  }
  
}






/*
 * OLDESET BIT IS STORED IN LSB
 */
void genPN(uint16_t lfsr, uint8_t pn[], int arrlength){
    lfsr = lfsr & 0x1FF; //MUST BE MASKED OCTET FOR CORRECT SEED PACKET SENDING
    for(int j = 0; j < arrlength; j++){
      uint8_t num = 0;
      for(int i = 0; i < 8; i++){
        int MSB = bitRead(lfsr, 8);
        //Serial.print(MSB);
        bitWrite(num, i, MSB);
        uint8_t XOR = (bitRead(lfsr, 4)) ^ (bitRead(lfsr, 8));
        
        lfsr = lfsr << 1;
        lfsr = lfsr & 0x1FF; //mask so no overflow
        bitWrite(lfsr, 0, XOR);
      }
      //Serial.println(num);
      pn[j] = num;
  }
}
