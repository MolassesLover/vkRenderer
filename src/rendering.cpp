#include "include/TEMPORARYNAME.h"
#include "include/rendering.h"
#include <X11/Xlib.h>

namespace TEMPORARYNAME
{
Display *display = XOpenDisplay(NULL);
Screen *screen = DefaultScreenOfDisplay(display);
const char gAppName[] = "TEMPORARYNAME";
const char gEngineName[] = "TEMPORARYNAME";
int gWindowHeight = screen->height / 1.5f;
int gWindowWidth = screen->width / 1.5f;
VkColorSpaceKHR gColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
VkFormat gFormat = VK_FORMAT_B8G8R8A8_SRGB;
VkImageUsageFlags gImageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
VkPresentModeKHR gPresentationMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
VkSurfaceTransformFlagBitsKHR gTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;

const std::set<std::string>& GetLayerNames() {
    static std::set<std::string> layers;

    if (layers.empty()) {
        layers.emplace("VK_LAYER_NV_optimus");
        layers.emplace("VK_LAYER_KHRONOS_validation");
    }

    return layers;
}

const std::set<std::string>& GetDeviceExtensionNames() {
    static std::set<std::string> layers;

    if (layers.empty()) {
        layers.emplace(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }

    return layers;
}

const std::vector<VkImageUsageFlags> GetImageUsages() {
    static std::vector<VkImageUsageFlags> usages;

    if (usages.empty()) {
        usages.emplace_back(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
    }

    return usages;
}

template<typename Template>

Template clamp(Template value, Template min, Template max) {
    return glm::clamp<Template>(value, min, max);
}

bool InitializeSDL() {
    // Initialize SDL!
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0) {
        return true;
    } else {
        std::cout << "Unable to initialize SDL\n";

        return false;
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType,
                                                    uint64_t obj,
                                                    size_t location,
                                                    int32_t code,
                                                    const char *layerPrefix,
                                                    const char *msg,
                                                    void *userData) {
    std::cout << "Validation layer: " << layerPrefix << ": " << msg << std::endl;

    return VK_FALSE;
}

VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback) {
    auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

bool SetupDebugCallback(VkInstance instance, VkDebugReportCallbackEXT& callback) {
    VkDebugReportCallbackCreateInfoEXT createInfo = {};

    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createInfo.pfnCallback = DebugCallback;

    if (CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS) {
        std::cout << "Unable to create debug report callback extension\n";

        return false;
    }

    return true;
}

void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks *pAllocator) {
    auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

    if (func != nullptr) {
        func(instance, callback, pAllocator);
    }
}

bool GetAvailableVulkanLayers(std::vector<std::string>& outLayers) {
    unsigned int instance_layer_count = 0;

    VkResult res = vkEnumerateInstanceLayerProperties(&instance_layer_count, NULL);

    if (res != VK_SUCCESS) {
        std::cout << "Unable to query Vulkan instance layer property count\n";

        return false;
    }

    std::vector<VkLayerProperties> instance_layer_names(instance_layer_count);

    res = vkEnumerateInstanceLayerProperties(&instance_layer_count, instance_layer_names.data());

    if (res != VK_SUCCESS) {
        std::cout << "Unable to retrieve Vulkan instance layer names\n";

        return false;
    }

    std::cout << "Found " << instance_layer_count << " instance layers:\n";
    std::vector<const char *> valid_instance_layer_names;
    const std::set<std::string>& lookup_layers = GetLayerNames();

    int count(0);

    outLayers.clear();

    for (const auto& name : instance_layer_names) {
        std::cout << count << ": " << name.layerName << ": " << name.description << "\n";

        auto it = lookup_layers.find(std::string(name.layerName));

        if (it != lookup_layers.end()) {
            outLayers.emplace_back(name.layerName);
        }

        count++;
    }

    // Print enabled layers.
    std::cout << "\n";

    for (const auto& layer : outLayers) {
        std::cout << "Applying layer: " << layer.c_str() << "\n";
    }

    return true;
}

// Find out what exensions Vulkan needs for the OS windowing.
bool GetAvailableVulkanExtensions(SDL_Window *window, std::vector<std::string>& outExtensions) {
    unsigned int ext_count = 0;

    if (!SDL_Vulkan_GetInstanceExtensions(window, &ext_count, nullptr)) {
        std::cout << "Unable to query the number of Vulkan instance extensions\n";

        return false;
    }

    // Use the extension count to retrieve extension namees.
    std::vector<const char *> ext_names(ext_count);

    if (!SDL_Vulkan_GetInstanceExtensions(window, &ext_count, ext_names.data())) {
        std::cout << "Unable to query the number of Vulkan instance extension names\n";

        return false;
    }

    // Print out which layers are used.
    std::cout << "Found " << ext_count << " these Vulkan instance extensions:\n";

    for (unsigned int i = 0; i < ext_count; i++) {
        std::cout << i << ": " << ext_names[i] << "\n";

        outExtensions.emplace_back(ext_names[i]);
    }

    // Add the Vulkan debug display extension.
    outExtensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

    std::cout << "\n";

    return true;
}

bool CreateVulkanInstance(const std::vector<std::string>& layerNames, const std::vector<std::string>& extensionNames, VkInstance& outInstance) {
    // Copy layers.
    std::vector<const char *> layer_names;

    for (const auto& layer : layerNames) {
        layer_names.emplace_back(layer.c_str());
    }

    // Copy extensions.
    std::vector<const char *> ext_names;

    for (const auto& ext : extensionNames) {
        ext_names.emplace_back(ext.c_str());
    }

    // Get the supported Vulkan API version.
    unsigned int api_version;

    vkEnumerateInstanceVersion(&api_version);

    // Initializes the VkApplicationInfo structure.
    VkApplicationInfo app_info = {};

    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = NULL;
    app_info.pApplicationName = gAppName;
    app_info.applicationVersion = 1;
    app_info.pEngineName = gEngineName;
    app_info.engineVersion = 1;
    app_info.apiVersion = VK_API_VERSION_1_0;

    // Initializes the VkInstanceCreateInfo structure.
    VkInstanceCreateInfo inst_info = {};

    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.pNext = NULL;
    inst_info.flags = 0;
    inst_info.pApplicationInfo = &app_info;
    inst_info.enabledExtensionCount = static_cast<uint32_t>(ext_names.size());
    inst_info.ppEnabledExtensionNames = ext_names.data();
    inst_info.enabledLayerCount = static_cast<uint32_t>(layer_names.size());
    inst_info.ppEnabledLayerNames = layer_names.data();

    // Create the Vulkan runtime instance!
    std::cout << "Initializing Vulkan instance\n\n";

    VkResult res = vkCreateInstance(&inst_info, NULL, &outInstance);

    switch (res) {
        case VK_SUCCESS:
            break;

        case VK_ERROR_INCOMPATIBLE_DRIVER:
            std::cout << "Unable to create Bulkan instance, cannot find a compatible Vulkan ICD\n";
            return false;

        default:
            std::cout << "Unable to create Vulkan instance: unknown error\n";
            return false;
    }

    return true;
}

bool CreateLogicalDevice(VkPhysicalDevice&               physicalDevice,
                         unsigned int                    queueFamilyIndex,
                         const std::vector<std::string>& layerNames,
                         VkDevice&                       outDevice) {
    // Copy layer names.
    std::vector<const char *> layer_names;

    for (const auto& layer : layerNames) {
        layer_names.emplace_back(layer.c_str());
    }

    // Get the number of available extensions for our graphics card
    uint32_t device_property_count(0);

    if (vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &device_property_count, NULL) != VK_SUCCESS) {
        std::cout << "Unable to acquire device extension property count\n";

        return false;
    }

    std::cout << "\nFound " << device_property_count << " device extensions\n";

    // Acquire their actual names.
    std::vector<VkExtensionProperties> device_properties(device_property_count);

    if (vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &device_property_count, device_properties.data()) != VK_SUCCESS) {
        std::cout << "Unable to acquire device extension property names\n";

        return false;
    }

