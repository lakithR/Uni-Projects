#define F_CPU 8000000UL 
#define BTN1 (PINB & 0x01) 
#define BTN2 (PINB & 0x02) 
 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <util/delay.h> 
 
uint8_t time_seconds = 0; 
uint8_t time_minutes = 0; 
uint8_t freeze = 0; // Variable for stopping or resuming clock updates
 
void decode(signed int count) { 
    if (count == 1) { PORTD = 0B00000110; } 
    else if (count == 2) { PORTD = 0B01011011; }  
    else if (count == 3) { PORTD = 0B01001111; } 
    else if (count == 4) { PORTD = 0B01100110; } 
    else if (count == 5) { PORTD = 0B01101101; }  
    else if (count == 6) { PORTD = 0B01111101; }  
    else if (count == 7) { PORTD = 0B00000111; }  
    else if (count == 8) { PORTD = 0B01111111; }  
    else if (count == 9) { PORTD = 0B01101111; } 
    else { PORTD = 0B00111111; } 
}

void update_display(void) { 
    // Display 0 
    PORTC = 0B00000001; 
    decode((int)(time_seconds % 10)); 
    _delay_ms(4); 
    PORTC = 0B00000000; 
  
    // Display 1 
    PORTC = 0B00000010; 
    decode((int)((time_seconds - time_seconds % 10) / 10)); 
    _delay_ms(4); 
    PORTC = 0B00000000; 
  
    // Display 2 
    PORTC = 0B00000100; 
    decode((int)(time_minutes % 10)); 
    _delay_ms(4); 
    PORTC = 0B00000000; 
  
    // Display 3 
    PORTC = 0B00001000; 
    decode((int)((time_minutes - time_minutes % 10) / 10)); 
    _delay_ms(4); 
    PORTC = 0B00000000; 
}

int main(void) { 
    // Port D is connected to the 7-segment 
    DDRD = 0B01111111; 
    PORTD = 0B00110000;  
 
    // Port C data direction setup 
    DDRC = 0B00001111; 
   
    // Port B is connected to push buttons 
    DDRB = 0B00000000; 
  
    // Timer setup
    SREG |= (1 << 7); // Global Interrupt Enable 
    TIMSK1 |= (1 << 1); // Timer1 Compare OCR1A Interrupt Enable 
    OCR1A = 0x3D09; // Required Value of the Timer Register to Time 1 second 
    TCCR1B |= (1 << 3); // Clear Timer1 on Compare OCR1A 
    // Select Clock Source to clk/1024 
    TCCR1B |= (1 << 0); 
    TCCR1B |= (1 << 2); 
  
    while (1) { 
        if (BTN1) { 
            freeze = 0; 
        } 
        if (BTN2) { 
            freeze = 1; 
        } 
        update_display(); 
    } 
}

ISR(TIMER1_COMPA_vect) { 
    if (!freeze) { 
        if (time_seconds >= 59) { 
            time_minutes++; 
            time_seconds = 0; 
        } else { 
            time_seconds++; 
        } 
    } 
}
