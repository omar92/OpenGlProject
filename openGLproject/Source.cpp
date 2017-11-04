#pragma comment(lib,"sfml-graphics.lib" )
#pragma comment(lib,"sfml-window.lib" )
#pragma comment(lib,"sfml-system.lib" )
#pragma comment(lib,"opengl32.lib" )
#pragma comment(lib,"glew32.lib")

#include <gl\glew\glew.h>

#include<gl\glm\glm.hpp>
#include<gl\glm\gtc\type_ptr.hpp>
#include<gl\glm\gtx\transform.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <iostream>
#include <fstream>
#include <memory>

#include "Window.h"
#include "OUtilities.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include <vector>

#define  SCREEN_WIDTH 500//1366	
#define  SCREEN_Height 500//768


void onHover(sf::Vector2i cPos);
void onStart(WindowHandler &uim);
void onUpdate(WindowHandler &uim);
void onRender(WindowHandler &uim);
void onEvent(WindowHandler &uim, sf::Event ev);

glm::vec2 convertMousePos(glm::vec2 pos);
std::shared_ptr<Shader> shader;
std::shared_ptr<std::vector<vertex>> coreVertices;
std::shared_ptr<std::vector<GLuint>> cubeindices;
Camera* activeCamera;
//Mesh mesh;
//Mesh mesh2;
Mesh Player;
float theta = 0;
char **level;
std::vector<std::vector<Mesh>> meshVectorBig;
int rows, cols;
int playerPosX, playerPosZ;
int playerPosY = 4;

void loadLevel()
{
	std::ifstream loadLvl;
	loadLvl.open("myLevel.txt");
	loadLvl >> cols >> rows;
	level = (char**)malloc(sizeof(char*)*cols);
	for (int i = 0; i < cols; i++)
		level[i] = (char*)malloc(sizeof(char)*rows);

	for (int i = 0; i<cols; i++)
	{
		for (int j = 0; j<rows; j++)
		{
			level[i][j] = loadLvl.get();
		}
	}
	loadLvl.close();
}
void drawLevel()
{
	int x = 0;
	for (int i = 0; i < cols; i++)
	{
		std::vector<Mesh> meshVectorSmall;
		for (int j = 0; j<rows; j++)
		{
			Mesh myMesh;
			myMesh = Mesh::create_cube(shader);
			myMesh.setColor(glm::vec3((float)i/ cols,.2, (float)j/ rows));
			if (level[i][j] == '$')
			{
				myMesh.translate(i, 1, j);
				meshVectorSmall.push_back(myMesh);
			}
			else if (level[i][j] == '#')
			{
				myMesh.translate(i, 3, j);
				meshVectorSmall.push_back(myMesh);
			}
			else
			{
				myMesh.translate(i, 0, j);
				meshVectorSmall.push_back(myMesh);
				if (level[i][j] == '@')
				{
					playerPosX = i;
					playerPosZ = j;
				}
			}
		}
		meshVectorBig.push_back(meshVectorSmall);
	}
}
void renderLevel()
{
	for (int i = 0; i < cols; i++)
	{
		for (int j = 0; j<rows; j++)
		{
			meshVectorBig[i][j].render(activeCamera);
		}
	}
}
//void rotateLevel()
//{
//	for (int i = 0; i < cols; i++)
//	{
//		for (int j = 0; j<rows; j++)
//		{
//			meshVectorBig[i][j].translate(0,0,0);
//		}
//	}
//}

