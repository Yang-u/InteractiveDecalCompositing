#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

unsigned int Texture::genTexture(const std::string& path,int* width,int* height,int* nrChannels) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char *data = stbi_load(path.c_str(), width, height, nrChannels, 0);
	if (data)
	{
		if(*nrChannels==3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if(*nrChannels==4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return texture;
}
void Texture::saveTexture(int w,int h,int n,unsigned char* data,const std::string& path) {

		stbi_write_png(path.c_str(), w, h, n, data, w * n);
}

void Texture::saveTexture(int w, int h, int n ,unsigned int texture, const std::string& path) {
	unsigned char * data = (unsigned char *)malloc(w * h * n);
	assert(data);

	glBindTexture(GL_TEXTURE_2D,texture);
	if (n == 3)
	{
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_write_png((path+".jpg").c_str(), w, h, n, data, 0);
	}
	else if (n == 4) {
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_write_png((path + ".png").c_str(), w, h, n, data, 0);
	}

}