#pragma once

#include "../utils/utils.h"
#include "../entities/boid.h"
#include <vector>
#include <string>

class UserInterface;

class Slider
{
public:
	Slider();

	void setRange(float min, float max);
	void setPercent(float percent);
	void setPercentFromValue(float value);
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

class TextBox
{
public:
	TextBox();

	void setPosition(const Vec2f& position);
	void setSize(const Vec2f& size);
	void setPadding(const Vec2f& padding);
	void setTextColor(const Vec4f& color);
	void setBoxColor(const Vec4f& color);
	void setFont(void* font);
	void setAutoSize(bool value);
	void setPrecision(std::streamsize precision);

	void setValueRef(std::string* textPtr);
	void setValueRef(float* valuePtr);

	void useFloat(bool value);

	void update();

	void draw();

	static void setDrawLists(GLuint boxList);
	static void initModels();

private:
	std::string* m_TextPtr;
	float* m_ValuePtr;
	std::string m_Text;

	Vec2f m_Position;
	Vec2f m_Size;
	Vec2f m_Padding;
	Vec4f m_TextColor;
	Vec4f m_BoxColor;
	
	void* m_Font;

	bool m_AutoSize;
	bool m_FromValue;
	std::streamsize m_Precision;

	static GLuint m_BoxList;

	friend class UserInterface;
};

class SelectionBox
{
public:
	SelectionBox();

	bool isSelected();

	void check(const Vec2f& mousePosition, int state);

	void update(const Vec2f& mousePosition);

	void draw();

	static void setDrawLists(GLuint boxList);
	static void initModels();

private:
	Boundary2f m_SelectionBoundary;
	Vec4f m_SelectionColor;
	Vec4f m_SelectionBoundaryColor;

	bool m_StartedSelection;
	bool m_Selected;

	static GLuint m_BoxList;

	friend class UserInterface;
};

class Button
{
public:
	Button();

	Vec2f getPosition() const;
	Vec2f getSize() const;

	void setPosition(const Vec2f& position);
	void setSize(const Vec2f& size);
	void setColor(const Vec4f& color);

	void useAsToggle(bool value);

	bool isClicked();

	void check(const Vec2f& mousePosition, int state);

	void update(const Vec2f& mousePosition);

	void draw();

	static void setDrawLists(GLuint boxList);
	static void initModels();

private:
	Boundary2f m_Boundary;
	Vec4f m_Color;

	bool m_ToggleMode;
	bool m_Clicked;

	static GLuint m_BoxList;

	friend class UserInterface;
};

class UserInterface
{
public:
	UserInterface();
	UserInterface(MouseStats* mouseStatsPtr);

	Vec2f getSize() const;
	Vec2f getPadding() const;

	Slider& getSlider(size_t idx);

	void setPosition(const Vec2f& position);
	void setPadding(const Vec2f& padding);
	void setColor(const Vec4f& color);
	void fitSize();
	
	Slider& addSlider();
	TextBox& addTextBox();

	void setMouseStatsPtr(MouseStats* mouseStatsPtr);

	void setBoidGroupStats(BoidGroup* boidGroup);
	
	void setActive(bool value);

	void check();

	void update();

	void draw();

	static void setDrawLists(GLuint panelList);
	static void initModels();

	void setBoidSystemRef(BoidSystem& boidSystem);

private:
	std::vector<Slider> m_Sliders;
	std::vector<TextBox> m_TextBoxes;
	SelectionBox m_SelectionBox;
	Button m_CloseButton;

	Vec2f m_Position;
	Vec2f m_Size;

	Vec2f m_Padding;
	Vec4f m_Color;

	bool m_ShouldResize;
	bool m_Active;

	MouseStats* m_MouseStatsPtr;

	static GLuint m_PanelList;

	BoidSystem* m_BoidSystemPtr;
};