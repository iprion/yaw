#pragma once

#include "shader.hpp"

class Triangle
{
public:
	Triangle(Device& device);
	void init();
	void draw();
	template<typename T> void setUniform(const std::string& name, T val) { shader.setUniform(name, val); };
	template<typename T> void setUniform(const std::string& name, T val1, T val2) { shader.setUniform(name, val1, val2); };
	template<typename T> void setUniform(const std::string& name, T val1, T val2, T val3) { shader.setUniform(name, val1, val2, val3); };
private :
  void create(unsigned int &vbo, unsigned int &vao, unsigned int &ebo);

private:
	Shader shader;
  	u_int32_t vbo, vao, ebo;
};