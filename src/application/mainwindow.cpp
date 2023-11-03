#include "mainwindow.hpp"

#include "utils/format.hpp"
#include <iostream>

#include <stdexcept>
#include <vector>

MainWindow::MainWindow(const std::string& appName, unsigned int width, unsigned int height) {
    
    glfwSetErrorCallback(MainWindow::glfwErrorCallback);
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW!");

	// Create window with Vulkan context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    mainWindow = glfwCreateWindow(width, height, appName.c_str(), nullptr, nullptr);
    if (mainWindow == nullptr)
        throw std::runtime_error("GLFW: Vulkan Not Supported");

    if (!glfwVulkanSupported())
        throw std::runtime_error("GLFW: Vulkan Not Supported");

    updateViewPort();
}

MainWindow::~MainWindow() {

	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}


int MainWindow::width() const { 
    int width, height;
    glfwGetFramebufferSize(mainWindow, &width, &height);
    return width; 
};

int MainWindow::height() const { 
    int width, height;
    glfwGetFramebufferSize(mainWindow, &width, &height);
    return height; 
};


void MainWindow::glfwErrorCallback(int error, const char* description)
{
    std::cerr << std::format("GLFW Error {0}: {1]", error, description) << std::endl;
}

std::vector<const char*>  MainWindow::requiredExtensions() {
    std::vector<const char*> extensions;
    uint32_t extensions_count = 0;
    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&extensions_count);

    for (uint32_t i = 0; i < extensions_count; i++)
        extensions.push_back(glfw_extensions[i]);
    
    return extensions;
}


void MainWindow::events() {
     glfwPollEvents();
}

bool MainWindow::closed() {
    return glfwWindowShouldClose(mainWindow);
}

VkSurfaceKHR MainWindow::createSurface(const Device& device) {

    VkSurfaceKHR surface;
    VkResult err = glfwCreateWindowSurface(device.g_Instance, mainWindow, device.g_Allocator, &surface);
    Device::check_vk_result(err);
    return surface;
}


void MainWindow::clear() {
 //   glClearColor(0.45f, 0.55f,0.60f,1.00f);
 //   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MainWindow::updateViewPort() {
     //glViewport(0, 0, width(), height());    
}


