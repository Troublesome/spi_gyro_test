
// PIC18F4680 Configuration Bit Settings

#include <p18F4680.h>
#include <delays.h>
#include <stdio.h>
#include <spi.h>
#include "Lcd.h"

// CONFIG1H
#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator, PLL enabled (Clock Frequency = 4 x FOSC1))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = BOHW     // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (VBOR set to 2.1V)

// CONFIG2H
#pragma config WDT = OFF         // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config BBSIZ = 1024     // Boot Block Size Select bits (1K words (2K bytes) Boot Block)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

#define SPI_CS LATAbits.LATA0
#define TRUE 1
#define FALSE 0

void delayms(int count) //Gives a delay of 1 Ms
{
    int i;
    int j;
    for (i = 0; i < count; i++) {
        for (j = 0; j < 25; j++);
    }
}

void DataTx(char data) {
    SPI_CS = 0;
    WriteSPI(data);
    SPI_CS = 1;
}

char DataRx() {
    return SSPBUF;
}

void InitializePort() {
    TRISAbits.RA0 = 0;
    SPI_CS = 1;
    TRISCbits.RC3 = 0;
    TRISCbits.RC4 = 1;
    TRISCbits.RC5 = 0;
    OpenSPI(SPI_FOSC_4, MODE_00, SMPEND);

    //Send HighByte command
    SPI_CS = 0;
    WriteSPI(0x80);
    delayms(100000);
    SPI_CS = 1;
    delayms(100000);
}

void PortsInit(){
    CMCON = 0x07;               // Comparators Off

    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD4 = 0;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
}

// main

void main(void) {
    char str[] = {"\0"};
    int k = 0;
    int i = 0;
    signed char highbyte = 0x00;
    unsigned char lowbyte = 0x00;
    unsigned char temp = 0x00;
    int angularrate = 0;
    char neg = 0x00;

    PortsInit();
    InitializePort();
    LCDInit();
    

    //loop
    while( 1 )
    {
        angularrate = 0;
#ifdef DEBUG
        Delay10KTCYx( 0xFF );
#endif
        //Send LowByte command
        DataTx( 0x8F );

        //Receive HighByte
        highbyte = DataRx();

        //Send HighByte command
        DataTx( 0x80 );

        //Receive LowByte
        lowbyte = DataRx();

        temp = lowbyte & 0x03;

        if( highbyte < 0 )
            neg = TRUE;
        else
            neg = FALSE;

        highbyte = highbyte & 0x7F;
        angularrate = ( (int)highbyte << 2 ) | temp;

        if( neg )
            angularrate = -512 + angularrate;
        if( angularrate > 30 )
            k = 1;
#ifdef DEBUG
            *p = angularrate;

            i++;
            p++;
            if( i == 100 )
            {
                p = values;
                i = 0;
            }
#endif

        k = 1;
        sprintf(str, "%d", angularrate);
        LCDWriteStr(str);
        Delay10KTCYx(50);
        LCDClear();
    }
}
