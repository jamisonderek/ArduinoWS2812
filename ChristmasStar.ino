/*
 * Use ATtiny85 to turn on a LED when the GREEN light of brightness 63 is requested.
 *   Based on a LED string that uses NeoPixel settings of...  NEO_GRB+NEO_KHZ800, BRIGHTNESS=63 (e.g. bottom 6 bits of GREEN LED on)
 * 
 * 
 *   Internal clock at 16MHz
 * 
 *   pin 1 of chip = No connection
 *   pin 2 of chip = LED (positive side)
 *   pin 3 of chip = No connection
 *   pin 4 of chip = Ground
 *   pin 5 of chip = No connection
 *   pin 6 of chip = No connection
 *   pin 7 of chip = Data of WS2812B (disconnect from Arduino Uno)
 *   pin 8 of chip = VCC (+5 volts)
 *   Other end of LED goes to 220ohm resistor.
 *   Other end of 220ohmm resistor goes to Ground.
 *   
 *   
 *   Learn how to program an ATtiny85 using an Arduino UNO...
 *   https://42bots.com/tutorials/how-to-program-attiny85-with-arduino-uno-part-1
 *    For programming the ATtiny85...
 *     pin 1 of chip = Arduino Uno '10'
 *     pin 2 of chip = LED
 *     pin 3 of chip = No connection
 *     pin 4 of chip = Ground
 *     pin 5 of chip = Arduino Uno '11'
 *     pin 6 of chip = Arduino Uno '12'
 *     pin 7 of chip = Arduino Uno '13'  [disconnect from WS2812B during programming)
 *     pin 8 of chip = VCC (+5 volts)   
 *   
 *   File/Examples/ArduinoISP/ArduinoISP
 *     Tools/Board/Ardunio AVR Boards/Arduino Uno
 *     Tools/Port/(choose the COM port of your Arduino Uno)
 *     Tools/Programmer/AVRISP mkii
 *     Sketch/Upload
 *     
 *   Connect 10uF capacitor across GROUND and RESET on the Arduino Uno (make sure negative side is connected to ground)
 *   
 *   Load this sketch in the IDE.
 *   
 *   Make sure you set the Internal clock at 16MHz.
 *     Tools/Board/ATtiny Microcontrollers/ATtiny25/45/85 
 *     Tools/Processor/ATtiny85
 *     Tools/Clock/Internal 16 MHz
 *     Tools/Port/(choose the COM port of your Arduino Uno)
 *     Tools/Programmer/Arduino as ISP
 *     Tools/Burn Bootloader                   (you only have to do this when you change the internal clock speed)
 *     Sketch/Upload using Programmer
 *     
 * Resources used:
 *   https://42bots.com/tutorials/how-to-program-attiny85-with-arduino-uno-part-1
 *   https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf     
 *   https://ww1.microchip.com/downloads/en/DeviceDoc/AVR-Instruction-Set-Manual-DS40002198A.pdf
 *     
 *   Logic 2 by Salele, connected to GND, Orange (channel 3) pin 2 of chip, Yellow (channel 4) pin 7 of chip.  
 *     Trigger on rising edge of channel 4.
 *     Capture duration 10 seconds.
 *     Digital 24MS/s
 *   
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void setup() {
  pinMode(PB4, OUTPUT);
  pinMode(PB3, OUTPUT);
  pinMode(PB2, INPUT);
  cli();
  GIMSK |= (1 << INT0); // Enable hardware interrupt INT0.
  MCUCR |= (1 << ISC01)|(1 << ISC00);  // Rising Edge triggers ISR.
  sei();
}

int lastIndex = 0;
int index = 0;
int color = 0;
void loop() {
  if (lastIndex != index) {
    int theColor = color;
    lastIndex = index;
    if (theColor & 1) {
      PORTB |= (1<<3);
    } else {
      PORTB &= ~(1<<3);
    }
  }

  _delay_us(300);
}

int GetHighCountPB2Delay(int jmpAmount)
{
   byte countHigh, countEnding = 0;
   
   asm volatile (
    "MOV %[countHigh],%[jmpAmount]; \n"
    "loop:\n\t"
    "DEC %[countHigh]\n\t"
    "BRNE loop\n\t"
    "NOP; \n\t"                  // for debugging timing you can change an output pin, like:   SBI 0x18, 4; \n\t
    
    // COUNT HOW MANY TIMES THE SIGNAL IS HIGH.
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"

    "SBIC 0x16, 2; \n\t INC %[countEnding]; \n\t SBIC 0x16, 2; \n\t INC %[countEnding]; \n\t SBIC 0x16, 2; \n\t INC %[countEnding]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countEnding]; \n\t SBIC 0x16, 2; \n\t INC %[countEnding]; \n\t SBIC 0x16, 2; \n\t INC %[countEnding]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countEnding]; \n\t SBIC 0x16, 2; \n\t INC %[countEnding]; \n\t SBIC 0x16, 2; \n\t INC %[countEnding]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countEnding]; \n\t SBIC 0x16, 2; \n\t INC %[countEnding]; \n\t SBIC 0x16, 2; \n\t INC %[countEnding]; \n\t"
    "NOP; \n\t"                   // for debugging timing you can change an output pin, like:  CBI 0x18, 4; \n\t
    
    :[countHigh]  "+r" (countHigh), [countEnding]  "+r" (countEnding), [jmpAmount] "+r" (jmpAmount)
  );

  return (countEnding>0) ? (countHigh+countEnding) : 0;
}


int GetHighCountPB2()
{
   byte countHigh=0;
   
   asm volatile (    
    // COUNT HOW MANY TIMES THE SIGNAL IS HIGH.
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"
    "SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t SBIC 0x16, 2; \n\t INC %[countHigh]; \n\t"

//    "NOP; \n\t"

    :[countHigh]  "+r" (countHigh)
  );

  return countHigh;
}

ISR(INT0_vect, ISR_BLOCK)
{
  byte countHighG = GetHighCountPB2();
  byte countHighR = GetHighCountPB2Delay(14);
  byte countHighB = GetHighCountPB2Delay(11);

  int c = (countHighG>21)?1:0;
  if (countHighR>21) {c+=2;}
  if (countHighB>21) {c+=4;}
  if (countHighG + countHighR + countHighB >10 && countHighB>0) {
     color = c;
     index++;
  } else if (countHighG + countHighR + countHighB == 0) {
    // This is when ISR gets invoked a second time.
//   asm volatile (
//    "SBI 0x18, 4; \n\t"
//    "CBI 0x18, 4; \n\t"
//   );        
  } else if (countHighB==0) {
    // Generate two pulses to indicate the last reading was zero (so we were too offset in the reading.)
/*
    asm volatile (
      "SBI 0x18, 4; \n\t"
      "CBI 0x18, 4; \n\t"
      "SBI 0x18, 4; \n\t"
      "CBI 0x18, 4; \n\t"
    );
*/
  } else {
    // Generate three pulses to indicate sum of pulses didn't meet expected threshold.
    for (c=0; c <3+countHighG + countHighR + countHighB; c++) {
      asm volatile (
        "SBI 0x18, 4; \n\t"
        "CBI 0x18, 4; \n\t"
      );        
     }
  }
  
  // Delay so that when we exit the next ISR will get invoked during the "LOW" part and will have a 0 countHigh.
  _delay_us(40);
}
