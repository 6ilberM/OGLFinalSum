// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School.
//
// File Name    :main.cpp
// Description    : THE MAIN FUNCTION
// Author        :Gilberto Moreno 
// Mail            :gilberto.mor6945@mediadesign.school.nz
//toolincludes
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <time.h>
#include <stdlib.h>

//OGLincludes
#include"glew.h"
#include "freeglut.h"
#include "soil\SOIL.h"
#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\matrix_transform.hpp"
#include "glm\glm\gtc\type_ptr.hpp"

//.h includes 
#include "camera.h"
#include "ShaderLoader.h"
#include "Primitive.h"
#include "Skybox.h"
#include "Utils.h"
#include "Terrain.h"
#include "FrameBuffer.h"
#include "Star.h"
#include "Tessel.h"
#include "Particle.h"
#include "Constraint.h"
#include "Sphere.h"
#include "Subtitle.h"
#include "Rain.h"

//Globals
GLuint program;
GLuint secprogram;
GLuint thirprogram;
GLuint qtoprogram;
GLuint fifprogram;
GLuint sextoprog;
GLuint textProgram;

GLuint texture;
GLuint lightVao;
GLuint labelprog;

//PtrToObject
Camera *mainCamera;
Skybox *SkyboxCube;
Primitive *Cuadrado;
CTerrain *TIERRA;
CfrBuff *FRAMEBUFF;
CSphere *esfera;
CRain *Lluvia;


TextLabel* label;
TextLabel* Wind;
TextLabel* WindVal;
TextLabel* Hooker;
TextLabel* HookVal;

//Time
GLfloat currentTime;
GLfloat lastTime;
GLfloat deltaTime;

float DeltaTime;
auto t2 = std::chrono::high_resolution_clock::now();

using std::cin;

//Mouse Input
#define MOUSE_LEFT 0 
#define MOUSE_MIDDLE 1 
#define MOUSE_RIGHT  2

unsigned char mouseState[3];
GLfloat yaw = -90.0f; GLfloat pitch = 0.0f;
GLfloat lastX = 800 / 2.0;
GLfloat lastY = 800 / 2.0;

bool firstMouse = true;
float f_pickplacex = 6.0f, f_pickplacey = 7.0f;

// Keyboard Input
#define BUTTON_UP 0
#define BUTTON_DOWN 1

unsigned char keyState[255];

//Picking 
glm::vec3 rayDirection;
float mouseY;
float mouseX;

void keyboard(unsigned char key, int x, int y)
{
	//Camera Move
	keyState[key] = BUTTON_DOWN;
}

void keyboard_up(unsigned char key, int x, int y)
{
	keyState[key] = BUTTON_UP;
}

void handleResize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 6000.0);
}

void mouse(int button, int button_state, int x, int y)
{
#define state ( (button_state == GLUT_DOWN) ? BUTTON_DOWN : BUTTON_UP)

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		mouseState[MOUSE_LEFT] = state;
		break;
	case GLUT_MIDDLE_BUTTON:
		mouseState[MOUSE_MIDDLE] = state;
		break;
	case GLUT_RIGHT_BUTTON:
		mouseState[MOUSE_RIGHT] = state;
		break;
	}
}
float pickx;
float picky;

