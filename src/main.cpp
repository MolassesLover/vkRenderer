#include "include/TEMPORARYNAME.h"
#include "include/rendering.h"
#include "include/select_gpu.h"

using namespace TEMPORARYNAME;

int main(int argc, char *argv[]) {
    // Initialize SDL!
    if (!InitializeSDL()) {
        return -1;
    }

    // Create vulkan compatible window!
    SDL_Window *window { createWindow() };

    if (!window) {
        SDL_Quit();

        return -1;
    }

    std::vector<std::string> found_extensions;

    if (!GetAvailableVulkanExtensions(window, found_extensions)) {
        return -1;
    }

    // Get available vulkan layer extensions, notify if not all were found.
    std::vector<std::string> found_layers;

    if (!GetAvailableVulkanLayers(found_layers)) {
        return -1;
    }

    // Warn when not all requested layers could be found
    if (found_layers.size() != GetLayerNames().size()) {
        std::cout << "Warning: not all requested layers could be found!\n";
    }

    // Create Vulkan Instance
    VkInstance instance;

    if (!CreateVulkanInstance(found_layers, found_extensions, instance)) {
        return -1;
    }

    // Vulkan messaging callback.
    VkDebugReportCallbackEXT callback;

    SetupDebugCallback(instance, callback);

    // Select GPU after succsessful creation of a vulkan instance.
    VkPhysicalDevice gpu;
    unsigned int graphics_queue_index(-1);

    if (!SelectGPU(instance, gpu, graphics_queue_index)) {
        return -1;
    }

    // Create a logical device that interfaces with the physical device.
    VkDevice device;

    if (!CreateLogicalDevice(gpu, graphics_queue_index, found_layers, device)) {
        return -1;
    }

    // Create the surface we want to render to, associated with the window we created before
    // Check if the created surface is compatible with the selected physical renering device.
    VkSurfaceKHR presentation_surface;

    if (!CreateSurface(window, instance, gpu, graphics_queue_index, presentation_surface)) {
        return -1;
    }

    VkSwapchainKHR swap_chain = NULL;

    if (!CreateSwapChain(presentation_surface, gpu, device, swap_chain)) {
        return -1;
    }

    // Get image handles from swap chain.
    std::vector<VkImage> chain_images;

    if (!GetSwapChainImageHandles(device, swap_chain, chain_images)) {
        return -1;
    }

    // Fetch the queue we want to submit the actual commands to.
    VkQueue graphics_queue;

    GetDeviceQueue(device, graphics_queue_index, graphics_queue);

    std::cout << "\nSuccessfully initialized Vulkan and physical device\n";
    std::cout << "Successfully created a window and compatible surface\n";
    std::cout << "Successfully created swapchain\n";
    std::cout << "Ready to render!\n";

    bool run = true;

    while (run) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run = false;
            }
        }
    }

    // Destroy Vulkan!
    Quit(instance, device, callback, swap_chain, presentation_surface);

    return 1;
}