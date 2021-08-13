#include	"vkApiPipelines.h"
#include	<stdexcept>
#include	<vector>
#include	<iostream>
#include	<fstream>

vkApiPipelines::vkApiPipelines()
{

}

vkApiPipelines::~vkApiPipelines()
{

}

std::vector<char> vkApiPipelines::ReadFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}


//
// fix me :: a transfomer en n pipeline objects
//
void vkApiPipelines::CreateAllPipelines()
{
        // 1st Pipeline 3d
        auto vertShaderCode     = ReadFile("shaders/3dTextureSimple_vert.spv");             // RGB Texture
        auto fragShaderCode     = ReadFile("shaders/3dTextureSimple_frag.spv");             // RGB Texture
        //auto vertShaderCode   = readFile("shaders/3dTextureSimple-YCoGg_vert.spv");       // DOOM3BFG YCoCg texture -> RGB
        //auto fragShaderCode   = readFile("shaders/3dTextureSimple-YCoGg_frag.spv");       // DOOM3BFG YCoCg texture -> RGB
        

        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        auto bindingDescription = Vertex::getBindingDescription();
        auto attributeDescriptions = Vertex::getAttributeDescriptions();

        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)swapChainExtent.width;
        viewport.height = (float)swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = swapChainExtent;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizerPolygon{};
        rasterizerPolygon.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizerPolygon.depthClampEnable = VK_FALSE;
        rasterizerPolygon.rasterizerDiscardEnable = VK_FALSE;
        rasterizerPolygon.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizerPolygon.lineWidth = 1.0f;
        rasterizerPolygon.cullMode = /*VK_CULL_MODE_FRONT_AND_BACK*/   VK_CULL_MODE_NONE   /*VK_CULL_MODE_BACK_BIT*/;
        rasterizerPolygon.frontFace = /*VK_FRONT_FACE_CLOCKWISE*/      VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizerPolygon.depthBiasEnable = VK_FALSE;

        VkPipelineRasterizationStateCreateInfo rasterizerLine{};
        rasterizerLine.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizerLine.depthClampEnable = VK_FALSE;
        rasterizerLine.rasterizerDiscardEnable = VK_FALSE;
        rasterizerLine.polygonMode = /*VK_POLYGON_MODE_FILL*/ VK_POLYGON_MODE_LINE;
        rasterizerLine.lineWidth = 1.0f;
        rasterizerLine.cullMode = /*VK_CULL_MODE_FRONT_AND_BACK*/   VK_CULL_MODE_NONE   /*VK_CULL_MODE_BACK_BIT*/;
        rasterizerLine.frontFace = /*VK_FRONT_FACE_CLOCKWISE*/      VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizerLine.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = /*VK_FALSE*/VK_TRUE;
        depthStencil.depthWriteEnable = /*VK_FALSE*/VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.stencilTestEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

        if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizerPolygon /*&rasterizerLine*/;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = /*nullptr;*/  &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.layout = pipelineLayout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline3dOneTexure) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

        vkDestroyShaderModule(device, fragShaderModule, nullptr);
        vkDestroyShaderModule(device, vertShaderModule, nullptr);


        // 2nd Pipeline 3d (YCoCg textures)
        auto vertShaderCodeYCoCg   = readFile("shaders/3dTextureSimple-YCoGg_vert.spv");       // DOOM3BFG YCoCg texture -> RGB
        auto fragShaderCodeYCoCg   = readFile("shaders/3dTextureSimple-YCoGg_frag.spv");       // DOOM3BFG YCoCg texture -> RGB
        

        VkShaderModule vertShaderModuleYCoCg = createShaderModule(vertShaderCodeYCoCg);
        VkShaderModule fragShaderModuleYCoCg = createShaderModule(fragShaderCodeYCoCg);

        VkPipelineShaderStageCreateInfo vertShaderStageInfoYCoCg{};
        vertShaderStageInfoYCoCg.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfoYCoCg.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfoYCoCg.module = vertShaderModuleYCoCg;
        vertShaderStageInfoYCoCg.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfoYCoCg{};
        fragShaderStageInfoYCoCg.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfoYCoCg.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfoYCoCg.module = fragShaderModuleYCoCg;
        fragShaderStageInfoYCoCg.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStagesYCoCg[] = { vertShaderStageInfoYCoCg, fragShaderStageInfoYCoCg };

        #if false
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        auto bindingDescription = Vertex::getBindingDescription();
        auto attributeDescriptions = Vertex::getAttributeDescriptions();

        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)swapChainExtent.width;
        viewport.height = (float)swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = swapChainExtent;

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizerPolygon{};
        rasterizerPolygon.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizerPolygon.depthClampEnable = VK_FALSE;
        rasterizerPolygon.rasterizerDiscardEnable = VK_FALSE;
        rasterizerPolygon.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizerPolygon.lineWidth = 1.0f;
        rasterizerPolygon.cullMode = /*VK_CULL_MODE_FRONT_AND_BACK*/   VK_CULL_MODE_NONE   /*VK_CULL_MODE_BACK_BIT*/;
        rasterizerPolygon.frontFace = /*VK_FRONT_FACE_CLOCKWISE*/      VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizerPolygon.depthBiasEnable = VK_FALSE;

        VkPipelineRasterizationStateCreateInfo rasterizerLine{};
        rasterizerLine.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizerLine.depthClampEnable = VK_FALSE;
        rasterizerLine.rasterizerDiscardEnable = VK_FALSE;
        rasterizerLine.polygonMode = /*VK_POLYGON_MODE_FILL*/ VK_POLYGON_MODE_LINE;
        rasterizerLine.lineWidth = 1.0f;
        rasterizerLine.cullMode = /*VK_CULL_MODE_FRONT_AND_BACK*/   VK_CULL_MODE_NONE   /*VK_CULL_MODE_BACK_BIT*/;
        rasterizerLine.frontFace = /*VK_FRONT_FACE_CLOCKWISE*/      VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizerLine.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = /*VK_FALSE*/VK_TRUE;
        depthStencil.depthWriteEnable = /*VK_FALSE*/VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.stencilTestEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

        if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
        #endif

        VkGraphicsPipelineCreateInfo pipelineInfoYCoCg{};
        pipelineInfoYCoCg.sType                  = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfoYCoCg.stageCount             = 2;
        pipelineInfoYCoCg.pStages                = shaderStagesYCoCg;
        pipelineInfoYCoCg.pVertexInputState      = &vertexInputInfo;
        pipelineInfoYCoCg.pInputAssemblyState    = &inputAssembly;
        pipelineInfoYCoCg.pViewportState         = &viewportState;
        pipelineInfoYCoCg.pRasterizationState    = &rasterizerPolygon /*&rasterizerLine*/;
        pipelineInfoYCoCg.pMultisampleState      = &multisampling;
        pipelineInfoYCoCg.pDepthStencilState     = /*nullptr;*/  &depthStencil;
        pipelineInfoYCoCg.pColorBlendState       = &colorBlending;
        pipelineInfoYCoCg.layout                 = pipelineLayout;
        pipelineInfoYCoCg.renderPass             = renderPass;
        pipelineInfoYCoCg.subpass                = 0;
        pipelineInfoYCoCg.basePipelineHandle     = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfoYCoCg, nullptr, &graphicsPipeline3dOneTexureYGoGg) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

        vkDestroyShaderModule(device, fragShaderModuleYCoCg, nullptr);
        vkDestroyShaderModule(device, vertShaderModuleYCoCg, nullptr);




        //// 2nd Pipeline 2d
        auto vertShaderCodeQuad = readFile("shaders/shader_2d_texture_vert.spv");
        auto fragShaderCodeQuad = readFile("shaders/shader_2d_texture_frag.spv");                       //  non transparent
        //auto fragShaderCodeQuad = readFile("shaders/shader_2d_texture_frag-Transparent.spv");         // transparent 0x000000

        VkShaderModule vertShaderModuleQuad = createShaderModule(vertShaderCodeQuad);
        VkShaderModule fragShaderModuleQuad = createShaderModule(fragShaderCodeQuad);

        VkPipelineShaderStageCreateInfo vertShaderStageInfoQuad{};
        vertShaderStageInfoQuad.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfoQuad.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfoQuad.module = vertShaderModuleQuad;
        vertShaderStageInfoQuad.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfoQuad{};
        fragShaderStageInfoQuad.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfoQuad.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfoQuad.module = fragShaderModuleQuad;
        fragShaderStageInfoQuad.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStagesQuad[] = { vertShaderStageInfoQuad, fragShaderStageInfoQuad };

        VkPipelineVertexInputStateCreateInfo vertexInputInfoQuad{};
        vertexInputInfoQuad.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        auto bindingDescriptionQuad = Vertex::getBindingDescription();
        auto attributeDescriptionsQuad = Vertex::getAttributeDescriptions();

        vertexInputInfoQuad.vertexBindingDescriptionCount = 1;
        vertexInputInfoQuad.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptionsQuad.size());
        vertexInputInfoQuad.pVertexBindingDescriptions = &bindingDescriptionQuad;
        vertexInputInfoQuad.pVertexAttributeDescriptions = attributeDescriptionsQuad.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyQuad{};
        inputAssemblyQuad.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyQuad.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemblyQuad.primitiveRestartEnable = VK_FALSE;

        ////VkViewport viewportQuad{};
        ////viewportQuad.x = 0.0f;
        ////viewportQuad.y = 0.0f;
        ////viewportQuad.width = (float)swapChainExtent.width;
        ////viewportQuad.height = (float)swapChainExtent.height;
        ////viewportQuad.minDepth = 0.0f;
        ////viewportQuad.maxDepth = 1.0f;

        ////VkRect2D scissorQuad{};
        ////scissorQuad.offset = { 0, 0 };
        ////scissorQuad.extent = swapChainExtent;

        ////VkPipelineViewportStateCreateInfo viewportStateQuad{};
        ////viewportStateQuad.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        ////viewportStateQuad.viewportCount = 1;
        ////viewportStateQuad.pViewports = &viewport;
        ////viewportStateQuad.scissorCount = 1;
        ////viewportStateQuad.pScissors = &scissor;

        ////VkPipelineRasterizationStateCreateInfo rasterizer{};
        ////rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        ////rasterizer.depthClampEnable = VK_FALSE;
        ////rasterizer.rasterizerDiscardEnable = VK_FALSE;
        ////rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        ////rasterizer.lineWidth = 1.0f;
        ////rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        ////rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        ////rasterizer.depthBiasEnable = VK_FALSE;

        ////VkPipelineMultisampleStateCreateInfo multisampling{};
        ////multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        ////multisampling.sampleShadingEnable = VK_FALSE;
        ////multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        ////VkPipelineDepthStencilStateCreateInfo depthStencil{};
        ////depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        ////depthStencil.depthTestEnable = VK_TRUE;
        ////depthStencil.depthWriteEnable = VK_TRUE;
        ////depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        ////depthStencil.depthBoundsTestEnable = VK_FALSE;
        ////depthStencil.stencilTestEnable = VK_FALSE;

        ////VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        ////colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        ////colorBlendAttachment.blendEnable = VK_FALSE;

        ////VkPipelineColorBlendStateCreateInfo colorBlending{};
        ////colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        ////colorBlending.logicOpEnable = VK_FALSE;
        ////colorBlending.logicOp = VK_LOGIC_OP_COPY;
        ////colorBlending.attachmentCount = 1;
        ////colorBlending.pAttachments = &colorBlendAttachment;
        ////colorBlending.blendConstants[0] = 0.0f;
        ////colorBlending.blendConstants[1] = 0.0f;
        ////colorBlending.blendConstants[2] = 0.0f;
        ////colorBlending.blendConstants[3] = 0.0f;

        VkPipelineLayoutCreateInfo pipelineLayoutInfoQuad{};
        pipelineLayoutInfoQuad.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfoQuad.setLayoutCount = 1;
        pipelineLayoutInfoQuad.pSetLayouts = &descriptorSetLayout;

        if (vkCreatePipelineLayout(device, &pipelineLayoutInfoQuad, nullptr, &pipelineLayoutQuad) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }

        VkGraphicsPipelineCreateInfo pipelineInfoQuad{};
        pipelineInfoQuad.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfoQuad.stageCount = 2;
        pipelineInfoQuad.pStages = shaderStagesQuad;
        pipelineInfoQuad.pVertexInputState = &vertexInputInfoQuad;
        pipelineInfoQuad.pInputAssemblyState = &inputAssemblyQuad;
        pipelineInfoQuad.pViewportState = &viewportState;
        pipelineInfoQuad.pRasterizationState = &rasterizerPolygon;
        pipelineInfoQuad.pMultisampleState = &multisampling;
        pipelineInfoQuad.pDepthStencilState = &depthStencil;
        pipelineInfoQuad.pColorBlendState = &colorBlending;
        pipelineInfoQuad.layout = pipelineLayoutQuad;
        pipelineInfoQuad.renderPass = renderPass;
        pipelineInfoQuad.subpass = 0;
        pipelineInfoQuad.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfoQuad, nullptr, &graphicsPipelineQuad) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

        vkDestroyShaderModule(device, fragShaderModuleQuad, nullptr);
        vkDestroyShaderModule(device, vertShaderModuleQuad, nullptr);



        //// 3rd Pipeline 2d Overlay
        auto vertShaderCodeQuadOverlay = readFile("shaders/shader_2d_texture_vert.spv");
        auto fragShaderCodeQuadOverlay = readFile("shaders/shader_2d_texture_frag-Transparent.spv");       // transparent 0x000000

        VkShaderModule vertShaderModuleQuadOverlay = createShaderModule(vertShaderCodeQuadOverlay);
        VkShaderModule fragShaderModuleQuadOverlay = createShaderModule(fragShaderCodeQuadOverlay);

        VkPipelineShaderStageCreateInfo vertShaderStageInfoQuadOverlay{};
        vertShaderStageInfoQuadOverlay.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfoQuadOverlay.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfoQuadOverlay.module = vertShaderModuleQuadOverlay;
        vertShaderStageInfoQuadOverlay.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfoQuadOverlay{};
        fragShaderStageInfoQuadOverlay.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfoQuadOverlay.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfoQuadOverlay.module = fragShaderModuleQuadOverlay;
        fragShaderStageInfoQuadOverlay.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStagesQuadOverlay[] = { vertShaderStageInfoQuadOverlay, fragShaderStageInfoQuadOverlay };

        //VkPipelineVertexInputStateCreateInfo vertexInputInfoQuad{};
        //vertexInputInfoQuad.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        //auto bindingDescriptionQuad = Vertex::getBindingDescription();
        //auto attributeDescriptionsQuad = Vertex::getAttributeDescriptions();

        //vertexInputInfoQuad.vertexBindingDescriptionCount = 1;
        //vertexInputInfoQuad.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptionsQuad.size());
        //vertexInputInfoQuad.pVertexBindingDescriptions = &bindingDescriptionQuad;
        //vertexInputInfoQuad.pVertexAttributeDescriptions = attributeDescriptionsQuad.data();

        //VkPipelineInputAssemblyStateCreateInfo inputAssemblyQuad{};
        //inputAssemblyQuad.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        //inputAssemblyQuad.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        //inputAssemblyQuad.primitiveRestartEnable = VK_FALSE;

        //VkPipelineLayoutCreateInfo pipelineLayoutInfoQuad{};
        //pipelineLayoutInfoQuad.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        //pipelineLayoutInfoQuad.setLayoutCount = 1;
        //pipelineLayoutInfoQuad.pSetLayouts = &descriptorSetLayout;

        //if (vkCreatePipelineLayout(device, &pipelineLayoutInfoQuad, nullptr, &pipelineLayoutQuad) != VK_SUCCESS) {
        //    throw std::runtime_error("failed to create pipeline layout!");
        //}

        VkGraphicsPipelineCreateInfo pipelineInfoQuadOverlay{};
        pipelineInfoQuadOverlay.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfoQuadOverlay.stageCount = 2;
        pipelineInfoQuadOverlay.pStages = shaderStagesQuadOverlay;
        pipelineInfoQuadOverlay.pVertexInputState = &vertexInputInfoQuad;
        pipelineInfoQuadOverlay.pInputAssemblyState = &inputAssemblyQuad;
        pipelineInfoQuadOverlay.pViewportState = &viewportState;
        pipelineInfoQuadOverlay.pRasterizationState = &rasterizerPolygon;
        pipelineInfoQuadOverlay.pMultisampleState = &multisampling;
        pipelineInfoQuadOverlay.pDepthStencilState = &depthStencil;
        pipelineInfoQuadOverlay.pColorBlendState = &colorBlending;
        pipelineInfoQuadOverlay.layout = pipelineLayoutQuad;
        pipelineInfoQuadOverlay.renderPass = renderPass;
        pipelineInfoQuadOverlay.subpass = 0;
        pipelineInfoQuadOverlay.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfoQuadOverlay, nullptr, &graphicsPipelineQuadOverlay) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create graphics pipeline!");
        }

        vkDestroyShaderModule(device, fragShaderModuleQuadOverlay, nullptr);
        vkDestroyShaderModule(device, vertShaderModuleQuadOverlay, nullptr);



}
