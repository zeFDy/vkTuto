#pragma once
#include <vector>
#include "vulkan.h"
#include "vkApiDevices.h"

/*
struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR		capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>	presentModes;
};
*/

class vkApiSwapChain
{
public:
									vkApiSwapChain(vkApiInstance *ourInstance, vkApiDevices *ourDevices);
									~vkApiSwapChain();
                            void	CleanupSwapChain();
						VkFormat	GetSwapChainImageFormat();

private:
						VkDevice	ourLogicalDevice;
				VkPhysicalDevice    ourPhysicalDevice;
				  VkSwapchainKHR	thisSwapChain;
						    void	CreateSwapChain(vkApiInstance *ourInstance, vkApiDevices *ourDevices);
							void	CreateImageViews();
					 VkImageView	CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
			  VkSurfaceFormatKHR	ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
			    VkPresentModeKHR	ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
			          VkExtent2D	ChooseSwapExtent(vkApiInstance *ourInstance, const VkSurfaceCapabilitiesKHR& capabilities);

			  std::vector<VkImage>	swapChainImages;
						  VkFormat	swapChainImageFormat;
						VkExtent2D	swapChainExtent;
		  std::vector<VkImageView>	swapChainImageViews;
		std::vector<VkFramebuffer>	swapChainFramebuffers;

};

