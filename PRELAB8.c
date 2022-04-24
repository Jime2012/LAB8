/* 
 * File:   ejercicios.c
 * Author: Jimena de la Rosa
 *
 * Created on 3 de abril de 2022, 1:41
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
uint8_t cont; // SE DEFINEN LAS VARIABLES A UTILIZAR
uint8_t contador;
uint8_t b = 0;
uint8_t u = 0;
uint8_t d = 0;
uint8_t c = 0;
uint8_t display1 = 0;
uint8_t display2 = 0;
uint8_t display3 = 0;
float valor;
int suma;


int display[10] = {0b10111111, 0b10000110, 0b11011011, 0b11001111, 0b11100110, 0b11101101, 0b11111101, 0b10000111, 0b11111111, 0b11101111};
// SE DEFINE UN ARREGLO PARA OBTENER EL VALOR DEL DISPLAY
/*------------------------------------------------------------------------------
 * CONSTANTES 
 ------------------------------------------------------------------------------*/
#define _XTAL_FREQ 4000000
/*------------------------------------------------------------------------------
 * VARIABLES 
 ------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/
void setup(void);
/*------------------------------------------------------------------------------
 * INTERRUPCIONES 
 ------------------------------------------------------------------------------*/
void __interrupt() isr (void){
    if (INTCONbits.T0IF){ // INTERRUPCION DEL TMR0
            TMR0 = 252; //SE INGRESA NUEVAMENTE EL VALOR DEL TMR0
            INTCONbits.T0IF = 0; // SE LIMPIA LA BANDERA
            PORTBbits.RB0 = 0;//SE DEJAN EN CERO LAS SALIDAS DEL PORTD
            PORTBbits.RB1 = 0;
            PORTBbits.RB2 = 0;
    
            if (b == 0){ // MULTIPLEXADO
                PORTBbits.RB0 = 1;
                PORTD = display1;// SE MUESTRA EL VALOR DE LA CENTENA
                b = 1;}
            else if (b == 1){
                PORTBbits.RB1 = 1;
                PORTD = display2; // SE MUESTRA EL VALOR DE LA DECENA
                b = 2;}
            else if (b == 2){
                PORTBbits.RB2 = 1;
                PORTD = display3;// SE MUESTRA EL VALOR DE LA UNIDAD
                b = 0;}
    }
   
             
           
    if(PIR1bits.ADIF){              // Fue interrupci n del ADC??
        if(ADCON0bits.CHS == 0){    // Verificamos sea AN0 el canal seleccionado
            PORTC = ADRESH;         // Mostramos ADRESH en PORTC
        }
        
        // *En caso de estar usando mas de un canal anal gico?
        else if (ADCON0bits.CHS == 1){
            
           for (cont = 0; cont < 3; cont++){ // CICLO FOR PARA OBTENER LOS VALORES 
               valor = (1.96)*ADRESH;  
               suma = (int)valor;
             if (cont == 0) 
                u = convertir(suma, cont); //SE OBTIENE EL VALOR DE UNIDAD
             else if (cont == 1) 
                d = convertir(suma, cont);// SE OBTIENE EL VALOR DE LA DECENA
             else
                c = convertir(suma, cont);// SE OBTIENE EL VALOR DE LA UNIDAD
             
            }
             display1 = display[c]; //SE LLAMA EL VALOR DEL ARREGLO NECESARIO PARA EL DISPLAY
             display2 = display[d];
             display3 = display[u];
          
        }
        PIR1bits.ADIF = 0;          // Limpiamos bandera de interrupci n?
    }
    return;
}
/*------------------------------------------------------------------------------
 * CICLO PRINCIPAL
 ------------------------------------------------------------------------------*/
