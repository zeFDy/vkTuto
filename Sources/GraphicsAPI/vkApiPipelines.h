#pragma once
#include "vulkan.h"
#include "vkApiDevices.h"
#include <vector>

class vkApiPipelines
{
public:		
				vkApiPipelines();
				~vkApiPipelines();



private:		
				void				CreateAllPipelines();
				std::vector<char>	ReadFile(const std::string& filename);


				VkPipelineLayout                pipelineLayout;
				VkPipeline                      graphicsPipeline3dOneTexure;
				VkPipeline                      graphicsPipeline3dOneTexureYGoGg;

				VkPipelineLayout                pipelineLayoutQuad;
				VkPipeline                      graphicsPipelineQuad;

				VkPipelineLayout                pipelineLayoutQuadOverlay;
				VkPipeline                      graphicsPipelineQuadOverlay;

};

