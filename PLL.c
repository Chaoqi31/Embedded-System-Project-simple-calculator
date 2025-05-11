#include "PLL.h"
#include "tm4c123gh6pm.h"

void PLL_Init(void) {
    SYSCTL_RCC2_R |= 0x80000000;  
    SYSCTL_RCC2_R |= 0x00000800;  
    SYSCTL_RCC_R = (SYSCTL_RCC_R & ~0x000007C0) + 0x00000540;  
    SYSCTL_RCC2_R &= ~0x00000070;  
    SYSCTL_RCC2_R &= ~0x00002000; 
    SYSCTL_RCC2_R |= 0x40000000; 
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~0x1FC00000) + (4 << 22); 
    while ((SYSCTL_RIS_R & 0x00000040) == 0); 
    SYSCTL_RCC2_R &= ~0x00000800; 
}

// Initialize SysTick timer
void SysTick_Init(void) {
    NVIC_ST_CTRL_R = 0;              
    NVIC_ST_RELOAD_R = 0x00FFFFFF;    
    NVIC_ST_CURRENT_R = 0;            
    NVIC_ST_CTRL_R = 0x05;           
}

// Millisecond delay
void Delay_ms(int ms) {
    for (int i = 0; i < ms; i++) {
        NVIC_ST_RELOAD_R = 80000 - 1; 
        NVIC_ST_CURRENT_R = 0;      
        while ((NVIC_ST_CTRL_R & 0x00010000) == 0); 
    }
}

// Microsecond delay
void Delay_us(int us) {
    for (int i = 0; i < us; i++) {
        NVIC_ST_RELOAD_R = 80 - 1;   
        NVIC_ST_CURRENT_R = 0;        
        while ((NVIC_ST_CTRL_R & 0x00010000) == 0); 
    }
}

