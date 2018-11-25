#include "ModelRenderer.h"

void ModelRenderer::initialize(Model* pm) {
	_model = pm;
	glGenFramebuffers(1, &_fbo);

	glGenTextures(1, &_texture_color_buffer);
	glBindTexture(GL_TEXTURE_2D, _texture_color_buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture_color_buffer, 0);

	glGenRenderbuffers(1, &_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "error" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
void ModelRenderer::render(const Shader& shader)
{
	shader.use();
	glm::mat4 view = camera.LookAt();
	glm::mat4 projection = glm::perspective<float>(glm::radians(camera._zoom), Width / Height, 0.1f, 100.0f);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("direction", camera._front);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_texture = fractalRenderer.getTexture();
	glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
	glBindTexture(GL_TEXTURE_2D, _texture);

	_model->render(shader);

}

void ModelRenderer::renderOffScreen(const Shader& colorShader, double x, double y)
{
	colorShader.use();
	glm::mat4 view = camera.LookAt();
	glm::mat4 projection = glm::perspective<float>(glm::radians(camera._zoom), Width / Height, 0.1f, 100.0f);
	colorShader.setMat4("view", view);
	colorShader.setMat4("projection", projection);

	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_model->renderOffScreen(colorShader);

	unsigned char * rgb = new unsigned char[3];

	glReadPixels(x, Height - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, rgb);
	int color = (int)rgb[0];
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (color == 255)_is_selected =false;
	else _is_selected = true;

}
