#include "TEMPORARYNAME.h"

namespace TEMPORARYNAME
{
const std::set<std::string>& GetLayerNames();
bool InitializeSDL();
VkResult CreateDebugReportCallbackEXT(VkInstance, const VkDebugReportCallbackCreateInfoEXT, const VkAllocationCallbacks, VkDebugReportCallbackEXT);
bool SetupDebugCallback(VkInstance, VkDebugReportCallbackEXT&);
void DestroyDebugReportCallbackEXT(VkInstance, VkDebugReportCallbackEXT, const VkAllocationCallbacks);
bool GetAvailableVulkanLayers(std::vector<std::string>&);
bool GetAvailableVulkanExtensions(SDL_Window *window, std::vector<std::string>&);
bool CreateVulkanInstance(const std::vector<std::string>&, const std::vector<std::string>&, VkInstance&);
bool CreateLogicalDevice(VkPhysicalDevice&, unsigned int, const std::vector<std::string>&, VkDevice&);
void GetDeviceQueue(VkDevice, int, VkQueue&);
bool CreateSurface(SDL_Window *, VkInstance, VkPhysicalDevice, uint32_t, VkSurfaceKHR&);
bool GetPresentationMode(VkSurfaceKHR, VkPhysicalDevice, VkPresentModeKHR&);
bool GetSurfaceProperties(VkPhysicalDevice, VkSurfaceKHR, VkSurfaceCapabilitiesKHR&);
unsigned int GetNumberOfSwapImages(const VkSurfaceCapabilitiesKHR&);
VkExtent2D GetSwapImageSize(const VkSurfaceCapabilitiesKHR&);
bool GetImageUsage(const VkSurfaceCapabilitiesKHR&, VkImageUsageFlags&);
bool GetFormat(VkPhysicalDevice, VkSurfaceKHR, VkSurfaceFormatKHR&);
bool CreateSwapChain(VkSurfaceKHR, VkPhysicalDevice, VkDevice, VkSwapchainKHR&);
bool GetSwapChainImageHandles(VkDevice, VkSwapchainKHR, std::vector<VkImage>&);
SDL_Window * createWindow();
void Quit(VkInstance, VkDevice, VkDebugReportCallbackEXT, VkSwapchainKHR, VkSurfaceKHR);
}