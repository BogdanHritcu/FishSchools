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
	m_Friendliness = 0.2f;

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
	m_Friendliness = 0.2f;

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

float* BoidGroup::getFriendliness()
{
	return &m_Friendliness;
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

void BoidGroup::setBoidFriendliness(float friendliness)
{
	m_Friendliness = friendliness;
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

	stats.count = m_Boids.size();
	return stats;
}

void BoidGroup::update(float dt, BoidSystem& boidSystem)
{	
	setCount(static_cast<size_t>(m_Countf), *boidSystem.getBoidBoundary());

	for (size_t i = 0; i < m_Boids.size(); i++)
	{
		BoidSystem::findNearBoids(m_Boids[i], boidSystem);

		m_Boids[i].cohere(m_Cohesion, BoidSystem::getNearFriendlyBoids(), 1.0f);
		m_Boids[i].cohere(m_Cohesion, BoidSystem::getNearStrangerBoids(), m_Friendliness);

		m_Boids[i].separate(m_Separation, m_MinSeparationDistance, BoidSystem::getNearFriendlyBoids(), 1.0f);
		m_Boids[i].separate(m_Separation, m_MinSeparationDistance, BoidSystem::getNearStrangerBoids(), m_Friendliness);

		m_Boids[i].align(m_Alignment, BoidSystem::getNearFriendlyBoids(), 1.0f);
		m_Boids[i].align(m_Alignment, BoidSystem::getNearStrangerBoids(), m_Friendliness);


		m_Boids[i].constrainBounds(*boidSystem.getBoidBoundary(), *boidSystem.getBoidBoundaryRepel());
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

BoidSystem::BoidSystem()
{
	m_BoundaryRepel = Vec2f(15.0f, 15.0f);

	setCount(0);
}

BoidSystem::BoidSystem(size_t count, const Boundary2f& boundary)
{
	m_BoundaryRepel = Vec2f(15.0f, 15.0f);
	m_Boundary = boundary;

	setCount(count);
}

float* BoidSystem::getCount()
{
	return &m_Countf;
}

Boundary2f* BoidSystem::getBoidBoundary()
{
	return &m_Boundary;
}

Vec2f* BoidSystem::getBoidBoundaryRepel()
{
	return &m_BoundaryRepel;
}

BoidGroup& BoidSystem::getGroup(size_t index)
{
	return m_BoidGroups[index];
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

void BoidSystem::update(float dt)
{
	for (size_t i = 0; i < m_BoidGroups.size(); i++)
	{
		m_BoidGroups[i].update(dt, *this);
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

BoidGroup& BoidSystem::addGroup()
{
	setCount(m_BoidGroups.size() + 1);

	return m_BoidGroups.back();
}

BoidGroup& BoidSystem::addGroup(size_t count)
{
	m_Countf += 1.0f;

	m_BoidGroups.push_back(BoidGroup(count, m_Boundary));

	return m_BoidGroups.back();
}

void BoidSystem::setBoidBoundary(const Boundary2f& bounds)
{
	m_Boundary = bounds;
}

void BoidSystem::findNearBoids(const Boid& boid, BoidSystem& boidSystem)
{
	m_NearFriendlyBoids.clear();
	m_NearStrangerBoids.clear();

	bool found = false;
	std::vector<Boid*> boidAux;
	boidAux.reserve(300);

	for (size_t i = 0; i < boidSystem.m_BoidGroups.size(); i++)
	{
		std::vector <Boid>& boids = boidSystem.m_BoidGroups[i].getBoids();
		for (size_t j = 0; j < boids.size(); j++)
		{
			if (&boid == &boids[j])
			{	
				found = true;
			}
			else
			{
				float distance2 = Vec2f::length2(boid.getPosition() - boids[j].getPosition());
				float viewDistance = *boidSystem.m_BoidGroups[i].getBoidViewDistance();
				if (distance2 <= viewDistance * viewDistance)
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

std::vector<Boid*>& BoidSystem::getNearFriendlyBoids()
{
	return m_NearFriendlyBoids;
}

std::vector<Boid*>& BoidSystem::getNearStrangerBoids()
{
	return m_NearStrangerBoids;
}

std::vector<BoidGroup>& BoidSystem::getGroups()
{
	return m_BoidGroups;
}