    // Match names against requested extension.
    std::vector<const char *> device_property_names;
    const std::set<std::string>& required_extension_names = GetDeviceExtensionNames();
    int count = 0;

    for (const auto& ext_property : device_properties) {
        std::cout << count << ": " << ext_property.extensionName << "\n";
        auto it = required_extension_names.find(std::string(ext_property.extensionName));

        if (it != required_extension_names.end()) {
            device_property_names.emplace_back(ext_property.extensionName);
        }

        count++;
    }

    // Warn if not all required extensions were found.
    if (required_extension_names.size() != device_property_names.size()) {
        std::cout << "Not all required device extensions are supported!\n";

        return false;
    }

    std::cout << "\n";

    for (const auto& name : device_property_names) {
        std::cout << "Applying device extension: " << name << "\n";
    }

    // Create queue information structure used by device based on the previously fetched queue information from the physical device.
    // We create one command processing queue for graphics.
    VkDeviceQueueCreateInfo queue_create_info;

    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queueFamilyIndex;
    queue_create_info.queueCount = 1;
    std::vector<float> queue_prio = { 1.0f };

    queue_create_info.pQueuePriorities = queue_prio.data();
    queue_create_info.pNext = NULL;
    queue_create_info.flags = 0;

    // Device creation information.
    VkDeviceCreateInfo create_info;

    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount = 1;
    create_info.pQueueCreateInfos = &queue_create_info;
    create_info.ppEnabledLayerNames = layer_names.data();
    create_info.enabledLayerCount = static_cast<uint32_t>(layer_names.size());
    create_info.ppEnabledExtensionNames = device_property_names.data();
    create_info.enabledExtensionCount = static_cast<uint32_t>(device_property_names.size());
    create_info.pNext = NULL;
    create_info.pEnabledFeatures = NULL;
    create_info.flags = 0;