void main(void) {
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;// VALORES INICIALES DE LAS ENTRADAS
    
    setup();// SE LLAMAN LAS CONFIGURACIONES
    while(1){
        PORTBbits.RB3 = 1;
        if(ADCON0bits.GO == 0){             // No hay proceso de conversion
            // *En caso de estar usando mas de un canal anal gico?
            
            if(ADCON0bits.CHS == 0b0000)    
                ADCON0bits.CHS = 0b0001;    // Cambio de canal
            else if(ADCON0bits.CHS == 0b0001)
                ADCON0bits.CHS = 0b0000;    // Cambio de canal
            __delay_us(40);                 // Tiempo de adquisici n?
           
            ADCON0bits.GO = 1;              // Iniciamos proceso de conversi n?
        }
        
        
    }
    return;
}
/*------------------------------------------------------------------------------
 * CONFIGURACION 
 ------------------------------------------------------------------------------*/
void setup(void){
    ANSEL = 0b00000011; // AN0 como entrada anal gica?
    ANSELH = 0;         // I/O digitales)
    
    //ANSEL = 0b00000111; // AN0, AN1 y AN2 como entrada anal gica?
    
    TRISA = 0b00000011; // AN0 como entrada
    //TRISA = 0b00000111; // AN0, AN1 y AN2 como entrada
    PORTA = 0; 
    
    TRISC = 0;// DEFINIR SALIDAS EN PORTC
    PORTC = 0;
    TRISD = 0;
    
   
    
    TRISBbits.TRISB0 = 0;//definir las salidas en PORTB
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    
    // Configuracion reloj interno?
    OSCCONbits.IRCF = 0b0110;   // 4MHz
    OSCCONbits.SCS = 1;         // Oscilador interno
    
    // Configuraciones del ADC
    ADCON0bits.ADCS = 0b01;     // Fosc/8
    
    ADCON1bits.VCFG0 = 0;       // VDD *Referencias internas
    ADCON1bits.VCFG1 = 1;       // VSS
    
    ADCON0bits.CHS = 0b0000;    // Seleccionamos AN0
    ADCON1bits.ADFM = 0;        // Justificado a la izquierda
    ADCON0bits.ADON = 1;        // Habilitamos modulo ADC
    __delay_us(40);
    
    ADCON0bits.CHS = 0b0001;    // Seleccionamos AN0
    ADCON1bits.ADFM = 0;        // Justificado a la izquierda
    ADCON0bits.ADON = 1;        // Habilitamos modulo ADC
    __delay_us(40);
    
    // Configuracion de interrupciones
    PIR1bits.ADIF = 0;          // Limpiamos bandera de int. ADC
    PIE1bits.ADIE = 1;          // Habilitamos int. de ADC
    INTCONbits.PEIE = 1;        // Habilitamos int. de perifericos
    INTCONbits.GIE = 1;         // Habilitamos int. globales
    
    IOCBbits.IOCB0 = 1;// SE ACTIVA LA INTERRUPCION DEL CAMBIO EN PORTB
    IOCBbits.IOCB1 = 1;// SE ACTIVA LA INTERRUPCION DEL CAMBIO EN PORTB
    PORTB = 0;
    INTCONbits.RBIF = 0;// SE LIMPIA LA BANDERA
    INTCONbits.RBIE = 1;
    
    OPTION_REGbits.T0CS = 0;// SE CATIVA LA INTERRUPCION DEL TMR0 DE 2MS
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 0;
    
    TMR0 = 252;
    INTCONbits.T0IF = 0; // SE  LIMPIA LA BANDERA
    INTCONbits.T0IE = 1;
}

int convertir(int a, int i){ //FUNCION PARA OBTENR VALORES
   int centenas;
   int decenas;
   int unidades;
   centenas = a/100; // OBTENENER LAS CENTENAS
   decenas = (a -(centenas*100))/10; // OBTNERE LAS DECENAS 
   unidades = ((a -(centenas*100)) -(decenas*10)); //OBTENER LAS UNIDADES
   int valores[3] = {unidades, decenas, centenas}; //FORMAR UN ARREGLO CON LOS TRES VALORES
   int j =+ valores[i]; //LLAMAR EL VALOR DESEADO
   return j;
}



