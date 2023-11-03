#include "application.hpp"

#include "utils/format.hpp"
#include <iostream>
#include <vector>

Application::Application(const std::string& appName, unsigned int width, unsigned int height, bool dark)
    : mainWindow(appName, width,height),
      device_(mainWindow.requiredExtensions()) {

    SetupVulkanWindow(mainWindow.width(), mainWindow.height());

    mainWindow.updateViewPort();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO(); (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  
    if (dark)
        ImGui::StyleColorsDark();
    else
        ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(mainWindow.window(), true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = device_.g_Instance;
    init_info.PhysicalDevice = device_.g_PhysicalDevice;
    init_info.Device = device_.g_Device;
    init_info.QueueFamily = device_.g_QueueFamily;
    init_info.Queue = device_.g_Queue;
    init_info.PipelineCache = device_.g_PipelineCache;
    init_info.DescriptorPool = device_.g_DescriptorPool;
    init_info.Subpass = 0;
    init_info.MinImageCount = device_.g_MinImageCount;
    init_info.ImageCount = vulkanWindow.ImageCount;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = device_.g_Allocator;
    init_info.CheckVkResultFn = Device::check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, vulkanWindow.RenderPass);

// Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Upload Fonts
    {
        // Use any command queue
        VkCommandPool command_pool = vulkanWindow.Frames[vulkanWindow.FrameIndex].CommandPool;
        VkCommandBuffer command_buffer = vulkanWindow.Frames[vulkanWindow.FrameIndex].CommandBuffer;

        VkResult err = vkResetCommandPool(device_.g_Device, command_pool, 0);
        Device::check_vk_result(err);
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(command_buffer, &begin_info);
        Device::check_vk_result(err);

        ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

        VkSubmitInfo end_info = {};
        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        end_info.commandBufferCount = 1;
        end_info.pCommandBuffers = &command_buffer;
        err = vkEndCommandBuffer(command_buffer);
        Device::check_vk_result(err);
        err = vkQueueSubmit(device_.g_Queue, 1, &end_info, VK_NULL_HANDLE);
        Device::check_vk_result(err);

        err = vkDeviceWaitIdle(device_.g_Device);
        Device::check_vk_result(err);
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    init();
}

Application::~Application() {
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
    CleanupVulkanWindow();
}


// All the ImGui_ImplVulkanH_XXX structures/functions are optional helpers used by the demo.
// Your real engine/app may not use them.
void Application::SetupVulkanWindow(int width, int height)
{
    VkSurfaceKHR surface = mainWindow.createSurface(device_);
 
    vulkanWindow.Surface = surface;

    // Check for WSI support
    VkBool32 res;
    vkGetPhysicalDeviceSurfaceSupportKHR(device_.g_PhysicalDevice, device_.g_QueueFamily, vulkanWindow.Surface, &res);
    if (res != VK_TRUE)
    {
        fprintf(stderr, "Error no WSI support on physical device 0\n");
        exit(-1);
    }

    // Select Surface Format
    const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
    const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    vulkanWindow.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(device_.g_PhysicalDevice, vulkanWindow.Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

    // Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
    VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
    VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
    vulkanWindow.PresentMode = ImGui_ImplVulkanH_SelectPresentMode(device_.g_PhysicalDevice, vulkanWindow.Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
    //printf("[vulkan] Selected PresentMode = %d\n", vulkanWindow.PresentMode);

    // Create SwapChain, RenderPass, Framebuffer, etc.
    IM_ASSERT(device_.g_MinImageCount >= 2);
    ImGui_ImplVulkanH_CreateOrResizeWindow(device_.g_Instance, device_.g_PhysicalDevice, device_.g_Device, &vulkanWindow, device_.g_QueueFamily, device_.g_Allocator, width, height, device_.g_MinImageCount);
}


void Application::CleanupVulkanWindow()
{
    ImGui_ImplVulkanH_DestroyWindow(device_.g_Instance, device_.g_Device, &vulkanWindow, device_.g_Allocator);
}

void Application::FrameRender(ImDrawData* draw_data)
{
    VkResult err;

    VkSemaphore image_acquired_semaphore  = vulkanWindow.FrameSemaphores[vulkanWindow.SemaphoreIndex].ImageAcquiredSemaphore;
    VkSemaphore render_complete_semaphore = vulkanWindow.FrameSemaphores[vulkanWindow.SemaphoreIndex].RenderCompleteSemaphore;
    err = vkAcquireNextImageKHR(device_.g_Device, vulkanWindow.Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &vulkanWindow.FrameIndex);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        device_.g_SwapChainRebuild = true;
        return;
    }
    Device::check_vk_result(err);

    ImGui_ImplVulkanH_Frame* fd = &vulkanWindow.Frames[vulkanWindow.FrameIndex];
    {
        err = vkWaitForFences(device_.g_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
        Device::check_vk_result(err);

        err = vkResetFences(device_.g_Device, 1, &fd->Fence);
        Device::check_vk_result(err);
    }
    {
        err = vkResetCommandPool(device_.g_Device, fd->CommandPool, 0);
        Device::check_vk_result(err);
        VkCommandBufferBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
        Device::check_vk_result(err);
    }
    {
        VkRenderPassBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.renderPass = vulkanWindow.RenderPass;
        info.framebuffer = fd->Framebuffer;
        info.renderArea.extent.width = vulkanWindow.Width;
        info.renderArea.extent.height = vulkanWindow.Height;
        info.clearValueCount = 1;
        info.pClearValues = &vulkanWindow.ClearValue;
        vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
    }

    // Record dear imgui primitives into command buffer
    ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

    // Submit command buffer
    vkCmdEndRenderPass(fd->CommandBuffer);
    {
        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &image_acquired_semaphore;
        info.pWaitDstStageMask = &wait_stage;
        info.commandBufferCount = 1;
        info.pCommandBuffers = &fd->CommandBuffer;
        info.signalSemaphoreCount = 1;
        info.pSignalSemaphores = &render_complete_semaphore;

        err = vkEndCommandBuffer(fd->CommandBuffer);
        Device::check_vk_result(err);
        err = vkQueueSubmit(device_.g_Queue, 1, &info, fd->Fence);
        Device::check_vk_result(err);
    }
}

void Application::FramePresent()
{
    if (device_.g_SwapChainRebuild)
        return;
    VkSemaphore render_complete_semaphore = vulkanWindow.FrameSemaphores[vulkanWindow.SemaphoreIndex].RenderCompleteSemaphore;
    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &render_complete_semaphore;
    info.swapchainCount = 1;
    info.pSwapchains = &vulkanWindow.Swapchain;
    info.pImageIndices = &vulkanWindow.FrameIndex;
    VkResult err = vkQueuePresentKHR(device_.g_Queue, &info);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        device_.g_SwapChainRebuild = true;
        return;
    }
    Device::check_vk_result(err);
    vulkanWindow.SemaphoreIndex = (vulkanWindow.SemaphoreIndex + 1) % vulkanWindow.ImageCount; // Now we can use the next set of semaphores
}

void Application::newFrame() {

    // Resize swap chain?
    if (device_.g_SwapChainRebuild)
    {
        int width = mainWindow.width();
        int height = mainWindow.height();
        glfwGetFramebufferSize(mainWindow.window(), &width, &height);
        if (width > 0 && height > 0)
        {
            ImGui_ImplVulkan_SetMinImageCount(device_.g_MinImageCount);
            ImGui_ImplVulkanH_CreateOrResizeWindow(device_.g_Instance, device_.g_PhysicalDevice, device_.g_Device, &vulkanWindow, device_.g_QueueFamily, device_.g_Allocator, width, height, device_.g_MinImageCount);
            vulkanWindow.FrameIndex = 0;
            device_.g_SwapChainRebuild = false;
        }
    }

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Application::clear() {
 //   glClearColor(0.45f, 0.55f,0.60f,1.00f);
 //   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Application::endFrame() {
    
    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
    if (!is_minimized)
    {
        vulkanWindow.ClearValue.color.float32[0] = clear_color.x * clear_color.w;
        vulkanWindow.ClearValue.color.float32[1] = clear_color.y * clear_color.w;
        vulkanWindow.ClearValue.color.float32[2] = clear_color.z * clear_color.w;
        vulkanWindow.ClearValue.color.float32[3] = clear_color.w;
        FrameRender(draw_data);
        FramePresent();
    }

    mainWindow.updateViewPort();

    //glfwSwapBuffers(mainWindow.window());
}




void Application::run() {
    
    while (!mainWindow.closed())
    {
        mainWindow.events();

        newFrame();
  
        mainWindow.clear();        

        ui();

        update();
        
        draw();
        
        endFrame();
    }
}


