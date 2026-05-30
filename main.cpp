#include <cstddef>
#include <vector>
#include <iostream>

namespace {
    PFN_vkCreateInstance pfnCreateInstance = (PFN_vkCreateInstance)
        glfwGetInstanceProcAddress(NULL, "vkCreateInstance");
    PFN_vkDestroySurfaceKHR pfnDestroySurface = (PFN_vkDestroySurfaceKHR)
        glfwGetInstanceProcAddress(NULL, "vkDestroySurfaceKHR");
    PFN_vkEnumeratePhysicalDevices pfnEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)
        glfwGetInstanceProcAddress(NULL, "vkEnumeratePhysicalDevices");
    PFN_vkGetPhysicalDeviceProperties2 pfnPhysicalDeviceProperties2 = (PFN_vkGetPhysicalDeviceProperties2)
        glfwGetInstanceProcAddress(NULL, "vkGetPhysicalDeviceProperties2");
    PFN_vkGetPhysicalDeviceQueueFamilyProperties pfnGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)
        glfwGetInstanceProcAddress(NULL, "vkGetPhysicalDeviceQueueFamilyProperties");
    PFN_vkCreateDevice pfnCreateDevice = (PFN_vkCreateDevice)
        glfwGetInstanceProcAddress(NULL, "vkCreateDevice");
    PFN_vkGetDeviceQueue pfnGetDeviceQueue = (PFN_vkGetDeviceQueue)
        glfwGetInstanceProcAddress(NULL, "vkGetDeviceQueue");
    PFN_vkDestroyDevice pfnDestroyDevice = (PFN_vkDestroyDevice)
        glfwGetInstanceProcAddress(NULL, "vkDestroyDevice");
    PFN_vkGetInstanceProcAddr pfnGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)
        glfwGetInstanceProcAddress(NULL, "vkGetInstanceProcAddr");
    PFN_vkGetDeviceProcAddr pfnGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)
        glfwGetInstanceProcAddress(NULL, "vkGetDeviceProcAddr");
    PFN_vkCreateImage pfnCreateImage = (PFN_vkCreateImage)
        glfwGetInstanceProcAddress(NULL, "vkCreateImage");
    PFN_vkGetPhysicalDeviceProperties pfnGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)
        glfwGetInstanceProcAddress(NULL, "vkGetPhysicalDeviceProperties");
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR pfnGetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
        glfwGetInstanceProcAddress(NULL, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
   PFN_vkCreateSwapchainKHR pfnCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)
        glfwGetInstanceProcAddress(NULL, "vkCreateSwapchainKHR");
   PFN_vkGetPhysicalDeviceFormatProperties2 pfnGetPhysicalDeviceFormatProperties2 = (PFN_vkGetPhysicalDeviceFormatProperties2)
        glfwGetInstanceProcAddress(NULL, "vkGetPhysicalDeviceFormatProperties2");
}


#include <cstring>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "ps_vulkan_fn_load.h"

#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include <vk_mem_alloc.h>

constexpr int ERR_NO_VULKAN_SUPPORT = 1;
constexpr int ERR_GLFW_INIT_FAILED = 2;
constexpr int ERR_GLFW_WINDOW_INIT_FAILED = 3;
constexpr int ERR_VULKAN_EXT_LOAD_ERR = 4;
constexpr int ERR_VULKAN_SURFACE_FAILED = 5;


// Custom error logger. Automatically newlines
void custom_err(int error, const std::string& desc){
    std::cout << "ERROR [" << error << "] " << desc << "\n";
}

// GLFW-specific error logger
void error_callback(int error, const char* description){
    std::cout << "GLFW RUNETIME ERR: " << description << "\n";
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
}

