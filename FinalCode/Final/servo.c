
#include "servo.h"
#include "button.h"


int pulse_period = 20 * 16000; 

#define REPLACE_ME 0x00


void servo_init1(void)
{
    unsigned int low_pulse = 0;
    
    SYSCTL_RCGCGPIO_R |= 0b0000010;              
    GPIO_PORTB_AFSEL_R |= 0b00100000;           
    GPIO_PORTB_PCTL_R &= 0xFF0FFFFF;              
    GPIO_PORTB_PCTL_R |= 0x00700000;             
    GPIO_PORTB_DEN_R |= 0b00100000;               
    GPIO_PORTB_DIR_R |= 0b00100000;               
 
    SYSCTL_RCGCTIMER_R |= 0b000010;            
    TIMER1_CTL_R &= 0xFEFF;                 
    TIMER1_CFG_R |= 0x4;                  
    TIMER1_TBMR_R |= 0b1010;                 

    TIMER1_TBILR_R |= ((pulse_period) & 0xFFFF);          
    TIMER1_TBPR_R = (((pulse_period) >> 16) & 0xFF);             
 
    TIMER1_TBMATCHR_R = (int)((pulse_period) & 0xFFFF);                 
    TIMER1_TBPMR_R = (int)(((pulse_period) >> 16) & 0xFF);                       

    TIMER1_CTL_R |= 0x0100;
}

void servo_move2(uint16_t degree) {
   
    float zerodeg = 19.58;
    float ninetydeg = 18.67;
    float oneeightydeg = 17.76;
    double pulsewidth;
    int matching;
    double test;
    
    float intercept = 0 ;           
    float slope = ( (zerodeg - oneeightydeg) / (180));              
    intercept = zerodeg;                      

    pulsewidth = (intercept - slope * (float)degree) ;    

    matching = pulsewidth * 16000;

    if(degree == 90){
        pulsewidth = ninetydeg;

        matching = pulsewidth * 16000;

        TIMER1_TBMATCHR_R = (matching & 0xFFFF);                           
        TIMER1_TBPMR_R = ((matching >> 16) & 0xFF);  
    }
    else{
        TIMER1_TBMATCHR_R = (matching & 0xFFFF);                           
        TIMER1_TBPMR_R = ((matching >> 16) & 0xFF); 
    }                    
    timer_waitMillis(20);
}

void servo_calibrate() {
    button_init();
    double pulsewidth = 19;
    uint8_t buttonval;
    int matching;
    double zeroval = 0;
    double oneeightyval = 0;

    while(1) {
        lcd_printf("1:L, 2:R, 3/4:val\nPulse Width: %.2f\n0 value:%.2f\n180 value:%.2f", pulsewidth, zeroval, oneeightyval);
        buttonval = button_getButton();

        switch(buttonval){
            case 1:
            pulsewidth = pulsewidth - 0.01;
            break;
            case 2:
            pulsewidth = pulsewidth + 0.01;
            break;
            case 3:
            oneeightyval = pulsewidth;
            break;
            case 4:
            zeroval = pulsewidth;
            break;
        }

        matching = pulsewidth * 16000;
        TIMER1_TBMATCHR_R = (matching & 0xFFFF);                           
        TIMER1_TBPMR_R = ((matching >> 16) & 0xFF);                        \
        timer_waitMillis(20);
    }

}
