#pragma once
#include <vector>
#include "vulkan.h"
#include "vkApiDevices.h"
#include "vkApiSwapChain.h"


class vkApiRenderPass
{
public:		
				vkApiRenderPass(vkApiDevices* Devices, vkApiSwapChain* SwapChain);
				~vkApiRenderPass();
	    void	CreateRenderPass();

private:
			VkRenderPass	thisRenderPass;
			vkApiDevices*	ourVkApiDevices;
		  vkApiSwapChain*	ourVkApiSwapChain;
				VkFormat	FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
				VkFormat	FindDepthFormat();

};

