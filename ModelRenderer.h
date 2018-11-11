#pragma once
#include "Model.h"


extern Camera camera;
extern double Width;
extern double Height;

class ModelRenderer
{
public:
	ModelRenderer(){}
	void init(Model* pm);

	void render(const Shader& shader);
	void renderOffScreen(const Shader& colorShader, double x, double y);
	bool isSelect() { return isSelected; };
	Model* getModel() { return p_model; }
private:
	unsigned int genTexture(const string& path);
private:
	Model* p_model;

	unsigned framebuffer;
	unsigned int textureColorbuffer;
	unsigned int rbo;
	unsigned int texture;

	bool isSelected;
};

