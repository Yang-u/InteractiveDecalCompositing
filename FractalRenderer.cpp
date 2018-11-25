#include "FractalRenderer.h"



void FractalRenderer::initialize(int w, int h ) {
	_width = w;
	_height = h;
	for (float i = 0; i < _height; ++i) {
		float p = -1.0 + 2 * (i / _height);
		for (float j = 0; j < _width; ++j) {
			float q = -1.0 + 2 * (j / _width);
			_vertices.push_back(glm::vec3(p, q, 0.0));
		}
	}
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), &_vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenFramebuffers(1, &_fbo);
	glGenTextures(1, &_texture);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	_sierpinski_shader =new Shader("shader/sierpinski.vs", "shader/sierpinski.fs");
	_julia_shader=new Shader("shader/julia.vs", "shader/julia.fs");
	_mandelbrot_shader =new Shader("shader/mandelbrot.vs", "shader/mandelbrot.fs");
	_newton_shader =new Shader("shader/julia_newton.vs", "shader/julia_newton.fs");
	_sierpinski_shader ->initialize();
	_julia_shader ->initialize();
	_mandelbrot_shader ->initialize();
	_newton_shader ->initialize();
}
void FractalRenderer::renderFractal(int type, int t, int r, int p, int q)
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	switch (type)
	{
	case sierpinski:
		_sierpinski_shader->use();
		//_sierpinski_shader->setInt("t", t);
		//_sierpinski_shader->setInt("r", r);
		_sierpinski_shader->setInt("t", 12);
		_sierpinski_shader->setInt("r", 100);
		break;
	case julia:
		_julia_shader->use();
		//_julia_shader->setInt("t", t);
		//_julia_shader->setFloat("r", r);
		//_julia_shader->setFloat("p", -p);
		//_julia_shader->setFloat("q", -q);
		_julia_shader->setInt("t", 100);
		_julia_shader->setFloat("r", 500);
		_julia_shader->setFloat("p", -0.615);
		_julia_shader->setFloat("q", -0.43);
		break;
	case mandelbrot:
		_mandelbrot_shader->use();
		//_mandelbrot_shader->setInt("t", t);
		//_mandelbrot_shader->setFloat("r", r);
		_mandelbrot_shader->setInt("t", 100);
		_mandelbrot_shader->setFloat("r", 500);
		break;
	case newton:
		_newton_shader->use();
		//_newton_shader->setInt("t", t);
		_newton_shader->setInt("t", 2048);
		break;
	default:
		break;
	}
	glViewport(0, 0, _width, _height);
	glBindVertexArray(_vao);
	glDrawArrays(GL_POINTS, 0, _vertices.size());
	glBindVertexArray(0);
}
FractalRenderer::~FractalRenderer()
{
	delete _sierpinski_shader;
	delete _julia_shader;
	delete _mandelbrot_shader;
	delete _newton_shader;
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);

}

unsigned int FractalRenderer::getTexture(int* width, int* height, int* nrChannels) {
	*width = _width;
	*height = _height;
	*nrChannels = 3;
	return _texture;
}