    // Finally we're ready to create a new device.
    VkResult res = vkCreateDevice(physicalDevice, &create_info, nullptr, &outDevice);

    if (res != VK_SUCCESS) {
        std::cout << "Failed to create logical device!\n";
        return false;
    }

    return true;
}

void GetDeviceQueue(VkDevice device, int familyQueueIndex, VkQueue& outGraphicsQueue) {
    vkGetDeviceQueue(device, familyQueueIndex, 0, &outGraphicsQueue);
}

bool CreateSurface(SDL_Window *window, VkInstance instance, VkPhysicalDevice gpu, uint32_t graphicsFamilyQueueIndex, VkSurfaceKHR& outSurface) {
    if (!SDL_Vulkan_CreateSurface(window, instance, &outSurface)) {
        std::cout << "Unable to create Vulkan compatible surface using SDL\n";

        return false;
    }

    // Make sure the surface is compatible with the queue family and gpu.
    VkBool32 supported = false;

    vkGetPhysicalDeviceSurfaceSupportKHR(gpu, graphicsFamilyQueueIndex, outSurface, &supported);

    if (!supported) {
        std::cout << "Surface is not supported by physical device!\n";

        return false;
    }

    return true;
}

bool GetPresentationMode(VkSurfaceKHR surface, VkPhysicalDevice device, VkPresentModeKHR& ioMode) {
    uint32_t mode_count(0);

    if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &mode_count, NULL) != VK_SUCCESS) {
        std::cout << "Unable to query present mode count for physical device\n";

        return false;
    }

    std::vector<VkPresentModeKHR> available_modes(mode_count);

    if (vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &mode_count, available_modes.data()) != VK_SUCCESS) {
        std::cout << "Unable to query the various present modes for physical device\n";

        return false;
    }

    for (auto& mode : available_modes) {
        if (mode == ioMode) {
            return true;
        }
    }

    std::cout << "Unable to obtain preferred display mode, fallback to FIFO\n";

    ioMode = VK_PRESENT_MODE_FIFO_KHR;

    return true;
}

