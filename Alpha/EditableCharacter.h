#pragma once
#include "Character.h"

/*Class which holds the info about the color, size and other text parameters*/
class EditableCharacter : public Character {

private:
	Vec3 m_color;
	float m_size;
	bool m_useOutline;
	float m_outlineWidth;
	Vec3 m_outlineColor;

public:
	bool operator=(const EditableCharacter& other) { return false; }
	EditableCharacter(const Character& t_character, Vec3& t_color, float t_size, bool t_useOutline, float t_outlineWidth, Vec3& t_outlineColor) 
		: Character(t_character), m_color(t_color), m_size(t_size), m_useOutline(t_useOutline), m_outlineWidth(t_outlineWidth), m_outlineColor(t_outlineColor) { }

	EditableCharacter(const Character& t_character, Vec3& t_color, float t_size)
		: Character(t_character), m_color(t_color), m_size(t_size), m_useOutline(true), m_outlineWidth(0.0f), m_outlineColor(Vec3(1.0f, 215 / 255.0f, 0.0f)) { }

	void SetColor(const Vec3& t_color) { m_color = t_color; }
	void SetSize(const float t_size) { m_size = t_size; }
	void SetUseOutline(const bool t_outline) { m_useOutline = t_outline; }
	void SetOutlineWidth(const float t_width) { m_outlineWidth = t_width; }
	void SetOutlineColor(const Vec3& t_color) { m_outlineColor = t_color; }

	Vec3 GetColor() const { return m_color; }
	float GetSize() const { return m_size; }
	bool GetUseOutline() const { return m_useOutline; }
	float GetOutlineWidth() const { return m_outlineWidth; }
	Vec3 GetOutlineColor() const { return m_outlineColor; }
};