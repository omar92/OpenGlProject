#pragma once
#pragma comment(lib,"sfml-graphics.lib" )
#pragma comment(lib,"sfml-window.lib" )
#pragma comment(lib,"sfml-system.lib" )
#pragma comment(lib,"opengl32.lib" )
#pragma comment(lib,"glew32.lib")

#include<gl\glew\glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include<fstream>
#include<memory>

class WindowHandler
{
	//sf::Vector2f screenWidth;
	static sf::Window *window;

public:
	void(*onPress)(sf::Vector2i  element);
	void(*onRelease)(sf::Vector2i  element);
	void(*onHover)(sf::Vector2i  element);
	void(*onStart)(WindowHandler &uim);
	void(*onUpdate)(WindowHandler &uim);
	void(*onRender)(WindowHandler &uim);
	void(*onClose)(WindowHandler &uim);
	void(*onEvent)(WindowHandler &uim, sf::Event ev);

	static std::shared_ptr<WindowHandler> createWindow(sf::Vector2i size, sf::Vector3f bgColor = sf::Vector3f(.5, .5, .5));
	~WindowHandler();
	void startDisplay();
private:
	static bool isGLEWInitialised;

	WindowHandler(sf::Window&);
	void start();
	void update();
	void render();
	void close();

	bool isOpen();
	void pullInputs();
	void clear();
};

