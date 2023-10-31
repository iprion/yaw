#include "ImGuiGLFWApp.h"
#include "utils/format.h"
#include <iostream>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

void ImGuiGLFWApp::glfwErrorCallback(int error, const char* description)
{
    std::cerr << std::format("GLFW Error {0}: {1]", error, description) << std::endl;
}

bool ImGuiGLFWApp::build(const std::string& appName, unsigned int width, unsigned int height, bool dark) {
    
    height_ = height;
    width_ = width;

    glfwSetErrorCallback(ImGuiGLFWApp::glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return false;
    }

		// Decide GL+GLSL versions
    #if __APPLE__
        // GL 3.2 + GLSL 150
        const char *glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // Required on Mac
    #else
        // GL 3.0 + GLSL 130
        const char *glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    #endif

    // Create window with graphics context
    mainWindow = glfwCreateWindow(width, height, appName.c_str(), nullptr, nullptr);
    if (mainWindow == nullptr)
        return false;
    glfwMakeContextCurrent(mainWindow);
    glfwSwapInterval(1); // Enable vsync

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
		return false;
	}
    
    updateViewPort();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO(); (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; 
  
    if (dark)
        ImGui::StyleColorsDark();
    else
        ImGui::StyleColorsLight();

    /*ImGuiStyle& style = ImGui::GetStyle();
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }*/

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return init();
}

void ImGuiGLFWApp::newFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
void ImGuiGLFWApp::shutdown() {
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

void ImGuiGLFWApp::clear() {
    glClearColor(0.45f, 0.55f,0.60f,1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ImGuiGLFWApp::endFrame() {
    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    updateViewPort();

    glfwSwapBuffers(mainWindow);
}

void ImGuiGLFWApp::updateViewPort() {
    // if resize
    int display_w, display_h;
    glfwGetFramebufferSize(mainWindow, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);    

}

void ImGuiGLFWApp::events() {
    
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    glfwPollEvents();
}

bool ImGuiGLFWApp::closed() {
    return glfwWindowShouldClose(mainWindow);
}

void ImGuiGLFWApp::start() {
    
    while (!closed())
    {
        events();

        newFrame();
  
        clear();        

        ui();
        update();
        draw();
        
        endFrame();
    }
}


