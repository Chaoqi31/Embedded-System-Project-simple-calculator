#include "calculator.h"
#include "lcd.h"
#include "keypad.h"
#include <string.h>
#include <stdio.h>

#define MAX_INPUT 16

char PASSWORD[MAX_INPUT] = "0000"; 
char inputBuffer[MAX_INPUT];
int inputIndex = 0;

#define LOCK_SEQUENCE_KEY '*' 
#define LOCK_CONFIRM_KEY '='   
int lockPendingCount = 0;      

void Calculator_Init(void) {
    inputIndex = 0;
    lockPendingCount = 0;
    for (int i = 0; i < MAX_INPUT; i++) {
        inputBuffer[i] = '\0';
    }
    LCD_Clear();
    LCD_DisplayString("Enter: ");
}

void Calculator_ChangePassword(void) {
    LCD_Clear();
    LCD_DisplayString("Old Password:");
    LCD_MoveCursor(1, 0);

    inputIndex = 0;
    while (1) {
        char key = Keypad_Read();
        if (key) {
            if (key == 'C') { 
                inputIndex = 0;
                for (int i = 0; i < MAX_INPUT; i++) {
                    inputBuffer[i] = '\0';
                }
                LCD_Clear();
                LCD_DisplayString("Old Password:");
                LCD_MoveCursor(1, 0);

            } else if (key == '=') {
                inputBuffer[inputIndex] = '\0';
                if (strcmp(inputBuffer, PASSWORD) == 0) {
                    LCD_Clear();
                    LCD_DisplayString("New Password:");
                    LCD_MoveCursor(1, 0);

                    inputIndex = 0;
                    while (1) {
                        key = Keypad_Read();
                        if (key) {
                            if (key == 'C') {
                                inputIndex = 0;
                                for (int i = 0; i < MAX_INPUT; i++) {
                                    inputBuffer[i] = '\0';
                                }
                                LCD_Clear();
                                LCD_DisplayString("New Password:");
                                LCD_MoveCursor(1, 0);

                            } else if (key == '=') {
                                inputBuffer[inputIndex] = '\0';
                                strcpy(PASSWORD, inputBuffer);
                                LCD_Clear();
                                LCD_DisplayString("Password Changed");
                                Delay_ms(2000);
                                return; 

                            } else {
                                if (inputIndex < MAX_INPUT - 1) {
                                    inputBuffer[inputIndex++] = key;
                                    LCD_DisplayChar('*');  
                                }
                            }
                        }
                    }
                } else {
                    LCD_Clear();
                    LCD_DisplayString("Password");
                    LCD_MoveCursor(1, 0);
                    LCD_DisplayString("Incorrect");
                    Delay_ms(2000);
                    return; 
                }
            } else {
                if (inputIndex < MAX_INPUT - 1) {
                    inputBuffer[inputIndex++] = key;
                    LCD_DisplayChar('*');  
                }
            }
        }
    }
}

int Calculator_CheckPassword(void) {
    LCD_Clear();
    LCD_DisplayString("Enter Password:");
    LCD_MoveCursor(1, 0);

    inputIndex = 0;
    while (1) {
        char key = Keypad_Read();
        if (key) {
            if (key == '/') {
                Calculator_ChangePassword();
                LCD_Clear();
                LCD_DisplayString("Enter Password:");
                LCD_MoveCursor(1, 0);
                inputIndex = 0;
            } else if (key == 'C') {
                inputIndex = 0;
                for (int i = 0; i < MAX_INPUT; i++) {
                    inputBuffer[i] = '\0';
                }
                LCD_Clear();
                LCD_DisplayString("Enter Password:");
                LCD_MoveCursor(1, 0);
            } else if (key == '=') {
                inputBuffer[inputIndex] = '\0';
                if (strcmp(inputBuffer, PASSWORD) == 0) {
                    LCD_Clear();
                    LCD_DisplayString("Password Correct");
                    Delay_ms(2000);
                    return 1; 
                } else {
                    LCD_Clear();
                    LCD_DisplayString("Password");
                    LCD_MoveCursor(1, 0);
                    LCD_DisplayString("Incorrect");
                    Delay_ms(2000);
                    inputIndex = 0;
                    LCD_Clear();
                    LCD_DisplayString("Enter Password:");
                    LCD_MoveCursor(1, 0);
                }
            } else {
                if (inputIndex < MAX_INPUT - 1) {
                    inputBuffer[inputIndex++] = key;
                    LCD_DisplayChar('*');  
                }
            }
        }
    }
}

