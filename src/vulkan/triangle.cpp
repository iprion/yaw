#include "triangle.hpp"
#include <iostream>

Triangle::Triangle(Device& device)
	: shader(device) {
}

void Triangle::init() {

	create(vbo, vao, ebo);	
    // init shader
	shader.init("src/shaders", "simple-shader", "simple-shader");
    shader.use();
	shader.setUniform("color", 1.0f,1.0f,1.0f);
    shader.setUniform("rotation", 0.0f);
    shader.setUniform("translation", 0.0f,0.0f);
}

void Triangle::draw() {
    // rendering our geometries
    shader.use();
	// glBindVertexArray(vao);
    // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    // glBindVertexArray(0);
	// glUseProgram(0);
}


void Triangle::create(unsigned int &vbo, unsigned int &vao, unsigned int &ebo)
{

	// create the triangle
	float triangle_vertices[] = {
		0.0f, 0.25f, 0.0f,	// position vertex 1
		1.0f, 0.0f, 0.0f,	 // color vertex 1
		0.25f, -0.25f, 0.0f,  // position vertex 1
		0.0f, 1.0f, 0.0f,	 // color vertex 1
		-0.25f, -0.25f, 0.0f, // position vertex 1
		0.0f, 0.0f, 1.0f,	 // color vertex 1
	};
	unsigned int triangle_indices[] = {0, 1, 2};
	
	// glGenVertexArrays(1, &vao);
	// glGenBuffers(1, &vbo);
	// glGenBuffers(1, &ebo);
	// glBindVertexArray(vao);
	// glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle_indices), triangle_indices, GL_STATIC_DRAW);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	// glEnableVertexAttribArray(1);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindVertexArray(0);
}
