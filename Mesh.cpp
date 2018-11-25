#include "Mesh.h"
#include <glad\glad.h>


Mesh::Mesh(const vector<Vertex>& vertices)
{
	this->_vertices = vertices;
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

	glBindVertexArray(0);
}

void Mesh::render(const Shader& shader)
{
	shader.use();
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
	glBindVertexArray(0);
}

void Mesh::renderOffScreen(const Shader& colorShader) {
	colorShader.use();
	colorShader.setFloat("color", 0);
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
	glBindVertexArray(0);
}