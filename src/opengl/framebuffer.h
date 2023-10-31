#ifndef framebuffer_hpp
#define framebuffer_hpp

#include <GL/glew.h>

#include <string>
#include <vector>

class Framebuffer
{
public:

	void create(unsigned int width, unsigned int height);
    void bind();
    void unbind();
    bool resize(unsigned int width, unsigned int height);
	void* texture() const;

private:
    GLuint fbo;
    GLuint rbo;
    GLuint textureId;
 	GLuint width;
    GLuint height;
};

#endif /* opengl_shader_hpp */
