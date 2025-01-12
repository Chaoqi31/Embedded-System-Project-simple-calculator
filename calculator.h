#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <stdint.h>

void Calculator_Init(void);
void Calculator_ChangePassword(void);
int Calculator_CheckPassword(void);
static void LockCalculator(void);
float Calculator_Evaluate(const char* expression);
void Calculator_Run(void);

#endif 