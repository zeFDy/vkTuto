#include	"vkApiSwapChain.h"
#include	"vkApiDevices.h"
#include	<stdexcept>
#define		GLFW_INCLUDE_VULKAN
#include	"../../External/GLFW/glfw3.h"


vkApiSwapChain::vkApiSwapChain(vkApiInstance *ourInstance, vkApiDevices *ourDevices)
{
	ourPhysicalDevice	= ourDevices->GetPhysicalDevice();
	ourLogicalDevice	= ourDevices->GetLogicalDevice();
	
	CreateSwapChain(ourInstance, ourDevices);
	CreateImageViews();
}

vkApiSwapChain::~vkApiSwapChain()
{
	CleanupSwapChain();
}

void vkApiSwapChain::CleanupSwapChain() 
{
	/*
    vkDestroyImageView(device, depthImageView, nullptr);
    vkDestroyImage(device, depthImage, nullptr);
    vkFreeMemory(device, depthImageMemory, nullptr);
	*/

    for (auto framebuffer : swapChainFramebuffers) 
	{
        vkDestroyFramebuffer(ourLogicalDevice, framebuffer, nullptr);
    }

   /*
    vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffersMainLoop.size()), commandBuffersMainLoop.data());
    vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffersSplash.size()),   commandBuffersSplash.data());

    vkDestroyPipeline(device, graphicsPipeline3dOneTexure,      nullptr);
    vkDestroyPipeline(device, graphicsPipeline3dOneTexureYGoGg, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

    vkDestroyPipeline(device, graphicsPipelineQuad, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayoutQuad, nullptr);

    vkDestroyPipeline(device, graphicsPipelineQuadOverlay, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayoutQuadOverlay, nullptr);


    vkDestroyRenderPass(device, renderPass, nullptr);
	*/

    for (auto imageView : swapChainImageViews) 
    {
        vkDestroyImageView(ourLogicalDevice, imageView, nullptr);
    }

    vkDestroySwapchainKHR(ourLogicalDevice, thisSwapChain, nullptr);

	/*
    for (size_t i = 0; i < swapChainImages.size(); i++) 
    {
        vkDestroyBuffer(device, uniformBuffers[i], nullptr);
        vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    }
	*/

	/*
    vkDestroyDescriptorPool(device, newDescriptorPool, nullptr);
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	*/
}

void vkApiSwapChain::CreateSwapChain(vkApiInstance *ourInstance, vkApiDevices *ourDevices) 
{
	
	SwapChainSupportDetails swapChainSupport =	 ourDevices->GetQuerySwapChainSupport();

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = ChooseSwapExtent(ourInstance, swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) 
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = ourInstance->GetSurface();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = ourDevices->findQueueFamilies(ourPhysicalDevice);
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    if (indices.graphicsFamily != indices.presentFamily) 
	{
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else 
	{
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    if (vkCreateSwapchainKHR(ourLogicalDevice, &createInfo, nullptr, &thisSwapChain) != VK_SUCCESS) 
	{
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(ourLogicalDevice, thisSwapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(ourLogicalDevice, thisSwapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

void vkApiSwapChain::CreateImageViews() 
{
    swapChainImageViews.resize(swapChainImages.size());

    for (uint32_t i = 0; i < swapChainImages.size(); i++) 
    {
        swapChainImageViews[i] = CreateImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

VkImageView vkApiSwapChain::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(ourLogicalDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}


VkSurfaceFormatKHR vkApiSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) 
{
    for (const auto& availableFormat : availableFormats) 
	{
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR vkApiSwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) 
{
    for (const auto& availablePresentMode : availablePresentModes) 
	{
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
		{
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D vkApiSwapChain::ChooseSwapExtent(vkApiInstance *ourInstance, const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX) 
	{
        return capabilities.currentExtent;
    }
    else 
	{
        int width, height;
        glfwGetFramebufferSize(ourInstance->GetWindow(), &width, &height);

        VkExtent2D actualExtent = 
		{
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

VkFormat	vkApiSwapChain::GetSwapChainImageFormat()
{
	return swapChainImageFormat;
}