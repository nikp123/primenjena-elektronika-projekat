#include <p30fxxxx.h>
#include <stdint.h>

#include "tajmeri.h"


_FOSC(CSW_FSCM_OFF & XT_PLL4);
_FWDT(WDT_OFF);


//---------------------------------
uint16_t brojac_01ms;
unsigned int stoperica,ms,sekund;

/*******************************************************************
//funkcija za kasnjenje u milisekundama
 *********************************************************************/
void delay_01ms (int vreme)//funkcija za kasnjenje u milisekundama
{
  stoperica = 0;
  while(stoperica < vreme);
}



void __attribute__ ((__interrupt__)) _T2Interrupt(void) // svakih 1ms
{
  TMR2 =0;

  brojac_01ms++;//brojac milisekundi
  stoperica++;//brojac za funkciju Delay_ms

  if (brojac_01ms==10000)//sek
  {
    brojac_01ms=0;
    sekund++;//fleg za sekundu
  }

  if(sekund == 20) {
    sekund = 0;
  }
  IFS0bits.T2IF = 0; 

}

uint8_t getPos() {
  if(PORTBbits.RB0 == 1)
    return 1;
  else if(PORTBbits.RB1 == 1)
    return 2;
  else if(PORTBbits.RB2 == 1)
    return 3;
  else if(PORTBbits.RB3 == 1)
    return 4;

  return 0;
}

int main (void)
{
  //ADPCFGbits.PCFG4=1;//defini?emo da pin B4 nije analogni vec digitalni
  //ADPCFGbits.PCFG5=1;//defini?emo da pin B5 nije analogni vec digitalni
  ADPCFG = 0x00ff; // zadnja 8 bita su digitalna
  TRISB = 0x00ff;  // zadnja 8 bita su ulaz

  TRISDbits.TRISD0=0;//konfigurisemo kao izlaz

  Init_T2();


  while(1)
  {  
    // 100hz
    uint16_t full_wave = 100;
              
    switch(getPos()) {
        case 1:
            full_wave = 50; // 200 hz
            break;
        case 2:
            full_wave = 33; // 300 hz
            break;
        case 3:
            full_wave = 25; // 400 hz
            break;
        case 4:
            full_wave = 20; // 500 hz
            break;
        default:
            break;
    }
      
    uint16_t wait_high = full_wave/10 + getPos()*full_wave/5;

    uint16_t wait_low = full_wave - wait_high;

    LATDbits.LATD0=1;
    delay_01ms (wait_high);
    LATDbits.LATD0=0;
    delay_01ms(wait_low);

  }//while(1)

  return 0;
}//main


