#include "vkApiRenderPass.h"
#include <stdexcept>
#include <array>
#include "vkApiDevices.h"
#include "vkApiSwapChain.h"

vkApiRenderPass::vkApiRenderPass(vkApiDevices* Devices, vkApiSwapChain* SwapChain)
{
	ourVkApiDevices		= Devices;
	ourVkApiSwapChain	= SwapChain;

	CreateRenderPass();
}

vkApiRenderPass::~vkApiRenderPass()
{
	vkDestroyRenderPass(ourVkApiDevices->GetLogicalDevice(), thisRenderPass, nullptr);
}

VkFormat vkApiRenderPass::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) 
{
    for (VkFormat format : candidates) 
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties( ourVkApiDevices->GetPhysicalDevice(), format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) 
        {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) 
        {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}

VkFormat vkApiRenderPass::FindDepthFormat() 
{
    return FindSupportedFormat(   { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
									VK_IMAGE_TILING_OPTIMAL,
									VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT    );
}

void vkApiRenderPass::CreateRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format			= ourVkApiSwapChain->GetSwapChainImageFormat();
    colorAttachment.samples			= VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout		= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format			= FindDepthFormat();
    depthAttachment.samples			= VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp			= VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout		= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = /*nullptr */  &depthAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(ourVkApiDevices->GetLogicalDevice(), &renderPassInfo, nullptr, &thisRenderPass) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create render pass!");
    }
}
