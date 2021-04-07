#pragma once

#include "../utils/utils.h"

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

	void check(int state, const Vec2f& mousePosition);

	void update(const Vec2f& mousePosition);

	void draw();

	static void setDrawLists(GLuint sliderList, GLuint buttonList);
	static void initModels();

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
};