bool GetSurfaceProperties(VkPhysicalDevice device, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR& capabilities) {
    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities) != VK_SUCCESS) {
        std::cout << "Unable to acquire surface capabilities\n";
        return false;
    }

    return true;
}

unsigned int GetNumberOfSwapImages(const VkSurfaceCapabilitiesKHR& capabilities) {
    unsigned int number = capabilities.minImageCount + 1;

    return number > capabilities.maxImageCount ? capabilities.minImageCount : number;
}

VkExtent2D GetSwapImageSize(const VkSurfaceCapabilitiesKHR& capabilities) {
    // Default size = window size.
    VkExtent2D size = { (unsigned int)gWindowWidth, (unsigned int)gWindowHeight };

    // This happens when the window scales based on the size of an image.
    if (capabilities.currentExtent.width == 0xFFFFFFF) {
        size.width = glm::clamp<unsigned int>(size.width,  capabilities.minImageExtent.width,  capabilities.maxImageExtent.width);
        size.height = glm::clamp<unsigned int>(size.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    } else {
        size = capabilities.currentExtent;
    }

    return size;
}

bool GetImageUsage(const VkSurfaceCapabilitiesKHR& capabilities, VkImageUsageFlags& outUsage) {
    const std::vector<VkImageUsageFlags>& desir_usages = GetImageUsages();

    assert(desir_usages.size() > 0);

    // Needs to be always present.
    outUsage = desir_usages[0];

    for (const auto& desired_usage : desir_usages) {
        VkImageUsageFlags image_usage = desired_usage & capabilities.supportedUsageFlags;

        if (image_usage != desired_usage) {
            std::cout << "Unsupported image usage flag: " << desired_usage << "\n";
            return false;
        }

        // Add bit if found as supported color.
        outUsage = (outUsage | desired_usage);
    }

    return true;
}

VkSurfaceTransformFlagBitsKHR getTransform(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.supportedTransforms & gTransform) {
        return gTransform;
    }

    std::cout << "Unsupported surface transform: " << gTransform;

    return capabilities.currentTransform;
}

bool GetFormat(VkPhysicalDevice device, VkSurfaceKHR surface, VkSurfaceFormatKHR& outFormat) {
    unsigned int count(0);

    if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr) != VK_SUCCESS) {
        std::cout << "Unable to query number of supported surface formats";

        return false;
    }

    std::vector<VkSurfaceFormatKHR> found_formats(count);

    if (vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, found_formats.data()) != VK_SUCCESS) {
        std::cout << "Unable to query all supported surface formats\n";

        return false;
    }

    if (found_formats.size() == 1 && found_formats[0].format == VK_FORMAT_UNDEFINED) {
        outFormat.format = gFormat;

        outFormat.colorSpace = gColorSpace;

        return true;
    }

    for (const auto& found_format_outer : found_formats) {
        if (found_format_outer.format == gFormat) {
            outFormat.format = found_format_outer.format;

            for (const auto& found_format_inner : found_formats) {
                if (found_format_inner.colorSpace == gColorSpace) {
                    outFormat.colorSpace = found_format_inner.colorSpace;
                    return true;
                }
            }

            // No matching color space, pick first one.
            std::cout << "Warning: no matching color space found, picking first available one\n!";
            outFormat.colorSpace = found_formats[0].colorSpace;

            return true;
        }
    }

    // No matching formats found.
    std::cout << "Warning: no matching color format found, picking first available one\n";
    outFormat = found_formats[0];

    return true;
}

