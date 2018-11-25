#pragma once
#include "Model.h"
#include "Texture.h"
#include "FractalRenderer.h"

extern Camera camera;
extern double Width;
extern double Height;
extern FractalRenderer fractalRenderer;

class ModelRenderer
{
public:
	ModelRenderer(){}
	void initialize(Model* pm);

	void render(const Shader& shader);
	void renderOffScreen(const Shader& colorShader, double x, double y);
	bool isSelect() { return _is_selected; };
	Model* getModel() { return _model; }

private:
	Model* _model;

	unsigned _fbo;
	unsigned int _texture_color_buffer;
	unsigned int _rbo;
	unsigned int _texture;

	bool _is_selected;
};

