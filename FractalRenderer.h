#pragma once
#include <vector>

#include "Shader.h"
enum ShaderType {
	sierpinski = 0,
	julia,
	mandelbrot,
	newton
};

class FractalRenderer
{
public:
	FractalRenderer()=default;
	~FractalRenderer();

	void initialize(int w, int h);
	void renderFractal(int type, int t, int r, int p, int q);
	unsigned int getTexture(int* width, int* height, int* nrChannels);
	unsigned int getTexture(){ return _texture; }
private:
	std::vector<glm::vec3> _vertices;
	unsigned int _vao, _vbo;
	unsigned int _fbo;
	unsigned int _texture;
	int _width, _height;
private:
	Shader* _sierpinski_shader;
	Shader* _julia_shader;
	Shader* _mandelbrot_shader;
	Shader* _newton_shader;
};

