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

void Boid::cohere(float cohesion, const std::vector<Boid*>& nearBoids, float friendliness)
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

	m_Velocity = m_Velocity + cohDir * cohesion * friendliness;
}

void Boid::separate(float separation, float minSeparationDistance, const std::vector<Boid*>& nearBoids, float friendliness)
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

	m_Velocity = m_Velocity + sepDir * separation * std::min(1.0f / (friendliness + 0.000001f), 2.0f);
}

void Boid::align(float alignment, const std::vector<Boid*>& nearBoids, float friendliness)
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

	m_Velocity = m_Velocity + aliDir * alignment * friendliness;
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

BoidGroup::BoidGroup()
{
	m_Size = Vec2f(1.0f, 1.0f);

	m_Cohesion = 0.2f;
	m_Separation = 0.5f;
	m_Alignment = 0.3f;

	m_ViewDistance = 10.0f;
	m_MinSeparationDistance = 40.0f;
	m_MaxSpeed = 60.0f;

	m_Color = Vec4f(0.1f, 0.8f, 0.3f);

	setCount(30, Boundary2f(Vec2f(0.0f, 0.0f), Vec2f(800.0f, 600.0f)));
}

BoidGroup::BoidGroup(size_t count, const Boundary2f& boundary)
{
	m_Size = Vec2f(1.0f, 1.0f);

	m_Cohesion = 0.2f;
	m_Separation = 0.5f;
	m_Alignment = 0.3f;

	m_ViewDistance = 10.0f;
	m_MinSeparationDistance = 40.0f;
	m_MaxSpeed = 60.0f;

	m_Color = Vec4f(0.1f, 0.8f, 0.3f);

	setCount(count, boundary);
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

std::vector<Boid>& BoidGroup::getBoids()
{
	return m_Boids;
}

void BoidGroup::setCount(size_t count, const Boundary2f& boundary)
{	
	m_Countf = static_cast<float>(count);
	size_t oldCount = m_Boids.size();
	m_Boids.resize(count);

	for (size_t i = oldCount; i < m_Boids.size(); i++)
	{
		m_Boids[i].setPosition(Vec2f(rand_float() * boundary.max.x, rand_float() * boundary.max.y));
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


void BoidGroup::setBoidMaxSpeed(float maxSpeed)
{
	m_MaxSpeed = maxSpeed;
}

void BoidGroup::setBoidColor(const Vec4f& color)
{
	m_Color = color;
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

	stats.color = m_Color;
	stats.modelList = m_ModelList;

	stats.count = m_Boids.size();
	return stats;
}

void BoidGroup::update(float dt, std::vector<Boid*> & nearFriendlyBoids, std::vector<Boid*>& nearStrangerBoids, 
	Boundary2f &boundary, Vec2f& boundaryRepel)
{	
	setCount(static_cast<size_t>(m_Countf), boundary);

	for (size_t i = 0; i < m_Boids.size(); i++)
	{
		BoidSystem::findNearBoids(m_Boids[i]);

		m_Boids[i].cohere(m_Cohesion, nearFriendlyBoids, 1.0f);
		m_Boids[i].cohere(m_Cohesion, nearStrangerBoids, m_Friendliness);

		m_Boids[i].separate(m_Separation, m_MinSeparationDistance, nearFriendlyBoids, 1.0f);
		m_Boids[i].separate(m_Separation, m_MinSeparationDistance, nearStrangerBoids, m_Friendliness);

		m_Boids[i].align(m_Alignment, nearFriendlyBoids, 1.0f);
		m_Boids[i].align(m_Alignment, nearStrangerBoids, m_Friendliness);


		m_Boids[i].constrainBounds(boundary, boundaryRepel);
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

std::vector <Boid*> BoidSystem::m_NearFriendlyBoids;
std::vector <Boid*> BoidSystem::m_NearStrangerBoids;

float* BoidSystem::getCount()
{
	return &m_Countf;
}

float* BoidSystem::getGroupCohesion()
{
	return &m_Cohesion;
}

float* BoidSystem::getGroupSeparation()
{
	return &m_Separation;
}

float* BoidSystem::getGroupAlignment()
{
	return &m_Alignment;
}

float* BoidSystem::getGroupViewDistance()
{
	return &m_ViewDistance;
}

float* BoidSystem::getGroupMinSeparationDistance()
{
	return &m_MinSeparationDistance;
}

Boundary2f* BoidSystem::getBoidBoundary()
{
	return &m_Boundary;
}

Vec2f* BoidSystem::getBoidBoundaryRepel()
{
	return &m_BoundaryRepel;
}

void BoidSystem::setCount(size_t count)
{
	m_Countf = static_cast<float>(count);
	size_t oldCount = m_BoidGroups.size();
	m_BoidGroups.resize(count);
	m_NearFriendlyBoids.reserve(300);
	m_NearFriendlyBoids.clear();
	m_NearStrangerBoids.reserve(300);
	m_NearStrangerBoids.clear();

	for (size_t i = oldCount; i < m_BoidGroups.size(); i++)
	{
		m_BoidGroups[i].setCount(rand_int(30, 100), m_Boundary);
	}
}

void BoidSystem::setGroupCohesion(float cohesion)
{
	m_Cohesion = cohesion;
}

void BoidSystem::setGroupSeparation(float separation)
{
	m_Separation = separation;
}

void BoidSystem::setGroupAlignment(float alignment)
{
	m_Alignment = alignment;
}

void BoidSystem::setGroupViewDistance(float viewDistance)
{
	m_ViewDistance = viewDistance;
}

void BoidSystem::setGroupMinSeparationDistance(float minDistance)
{
	m_MinSeparationDistance = minDistance;
}

void BoidSystem::update(float dt)
{
	for (size_t i = 0; i < m_BoidGroups.size(); i++)
	{
		m_BoidGroups[i].update(dt, m_NearFriendlyBoids, m_NearStrangerBoids, m_Boundary, m_BoundaryRepel);
	}
}

void BoidSystem::draw() const
{
	for (size_t i = 0; i < m_BoidGroups.size(); i++) 
	{
		m_BoidGroups[i].draw();
	}
}

void BoidSystem::setBoidBoundaryRepel(const Vec2f& v)
{
	m_BoundaryRepel = v;
}

void BoidSystem::setBoidBoundary(const Boundary2f& bounds)
{
	m_Boundary = bounds;
}

void BoidSystem::findNearBoids(const Boid& boid)
{
	m_NearFriendlyBoids.clear();
	m_NearStrangerBoids.clear();

	bool found = false;
	std::vector<Boid*> boidAux;
	boidAux.reserve(300);

	for (size_t i = 0; i < m_BoidGroups.size(); i++)
	{
		std::vector <Boid>& boids = m_BoidGroups[i].getBoids();
		for (size_t j = 0; j < boids.size(); i++)
		{
			if (&boid == &boids[j])
			{	
				found = true;
			}
			else
			{
				float distance2 = Vec2f::length2(boid.getPosition() - boids[j].getPosition());

				if (distance2 <= (*m_BoidGroups[i].getBoidViewDistance()) * (*m_BoidGroups[i].getBoidViewDistance()))
				{
					boidAux.push_back(&boids[j]);
				}
			}
		}
		if (found)
		{	
			m_NearFriendlyBoids = boidAux;
			found = false;
		}
		else
		{
			for (size_t j = 0; j < boidAux.size(); j++)
			{
				m_NearStrangerBoids.push_back(boidAux[j]);
			}
		}
		boidAux.clear();
	}
}
