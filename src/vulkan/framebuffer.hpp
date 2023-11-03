#pragma once

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
    u_int32_t fbo;
    u_int32_t rbo;
    u_int32_t textureId;
 	u_int32_t width;
    u_int32_t height;
};