bool CreateSwapChain(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice, VkDevice device, VkSwapchainKHR& outSwapChain) {
    // Get properties of surface, this is necessary for swap-chain creation.
    VkSurfaceCapabilitiesKHR surface_properties;

    if (!GetSurfaceProperties(physicalDevice, surface, surface_properties)) return false;

    // Get the image presentation mode.
    VkPresentModeKHR presentation_mode = gPresentationMode;

    if (!GetPresentationMode(surface, physicalDevice, presentation_mode)) return false;

    // Get other swap chain related features.
    unsigned int swap_image_count = GetNumberOfSwapImages(surface_properties);

    // Size of the images.
    VkExtent2D swap_image_extent = GetSwapImageSize(surface_properties);

    // Get image usage (color, etc.)
    VkImageUsageFlags usage_flags;

    if (!GetImageUsage(surface_properties, usage_flags)) return false;

    // Get the transform, falls back on current transform when transform is not supported.
    VkSurfaceTransformFlagBitsKHR transform = getTransform(surface_properties);

    // Get swapchain image format.
    VkSurfaceFormatKHR image_format;

    if (!GetFormat(physicalDevice, surface, image_format)) return false;

    // Old swap chain.
    VkSwapchainKHR old_swap_chain = outSwapChain;

    // Populate swapchain creation info.
    VkSwapchainCreateInfoKHR swap_info;

    swap_info.pNext = nullptr;
    swap_info.flags = 0;
    swap_info.surface = surface;
    swap_info.minImageCount = swap_image_count;
    swap_info.imageFormat = image_format.format;
    swap_info.imageColorSpace = image_format.colorSpace;
    swap_info.imageExtent = swap_image_extent;
    swap_info.imageArrayLayers = 1;
    swap_info.imageUsage = usage_flags;
    swap_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swap_info.queueFamilyIndexCount = 0;
    swap_info.pQueueFamilyIndices = nullptr;
    swap_info.preTransform = transform;
    swap_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swap_info.presentMode = presentation_mode;
    swap_info.clipped = true;
    swap_info.oldSwapchain = NULL;
    swap_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

    // Destroy old swap chain.
    if (old_swap_chain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(device, old_swap_chain, nullptr);

        old_swap_chain = VK_NULL_HANDLE;
    }

    // Create new swap chain.
    if (vkCreateSwapchainKHR(device, &swap_info, nullptr, &old_swap_chain) != VK_SUCCESS) {
        std::cout << "Unable to create swap chain\n";

        return false;
    }

    // Store handle.
    outSwapChain = old_swap_chain;

    return true;
}

bool GetSwapChainImageHandles(VkDevice device, VkSwapchainKHR chain, std::vector<VkImage>& outImageHandles) {
    unsigned int image_count(0);
    VkResult res = vkGetSwapchainImagesKHR(device, chain, &image_count, nullptr);

    if (res != VK_SUCCESS) {
        std::cout << "Unable to get number of images in swap chain\n";

        return false;
    }

    outImageHandles.clear();
    outImageHandles.resize(image_count);

    if (vkGetSwapchainImagesKHR(device, chain, &image_count, outImageHandles.data()) != VK_SUCCESS) {
        std::cout << "Unable to get image handles from swap chain\n";

        return false;
    }

    return true;
}

SDL_Window * createWindow() {
    return SDL_CreateWindow(gAppName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gWindowWidth, gWindowHeight, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);
}

void Quit(VkInstance instance, VkDevice device, VkDebugReportCallbackEXT callback, VkSwapchainKHR chain, VkSurfaceKHR presentation_surface) {
    vkDestroySwapchainKHR(device, chain, nullptr);
    vkDestroyDevice(device, nullptr);
    DestroyDebugReportCallbackEXT(instance, callback, nullptr);
    vkDestroySurfaceKHR(instance, presentation_surface, nullptr);
    vkDestroyInstance(instance, nullptr);
    SDL_Quit();
}
}