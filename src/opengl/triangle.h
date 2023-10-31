#ifndef triangle_hpp
#define triangle_hpp

#include "shader.h"
#include <GL/glew.h> 

class Triangle
{
public:
	Triangle();
	void init();
	void draw();
	template<typename T> void setUniform(const std::string& name, T val) { shader.setUniform(name, val); };
	template<typename T> void setUniform(const std::string& name, T val1, T val2) { shader.setUniform(name, val1, val2); };
	template<typename T> void setUniform(const std::string& name, T val1, T val2, T val3) { shader.setUniform(name, val1, val2, val3); };
private :
  void create(unsigned int &vbo, unsigned int &vao, unsigned int &ebo);

private:
	Shader shader;
  	GLuint vbo, vao, ebo;
};

#endif /* opengl_shader_hpp */
