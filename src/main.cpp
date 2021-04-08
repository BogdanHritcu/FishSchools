#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <cmath>
#include <iostream>

#include "utils/utils.h"
#include "entities/boid.h"
#include "interface/interface.h"

int WIDTH = 1200;
int HEIGHT = 700;
MouseStats mouseStats;

float old_time;
float current_time;
float delta_time;

float cohesion;
float separation;
float alignment;
float count;
std::string strCohesion;
std::string strSeparation;
std::string strAlignment;
std::string strCount;

BoidSystem boidSystem(50, Boundary2f(0.0f, 0.0f, WIDTH, HEIGHT));

UserInterface userInterface(&mouseStats);

void init()
{
	srand((unsigned int)time(nullptr));

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, WIDTH, HEIGHT, 0.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// models
	BoidSystem::initModels();
	Slider::initModels();
	TextBox::initModels();
	UserInterface::initModels();

	boidSystem.setBoidSize(Vec2f(15.0f, 5.0f));
	boidSystem.setBoidViewDistance(60.0f);
	boidSystem.setBoidMinSeparationDistance(15.0f);
	boidSystem.setBoidMaxSpeed(100.0f);
	boidSystem.setBoidBoundaryRepel(Vec2f(5.0f, 5.0f));

	boidSystem.setBoidColor(Vec4f(0.0f, 1.0f, 0.0f));

	//UI
	userInterface.setPosition(Vec2f(10.0f, 10.0f));
	userInterface.setPadding(Vec2f(10.0f, 10.0f));
	userInterface.setColor(Vec4f(0.4f, 0.1f, 0.7f));

	Slider* slider;
	slider = &userInterface.addSlider();
	slider->setPosition(Vec2f(0.0f, 0.0f));
	slider->setSize(Vec2f(400.0f, 10.0f));
	slider->setButtonDiameterPercent(1.5f);
	slider->setSliderColor(Vec4f(0.1f, 0.3f, 0.5f));
	slider->setButtonColor(Vec4f(0.8f, 0.1f, 0.2f));
	slider->setPercent(0.5f);
	slider->setRange(0.0f, 1.0f);
	slider->setValueRef(&cohesion);

	slider = &userInterface.addSlider();
	slider->setPosition(Vec2f(0.0f, 20.0f));
	slider->setSize(Vec2f(400.0f, 10.0f));
	slider->setButtonDiameterPercent(1.5f);
	slider->setSliderColor(Vec4f(0.1f, 0.3f, 0.5f));
	slider->setButtonColor(Vec4f(0.8f, 0.1f, 0.2f));
	slider->setPercent(0.5f);
	slider->setRange(0.0f, 1.0f);
	slider->setValueRef(&separation);

	slider = &userInterface.addSlider();
	slider->setPosition(Vec2f(0.0f, 40.0f));
	slider->setSize(Vec2f(400.0f, 10.0f));
	slider->setButtonDiameterPercent(1.5f);
	slider->setSliderColor(Vec4f(0.1f, 0.3f, 0.5f));
	slider->setButtonColor(Vec4f(0.8f, 0.1f, 0.2f));
	slider->setPercent(0.5f);
	slider->setRange(0.0f, 1.0f);
	slider->setValueRef(&alignment);

	slider = &userInterface.addSlider();
	slider->setPosition(Vec2f(0.0f, 60.0f));
	slider->setSize(Vec2f(400.0f, 10.0f));
	slider->setButtonDiameterPercent(1.5f);
	slider->setSliderColor(Vec4f(0.1f, 0.3f, 0.5f));
	slider->setButtonColor(Vec4f(0.8f, 0.1f, 0.2f));
	slider->setPercent(0.1f);
	slider->setRange(30.0f, 3000.0f);
	slider->setValueRef(&count);

	TextBox* textBox; 
	textBox = &userInterface.addTextBox();
	textBox->setPosition(Vec2f(420.0f, 0.0f));
	textBox->setSize(Vec2f(40.0f, 14.0f));
	textBox->setAutoSize(false);
	textBox->setPadding(Vec2f(6.0f, 6.0f));
	textBox->setBoxColor(Vec4f(0.1f, 0.3f, 0.5f));
	textBox->setTextColor(Vec4f(1.0f, 1.0f, 0.8f));
	textBox->setValueRef(&strCohesion);

	textBox = &userInterface.addTextBox();
	textBox->setPosition(Vec2f(420.0f, 20.0f));
	textBox->setSize(Vec2f(40.0f, 14.0f));
	textBox->setAutoSize(false);
	textBox->setPadding(Vec2f(6.0f, 6.0f));
	textBox->setBoxColor(Vec4f(0.1f, 0.3f, 0.5f));
	textBox->setTextColor(Vec4f(1.0f, 1.0f, 0.8f));
	textBox->setValueRef(&strSeparation);

	textBox = &userInterface.addTextBox();
	textBox->setPosition(Vec2f(420.0f, 40.0f));
	textBox->setSize(Vec2f(40.0f, 14.0f));
	textBox->setAutoSize(false);
	textBox->setPadding(Vec2f(6.0f, 6.0f));
	textBox->setBoxColor(Vec4f(0.1f, 0.3f, 0.5f));
	textBox->setTextColor(Vec4f(1.0f, 1.0f, 0.8f));
	textBox->setValueRef(&strAlignment);

	textBox = &userInterface.addTextBox();
	textBox->setPosition(Vec2f(420.0f, 60.0f));
	textBox->setSize(Vec2f(40.0f, 14.0f));
	textBox->setAutoSize(false);
	textBox->setPadding(Vec2f(6.0f, 6.0f));
	textBox->setBoxColor(Vec4f(0.1f, 0.3f, 0.5f));
	textBox->setTextColor(Vec4f(1.0f, 1.0f, 0.8f));
	textBox->setValueRef(&strCount);
	////////////////////////////////////////////////////
	
	old_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
}


