#ifndef IMGUI_OPENGL_APP_H
#define IMGUI_OPENGL_APP_H

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <string>

class ImGuiGLFWApp {

public :
    bool build(const std::string& appName, unsigned int width, unsigned int height, bool dark);
    void start();
    void shutdown();
    unsigned int width() const { return width_; };
    unsigned int height() const { return height_; };

protected:
    virtual void ui() {};
    virtual void draw() {};
    virtual void update() {};
    virtual bool init() { return true; };

private : 
    static void glfwErrorCallback(int error, const char* description);
    static void check_vk_result(VkResult err);
    static bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);
    static VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();
    static void SetupVulkan(ImVector<const char*> instance_extensions);
    static void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);
    static void CleanupVulkan();
    static void CleanupVulkanWindow();
    static void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
    static void FramePresent(ImGui_ImplVulkanH_Window* wd);

    void updateViewPort();
    void newFrame();
    void endFrame();
    void clear();
    void events();
    bool closed();

private :
    GLFWwindow* mainWindow;
    ImGui_ImplVulkanH_Window* vulkanWindow;
    ImVec4 clear_color;    
    unsigned int height_;
    unsigned int width_;
    ImGuiIO* io;

    // Data
    static VkAllocationCallbacks*   g_Allocator;
    static VkInstance               g_Instance;
    static VkPhysicalDevice         g_PhysicalDevice;
    static VkDevice                 g_Device;
    static uint32_t                 g_QueueFamily;
    static VkQueue                  g_Queue;
    static VkDebugReportCallbackEXT g_DebugReport;
    static VkPipelineCache          g_PipelineCache;
    static VkDescriptorPool         g_DescriptorPool;
    static ImGui_ImplVulkanH_Window g_MainWindowData;
    static int                      g_MinImageCount;
    static bool                     g_SwapChainRebuild;
};


#endif