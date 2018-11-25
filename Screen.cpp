#include "Screen.h"

Screen::Screen() {
	_screen = new nanogui::Screen();
}
Screen::~Screen()
{
	//delete _screen;
	//delete _gui;
}
void Screen::initialize(GLFWwindow* window, bool flag) {

	assert(window);
	_screen->initialize(window, flag);
	_gui = new 	nanogui::FormHelper(_screen);

	_fractal_window = _gui->addWindow(Eigen::Vector2i(0, 0), "");
	_fractal_window->setFixedSize(Eigen::Vector2i(230, _screen->height()));
	_fractal_window->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical,
		nanogui::Alignment::Middle, 0,20));


	
	nanogui::Widget* panel = new nanogui::Widget(_fractal_window);
	panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical,
		nanogui::Alignment::Middle, 0, 30));
	_gui->addWidget("", panel);

	unsigned int texture = fractalRenderer.getTexture();
	_image_view = new nanogui::ImageView(panel, texture);
	nanogui::Vector2i _texture_size = nanogui::Vector2i(230, 230);
	_image_view->setFixedSize(_texture_size);

	nanogui::Label* label = new nanogui::Label(panel, "shader parameters");
	label->setFontSize(20);

	_combobox=new nanogui::ComboBox(panel, { "sierpinski", "julia", "mandelbrot" ,"newton"});
	_combobox->setFixedWidth(150);
	_combobox->setChangeCallback([&](bool b) {
		fractalRenderer.renderFractal(_combobox->selectedIndex(), _sliders[0]->value(), _sliders[1]->value(),
			_sliders[2]->value(), _sliders[3]->value());
	});
	for (int i = 0; i < 4; ++i) {
		_sliders[i] = new nanogui::Slider(panel);
		_sliders[i]->setValue(0.5f);
		_sliders[i]->setFixedWidth(200);
		_sliders[i]->setFinalCallback([&](float value) {
			fractalRenderer.renderFractal(_combobox->selectedIndex(), _sliders[0]->value(), _sliders[1]->value(),
				_sliders[2]->value(), _sliders[3]->value());
		});
	}
	_screen->performLayout();
}

void Screen::update() {

	_fractal_window->setPosition(Eigen::Vector2i(0, 0));
	_fractal_window->setHeight(_screen->height());

}