void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	boidSystem.draw();

	userInterface.draw();

	glutSwapBuffers();
}

void idle()
{
	current_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	delta_time = current_time - old_time;
	old_time = current_time;

	//printf("fps: %.2f\n", 1.0f / delta_time);
	userInterface.update();

	boidSystem.setCount(static_cast<size_t>(count));
	boidSystem.setBoidCohesion(cohesion);
	boidSystem.setBoidSeparation(separation);
	boidSystem.setBoidAlignment(alignment);
	boidSystem.update(delta_time);

	strCohesion = std::to_string(cohesion);
	strCohesion.resize(5);
	strSeparation = std::to_string(separation);
	strSeparation.resize(5);
	strAlignment = std::to_string(alignment);
	strAlignment.resize(5);
	strCount = std::to_string(static_cast<int>(count));
	strCount.resize(5);

	glutPostRedisplay();
}

void click_callback(int button, int state, int x, int y)
{
	mouseStats.update(Vec2f(static_cast<float>(x), static_cast<float>(y)), button, state);

	userInterface.check();
}

void keyboard_callback(unsigned char key, int x, int y)
{

}

void mouse_position_callback(int x, int y)
{
	mouseStats.position = Vec2f(static_cast<float>(x), static_cast<float>(y));
}

void resize_callback(int width, int height)
{
	WIDTH = width;
	HEIGHT = height;

	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, WIDTH, HEIGHT, 0.0, -1.0, 1.0);

	boidSystem.setBoidBoundary(Boundary2f(0.0f, 0.0f, WIDTH, HEIGHT));
}

int main(int argc, char** argv)
{
	// init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Schools Of Fish");
	init();

	// mouse callbacks
	glutMouseFunc(click_callback);
	glutPassiveMotionFunc(mouse_position_callback);
	glutMotionFunc(mouse_position_callback);
	glutReshapeFunc(resize_callback);

	//keyboard callbacks
	glutKeyboardFunc(keyboard_callback);

	// draw function
	glutDisplayFunc(draw);

	//idle function
	glutIdleFunc(idle);

	// main loop
	glutMainLoop();

	return 0;
}