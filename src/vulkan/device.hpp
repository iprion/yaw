#pragma once

#include <vulkan/vulkan.h>

#include <string>

class Device {

public :
    Device(std::vector<const char*> requiredExtensions);
    ~Device();
    static void check_vk_result(VkResult err);
    VkDevice device() { return g_Device; };

protected:
    virtual void ui() {};
    virtual void draw() {};
    virtual void update() {};
    virtual bool init() { return true; };

private : 
    bool IsExtensionAvailable(const std::vector<VkExtensionProperties>& properties, const char* extension);
    VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();
    void SetupVulkan(std::vector<const char*> instance_extensions);
    void CleanupVulkan();

    void updateViewPort();
    void newFrame();
    void endFrame();
    void clear();
    void events();
    bool closed();

public :

    // Data
    VkAllocationCallbacks*   g_Allocator;
    VkInstance               g_Instance;
    VkPhysicalDevice         g_PhysicalDevice;
    VkDevice                 g_Device;
    uint32_t                 g_QueueFamily;
    VkQueue                  g_Queue;
    VkDebugReportCallbackEXT g_DebugReport;
    VkPipelineCache          g_PipelineCache;
    VkDescriptorPool         g_DescriptorPool;
    int                      g_MinImageCount;
    bool                     g_SwapChainRebuild;
};
