#pragma once
#define		GLFW_INCLUDE_VULKAN
#include	"../../External/GLFW/glfw3.h"
#include	<vector>
#include	"vulkan.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
//const bool enableValidationLayers = false;         //true plante sur portables et ok sur i9 (c'etait un pb de mauvais layer definition)
const bool enableValidationLayers = true;            //et ok avec nouvrau nom car portable devait avoir un driver a different niveau 
#endif

const std::vector<const char*> validationLayers =
{
	"VK_LAYER_KHRONOS_validation"
	//"VK_LAYER_LUNARG_standard_validation"
};

const std::vector<const char*> deviceExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


class vkApiInstance
{

public:				vkApiInstance(char* caOurApplicationName, GLFWwindow* window);
					~vkApiInstance();

private:						void	EnumLayers(void);
								bool	CheckValidationLayerSupport();
			std::vector<const char*>	GetRequiredExtensions();
			VkDebugUtilsMessengerEXT    DebugMessenger;
    					  VkInstance	thisInstance;
						VkSurfaceKHR	ourSurface;
};

