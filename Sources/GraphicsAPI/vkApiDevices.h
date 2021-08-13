#pragma once
#define		GLFW_INCLUDE_VULKAN
#include	"../../External/GLFW/glfw3.h"
#include	<vector>
#include	<optional>
#include	"vulkan.h"
#include	"vkApiInstance.h"


struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() 
	{
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR		capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>	presentModes;
};


class vkApiDevices
{
	public:							vkApiDevices(vkApiInstance*	myVkApiInstance);
									~vkApiDevices();
				VkPhysicalDevice    GetPhysicalDevice();
						VkDevice    GetLogicalDevice();				
		 SwapChainSupportDetails	GetQuerySwapChainSupport();
			  QueueFamilyIndices	findQueueFamilies(VkPhysicalDevice device);

	private:	
				VkInstance			instance;
				VkSurfaceKHR		surface;
			    VkPhysicalDevice    ourPhysicalDevice = VK_NULL_HANDLE;
				VkDevice            ourLogicalDevice;
				VkQueue             graphicsQueue;
				VkQueue				presentQueue;


					  void	PickPhysicalDevice();
					  void	CreateLogicalDevice();

					  bool	isDeviceSuitable(VkPhysicalDevice device);
					  bool	checkDeviceExtensionSupport(VkPhysicalDevice device);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    


};
