﻿#pragma once

#include <vk_initializers.h>
#include <vk_types.h>

// bootstrap library
#include "VkBootstrap.h"

/*
Doing callbacks like this is inneficient at scale, because we are storing whole std::functions for every object we are
deleting, which is not going to be optimal. For the amount of objects we will use in this tutorial, its going to be
fine. but if you need to delete thousands of objects and want them deleted faster, a better implementation would be to
store arrays of vulkan handles of various types such as VkImage, VkBuffer, and so on. And then delete those from a
loop.
*/

constexpr unsigned int FRAME_OVERLAP = 2;

class VulkanEngine
{
  public:
    DeletionQueue _mainDeletionQueue;
    FrameData _frames[FRAME_OVERLAP];
    VkInstance _instance;                      // Vulkan library handle
    VkDebugUtilsMessengerEXT _debug_messenger; // Vulkan debug output handle
    VkPhysicalDevice _chosenGPU;               // GPU chosen as the default device
    VkDevice _device;                          // Vulkan device for commands
    VkSurfaceKHR _surface;
    VkQueue _graphicQueue;
    uint32_t _graphicsQueueFamily;

    VkSwapchainKHR _swapchain;
    VkFormat _swapchainImageFormat;

    std::vector<VkImage> _swapchainImages;
    std::vector<VkImageView> _swapchainImageViews;
    VkExtent2D _swapchainExtent; // Vulkan window surface

    AllocatedImage _drawImage;
    VkExtent2D _drawExtent;

    VmaAllocator _allocator;

    // draw resources

    bool _isInitialized{false};
    int _frameNumber{0};
    bool stop_rendering{false};
    VkExtent2D _windowExtent{1700, 900};

    struct SDL_Window *_window{nullptr};

    static VulkanEngine &Get();
    FrameData &get_current_frame() { return _frames[_frameNumber % FRAME_OVERLAP]; }

    // initializes everything in the engine
    void init();

    // shuts down the engine
    void cleanup();

    // draw loop
    void draw();

    // run main loop
    void run();

  private:
    void init_vulkan();
    void init_swapchain();
    void init_commands();
    void init_sync_structures();
    void create_swapchain(uint32_t width, uint32_t height);
    void destroy_swapchain();
    void draw_background(VkCommandBuffer cmd);
};
