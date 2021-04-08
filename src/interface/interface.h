#pragma once

#include "../utils/utils.h"
#include <vector>

class UserInterface;

class Slider
{
public:
	Slider();

	void setRange(float min, float max);
	void setPercent(float percent);
	void setPosition(const Vec2f& position);
	void setSize(const Vec2f& size);
	void setButtonDiameterPercent(float percent);
	void setSliderColor(const Vec4f& color);
	void setButtonColor(const Vec4f& color);
	
	void setValueRef(float* valuePtr);

	void check(const Vec2f& mousePosition, int state);

	void update(const Vec2f& mousePosition);

	void draw();

	static void setDrawLists(GLuint sliderList, GLuint buttonList);
	static void initModels();

private:
	void resetValue();

private:
	float* m_ValuePtr;

	float m_Min;
	float m_Max;
	float m_Percent;

	Vec2f m_Position;
	Vec2f m_Size;
	float m_ButtonRadiusPercent;
	Vec4f m_SliderColor;
	Vec4f m_ButtonColor;

	bool m_ButtonGrabbed;

	static GLuint m_SliderList;
	static GLuint m_ButtonList;

	friend class UserInterface;
};

class UserInterface
{
public:
	UserInterface();
	UserInterface(Vec2f* mousePositionPtr, int* mouseStatePtr);

	Vec2f getSize() const;
	Vec2f getPadding() const;

	Slider& getSlider(size_t idx);

	void setPosition(const Vec2f& position);
	void setPadding(const Vec2f& padding);
	void setColor(const Vec4f& color);
	
	Slider& addSlider();

	void setMousePositionPtr(Vec2f* mousePositionPtr);
	void setMouseStatePtr(int* mouseStatePtr);

	void check();

	void update();

	void draw();

	static void setDrawLists(GLuint panelList);
	static void initModels();

private:
	std::vector<Slider> m_Sliders;

	Vec2f m_Position;
	Vec2f m_Size;

	Vec2f m_Padding;
	Vec2f m_Color;

	Vec2f* m_MousePositionPtr;
	int* m_MouseStatePtr;

	static GLuint m_PanelList;
	
	bool m_ShouldResize;
};