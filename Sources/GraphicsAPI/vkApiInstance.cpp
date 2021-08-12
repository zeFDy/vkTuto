#include	"vkApiInstance.h"
#define		GLFW_INCLUDE_VULKAN
#include	"../../External/GLFW/glfw3.h"
#include	<vector>
#include	<iostream>
#include	<iomanip>


VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
}

//static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) 
static VkBool32 DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
}


vkApiInstance::vkApiInstance(char* ourApplicationName, GLFWwindow* window)
{
		VkApplicationInfo		ourAppInfo		= {};
		VkInstanceCreateInfo	ourInstanceInfo = {};

		if (enableValidationLayers && !CheckValidationLayerSupport()) 
		{
			throw std::runtime_error("validation layers requested, but not available!");
		}


		ourAppInfo.sType					= VK_STRUCTURE_TYPE_APPLICATION_INFO;
		ourAppInfo.pNext					= NULL;
		ourAppInfo.pApplicationName			= ourApplicationName;
		ourAppInfo.applicationVersion		= 1;
		ourAppInfo.pEngineName				= ourApplicationName;
		ourAppInfo.engineVersion			= 1;
		ourAppInfo.apiVersion				= VK_API_VERSION_1_0;

		ourInstanceInfo.sType				= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		ourInstanceInfo.pNext				= NULL;
		ourInstanceInfo.flags				= 0;
		ourInstanceInfo.pApplicationInfo	= &ourAppInfo;
		
		auto extensions = GetRequiredExtensions();
		
		ourInstanceInfo.enabledExtensionCount	= static_cast<uint32_t>(extensions.size());
		ourInstanceInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (enableValidationLayers) 
		{
			ourInstanceInfo.enabledLayerCount	= static_cast<uint32_t>(validationLayers.size());
			ourInstanceInfo.ppEnabledLayerNames = validationLayers.data();
			
			debugCreateInfo = {};
			debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugCreateInfo.pfnUserCallback = DebugCallback;

			ourInstanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else 
		{
			ourInstanceInfo.enabledLayerCount	= 0;
			ourInstanceInfo.pNext				= nullptr;
		}

				
		if (vkCreateInstance(&ourInstanceInfo, NULL, &thisInstance) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create instance!");
		}

		//EnumLayers();

		// SetupDebugMessenger
		if (enableValidationLayers)
		{

			VkDebugUtilsMessengerCreateInfoEXT createInfo;
			createInfo = {};
			createInfo.sType			= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			createInfo.messageSeverity	= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			createInfo.messageType		= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			createInfo.pfnUserCallback	= DebugCallback;

			if (CreateDebugUtilsMessengerEXT(thisInstance, &createInfo, nullptr, &DebugMessenger) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to set up debug messenger!");
			}
		}

		// Create Surface
		if (glfwCreateWindowSurface(thisInstance, window, nullptr, &ourSurface) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to create window surface!");
		}
}

vkApiInstance::~vkApiInstance()
{
		vkDestroyInstance(thisInstance, NULL);
}

void	vkApiInstance::EnumLayers(void)
{
		uint32_t						instanceLayerCount;
		std::vector<VkLayerProperties>	layerProperties;
		VkResult						result;

		result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, NULL);
		layerProperties.resize(instanceLayerCount);
		result = vkEnumerateInstanceLayerProperties(&instanceLayerCount, layerProperties.data());
		
		for (auto globalLayerProp : layerProperties)
		{
			std::cout << "\n\n" << globalLayerProp.description << "\n\t|\n\t|--[Layer Name]-->" << globalLayerProp.layerName;
			uint32_t instanceExtensionCount;
			result = vkEnumerateInstanceExtensionProperties(globalLayerProp.layerName, &instanceExtensionCount, NULL);
			if (instanceExtensionCount > 0)
			{
				std::vector<VkExtensionProperties>	allProperties;
				allProperties.resize(instanceExtensionCount);
				
				std::cout << "\n\t\t|";

				result = vkEnumerateInstanceExtensionProperties(globalLayerProp.layerName, &instanceExtensionCount, allProperties.data());
				for (auto thisProperties : allProperties)
				{
					std::cout << "\n\t\t|--[Extension Name]-->" << std::left << std::setw(40) << thisProperties.extensionName << "\t" << thisProperties.specVersion ;
				}
			}
		}
	
}


bool	vkApiInstance::CheckValidationLayerSupport()
{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) 
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) 
			{
				if (strcmp(layerName, layerProperties.layerName) == 0) 
				{
					layerFound = true;				// fix me : il y a un bug quand on mets plus d'une chaine dans la liste...
					//break;
					return true;
				}
			}

			if (!layerFound) 
			{
				return false;
			}
		}

		return true;
}

std::vector<const char*> vkApiInstance::GetRequiredExtensions() 
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) 
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}
