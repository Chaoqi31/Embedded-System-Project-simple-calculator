#include "lcd.h"
#include "keypad.h"
#include "calculator.h"
#include "PLL.h"

int main(void) {
    PLL_Init();        // Initialize system clock to 80 MHz
    SysTick_Init();    // Initialize SysTick for delay functions
    LCD_Init();        // Initialize the LCD
    Keypad_Init();     // Initialize the keypad

    // Password check
    if (Calculator_CheckPassword()) {
        Calculator_Run();  // Run the calculator if password is correct
    }

    return 0;
}

