#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <strstream>

#include "Shader.h"

using std::string;
using std::vector;
using std::stringstream;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

class Mesh {
public:
	Mesh(const vector<Vertex>& vertices);
	void render(const Shader& shader);
	void renderOffScreen(const Shader& colorShader);
public:
	vector<Vertex> _vertices;
private:
	unsigned int _vao, _vbo;
};