void GLFW_KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods){
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

 
int main(){
    GLFWwindow *window = nullptr;

    glfwSetErrorCallback(error_callback);

    if (glfwInit() == 0){
        custom_err(ERR_GLFW_INIT_FAILED, "glfw could not initialize");
        return 1;
    }

    if (glfwVulkanSupported() == 0){
        custom_err(ERR_NO_VULKAN_SUPPORT, "glfw could not identify vulkan support");
        return ERR_NO_VULKAN_SUPPORT;
    }

    uint32_t extensionsCount = 0;
    const char** extensions = glfwGetRequiredInstanceExtensions(&extensionsCount);

    if (extensions == nullptr){
        custom_err(ERR_VULKAN_EXT_LOAD_ERR, "glfw could not load reuqired vulkan extensions");
        return ERR_VULKAN_EXT_LOAD_ERR;
    }

    // FUNCTION LOADING
    // NOLINTBEGIN casting to c-functions
   
    // NOLINTEND

    VkApplicationInfo appInfo {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Piston-Slap",
        .apiVersion = VK_API_VERSION_1_3
    };

    VkInstanceCreateInfo instanceCreateInfo {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = extensionsCount,
        .ppEnabledExtensionNames = extensions,
    };
  
    // Main Instance handle
    VkInstance instance{nullptr};
    pfnCreateInstance(&instanceCreateInfo, nullptr, &instance);

    // Devices query
    uint32_t deviceCount{ 0 };
    pfnEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    std::vector<VkPhysicalDevice> devices(deviceCount);
    pfnEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    std::cout << "Device count " << deviceCount << "\n";

    if (deviceCount == 0){
        custom_err(-1, "vulkan found no devices");
        return 1;
    }

    // device query
    VkPhysicalDeviceProperties2 devicePropertiesInfo{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2 };

    for (uint32_t i = 0; i < deviceCount; ++i){
        pfnPhysicalDeviceProperties2(devices[i], &devicePropertiesInfo);
        std::cout << "Device index " << i << " " << devicePropertiesInfo.properties.deviceName << "\n";
    }

    // selecting a device
    uint32_t deviceIndex{deviceCount};
    const std::string INVALID_MSG = "Invalid selection\n";
    const std::string DEVICE_SELECTION_MSG = "Type device index for renderer\n";

    while (true) {
        std::cout << DEVICE_SELECTION_MSG;
        std::cin >> deviceIndex;

        if (deviceIndex >= deviceCount){
            std::cout << INVALID_MSG;
        }
        else {
            break;
        }
    }

    pfnPhysicalDeviceProperties2(devices[deviceIndex], &devicePropertiesInfo);
    std::cout << "Using " << devicePropertiesInfo.properties.deviceName << "\n";

    // Selecting queue fam from the selected device
    uint32_t queueFamilyCount{ 0 };
    pfnGetPhysicalDeviceQueueFamilyProperties(devices[deviceIndex], &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    pfnGetPhysicalDeviceQueueFamilyProperties(devices[deviceIndex], &queueFamilyCount, queueFamilies.data());

    uint32_t queueFamilyIndex{ 0 };
    for (size_t i = 0; i < queueFamilies.size(); i++) {
        if ((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0U) {
            queueFamilyIndex = i;
            std::cout << "Selecting queue family " << i << "\n";
            break;
        }
    }

    // connecting to logical device
    const float qfpriorities{ 1.0F };
    VkDeviceQueueCreateInfo queueCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = queueFamilyIndex,
        .queueCount = 1,
        .pQueuePriorities = &qfpriorities
    };

    const std::vector<const char*> deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    VkPhysicalDeviceVulkan12Features enabledVk12Features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .descriptorIndexing = 1U,
        .shaderSampledImageArrayNonUniformIndexing = 1U,
        .descriptorBindingVariableDescriptorCount = 1U,
        .runtimeDescriptorArray = 1U,
        .bufferDeviceAddress = 1U
    };

    VkPhysicalDeviceVulkan13Features enabledVk13Features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .pNext = &enabledVk12Features,
        .synchronization2 = 1U,
        .dynamicRendering = 1U,
    };

    VkPhysicalDeviceFeatures enabledVk10Features{
        .samplerAnisotropy = VK_TRUE
    };

    VkDeviceCreateInfo deviceCreateInfo{
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext = &enabledVk13Features,
    .queueCreateInfoCount = 1,
    .pQueueCreateInfos = &queueCreateInfo,
    .enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
    .ppEnabledExtensionNames = deviceExtensions.data(),
    .pEnabledFeatures = &enabledVk10Features
    };

    VkDevice device{NULL};
    VkQueue queue{NULL};
    pfnCreateDevice(devices[deviceIndex], &deviceCreateInfo, nullptr, &device);
    pfnGetDeviceQueue(device, queueFamilyIndex, 0, &queue);

    if (device == NULL){
        custom_err(-1, "vulkan could not create logical device");
        return 1;
    }

    if (queue == NULL){
        custom_err(-1, "vulkan could not create logical queue");
        return 1;
    }

    // VMA setup
    VmaAllocator allocator{NULL};

    VmaVulkanFunctions vkFunctions{};

    vkFunctions.vkGetInstanceProcAddr = pfnGetInstanceProcAddr;
    vkFunctions.vkGetDeviceProcAddr = pfnGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocatorCI{
        .flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
        .physicalDevice = devices[deviceIndex],
        .device = device,
        .pVulkanFunctions = &vkFunctions,
        .instance = instance
    };

    vmaCreateAllocator(&allocatorCI, &allocator);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // ensure no GL context
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(800, 800, "Piston-Slap Renderer", nullptr, nullptr);

    if (window == nullptr){
        glfwTerminate();
        return ERR_GLFW_WINDOW_INIT_FAILED;

    }

    VkSurfaceKHR surface = nullptr;
    VkResult err = glfwCreateWindowSurface(instance, window, NULL, &surface);

    if (VkResult::VK_SUCCESS != err) {
        custom_err(ERR_VULKAN_SURFACE_FAILED, "could not create vulkan surface");
    }

    VkSurfaceCapabilitiesKHR surfaceCaps{};
    pfnGetPhysicalDeviceSurfaceCapabilitiesKHR(devices[deviceIndex], surface, &surfaceCaps);

    // Wayland special value width check
    VkExtent2D swapchainExtent{ surfaceCaps.currentExtent };
    if (surfaceCaps.currentExtent.width == 0xFFFFFFFF) {
        swapchainExtent = { .width = 800U, .height = 800U };
    }

    VkSwapchainKHR swapchain{0};
    const VkFormat imageFormat{ VK_FORMAT_B8G8R8A8_SRGB };
    VkSwapchainCreateInfoKHR swapchainCI{
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface,
        .minImageCount = surfaceCaps.minImageCount,
        .imageFormat = imageFormat,
        .imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
        .imageExtent{.width = swapchainExtent.width, .height = swapchainExtent.height },
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR
    };
    pfnCreateSwapchainKHR(device, &swapchainCI, nullptr, &swapchain);

    // after the swapchain, the tutorial suggests to ensure there's depth testing support from the gpu
    std::vector<VkFormat> depthFormatList{ VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };

    VkFormat depthFormat{ VK_FORMAT_UNDEFINED };
    for (VkFormat& format : depthFormatList) {
        VkFormatProperties2 formatProperties{ .sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2 };
        pfnGetPhysicalDeviceFormatProperties2(devices[deviceIndex], format, &formatProperties);
        if ((formatProperties.formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) != 0U) {
            depthFormat = format;
            break;
        }
    }
    // looks like this iterates through a target list and breaks when a cmp val is valid, which reassigns @depthFormat to this val

    // this info struct is for the VK Depth Image creation. Looks to be used as a Stencil Attachment
    VkImageCreateInfo depthImageCI{
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = depthFormat,
        .extent{.width = 800U, .height = 800u, .depth = 1u},
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    // from the turorial, this is the first use-case of the VMA lib I've linked.
    // this structure is a baseline example for further usage
    VkImage depthImage{NULL};
    VmaAllocation depthImageAllocation{NULL};

    VmaAllocationCreateInfo allocCI {
        .flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO
    };

    vmaCreateImage(allocator, &depthImageCI, &allocCI, &depthImage, &depthImageAllocation, nullptr);

    // VK SETUP DONE //
    glfwSetKeyCallback(window, GLFW_KeyCallback);

    while (!glfwWindowShouldClose(window)){

        glfwPollEvents();
    }

    pfnDestroyDevice(device, NULL);
    pfnDestroySurface(instance, surface, NULL);
    glfwTerminate();
    std::cout << "Piston-Slap terminated successfully\n";
    return 0;
}