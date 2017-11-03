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
glm::vec2 convertMousePos(glm::vec2 pos);
std::shared_ptr<Shader> shader;
std::shared_ptr<std::vector<vertex>> coreVertices;
std::shared_ptr<std::vector<GLuint>> cubeindices;
Camera* activeCamera;
//Mesh mesh;
//Mesh mesh2;
float theta = 0;
char **level;
std::vector<std::vector<Mesh>> meshVectorBig;
int rows, cols;
int playerPosX, playerPosZ;
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

			if (level[i][j] == '$')
			{
				myMesh.translate(i, 0, j);
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

void rotateLevel()
{
	for (int i = 0; i < cols; i++)
	{
		for (int j = 0; j<rows; j++)
		{
			meshVectorBig[i][j].rotate(theta, glm::vec3(0, 1, 0));
		}
	}
}

int main()
{
	auto window = WindowHandler::createWindow(sf::Vector2i(SCREEN_WIDTH, SCREEN_Height));
	window->onStart = onStart;
	window->onUpdate = onUpdate;
	window->onHover = onHover;
	window->onRender = onRender;
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
}

void onUpdate(WindowHandler & uim)
{
	//theta += 0.001f;
	rotateLevel();
	//mesh.rotate(theta, glm::vec3(0, 1, 0));
	//mesh2.rotate(-theta, glm::vec3(0, 1, 0));
}

void onRender(WindowHandler & uim)
{
	renderLevel();
	//mesh.render(activeCamera);
	//mesh2.render(activeCamera);
}



void onHover(sf::Vector2i cPos)
{
	auto p = convertMousePos(glm::vec2(cPos.x, cPos.y));
	activeCamera->lookAt(glm::vec3(p.x, p.y, 0.0f));
}


glm::vec2 convertMousePos(glm::vec2 pos) {
	//printf("\n(%f,%f):\t", pos.x, pos.y);
	pos.x = -1 + ((pos.x * 2) / SCREEN_WIDTH);
	pos.y = 1 - ((pos.y * 2) / SCREEN_Height);
	return pos;
}
