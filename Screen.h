#pragma once
#include <glad/glad.h>
#include <nanogui/nanogui.h>
#include "Texture.h"
#include "FractalRenderer.h"

extern FractalRenderer fractalRenderer;

class Screen
{
public:
	Screen();
	~Screen();
	nanogui::Screen* getScreen() { return _screen; }
	void initialize(GLFWwindow*, bool);
	void update();

private:
	// ui elements
	nanogui::Screen* _screen = nullptr;
	nanogui::FormHelper* _gui = nullptr;
	nanogui::Window* _fractal_window = nullptr;
	nanogui::ImageView* _image_view = nullptr;
	nanogui::ComboBox* _combobox = nullptr;
	nanogui::Slider* _sliders[4];
};

