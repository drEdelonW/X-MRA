#include "x11.h"

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void drawWindow(){
    Display *display;
    Window window;
    XEvent event;
    char *msg = "Hello, X Window!";
    int s;

    // Открываем соединение с X сервером
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Невозможно открыть дисплей\n");
        exit(1);
    }

    s = DefaultScreen(display);

    // Создаем окно
    window = XCreateSimpleWindow(display, RootWindow(display, s), 10, 10, 200, 100, 1,
                                 BlackPixel(display, s), WhitePixel(display, s));

    // Выбираем ввод событий для нашего окна
    XSelectInput(display, window, ExposureMask | KeyPressMask);

    // Делаем окно видимым на экране
    XMapWindow(display, window);

    // Бесконечный цикл для обработки событий
    while (1) {
        XNextEvent(display, &event);
        
        // Рисуем или перерисовываем окно
        if (event.type == Expose) {
            XFillRectangle(display, window, DefaultGC(display, s), 20, 20, 10, 10);
            XDrawString(display, window, DefaultGC(display, s), 50, 50, msg, strlen(msg));
        }
        
        // Закрытие окна при нажатии клавиши
        if (event.type == KeyPress)
            break;
    }

    // Закрываем соединение с X сервером
    XCloseDisplay(display);

}