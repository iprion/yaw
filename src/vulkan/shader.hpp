#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "device.hpp"
#include <vector>

struct PipelineConfigInfo {
  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

class Shader
{
public:
	Shader(Device& device);
	
	void init(const std::vector<char>& vertex_code, const std::vector<char>& fragment_code);
	void init(const std::string& path, const std::string& vertex_code_file_name, const std::string& fragment_code_file_name);

	void use();

	template<typename T> void setUniform(const std::string& name, T val);
	template<typename T> void setUniform(const std::string& name, T val1, T val2);
	template<typename T> void setUniform(const std::string& name, T val1, T val2, T val3);

	static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

private :
	std::vector<char> loadFile(const std::string& filename) const;
	void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

private:
	void checkCompileErr();
	void checkLinkingErr();
	unsigned int vertex_id_, fragment_id_, id_;
	std::vector<char> vertex_code_;
	std::vector<char> fragment_code_;

	Device& device;
	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;
};
