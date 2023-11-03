#include "yaw.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "TestMyGLFWWindow.h"
#include <iostream>


Yaw::Yaw(const std::string& appName, unsigned int width, unsigned int height, bool dark)
    : Application(appName, width,height, dark) {

}

bool Yaw::init() {

    // create our geometries
	//triangle.init();
	//triangle2.init();

    //opengGLWindow1 = new TestMyGLFWWindow("Test OpenGl 1", 200,200);
    //opengGLWindow2 = new TestMyGLFWWindow("Test OpenGl 2", 200,200);
    
    //viewer.initializeGL();
    //viewer.resizeGL(width(), height());

    return true;
}

void Yaw::ui() { 
    // render your GUI
    ImGui::Begin("Triangle Position/Color");
    static float rotation = 0.0;
    if (ImGui::SliderFloat("rotation", &rotation, 0, 2 * M_PI))
        triangle.setUniform("rotation", rotation);

    static float translation[] = {0.0, 0.0};
    if (ImGui::SliderFloat2("position", translation, -1.0, 1.0))
        triangle.setUniform("translation", translation[0], translation[1]);
    
    static float color[4] = { 1.0f,1.0f,1.0f,1.0f };
    if (ImGui::ColorEdit3("color", color))
        triangle.setUniform("color", color[0], color[1], color[2]);
    
    ImGui::End();

    //opengGLWindow1->ui();
    //opengGLWindow2->ui();
}

void Yaw::draw() {   

    //viewer.paintGL();
    
    //triangle.draw();
    //triangle2.draw();
    
    //opengGLWindow1->draw();
    //opengGLWindow2->draw();
}

void Yaw::update() {
    //viewer.update();
    //opengGLWindow1->update();
    //opengGLWindow2->update();
}


