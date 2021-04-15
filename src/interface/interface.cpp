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
		float angle = 2.0f * static_cast<float>(M_PI * i) / static_cast<float>(pointCount);
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
	m_ValuePtr = nullptr;
	m_Font = GLUT_BITMAP_8_BY_13;
	m_AutoSize = true;
	m_FromValue = false;
	m_Precision = 3;
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

void TextBox::setPrecision(std::streamsize precision)
{
	m_Precision = precision;
}

void TextBox::setText(const std::string& string)
{
	m_Text = string;
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
		m_Text = to_stringn(*m_ValuePtr, m_Precision);
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

GLuint SelectionBox::m_BoxList = 0;

SelectionBox::SelectionBox()
{
	m_SelectionColor = Vec4f(0.0f, 0.0f, 0.2f, 0.3f);
	m_SelectionBoundaryColor = Vec4f(0.0f, 0.0f, 0.2f, 0.7f);

	m_StartedSelection = false;
	m_Selected = false;
}

bool SelectionBox::isSelected()
{
	bool value = m_Selected;
	m_Selected = false;

	return value;
}

void SelectionBox::check(const Vec2f& mousePosition, int state, const Boundary2f& boundary)
{	
	
	if (!m_StartedSelection && state == GLUT_DOWN && !boundary.contains(mousePosition))
	{
		m_StartedSelection = true;
		m_Selected = false;

		m_SelectionBoundary.min = mousePosition;
		m_SelectionBoundary.max = mousePosition;
	}

	if (m_StartedSelection && state == GLUT_UP)
	{
		m_StartedSelection = false;
		m_Selected = true;
	}
}

void SelectionBox::update(const Vec2f& mousePosition)
{
	if (m_StartedSelection)
	{
		m_SelectionBoundary.max = mousePosition;
	}
}

void SelectionBox::draw()
{
	if (!m_StartedSelection)
	{
		return;
	}

	Vec2f size = m_SelectionBoundary.max - m_SelectionBoundary.min;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(m_SelectionBoundary.min.x, m_SelectionBoundary.min.y, 0.0f);
	glScalef(size.x, size.y, 1.0f);
	glColorVec4f(m_SelectionColor);
	glCallList(m_BoxList);
	
	glColorVec4f(m_SelectionBoundaryColor);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glCallList(m_BoxList);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPopMatrix();
}

void SelectionBox::setDrawLists(GLuint drawList)
{
	m_BoxList = drawList;
}

void SelectionBox::initModels()
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

GLuint Button::m_BoxList = 0;
Button::Button()
{
	m_Clicked = false;
	m_ToggleMode = false;
}

Vec2f Button::getPosition() const
{
	return m_Boundary.min;
}

Vec2f Button::getSize() const
{
	return m_Boundary.max - m_Boundary.min;
}

void Button::setPosition(const Vec2f& position)
{
	m_Boundary.min = position;
}

void Button::setSize(const Vec2f& size)
{
	m_Boundary.max = m_Boundary.min + size;
}

void Button::setColor(const Vec4f& color)
{
	m_Color = color;
}

void Button::useAsToggle(bool value)
{
	m_ToggleMode = value;
	m_Clicked = false;
}

bool Button::isClicked()
{
	bool value = m_Clicked;

	if (!m_ToggleMode)
	{
		m_Clicked = false;
	}

	return value;
}

void Button::check(const Vec2f& mousePosition, int state)
{
	if (m_Boundary.contains(mousePosition) && state == GLUT_DOWN)
	{
		if (m_ToggleMode)
		{
			m_Clicked = !m_Clicked;
		}
		else if (!m_Clicked)
		{
			m_Clicked = true;
		}
	}

	if (!m_ToggleMode && m_Clicked && state == GLUT_UP)
	{
		m_Clicked = false;
	}
}

void Button::update(const Vec2f& mousePosition)
{
	
}

void Button::draw()
{
	Vec2f size = m_Boundary.max - m_Boundary.min;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(m_Boundary.min.x, m_Boundary.min.y, 0.0f);
	glScalef(size.x, size.y, 1.0f);
	glColorVec4f(m_Color);
	glCallList(m_BoxList);
	glPopMatrix();
}

void Button::setDrawLists(GLuint boxList)
{
	m_BoxList = boxList;
}

void Button::initModels()
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
	m_Active = false;
	m_BoidSystemPtr = nullptr;
	m_PreviewBoidPtr = nullptr;

	m_CloseButton.setPosition(0.0f);
	m_CloseButton.setSize(Vec2f(16.0f, 16.0f));
	m_CloseButton.setColor(Vec4f(1.0f, 0.0f, 0.3f));
	m_CloseButton.useAsToggle(false);
}

