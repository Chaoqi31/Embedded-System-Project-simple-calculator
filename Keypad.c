#include "keypad.h"
#include "tm4c123gh6pm.h"
#include "PLL.h"

// Keypad mapping
const char KEYPAD_MAP[4][4] = {
    {'1', '2', '3', '+'},  
    {'4', '5', '6', '-'},  
    {'7', '8', '9', '*'},  
    {'C', '0', '=', '/'}  
};

void Keypad_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x18;  
    while ((SYSCTL_PRGPIO_R & 0x18) == 0); 

    GPIO_PORTD_DIR_R = 0x0F;  // PD[0:3] (COL1-COL4)
    GPIO_PORTD_DEN_R = 0x0F;  // PD[0:3]
    GPIO_PORTD_DATA_R = 0x00; 

    GPIO_PORTE_DIR_R = 0x00;  // PE[0:3] (ROW1-ROW4)
    GPIO_PORTE_DEN_R = 0x0F;  
    GPIO_PORTE_PDR_R = 0x0F;  
}

char Keypad_Read(void) {
    for (int col = 0; col < 4; col++) {
        GPIO_PORTD_DATA_R = (1 << col);  

        for (int row = 0; row < 4; row++) {
            if (GPIO_PORTE_DATA_R & (1 << row)) { 
                Delay_ms(20);  
                if (GPIO_PORTE_DATA_R & (1 << row)) {  
                    char key = KEYPAD_MAP[row][col];  
                    while (GPIO_PORTE_DATA_R & (1 << row));  
                    GPIO_PORTD_DATA_R = 0x00;  
                    return key;
                }
            }
        }
    }
    GPIO_PORTD_DATA_R = 0x00;  
    return 0; 
}