void mousePassiveMove(int x, int y)
{
	if (firstMouse)
	{
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	mouseX = (2.0f * x) / 800.0f - 1.0f;
	mouseY = 1.0f - (2.0f * y) / 800.0f;

	GLfloat xoffset = x - lastX;
	GLfloat yofsset = lastY - y; //reversed since y coordinates go from bottom to left

	pickx = xoffset;
	picky = yofsset;

	lastX = x;
	lastY = y;
	

	GLfloat sensitivity = 0.60f;
	xoffset *= sensitivity;
	yofsset *= sensitivity;
	yaw += xoffset;
	pitch += yofsset;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	
	mainCamera->setCameraFront(front);
}

bool updateMousePicking()
{

	return(true);
}

CTerrain::InitInfo s_parameter;
glm::vec3 ball_pos(0, -0.1, 0); //the center of our one ball
float ball_radius = .5f; //the radius of our one ball

void init()
{
	//Camera Init
	mainCamera = new Camera();
	mainCamera->init();
	srand(time(NULL));

	shaderloader sLoader;
	program = sLoader.createprogram("Assets/Shaders/VAO_Triangle.vs", "Assets/Shaders/VAO_Triangle.fs");
	secprogram = sLoader.createprogram("Assets/Shaders/AlphaTriangle.vs", "Assets/Shaders/AlphaTriangle.fs");
	thirprogram = sLoader.createprogram("Assets/Shaders/BADCRT.vs", "Assets/Shaders/BADCRT.fs");
	qtoprogram = sLoader.createprogram("Assets/Shaders/Star.vs", "Assets/Shaders/Star.fs", "Assets/Shaders/Star.gs");
	fifprogram = sLoader.createProgram("Assets/Shaders/TriangleTs.vs", "Assets/Shaders/TriangleTs.fs", "Assets/Shaders/TriangleTs.tcs", "Assets/Shaders/TriangleTs.tes");
	sextoprog = sLoader.createprogram("Assets/Shaders/Particle.vs", "Assets/Shaders/Particle.fs", "Assets/Shaders/Particle.gs");
	textProgram = sLoader.createprogram("Assets/Shaders/text.vs", "Assets/Shaders/text.fs");

#pragma region Other Cubes

	Cuadrado = new Primitive(mainCamera, secprogram);
	Cuadrado->setPosition(glm::vec3(1.0f, -1.0f, 0.0f));
	Cuadrado->setRotation(glm::vec3(0.0f, 30.0f, 0.0f));

#pragma endregion

	GLuint skyProgram = sLoader.createprogram("Assets/Shaders/Skybox.vs", "Assets/Shaders/Skybox.fs");

	std::vector<std::string> cubemapTex;
	cubemapTex.push_back((std::string)"Assets/skybox1/right.jpg");
	cubemapTex.push_back((std::string)"Assets/skybox1/left.jpg");
	cubemapTex.push_back((std::string)"Assets/skybox1/top.jpg");
	cubemapTex.push_back((std::string)"Assets/skybox1/bottom.jpg");
	cubemapTex.push_back((std::string)"Assets/skybox1/back.jpg");
	cubemapTex.push_back((std::string)"Assets/skybox1/front.jpg");

	SkyboxCube = new Skybox(Cube, mainCamera, cubemapTex, skyProgram);
#pragma region Terrain

#pragma endregion

	esfera = new CSphere(mainCamera, secprogram, 0.08);
	esfera->setPosition(glm::vec3(0, 0.0, 0));
	esfera->setScale(glm::vec3(1, 1, 1));

	Lluvia = new CRain(glm::vec3(0,0,0),4000,mainCamera,sextoprog);

}
int tempval = 2;

float ball_time = 0;
bool b_iftrue = false;

void update()
{
	ball_time++;

	ball_pos.z = glm::cos((ball_time*10.0f)/23);

	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float, std::milli> fp_ms = t1 - t2;
	DeltaTime = fp_ms.count();
	DeltaTime = DeltaTime * 1 / 1000.0f;
	t2 = t1;

	glutPostRedisplay();
	mainCamera->keyMoveCamera(keyState, DeltaTime);
	esfera->update(keyState, ball_pos);
	SkyboxCube->update(1.0f);

}

void render()
{
	//FRAMEBUFF->bindFbo();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//glStencilMask(0x00);
	SkyboxCube->render();
	//esfera->render();
	Lluvia->render(DeltaTime);
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
#pragma region Init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE | GLUT_STENCIL);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 800);

	glutCreateWindow("OpenGLFinal");
	glClearColor(0.0, 0.0, 0.0, 1.0);
#pragma endregion

	glewInit();
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glutSetOption(GLUT_MULTISAMPLE, 16);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_STENCIL_TEST);

	init();

	glutMouseFunc(mouse);

	glutPassiveMotionFunc(mousePassiveMove);
	glutDisplayFunc(render);
	glutReshapeFunc(handleResize);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);

	glutIdleFunc(update);
	glutMainLoop();

	return 0;
}