void MovePlayerDown()
{
	int blocked = 0;
	while (playerPosZ < rows-1 && !blocked)
	{
		if (level[playerPosX][playerPosZ] != '#')
		{
			playerPosZ++;
		}
		else
		{
			blocked = 1;
			playerPosZ--;
		}
	}
	if (level[playerPosX][playerPosZ] == '#')
	{
		playerPosZ--;
	}
	if (level[playerPosX][playerPosZ] == '$')
	{
		std::cout << "Gratz you Won";
	}
	Player.set_position(glm::vec3(playerPosX, playerPosY, playerPosZ));
}
void MovePlayerUp()
{
	int blocked = 0;
	while (playerPosZ > 0 && !blocked)
	{
		if (level[playerPosX][playerPosZ] != '#')
		{
			playerPosZ--;
		}
		else
		{
			blocked = 1;
			playerPosZ++;
		}
	}
	if (level[playerPosX][playerPosZ] == '#')
	{
		playerPosZ++;
	}
	if (level[playerPosX][playerPosZ] == '$')
	{
		std::cout << "Gratz you Won";
	}
	Player.set_position(glm::vec3(playerPosX, playerPosY, playerPosZ));
}
void MovePlayerRight()
{
	int blocked = 0;
	while (playerPosX < cols-1 && !blocked)
	{
		if (level[playerPosX][playerPosZ] != '#')
		{
			playerPosX++;
		}
		else
		{
			blocked = 1;
			playerPosX--;
		}
	}
	if (level[playerPosX][playerPosZ] == '#')
	{
		playerPosX--;
	}
	if (level[playerPosX][playerPosZ] == '$')
	{
		std::cout << "Gratz you Won";
	}
	Player.set_position(glm::vec3(playerPosX, playerPosY, playerPosZ));
}
void MovePlayerLeft()
{
	int blocked = 0;
	while (playerPosX > 0 && !blocked)
	{
		if (level[playerPosX][playerPosZ] != '#')
		{
			playerPosX--;
		}
		else
		{
			blocked = 1;
			playerPosX++;
		}
	}
	if (level[playerPosX][playerPosZ] == '#')
	{
		playerPosX++;
	}
	if (level[playerPosX][playerPosZ] == '$')
	{
		std::cout << "Gratz you Won";
	}
	Player.set_position(glm::vec3(playerPosX, playerPosY, playerPosZ));
}

int main()
{
	auto window = WindowHandler::createWindow(sf::Vector2i(SCREEN_WIDTH, SCREEN_Height));
	window->onStart = onStart;
	window->onUpdate = onUpdate;
	window->onHover = onHover;
	window->onRender = onRender;
	window->onEvent = onEvent;
	shader = Shader::LoadFromFile("texture_vs.glsl", "texture_fs.glsl");
	//shader->use();
	window->startDisplay();
}	

void onStart(WindowHandler & uim)
{
	activeCamera = new Camera(PROJECTION_TYPE::PERSPECTIVE, glm::vec2(SCREEN_WIDTH, SCREEN_Height));
	activeCamera->set_position(glm::vec3(10, 6, 12));
	activeCamera->set_fov(60);
	activeCamera->lookAt(glm::vec3(0, 0, 0));
	//mesh = Mesh::create_cube(shader);
	//mesh2 = Mesh::create_cube(shader);
	//mesh.translate(2, 0, 0);
	loadLevel();
	drawLevel();
	Player = Mesh::create_cube(shader);
	Player.setColor(glm::vec3(0, 1, 0));
	Player.translate(playerPosX, playerPosY, playerPosZ);

	/*
	MovePlayerDown();
	MovePlayerRight();
	MovePlayerDown();
	MovePlayerRight();
	MovePlayerUp();
	MovePlayerLeft();
	*/

	std::cout << "\n" << playerPosX << " " << playerPosZ;
}

void onUpdate(WindowHandler & uim)
{
	//theta += 0.001f;
//	rotateLevel();
	Player.rotate(theta, glm::vec3(0, 1, 0));
	//mesh.rotate(theta, glm::vec3(0, 1, 0));
	//mesh2.rotate(-theta, glm::vec3(0, 1, 0));
}

void onRender(WindowHandler & uim)
{
	renderLevel();
	Player.render(activeCamera);
	//mesh.render(activeCamera);
	//mesh2.render(activeCamera);
}

void onHover(sf::Vector2i cPos)
{
	auto p = convertMousePos(glm::vec2(cPos.x, cPos.y));
	activeCamera->lookAt(glm::vec3(p.x, p.y, 0.0f));
}

void onEvent(WindowHandler &uim, sf::Event ev)
{
	switch (ev.type)
	{
	case sf::Event::EventType::KeyPressed:
		switch (ev.key.code)
		{
		case sf::Keyboard::W:
			MovePlayerUp();
			break;
		case sf::Keyboard::S:
			MovePlayerDown();
			break;
		case sf::Keyboard::D:
			MovePlayerRight();
			break;
		case sf::Keyboard::A:
			MovePlayerLeft();
			break;
		case sf::Keyboard::Delete:

			break;
		default:
			break;
		}

		break;
	default:
		break;
	}
}

glm::vec2 convertMousePos(glm::vec2 pos) 
{
	//printf("\n(%f,%f):\t", pos.x, pos.y);
	pos.x = -1 + ((pos.x * 2) / SCREEN_WIDTH);
	pos.y = 1 - ((pos.y * 2) / SCREEN_Height);
	return pos;
}
