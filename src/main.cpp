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

float val;
BoidSystem boidSystem(300, Boundary2f(0.0f, 0.0f, WIDTH, HEIGHT));
Slider slider;

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

	boidSystem.setBoidCohesion(0.4f);
	boidSystem.setBoidSeparation(0.3f);
	boidSystem.setBoidAlignment(0.3f);
	
	boidSystem.setBoidViewDistance(60.0f);
	boidSystem.setBoidMinSeparationDistance(40.0f);
	boidSystem.setBoidMaxSpeed(100.0f);
	boidSystem.setBoidBoundaryRepel(Vec2f(5.0f, 5.0f));

	boidSystem.setBoidColor(Vec4f(0.0f, 1.0f, 0.0f));
	
	
	////////////////////////////////////////////////////
	slider.setPosition(Vec2f(20.0f, 20.0f));
	slider.setSize(Vec2f(400.0f, 10.0f));
	slider.setButtonDiameterPercent(1.5f);
	slider.setPercent(0.5f);
	slider.setSliderColor(Vec4f(0.1f, 0.3f, 0.5f));
	slider.setButtonColor(Vec4f(0.8f, 0.1f, 0.2f));

	////////////////////////////////////////////////////
	
	old_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
}


void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//drawText(Vec2f(0.0f, 0.0f), text);
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();

	//glTranslatef(WIDTH / 2.0f, HEIGHT / 2.0f, 0.0f);
	//glRotatef(90.0f, 0.0f, 0.0f, -1.0f);
	//glTranslatef(-WIDTH / 2.0f, -HEIGHT / 2.0f, 0.0f);
	//glBegin(GL_LINES);
	//glColorVec4f(Vec4f(0.0f, 1.0f, 0.0f));
	//glVertexVec2f(Vec2f(WIDTH / 2.0f, HEIGHT / 2.0f));
	//glVertexVec2f(Vec2f(WIDTH, HEIGHT / 2.0f));
	//glEnd();
	//glPopMatrix();

	//glBegin(GL_LINES);
	//glColorVec4f(Vec4f(0.0f, 0.0f, 1.0f));
	//glVertexVec2f(Vec2f(WIDTH / 2.0f, HEIGHT / 2.0f));
	//glVertexVec2f(Vec2f(WIDTH, HEIGHT / 2.0f));
	//glEnd();

	boidSystem.draw();

	slider.draw();

	glutSwapBuffers();
}

void idle()
{
	current_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	delta_time = current_time - old_time;
	old_time = current_time;

	printf("fps: %.2f\n", 1.0f / delta_time);
	boidSystem.update(delta_time);

	slider.update(mousePosition);

	glutPostRedisplay();
}

void click_callback(int button, int state, int x, int y)
{
	slider.check(state, mousePosition);
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