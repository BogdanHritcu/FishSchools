#include "boid.h"
#include <algorithm>

/************************************************************************************************************
*												Boid
*************************************************************************************************************/

Vec2f Boid::getDirection() const
{
	return Vec2f::normalize(m_Velocity);
}

float Boid::getAngle() const
{
	return Vec2f::angleDeg(m_Velocity, Vec2f(1.0f, 0.0f));
}

Vec2f Boid::getPosition() const
{
	return m_Position;
}

Vec2f Boid::getVelocity() const
{
	return m_Velocity;
}

void Boid::setPosition(const Vec2f& v)
{
	m_Position = v;
}

void Boid::setVelocity(const Vec2f& v)
{
	m_Velocity = v;
}

void Boid::cohere(float cohesion, const std::vector<Boid*>& nearBoids)
{
	if (nearBoids.empty())
	{
		return;
	}

	Vec2f averagePosition(0.0f, 0.0f);

	for (size_t i = 0; i < nearBoids.size(); i++)
	{
		averagePosition = averagePosition + nearBoids[i]->getPosition();
	}

	averagePosition = averagePosition / static_cast<float>(nearBoids.size());
	Vec2f cohDir = averagePosition - m_Position;

	m_Velocity = m_Velocity + cohDir * cohesion;
}

void Boid::separate(float separation, float minSeparationDistance, const std::vector<Boid*>& nearBoids)
{
	if (nearBoids.empty())
	{
		return;
	}

	Vec2f sepDir(0.0f, 0.0f);

	for (size_t i = 0; i < nearBoids.size(); i++)
	{
		Vec2f v = m_Position - nearBoids[i]->getPosition();

		sepDir = sepDir + v / (Vec2f::length2(v) / (minSeparationDistance * minSeparationDistance));
	}

	m_Velocity = m_Velocity + sepDir * separation;
}

void Boid::align(float alignment, const std::vector<Boid*>& nearBoids)
{
	if (nearBoids.empty())
	{
		return;
	}

	Vec2f aliDir(0.0f, 0.0f);

	for (size_t i = 0; i < nearBoids.size(); i++)
	{
		aliDir = aliDir + nearBoids[i]->getVelocity();
	}
	
	aliDir = aliDir / static_cast<float>(nearBoids.size());
	Vec2f v = aliDir - m_Velocity;

	m_Velocity = m_Velocity + aliDir * alignment;
}

void Boid::constrainBounds(const Boundary2f& bounds, const Vec2f& boundaryRepel)
{
	if (m_Position.x < bounds.min.x)
	{
		m_Velocity.x += boundaryRepel.x;
	}
	else if (m_Position.x > bounds.max.x)
	{
		m_Velocity.x -= boundaryRepel.x;
	}

	if (m_Position.y < bounds.min.y)
	{
		m_Velocity.y += boundaryRepel.y;
	}
	else if (m_Position.y > bounds.max.y)
	{
		m_Velocity.y -= boundaryRepel.y;
	}
}

void Boid::constrainSpeed(float maxSpeed)
{
	if (Vec2f::length2(m_Velocity) >= maxSpeed * maxSpeed)
	{
		m_Velocity = Vec2f::normalize(m_Velocity) * maxSpeed;
	}
}

void Boid::update(float dt)
{
	m_Position = m_Position + m_Velocity * dt;
}

void Boid::draw(GLuint modelList, const Vec2f& size) const
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(m_Position.x, m_Position.y, 0.0f);
	glRotatef(getAngle(), 0.0f, 0.0f, -1.0f);
	glScalef(size.x, size.y, 1.0f);

	glCallList(modelList);

	glPopMatrix();
}

/************************************************************************************************************
*											BoidGroup
*************************************************************************************************************/

GLuint BoidGroup::m_ModelList = 0;

BoidGroup::BoidGroup(size_t count, const Boundary2f& bounds)
{
	m_Size = Vec2f(1.0f, 1.0f);

	m_Cohesion = 0.2f;
	m_Separation = 0.5f;
	m_Alignment = 0.3f;

	m_ViewDistance = 10.0f;
	m_MinSeparationDistance = 40.0f;
	m_MaxSpeed = 60.0f;
	m_BoundaryRepel = Vec2f(10.0f, 10.0f);
	m_Boundary = bounds;

	m_Color = Vec4f(0.1f, 0.8f, 0.3f);

	setCount(count);
}

float* BoidGroup::getCount()
{
	return &m_Countf;
}

Vec2f* BoidGroup::getBoidSize()
{
	return &m_Size;
}

float* BoidGroup::getBoidCohesion()
{
	return &m_Cohesion;
}

float* BoidGroup::getBoidSeparation()
{
	return &m_Separation;
}

float* BoidGroup::getBoidAlignment()
{
	return &m_Alignment;
}

float* BoidGroup::getBoidViewDistance()
{
	return &m_ViewDistance;
}

float* BoidGroup::getBoidMinSeparationDistance()
{
	return &m_MinSeparationDistance;
}

Boundary2f* BoidGroup::getBoidBoundary()
{
	return &m_Boundary;
}

Vec2f* BoidGroup::getBoidBoundaryRepel()
{
	return &m_BoundaryRepel;
}

float* BoidGroup::getBoidMaxSpeed()
{
	return &m_MaxSpeed;
}