static void LockCalculator(void) {
    LCD_Clear();
    LCD_DisplayString("Calculator");
    LCD_MoveCursor(1, 0);
    LCD_DisplayString("Locked");
    Delay_ms(2000);

    while (!Calculator_CheckPassword()) {
    }
    Calculator_Init();
}

float PerformOperation(float num1, float num2, char op) {
    switch (op) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/':
            if (num2 == 0) {
                LCD_Clear();
                LCD_DisplayString("Error: Div by 0");
                Delay_ms(2000);
                Calculator_Init();
                return 0;
            }
            return num1 / num2;
        default: 
            return 0;  
    }
}

int GetPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;  
}

float Calculator_Evaluate(const char* expression) {
    float numStack[MAX_INPUT];  
    char opStack[MAX_INPUT];    
    int numTop = -1, opTop = -1;

    int i = 0;
    while (expression[i] != '\0') {
        if (expression[i] >= '0' && expression[i] <= '9') {
            float num = 0;
            while (expression[i] >= '0' && expression[i] <= '9') {
                num = num * 10 + (expression[i] - '0');
                i++;
            }
            if (expression[i] == '.') {
                i++;
                float decimalPlace = 0.1f;
                while (expression[i] >= '0' && expression[i] <= '9') {
                    num += (expression[i] - '0') * decimalPlace;
                    decimalPlace *= 0.1f;
                    i++;
                }
            }
            numStack[++numTop] = num;
        }
        else if (expression[i] == '+' || expression[i] == '-' ||
                 expression[i] == '*' || expression[i] == '/') {
            while (opTop >= 0 && GetPrecedence(opStack[opTop]) >= GetPrecedence(expression[i])) {
                float num2 = numStack[numTop--];
                float num1 = numStack[numTop--];
                char op = opStack[opTop--];
                numStack[++numTop] = PerformOperation(num1, num2, op);
            }
            opStack[++opTop] = expression[i];
            i++;
        }
        else {
            i++;
        }
    }

    while (opTop >= 0) {
        float num2 = numStack[numTop--];
        float num1 = numStack[numTop--];
        char op = opStack[opTop--];
        numStack[++numTop] = PerformOperation(num1, num2, op);
    }

    return numStack[numTop];
}

void Calculator_Run(void) {
    char key;
    while (1) {
        key = Keypad_Read();
        if (key) {
            if (key == LOCK_SEQUENCE_KEY) {  
                lockPendingCount++;  
                if (lockPendingCount == 1) {
                    if (inputIndex < MAX_INPUT - 1) {
                        inputBuffer[inputIndex++] = '*';
                        LCD_DisplayChar('*');
                    }
                }
                else if (lockPendingCount == 2) {
                    LCD_DisplayChar('*'); 
                }
                
            } else if (key == LOCK_CONFIRM_KEY && lockPendingCount >= 2) {
                LockCalculator();
                lockPendingCount = 0;

            } else {
                lockPendingCount = 0;

                if (key == 'C') {  
                    Calculator_Init();

                } else if (key == '=') {
                    inputBuffer[inputIndex] = '\0';
                    if (inputIndex == 0) {
                        LCD_Clear();
                        LCD_DisplayString("Invalid Input");
                        Delay_ms(2000);
                        LCD_Clear();
                        LCD_DisplayString("Enter: ");
                        continue;
                    }
                    float result = Calculator_Evaluate(inputBuffer);
                    LCD_Clear();
                    char resultStr[16];
                    sprintf(resultStr, "Result: %.2f", result);
                    LCD_DisplayString(resultStr);
                    Delay_ms(3000);
                    Calculator_Init(); 

                } else {
                    if (inputIndex < MAX_INPUT - 1) {
                        inputBuffer[inputIndex++] = key;
                        LCD_DisplayChar(key);
                    }
                }
            }
        }
    }
}
