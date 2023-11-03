#define GL_SILENCE_DEPRECATION

#include "yaw.hpp"

int main(int, char**)
{
    Yaw app("Yet Another Wheel", 1280, 720, true);
    
    app.run();

    return 0;
}
