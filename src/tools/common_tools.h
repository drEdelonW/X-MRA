#pragma once
#include "terminal_tools.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#define DEG_TO_RAD(x) ((x) * M_PI / 180.0f)
#define RAD_TO_DEG(x) ((x) * 180.0f / M_PI)


#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define CUT_MAX(v, max) {v = (v > max)? max : v; }
#define CUT_MIN(v, min) {v = (v < min)? min : v; }

#define BUFF_AND_SIZE(x)    x, sizeof(x)
// #define TODO(x) _Pragma(TO_STRING(message("TODO: " x)));

#include <stdio.h>

inline void ver_info() {
    printf("Branch: %s\n" TEXT_RESET, TEXT_BOLD GIT_BRANCH);
    printf("Commit: %s\n" TEXT_RESET, TEXT_BOLD GIT_COMMIT_HASH);
    printf("Compile date: %s  %s\n" TEXT_RESET, TEXT_BOLD __DATE__, __TIME__);
}

// #include <stdio.h>
// #include <string.h>
// #include <math.h>

// inline void floatToString(float value, char* buffer, int decimalPlaces) {
//     if (decimalPlaces < 0) {
//         decimalPlaces = 0;
//     }
//     int integerPart = (int)value;   // Получаем целую часть числа
//     float fractionalPart = value - (float)integerPart;    // Получаем дробную часть числа
//     sprintf(buffer, "%d", integerPart);    // Конвертируем целую часть в строку
//     if (decimalPlaces > 0) {    // Если необходимы знаки после запятой
//         strcat(buffer, ".");    // Добавляем десятичную точку
//         // Получаем дробную часть как целое число
//         fractionalPart = fabs(fractionalPart); // Убираем знак
//         for (int i = 0; i < decimalPlaces; i++) {
//             fractionalPart *= 10;
//         }
//         char fractionalStr[16];
//         sprintf(fractionalStr, "%d", (int)fractionalPart);
//         strcat(buffer, fractionalStr);
//     }
// }

// inline void print_float(float number) {
//     char buffer[50];
//     floatToString(number, buffer, 4);
//     printf("%s", buffer);
// }