Vec4f* BoidGroup::getBoidColor(const Vec4f& color)
{
	return &m_Color;
}

Vec2f BoidGroup::getAveragePosition() const
{
	Vec2f averagePosition(0.0f, 0.0f);

	for (size_t i = 0; i < m_Boids.size(); i++)
	{
		averagePosition = averagePosition + m_Boids[i].getPosition();
	}

	if (!m_Boids.empty())
	{
		averagePosition = averagePosition / static_cast<float>(m_Boids.size());
	}

	return averagePosition;
}

Vec2f BoidGroup::getAverageVelocity() const
{
	Vec2f averageVelocity(0.0f, 0.0f);

	for (size_t i = 0; i < m_Boids.size(); i++)
	{
		averageVelocity = averageVelocity + m_Boids[i].getVelocity();
	}

	if (!m_Boids.empty())
	{
		averageVelocity = averageVelocity / static_cast<float>(m_Boids.size());
	}

	return averageVelocity;
}

void BoidGroup::setCount(size_t count)
{	
	m_Countf = static_cast<float>(count);
	size_t oldCount = m_Boids.size();
	m_Boids.resize(count);
	m_NearBoids.reserve(count);
	m_NearBoids.clear();

	for (size_t i = oldCount; i < m_Boids.size(); i++)
	{
		m_Boids[i].setPosition(Vec2f(rand_float() * m_Boundary.max.x, rand_float() * m_Boundary.max.y));
		m_Boids[i].setVelocity(rand_direction() * (rand_float(0.5f, 1.0f) * m_MaxSpeed));
	}
}

void BoidGroup::setBoidSize(const Vec2f& v)
{
	m_Size = v;
}

void BoidGroup::setBoidViewDistance(float viewDistance)
{
	m_ViewDistance = viewDistance;
}

void BoidGroup::setBoidMinSeparationDistance(float minDistance)
{
	m_MinSeparationDistance = minDistance;
}

void BoidGroup::setBoidCohesion(float cohesion)
{
	m_Cohesion = cohesion;
}

void BoidGroup::setBoidSeparation(float separation)
{
	m_Separation = separation;
}

void BoidGroup::setBoidAlignment(float alignment)
{
	m_Alignment = alignment;
}

void BoidGroup::setBoidBoundaryRepel(const Vec2f& v)
{
	m_BoundaryRepel = v;
}

void BoidGroup::setBoidBoundary(const Boundary2f& bounds)
{
	m_Boundary = bounds;
}

void BoidGroup::setBoidMaxSpeed(float maxSpeed)
{
	m_MaxSpeed = maxSpeed;
}

void BoidGroup::setBoidColor(const Vec4f& color)
{
	m_Color = color;
}

void BoidGroup::findNearBoids(const Boid& boid)
{
	m_NearBoids.clear();

	for (size_t i = 0; i < m_Boids.size(); i++)
	{
		if (&boid != &m_Boids[i])
		{
			float distance2 = Vec2f::length2(boid.getPosition() - m_Boids[i].getPosition());

			if (distance2 <= m_ViewDistance * m_ViewDistance)
			{
				m_NearBoids.push_back(const_cast<Boid*>(&m_Boids[i]));
			}
		}
	}
}

BoidGroupStats BoidGroup::getStats() const
{	
	BoidGroupStats stats;

	stats.boidSize = m_Size;
	stats.cohesion = m_Cohesion;   //[0, 1]
	stats.separation = m_Separation; //[0, 1]
	stats.alignment = m_Alignment;  //[0, 1]
	
	stats.viewDistance = m_ViewDistance;
	stats.minSeparationDistance = m_MinSeparationDistance;
	stats.maxSpeed = m_MaxSpeed;
	stats.boundaryRepel = m_BoundaryRepel;
	stats.boundary = m_Boundary;

	stats.color = m_Color;
	stats.modelList = m_ModelList;

	stats.count = m_Boids.size();
	return stats;
}

void BoidGroup::update(float dt)
{	
	setCount(static_cast<size_t>(m_Countf));

	for (size_t i = 0; i < m_Boids.size(); i++)
	{
		findNearBoids(m_Boids[i]);

		m_Boids[i].cohere(m_Cohesion, m_NearBoids);
		m_Boids[i].separate(m_Separation, m_MinSeparationDistance, m_NearBoids);
		m_Boids[i].align(m_Alignment, m_NearBoids);
		m_Boids[i].constrainBounds(m_Boundary, m_BoundaryRepel);
		m_Boids[i].constrainSpeed(m_MaxSpeed);

		m_Boids[i].update(dt);
	}
}

void BoidGroup::draw() const
{
	glColorVec4f(m_Color);
	
	for (size_t i = 0; i < m_Boids.size(); i++)
	{
		m_Boids[i].draw(m_ModelList, m_Size);
	}
}

void BoidGroup::setModelList(GLuint modelList)
{
	m_ModelList = modelList;
}

void BoidGroup::initModels()
{
	m_ModelList = glGenLists(1);

	glNewList(m_ModelList, GL_COMPILE);

	glBegin(GL_TRIANGLES);
	glVertexVec2f(Vec2f(0.66f, 0.0f));
	glVertexVec2f(Vec2f(-0.33f, 0.94f));
	glVertexVec2f(Vec2f(-0.33f, -0.94f));
	glEnd();
	glEndList();
}
