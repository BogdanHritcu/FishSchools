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

void Slider::setPercentFromValue(float value)
{
	if (value < m_Min)
	{
		m_Percent = 0.0f;
		return;
	}

	if (value > m_Max)
	{
		m_Percent = 1.0f;
		return;
	}

	m_Percent = (value - m_Min) / (m_Max - m_Min);
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

void Slider::check(const Vec2f& mousePosition, int state)
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

GLuint TextBox::m_BoxList = 0;

TextBox::TextBox()
{
	m_TextPtr = nullptr;
	m_Font = GLUT_BITMAP_8_BY_13;
	m_AutoSize = true;
}

void TextBox::setPosition(const Vec2f& position)
{
	m_Position = position;
}

void TextBox::setSize(const Vec2f& size)
{
	m_Size = size;	
}

void TextBox::setPadding(const Vec2f& padding)
{
	m_Padding = padding;
}

void TextBox::setTextColor(const Vec4f& color)
{
	m_TextColor = color;
}

void TextBox::setBoxColor(const Vec4f& color)
{
	m_BoxColor = color;
}

void TextBox::setFont(void* font)
{
	m_Font = font;
}

void TextBox::setAutoSize(bool value)
{
	m_AutoSize = value;
}

void TextBox::setValueRef(std::string* textPtr)
{
	m_TextPtr = textPtr;
	m_FromValue = false;
}

void TextBox::setValueRef(float* valuePtr)
{
	m_ValuePtr = valuePtr;
	m_FromValue = true;
}

void TextBox::useFloat(bool value)
{
	m_FromValue = value;
}

void TextBox::update()
{	
	if (m_FromValue)
	{
		if (!m_ValuePtr)
		{
			return;
		}
		m_Text = std::to_string(*m_ValuePtr);
	}

	else
	{
		if (!m_TextPtr)
		{
			return;
		}
		m_Text = *m_TextPtr;
	}

	if (!m_AutoSize)
	{
		return;
	}

	size_t lines = 1;
	for (size_t i = 0; i < m_Text.size(); i++)
	{
		if (m_Text[i] == '\n')
		{
			lines++;
		}
	}

	m_Size = Vec2f(static_cast<float>(glutBitmapLength(m_Font, reinterpret_cast<const unsigned char*>(m_Text.c_str()))),
		static_cast<float>(glutBitmapHeight(m_Font) * lines));
}

void TextBox::draw()
{
	float h = static_cast<float>(glutBitmapHeight(m_Font));
	Vec2f off(0.0f, h / 2.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.0f);
	glPushMatrix();
	glScalef(m_Size.x + m_Padding.x * 2.0f - off.x, m_Size.y + m_Padding.y * 2.0f - off.y, 1.0f);
	glColorVec4f(m_BoxColor);
	glCallList(m_BoxList);
	glPopMatrix();

	drawText(m_Padding - off, m_Text, m_TextColor, m_Font);
	
	glPopMatrix();
}

void TextBox::setDrawLists(GLuint boxList)
{
	m_BoxList = boxList;
}

void TextBox::initModels()
{
	m_BoxList = glGenLists(1);

	glNewList(m_BoxList, GL_COMPILE);
	glBegin(GL_QUADS);
	glVertexVec2f(Vec2f(0.0f, 0.0f));
	glVertexVec2f(Vec2f(1.0f, 0.0f));
	glVertexVec2f(Vec2f(1.0f, 1.0f));
	glVertexVec2f(Vec2f(0.0f, 1.0f));
	glEnd();
	glEndList();
}

GLuint UserInterface::m_PanelList = 0;

UserInterface::UserInterface()
{
	m_MouseStatsPtr = nullptr;
	m_ShouldResize = false;
}

UserInterface::UserInterface(MouseStats* mouseStatsPtr)
{
	m_MouseStatsPtr = mouseStatsPtr;
	m_ShouldResize = false;
}

Vec2f UserInterface::getSize() const
{
	return m_Size;
}

Vec2f UserInterface::getPadding() const
{
	return m_Padding;
}

Slider& UserInterface::getSlider(size_t idx)
{
	m_ShouldResize = true;

	return m_Sliders[idx];
}

void UserInterface::setPosition(const Vec2f& position)
{
	m_Position = position;
}

void UserInterface::setPadding(const Vec2f& padding)
{
	m_Padding = padding;
}

void UserInterface::setColor(const Vec4f& color)
{
	m_Color = color;
}

Slider& UserInterface::addSlider()
{
	m_Sliders.push_back(Slider());
	m_ShouldResize = true;

	return m_Sliders.back();
}

TextBox& UserInterface::addTextBox()
{
	m_TextBoxes.push_back(TextBox());
	m_ShouldResize = true;

	return m_TextBoxes.back();
}

void UserInterface::setMouseStatsPtr(MouseStats* mouseStatsPtr)
{
	m_MouseStatsPtr = mouseStatsPtr;
}

void UserInterface::setBoidSystemStats(BoidSystem* boidSystem)
{
	m_Sliders[0].setValueRef(boidSystem->getBoidCohesion());
	m_Sliders[0].setPercentFromValue(*boidSystem->getBoidCohesion());

	m_Sliders[1].setValueRef(boidSystem->getBoidSeparation());
	m_Sliders[1].setPercentFromValue(*boidSystem->getBoidSeparation());

	m_Sliders[2].setValueRef(boidSystem->getBoidAlignment());
	m_Sliders[2].setPercentFromValue(*boidSystem->getBoidAlignment());

	m_Sliders[3].setValueRef(boidSystem->getCount());
	m_Sliders[3].setPercentFromValue(*boidSystem->getCount());
	
	m_TextBoxes[0].setValueRef(boidSystem->getBoidCohesion());
	m_TextBoxes[1].setValueRef(boidSystem->getBoidSeparation());
	m_TextBoxes[2].setValueRef(boidSystem->getBoidAlignment());
	m_TextBoxes[3].setValueRef(boidSystem->getCount());

}

void UserInterface::check()
{
	if (!m_MouseStatsPtr)
	{
		return;
	}

	for (size_t i = 0; i < m_Sliders.size(); i++)
	{
		m_Sliders[i].check(m_MouseStatsPtr->position - m_Position - m_Padding, m_MouseStatsPtr->leftState);
	}
}

void UserInterface::update()
{
	if (true/*m_ShouldResize*/)
	{
		Vec2f extent(0.0f, 0.0f);

		for (size_t i = 0; i < m_Sliders.size(); i++)
		{
			if (m_Sliders[i].m_Position.x + m_Sliders[i].m_Size.x > extent.x - m_Padding.x)
			{
				extent.x = m_Sliders[i].m_Position.x + m_Sliders[i].m_Size.x;
			}

			if (m_Sliders[i].m_Position.y + m_Sliders[i].m_Size.y > extent.y - m_Padding.y)
			{
				extent.y = m_Sliders[i].m_Position.y + m_Sliders[i].m_Size.y;
			}
		}

		for (size_t i = 0; i < m_TextBoxes.size(); i++)
		{
			if (m_TextBoxes[i].m_Position.x + m_TextBoxes[i].m_Size.x + m_TextBoxes[i].m_Padding.x * 2.0f > extent.x - m_Padding.x)
			{
				extent.x = m_TextBoxes[i].m_Position.x + m_TextBoxes[i].m_Size.x + m_TextBoxes[i].m_Padding.x * 2.0f;
			}

			if (m_TextBoxes[i].m_Position.y + m_TextBoxes[i].m_Size.y + m_TextBoxes[i].m_Padding.y * 2.0f > extent.y - m_Padding.y)
			{
				extent.y = m_TextBoxes[i].m_Position.y + m_TextBoxes[i].m_Size.y + m_TextBoxes[i].m_Padding.y * 2.0f;
			}
		}

		if (extent.x > m_Size.x)
		{
			m_Size.x = extent.x;
		}

		if (extent.y > m_Size.y)
		{
			m_Size.y = extent.y;
		}

		m_ShouldResize = false;
	}

	bool value = false;
	for (size_t i = 0; i < m_TextBoxes.size(); i++)
	{
		m_TextBoxes[i].update();

		value |= m_TextBoxes[i].m_AutoSize;
	}
	m_ShouldResize = value;

	if (!m_MouseStatsPtr)
	{
		return;
	}
	
	for (size_t i = 0; i < m_Sliders.size(); i++)
	{		
		m_Sliders[i].update(m_MouseStatsPtr->position - m_Position - m_Padding);
	}
}

void UserInterface::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.0f);
	glPushMatrix();
	glScalef(m_Size.x + m_Padding.x * 2.0f, m_Size.y + m_Padding.y * 2.0f, 1.0f);
	glColorVec4f(m_Color);
	glCallList(m_PanelList);
	glPopMatrix();

	glTranslatef(m_Padding.x, m_Padding.y, 0.0f);

	for (size_t i = 0; i < m_TextBoxes.size(); i++)
	{
		m_TextBoxes[i].draw();
	}

	for (size_t i = 0; i < m_Sliders.size(); i++)
	{
		m_Sliders[i].draw();
	}

	glPopMatrix();
}

void UserInterface::setDrawLists(GLuint panelList)
{
	m_PanelList = panelList;
}

void UserInterface::initModels()
{
	m_PanelList = glGenLists(1);

	glNewList(m_PanelList, GL_COMPILE);
	glBegin(GL_QUADS);
	glVertexVec2f(Vec2f(0.0f, 0.0f));
	glVertexVec2f(Vec2f(1.0f, 0.0f));
	glVertexVec2f(Vec2f(1.0f, 1.0f));
	glVertexVec2f(Vec2f(0.0f, 1.0f));
	glEnd();
	glEndList();
}
