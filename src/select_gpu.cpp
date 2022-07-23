#include "include/TEMPORARYNAME.h"
#include "include/rendering.h"

bool SelectGPU(VkInstance instance, VkPhysicalDevice& outDevice, unsigned int& outQueueFamilyIndex) {
    // Gets the number of available physical devices, at least one is required.
    unsigned int physical_device_count(0);

    vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);

    if (physical_device_count == 0) {
        std::cout << "No physical devices found\n";
        return false;
    }

    // Now get the devices.
    std::vector<VkPhysicalDevice> physical_devices(physical_device_count);

    vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data());

    // Show device information.
    std::cout << "Found " << physical_device_count << " GPU(s):\n";

    int count(0);

    std::vector<VkPhysicalDeviceProperties> physical_device_properties(physical_devices.size());

    for (auto& physical_device : physical_devices) {
        vkGetPhysicalDeviceProperties(physical_device, &(physical_device_properties[count]));

        std::cout << count << ": " << physical_device_properties[count].deviceName << "\n";

        count++;
    }

    // Select one if more than one is available.
    unsigned int selection_id = 0;

    bool selectGPU = false;

    if (physical_device_count > 1 && selectGPU) {
        while (true) {
            std::cout << "Select device: ";
            std::cin  >> selection_id;

            if (selection_id >= physical_device_count || selection_id < 0) {
                std::cout << "Invalid selection, expected a value between 0 and " << physical_device_count - 1 << "\n";

                continue;
            }

            break;
        }
    }

    std::cout << "Selected: " << physical_device_properties[selection_id].deviceName << "\n";
    VkPhysicalDevice selected_device = physical_devices[selection_id];

    // Find the number queues this device supports, we want one that supports graphics commands.
    unsigned int family_queue_count(0);

    vkGetPhysicalDeviceQueueFamilyProperties(selected_device, &family_queue_count, nullptr);

    if (family_queue_count == 0) {
        std::cout << "Device has no family of queues associated with it!\n";

        return false;
    }

    // Extract the properties of all the queue families.
    std::vector<VkQueueFamilyProperties> queue_properties(family_queue_count);

    vkGetPhysicalDeviceQueueFamilyProperties(selected_device, &family_queue_count, queue_properties.data());

    // Make sure the family of commands contains an option to issue graphical commands.
    unsigned int queue_node_index = -1;

    for (unsigned int i = 0; i < family_queue_count; i++) {
        if (queue_properties[i].queueCount > 0 && queue_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queue_node_index = i;

            break;
        }
    }

    if (queue_node_index < 0) {
        std::cout << "Unable to find a queue command family that accepts graphics commands\n";

        return false;
    }

    // Set the output variables.
    outDevice = selected_device;
    outQueueFamilyIndex = queue_node_index;

    return true;
}