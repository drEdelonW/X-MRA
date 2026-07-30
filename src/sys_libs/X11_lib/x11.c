#include "x11.h"

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void drawWindow() {
    Display* display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Can't open display\n");
        exit(1);
    }

    int s = DefaultScreen(display);
    Window window = XCreateSimpleWindow(
        display, RootWindow(display, s),
        10, 10,
        200, 100,
        1,
        BlackPixel(display, s),
        WhitePixel(display, s)
    );

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    while (1) {
        XEvent event;
        XNextEvent(display, &event);

        if (event.type == Expose) {
            XFillRectangle(
                display, window, DefaultGC(display, s),
                20, 20,
                10, 10
            );
            char msg[] = "Hello, X Window!";
            XDrawString(
                display, window, DefaultGC(display, s),
                50, 50,
                msg, strlen(msg)
            );
        }

        if (event.type == KeyPress)
            break;
    }

    XCloseDisplay(display);

}