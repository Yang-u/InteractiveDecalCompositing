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
	void draw(const Shader& shader);

public:
	vector<Vertex> vertices;
private:
	unsigned int VAO, VBO;
};
