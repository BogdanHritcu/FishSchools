#pragma once

#include "../utils/utils.h"
#include <vector>
#include <GL/freeglut.h>

class Boid
{
public:
	Vec2f getPosition() const;
	Vec2f getVelocity() const;
	Vec2f getDirection() const;
	float getAngle() const;
	
	void setPosition(const Vec2f& v);
	void setVelocity(const Vec2f& v);

	void cohere(float cohesion, const std::vector<Boid*>& nearBoids);
	void separate(float separation, float minSeparationDistance, const std::vector<Boid*>& nearBoids);
	void align(float alignment, const std::vector<Boid*>& nearBoids);
	void constrainBounds(const Boundary2f& bounds, const Vec2f& boundaryRepel);
	void constrainSpeed(float maxSpeed);

	void update(float dt);

	void draw(GLuint modelList, const Vec2f& size) const;

private:
	Vec2f m_Position;
	Vec2f m_Velocity;
};

class BoidSystem
{
public:
	BoidSystem(size_t count, const Boundary2f& bounds);

	float* getCount();
	Vec2f* getBoidSize();
	float* getBoidCohesion();
	float* getBoidSeparation();
	float* getBoidAlignment();
	float* getBoidViewDistance();
	float* getBoidMinSeparationDistance();
	Boundary2f* getBoidBoundary();
	Vec2f* getBoidBoundaryRepel();
	float* getBoidMaxSpeed();
		 
	Vec4f* getBoidColor(const Vec4f& color);

	void setCount(size_t count);
	void setBoidSize(const Vec2f& v);
	void setBoidCohesion(float cohesion);
	void setBoidSeparation(float separation);
	void setBoidAlignment(float alignment);
	void setBoidViewDistance(float viewDistance);
	void setBoidMinSeparationDistance(float minDistance);
	void setBoidBoundary(const Boundary2f& bounds);
	void setBoidBoundaryRepel(const Vec2f& v);
	void setBoidMaxSpeed(float maxSpeed);

	void setBoidColor(const Vec4f& color);

	void findNearBoids(const Boid& boid);

	BoidSystemStats getStats() const;

	void update(float dt);

	void draw() const;

	static void setModelList(GLuint drawList);
	static void initModels();

private:
	std::vector<Boid> m_Boids;

	float m_Countf;
	Vec2f m_Size;

	float m_Cohesion;   //[0, 1]
	float m_Separation; //[0, 1]
	float m_Alignment;  //[0, 1]

	float m_ViewDistance;
	float m_MinSeparationDistance;
	float m_MaxSpeed;
	Vec2f m_BoundaryRepel;
	Boundary2f m_Boundary;

	Vec4f m_Color;
	static GLuint m_ModelList;

	std::vector<Boid*> m_NearBoids;
};