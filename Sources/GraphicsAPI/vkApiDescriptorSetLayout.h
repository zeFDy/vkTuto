#pragma once
#include "vulkan.h"
#include "vkApiDevices.h"

class vkApiDescriptorSetLayout
{
public:		
				vkApiDescriptorSetLayout(vkApiDevices* Devices);
				~vkApiDescriptorSetLayout();


private:		void	CreateDescriptorSetLayout(vkApiDevices* Devices);

						vkApiDevices*	ourDevices;
				VkDescriptorSetLayout	thisDescriptorSetLayout;


};