UserInterface::UserInterface(MouseStats* mouseStatsPtr)
{
	m_MouseStatsPtr = mouseStatsPtr;
	m_ShouldResize = false;
	m_Active = false;
	m_BoidSystemPtr = nullptr;
	m_PreviewBoidPtr = nullptr;

	m_CloseButton.setPosition(0.0f);
	m_CloseButton.setSize(Vec2f(16.0f, 16.0f));
	m_CloseButton.setColor(Vec4f(1.0f, 0.0f, 0.3f));
	m_CloseButton.useAsToggle(false);
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

void UserInterface::fitSize()
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

void UserInterface::setBoidGroupStats(BoidGroup* boidGroup)
{
	size_t k = 0;
	m_PreviewBoidPtr = boidGroup;

	//cohesion
	m_Sliders[k].setPercentFromValue(*boidGroup->getBoidCohesion());
	m_Sliders[k].setValueRef(boidGroup->getBoidCohesion());

	m_TextBoxes[k].setPrecision(2);
	m_TextBoxes[k].setValueRef(boidGroup->getBoidCohesion());
	k++; 

	//separation
	m_Sliders[k].setPercentFromValue(*boidGroup->getBoidSeparation());
	m_Sliders[k].setValueRef(boidGroup->getBoidSeparation());

	m_TextBoxes[k].setPrecision(2);
	m_TextBoxes[k].setValueRef(boidGroup->getBoidSeparation());
	k++;

	//alignment
	m_Sliders[k].setPercentFromValue(*boidGroup->getBoidAlignment());
	m_Sliders[k].setValueRef(boidGroup->getBoidAlignment());

	m_TextBoxes[k].setPrecision(2);
	m_TextBoxes[k].setValueRef(boidGroup->getBoidAlignment());
	k++;

	//friendliness
	m_Sliders[k].setPercentFromValue(*boidGroup->getFriendliness());
	m_Sliders[k].setValueRef(boidGroup->getFriendliness());

	m_TextBoxes[k].setPrecision(2);
	m_TextBoxes[k].setValueRef(boidGroup->getFriendliness());
	k++;

	//size.x
	m_Sliders[k].setPercentFromValue(boidGroup->getBoidSize().x);
	m_Sliders[k].setValueRef(&boidGroup->getBoidSize().x);

	m_TextBoxes[k].setPrecision(1);
	m_TextBoxes[k].setValueRef(&boidGroup->getBoidSize().x);
	k++;

	//size.y
	m_Sliders[k].setPercentFromValue(boidGroup->getBoidSize().y);
	m_Sliders[k].setValueRef(&boidGroup->getBoidSize().y);

	m_TextBoxes[k].setPrecision(1);
	m_TextBoxes[k].setValueRef(&boidGroup->getBoidSize().y);
	k++;

	//count
	m_Sliders[k].setPercentFromValue(*boidGroup->getCount());
	m_Sliders[k].setValueRef(boidGroup->getCount());

	m_TextBoxes[k].setPrecision(0);
	m_TextBoxes[k].setValueRef(boidGroup->getCount());
	k++;

	//R
	m_Sliders[k].setPercentFromValue(boidGroup->getBoidColor().x);
	m_Sliders[k].setValueRef(&boidGroup->getBoidColor().x);

	m_TextBoxes[k].setPrecision(2);
	m_TextBoxes[k].setValueRef(&boidGroup->getBoidColor().x);
	k++;

	//G
	m_Sliders[k].setPercentFromValue(boidGroup->getBoidColor().y);
	m_Sliders[k].setValueRef(&boidGroup->getBoidColor().y);

	m_TextBoxes[k].setPrecision(2);
	m_TextBoxes[k].setValueRef(&boidGroup->getBoidColor().y);
	k++;

	//B
	m_Sliders[k].setPercentFromValue(boidGroup->getBoidColor().z);
	m_Sliders[k].setValueRef(&boidGroup->getBoidColor().z);

	m_TextBoxes[k].setPrecision(2);
	m_TextBoxes[k].setValueRef(&boidGroup->getBoidColor().z);
	k++;
}

void UserInterface::setActive(bool value)
{
	m_Active = value;
}

void UserInterface::check()
{
	m_CloseButton.check(m_MouseStatsPtr->position - m_Position - Vec2f(m_Size.x + m_Padding.x * 2.0f - m_CloseButton.getSize().x, 0.0f),
		m_MouseStatsPtr->leftState);

	if (m_CloseButton.isClicked())
	{
		m_Active = false;
		return;
	}

	if (!m_MouseStatsPtr)
	{
		return;
	}

	Boundary2f panelBoundary(m_Position, m_Size + m_Padding * 2.0f);

	if (!m_Active)
	{
		panelBoundary = Boundary2f(0.0f, 0.0f, 0.0f, 0.0f); //TODO: can't start a selection box from (0, 0)
	}
	
	m_SelectionBox.check(m_MouseStatsPtr->position, m_MouseStatsPtr->leftState, panelBoundary);

	if (m_SelectionBox.isSelected() && m_BoidSystemPtr)
	{	
		std::vector <BoidGroup>& boidGroups = m_BoidSystemPtr->getGroups();
		size_t index = 0, count = 0, max = 0;
		for (size_t i = 0; i < boidGroups.size(); i++)
		{
			count = 0;
			std::vector <Boid>& boids = boidGroups[i].getBoids();
			for (size_t j = 0; j < boids.size(); j++)
			{
				if (m_SelectionBox.m_SelectionBoundary.contains(boids[j].getPosition()))
				{
					count++;
				}
			}
			if (count > max)
			{
				max = count;
				index = i;
			}
		}

		if (max)
		{
			m_Active = true;
			setBoidGroupStats(&m_BoidSystemPtr->getGroup(index));
		}
	}

	if (!m_Active)
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
	//m_CloseButton.update(m_MouseStatsPtr->position);

	if (!m_MouseStatsPtr)
	{
		return;
	}

	m_SelectionBox.update(m_MouseStatsPtr->position);

	if (!m_Active)
	{
		return;
	}

	bool value = false;
	for (size_t i = 0; i < m_TextBoxes.size(); i++)
	{
		m_TextBoxes[i].update();

		value |= m_TextBoxes[i].m_AutoSize;
	}
	m_ShouldResize |= value;

	if (m_ShouldResize)
	{
		fitSize();
	}

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
	m_SelectionBox.draw();

	if (!m_Active)
	{
		return;
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(m_Position.x, m_Position.y, 0.0f);
	glPushMatrix();
	glScalef(m_Size.x + m_Padding.x * 2.0f, m_Size.y + m_Padding.y * 2.0f, 1.0f);
	glColorVec4f(m_Color);
	glCallList(m_PanelList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(m_Size.x + m_Padding.x * 2.0f - m_CloseButton.getSize().x, 0.0f, 0.0f);
	m_CloseButton.draw();
	glPopMatrix();

	glTranslatef(m_Padding.x, m_Padding.y, 0.0f);

	if (m_PreviewBoidPtr)
	{
		Boid& boid = m_PreviewBoidPtr->getBoids()[0];
		Vec2f boidPosition = boid.getPosition();
		boid.setPosition(Vec2f(0.0f, 0.0f));
		
		glPushMatrix();

		glTranslatef(430.0f, 340.0f, 0.0f);
		glScalef(3.0f, 3.0f, 1.0f);
		boid.draw(m_PreviewBoidPtr->getBoidModel(), m_PreviewBoidPtr->getBoidSize(), m_PreviewBoidPtr->getBoidColor());
		boid.setPosition(boidPosition);

		glPopMatrix();
	}
	

	
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

	Slider::initModels();
	TextBox::initModels();
	SelectionBox::initModels();
	Button::initModels();
}

void UserInterface::setBoidSystemRef(BoidSystem& boidSystem)
{
	m_BoidSystemPtr = &boidSystem;
}
