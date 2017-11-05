#include "Window.h"
bool WindowHandler::isGLEWInitialised = false;
sf::Window* WindowHandler::window = nullptr;
WindowHandler::WindowHandler(sf::Window & rw)
{
	onPress = nullptr;
	onRelease = nullptr;
	onHover = nullptr;
	onUpdate = nullptr;
	onClose = nullptr;
	onEvent = nullptr;
	onRender = nullptr;
}

bool WindowHandler::isOpen()
{
	return window->isOpen();
}

void WindowHandler::close()
{
	window->close();
}

std::shared_ptr<WindowHandler> WindowHandler::createWindow(sf::Vector2i size, sf::Vector3f bgColor)
{
	//create sfml window
	sf::ContextSettings context;
	context.depthBits = 24;
	window = new sf::Window(sf::VideoMode(size.x, size.y), "SFML works!", sf::Style::Close, context);
	//window->create(sf::VideoMode(size.x, size.y), "SFML works!");

	if (!isGLEWInitialised) {
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			std::cout << "Error( " << err << " )";
			window->close();
			getchar();
		}
		else
		{
			if (GLEW_VERSION_3_0)
				std::cout << "Driver support OpenGL 3.0\nDetails:\n";
			std::cout << "\tUsing glew " << glewGetString(GLEW_VERSION) << std::endl;
			std::cout << "\tVendor: " << glGetString(GL_VENDOR) << std::endl;
			std::cout << "\tRenderer: " << glGetString(GL_RENDERER) << std::endl;
			std::cout << "\tVersion: " << glGetString(GL_VERSION) << std::endl;
			std::cout << "\tGLSL:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
			isGLEWInitialised = true;
		}
	}

	//set openGl clearColor
	glClearColor(bgColor.x, bgColor.y, bgColor.z, 1);

	std::shared_ptr<WindowHandler> sPtr = std::shared_ptr<WindowHandler>(new WindowHandler(*window));
	return sPtr;
}

WindowHandler::~WindowHandler()
{
	delete(window);
}

void WindowHandler::startDisplay()
{
	_start();
	while (isOpen())
	{
		_update();
		_render();
	}
	_close();
}
void WindowHandler::_start()
{


	onStart(*this);
}
void WindowHandler::_update()
{
	pullInputs();
	if (onUpdate != nullptr)
		onUpdate(*this);

	//window.display();
}
void WindowHandler::_render()
{
	//glClearColor(1, 1, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glBegin(GL_TRIANGLES);
	//glVertex2f(0.0,1.0);
	//glColor3f(1,0,0);
	//glVertex2f(-1,-1);
	//glColor3f(1,0,0);
	//glVertex2f(1,-1);
	//glColor3f(1,0,0);
	//glEnd();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (onRender != nullptr)
		onRender(*this);

	window->display();
}
void WindowHandler::_close()
{
	if (onClose != nullptr)
		onClose(*this);
}

void WindowHandler::clear() {
	//window->clear();
}


void WindowHandler::pullInputs()
{
	sf::Event ev;
	//sf::Vector2i cPos(0,0);
	sf::Vector2i cPos = sf::Mouse::getPosition(*window);
	while (window->pollEvent(ev))
	{
		if (onEvent != nullptr)
			onEvent(*this, ev);
		switch (ev.type)
		{
		case sf::Event::EventType::MouseButtonPressed:
		case sf::Event::EventType::MouseButtonReleased:
		case sf::Event::EventType::MouseMoved:
			switch (ev.type)
			{
			case sf::Event::EventType::MouseButtonPressed:
			{
				if (onPress != nullptr)
					onPress(cPos);
			}
			break;
			case sf::Event::EventType::MouseButtonReleased:
			{
				if (onRelease != nullptr)
					onRelease(cPos);
			}
			break;
			case sf::Event::EventType::MouseMoved:
			{
				if (onHover != nullptr)
					onHover(cPos);
			}
			break;
			default:
				break;
			}
			break;
		case sf::Event::Closed:
			window->close();
			break;

		default:
			break;
		}
	}
}
