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

std::vector<BoidGroup> boidGroups;

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
	BoidGroup::initModels();
	Slider::initModels();
	TextBox::initModels();
	UserInterface::initModels();

	BoidGroup boidGroup(50, Boundary2f(0.0f, 0.0f, WIDTH, HEIGHT));
	boidGroup.setBoidSize(Vec2f(15.0f, 5.0f));
	boidGroup.setBoidViewDistance(60.0f);
	boidGroup.setBoidMinSeparationDistance(15.0f);
	boidGroup.setBoidMaxSpeed(100.0f);
	boidGroup.setBoidBoundaryRepel(Vec2f(5.0f, 5.0f));
	boidGroup.setBoidColor(Vec4f(0.0f, 1.0f, 0.0f));

	boidGroups.push_back(boidGroup);

	boidGroup.setBoidColor(Vec4f(0.0f, 0.0f, 1.0f));

	boidGroups.push_back(boidGroup);

	//UI
	userInterface.setPosition(Vec2f(10.0f, 10.0f));
	userInterface.setPadding(Vec2f(10.0f, 10.0f));
	userInterface.setColor(Vec4f(0.4f, 0.1f, 0.7f));

	Slider* slider;
	const size_t sliderCount = 4;
	Vec2f sliderPosition(0.0f, 0.0f);
	Vec2f sliderOff(0.0f, 20.0f);
	Vec2f sliderRanges[sliderCount] =
	{
		Vec2f(0.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(20.0f, 3000.0f),
	};
	float sliderPercents[sliderCount] =
	{
		0.5f,
		0.5f,
		0.5f,
		0.05f
	};

	TextBox* textBox;
	Vec2f textBoxPosition(420.0f, 0.0f);
	Vec2f textBoxOff(0.0f, 20.0f);

	for (size_t i = 0; i < sliderCount; i++)
	{
		// sliders
		slider = &userInterface.addSlider();
		slider->setPosition(sliderPosition + sliderOff * i);
		slider->setPercent(sliderPercents[i]);
		slider->setRange(sliderRanges[i].x, sliderRanges[i].y);

		slider->setSliderColor(Vec4f(0.1f, 0.3f, 0.5f));
		slider->setButtonColor(Vec4f(0.8f, 0.1f, 0.2f));
		slider->setSize(Vec2f(400.0f, 10.0f));
		slider->setButtonDiameterPercent(1.5f);

		// value text boxes
		textBox = &userInterface.addTextBox();
		textBox->setPosition(textBoxPosition + textBoxOff * i);

		textBox->setSize(Vec2f(40.0f, 14.0f));
		textBox->setAutoSize(false);
		textBox->setPadding(Vec2f(6.0f, 6.0f));
		textBox->setBoxColor(Vec4f(0.1f, 0.3f, 0.5f));
		textBox->setTextColor(Vec4f(1.0f, 1.0f, 0.8f));
	}

	userInterface.setBoidGroupStats(&boidGroups[0]);
	////////////////////////////////////////////////////
	
	old_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
}


void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (size_t i = 0; i < boidGroups.size(); i++)
	{
		boidGroups[i].draw();
	}

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

	for (size_t i = 0; i < boidGroups.size(); i++)
	{
		boidGroups[i].update(delta_time);
	}

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

	for (size_t i = 0; i < boidGroups.size(); i++)
	{
		boidGroups[i].setBoidBoundary(Boundary2f(0.0f, 0.0f, WIDTH, HEIGHT));
	}
	
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