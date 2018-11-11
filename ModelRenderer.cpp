#include "ModelRenderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


void ModelRenderer::init(Model* pm) {
	p_model = pm;
	texture = genTexture("texture/Chess_Board.png");
	glGenFramebuffers(1, &framebuffer);
	glGenTextures(1, &textureColorbuffer);
	glGenRenderbuffers(1, &rbo);
}
void ModelRenderer::render(const Shader& shader)
{
	shader.use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
	glBindTexture(GL_TEXTURE_2D, texture);



	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective<float>(glm::radians(camera.Zoom), Width / Height, 0.1f, 100.0f);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setVec3("direction", camera.Front);

	p_model->render(shader);

}

void ModelRenderer::renderOffScreen(const Shader& colorShader, double x, double y)
{
	colorShader.use();
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "error" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective<float>(glm::radians(camera.Zoom), Width / Height, 0.1f, 100.0f);
	colorShader.setMat4("view", view);
	colorShader.setMat4("projection", projection);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	p_model->renderOffScreen(colorShader);

	unsigned char * rgb = new unsigned char[3];

	glReadPixels(x, Height - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, rgb);
	int color = (int)rgb[0];
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (color == 255)isSelected=false;
	else isSelected = true;

}


unsigned int ModelRenderer::genTexture(const string& path) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}