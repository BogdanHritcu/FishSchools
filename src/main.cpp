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

int WIDTH = 800;
int HEIGHT = 600;
Vec2f mousePosition;
float old_time;
float current_time;
float delta_time;

float cohesion;
float separation;
float alignment;
float count;

BoidSystem boidSystem(50, Boundary2f(0.0f, 0.0f, WIDTH, HEIGHT));
Slider cohSlider;
Slider sepSlider;
Slider aliSlider;
Slider countSlider;

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

	boidSystem.setBoidSize(Vec2f(15.0f, 5.0f));

	boidSystem.setBoidCohesion(0.5f);
	boidSystem.setBoidSeparation(0.5f);
	boidSystem.setBoidAlignment(0.5f);
	
	boidSystem.setBoidViewDistance(60.0f);
	boidSystem.setBoidMinSeparationDistance(15.0f);
	boidSystem.setBoidMaxSpeed(100.0f);
	boidSystem.setBoidBoundaryRepel(Vec2f(5.0f, 5.0f));

	boidSystem.setBoidColor(Vec4f(0.0f, 1.0f, 0.0f));
	
	////////////////////////////////////////////////////
	cohSlider.setPosition(Vec2f(10.0f, 10.0f));
	cohSlider.setSize(Vec2f(400.0f, 10.0f));
	cohSlider.setButtonDiameterPercent(1.5f);
	cohSlider.setPercent(0.5f);
	cohSlider.setRange(0.0f, 1.0f);
	cohSlider.setSliderColor(Vec4f(0.1f, 0.3f, 0.5f));
	cohSlider.setButtonColor(Vec4f(0.8f, 0.1f, 0.2f));
	cohSlider.setValueRef(&cohesion);

	sepSlider.setPosition(Vec2f(10.0f, 30.0f));
	sepSlider.setSize(Vec2f(400.0f, 10.0f));
	sepSlider.setButtonDiameterPercent(1.5f);
	sepSlider.setPercent(0.5f);
	sepSlider.setRange(0.0f, 1.0f);
	sepSlider.setSliderColor(Vec4f(0.1f, 0.3f, 0.5f));
	sepSlider.setButtonColor(Vec4f(0.8f, 0.1f, 0.2f));
	sepSlider.setValueRef(&separation);

	aliSlider.setPosition(Vec2f(10.0f, 50.0f));
	aliSlider.setSize(Vec2f(400.0f, 10.0f));
	aliSlider.setButtonDiameterPercent(1.5f);
	aliSlider.setPercent(0.5f);
	aliSlider.setRange(0.0f, 1.0f);
	aliSlider.setSliderColor(Vec4f(0.1f, 0.3f, 0.5f));
	aliSlider.setButtonColor(Vec4f(0.8f, 0.1f, 0.2f));
	aliSlider.setValueRef(&alignment);

	countSlider.setPosition(Vec2f(10.0f, 70.0f));
	countSlider.setSize(Vec2f(400.0f, 10.0f));
	countSlider.setButtonDiameterPercent(1.5f);
	countSlider.setPercent(0.1f);
	countSlider.setRange(10.0f, 3000.0f);
	countSlider.setSliderColor(Vec4f(0.1f, 0.3f, 0.5f));
	countSlider.setButtonColor(Vec4f(0.8f, 0.1f, 0.2f));
	countSlider.setValueRef(&count);
	////////////////////////////////////////////////////
	
	old_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
}


void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	boidSystem.draw();

	cohSlider.draw();
	sepSlider.draw();
	aliSlider.draw();
	countSlider.draw();

	glutSwapBuffers();
}

void idle()
{
	current_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	delta_time = current_time - old_time;
	old_time = current_time;

	printf("fps: %.2f\n", 1.0f / delta_time);
	boidSystem.update(delta_time);

	cohSlider.update(mousePosition);
	sepSlider.update(mousePosition);
	aliSlider.update(mousePosition);
	countSlider.update(mousePosition);

	boidSystem.setCount(static_cast<size_t>(count));
	boidSystem.setBoidCohesion(cohesion);
	boidSystem.setBoidSeparation(separation);
	boidSystem.setBoidAlignment(alignment);

	glutPostRedisplay();
}

void click_callback(int button, int state, int x, int y)
{
	cohSlider.check(state, mousePosition);
	sepSlider.check(state, mousePosition);
	aliSlider.check(state, mousePosition);
	countSlider.check(state, mousePosition);
}

void keyboard_callback(unsigned char key, int x, int y)
{

}

void mouse_position_callback(int x, int y)
{
	mousePosition = Vec2f((float)x, (float)y);
}

void resize_callback(int width, int height)
{
	WIDTH = width;
	HEIGHT = height;

	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, WIDTH, HEIGHT, 0.0, -1.0, 1.0);
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