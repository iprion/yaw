#define GL_SILENCE_DEPRECATION

#include "yaw.h"

int main(int, char**)
{
    Yaw app;

    if (!app.build("Yet Another Wheel", 1280, 720, true))
        return 1;
    
    app.start();

    app.shutdown();

    return 0;
}
