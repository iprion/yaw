
#include "shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(Device& device) : device{device} {
}

void Shader::init(const std::string& path, const std::string& vertex_code_file_name, const std::string& fragment_code_file_name) {
	std::string vertex_code_file_path = path + "/" + vertex_code_file_name + ".vert.spv";
	std::string fragment_code_file_path = path + "/" + fragment_code_file_name + ".frag.spv";
	std::vector<char> vsCode = loadFile(vertex_code_file_path);
	std::vector<char> fsCode  = loadFile(fragment_code_file_path);
	init(vsCode, fsCode);
}

void Shader::init(const std::vector<char>& vertex_code, const std::vector<char>& fragment_code) {
	vertex_code_ = vertex_code;
	fragment_code_ = fragment_code;
}

void Shader::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	if (vkCreateShaderModule(device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create shader module");
	}


}


std::vector<char> Shader::loadFile(const std::string& filename) const {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open())
		throw std::runtime_error("Failed to load Shader File "+ filename);

	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();
	return buffer;
}

void Shader::use() {
    //glUseProgram(id_);
}

template<>
void Shader::setUniform<int>(const std::string& name, int val) {
	use();
	//glUniform1i(glGetUniformLocation(id_, name.c_str()), val);
}

template<>
void Shader::setUniform<bool>(const std::string& name, bool val) {
	use();
	//glUniform1i(glGetUniformLocation(id_, name.c_str()), val);
}

template<>
void Shader::setUniform<float>(const std::string& name, float val) {
	use();
	//glUniform1f(glGetUniformLocation(id_, name.c_str()), val);
}

template<>
void Shader::setUniform<float>(const std::string& name, float val1, float val2) {
	use();
	//glUniform2f(glGetUniformLocation(id_, name.c_str()), val1, val2);
}

template<>
void Shader::setUniform<float>(const std::string& name, float val1, float val2, float val3) {
	use();
	//glUniform3f(glGetUniformLocation(id_, name.c_str()), val1, val2, val3);
}

template<>
void Shader::setUniform<float*>(const std::string& name, float* val) {
	use();
	//glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, val);
}

void Shader::checkCompileErr() {
    // int success;
    // char infoLog[1024];
    // glGetShaderiv(vertex_id_, GL_COMPILE_STATUS, &success);
    // if (!success) {
    //     glGetShaderInfoLog(vertex_id_, 1024, NULL, infoLog);
    //     std::cerr << "Error compiling Vertex Shader:\n" << infoLog << std::endl;
    // }
	// glGetShaderiv(fragment_id_, GL_COMPILE_STATUS, &success);
	// if (!success) {
	// 	glGetShaderInfoLog(fragment_id_, 1024, NULL, infoLog);
	// 	std::cerr << "Error compiling Fragment Shader:\n" << infoLog << std::endl;
	// }
}

void Shader::checkLinkingErr() {
	// int success;
	// char infoLog[1024];
	// glGetProgramiv(id_, GL_LINK_STATUS, &success);
	// if (!success) {
	// 	glGetProgramInfoLog(id_, 1024, NULL, infoLog);
	// 	std::cerr << "Error Linking Shader Program:\n" << infoLog << std::endl;
	// }
}
