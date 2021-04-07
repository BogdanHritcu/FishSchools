#include "interface.h"

GLuint Slider::m_SliderList = 0;
GLuint Slider::m_ButtonList = 0;

Slider::Slider()
{
	m_Min = 0.0f;
	m_Max = 1.0f;
	m_Percent = 0.5f;
	m_ButtonRadiusPercent = 1.1f;

	m_ButtonGrabbed = false;

	m_ValuePtr = nullptr;
}

void Slider::setRange(float min, float max)
{
	m_Min = min;
	m_Max = max;

	resetValue();
}

void Slider::setPercent(float percent)
{
	m_Percent = percent;

	resetValue();
}

void Slider::setPosition(const Vec2f& position)
{
	m_Position = position;
}

void Slider::setSize(const Vec2f& size)
{
	m_Size = size;
}

void Slider::setButtonDiameterPercent(float percent)
{
	m_ButtonRadiusPercent = percent / 2.0f;
}

void Slider::setSliderColor(const Vec4f& color)
{
	m_SliderColor = color;
}

void Slider::setButtonColor(const Vec4f& color)
{
	m_ButtonColor = color;
}

void Slider::setValueRef(float* valuePtr)
{
	m_ValuePtr = valuePtr;

	resetValue();
}

void Slider::check(int state, const Vec2f& mousePosition)
{
	CircleBoundary2f button(m_Position + Vec2f(m_Percent * m_Size.x, 0.5f * m_Size.y), m_Size.y * m_ButtonRadiusPercent);

	if (!m_ButtonGrabbed && state == GLUT_DOWN)
	{
		if (button.contains(mousePosition))
		{
			m_ButtonGrabbed = true;
		}
	}

	if (m_ButtonGrabbed && state == GLUT_UP)
	{
		m_ButtonGrabbed = false;
	}
}

void Slider::update(const Vec2f& mousePosition)
{
	if (!m_ButtonGrabbed)
	{
		return;
	}

	if (mousePosition.x <= m_Position.x)
	{
		m_Percent = 0.0f;
	}
	else if (mousePosition.x >= m_Position.x + m_Size.x)
	{
		m_Percent = 1.0f;
	}
	else
	{
		m_Percent = (mousePosition.x - m_Position.x) / m_Size.x;
	}

	resetValue();
}

void Slider::draw()
{
	Vec2f buttonPosition = m_Position + Vec2f(m_Percent * m_Size.x, 0.5f * m_Size.y);
	float radius = m_Size.y * m_ButtonRadiusPercent;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.0f);
	glScalef(m_Size.x, m_Size.y, 1.0f);
	glColorVec4f(m_SliderColor);
	glCallList(m_SliderList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(buttonPosition.x, buttonPosition.y, 0.0f);
	glScalef(radius, radius, 1.0f);
	glColorVec4f(m_ButtonColor);
	glCallList(m_ButtonList);
	glPopMatrix();
}

void Slider::setDrawLists(GLuint sliderList, GLuint buttonList)
{
	m_SliderList = sliderList;
	m_ButtonList = m_ButtonList;
}

void Slider::initModels()
{
	m_SliderList = glGenLists(1);

	glNewList(m_SliderList, GL_COMPILE);
	glBegin(GL_QUADS);
	glVertexVec2f(Vec2f(0.0f, 0.0f));
	glVertexVec2f(Vec2f(1.0f, 0.0f));
	glVertexVec2f(Vec2f(1.0f, 1.0f));
	glVertexVec2f(Vec2f(0.0f, 1.0f));
	glEnd();
	glEndList();

	m_ButtonList = glGenLists(1);

	const size_t pointCount = 30;
	glNewList(m_ButtonList, GL_COMPILE);
	glBegin(GL_POLYGON);
	for (size_t i = 0; i < pointCount; i++)
	{
		float angle = 2 * M_PI * i / pointCount;
		glVertex2f(cosf(angle), sinf(angle));
	}
	glEnd();
	glEndList();
}

void Slider::resetValue()
{
	if (m_ValuePtr)
	{
		*m_ValuePtr = m_Min + (m_Max - m_Min) * m_Percent;
	}
}
