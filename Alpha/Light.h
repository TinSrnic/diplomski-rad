#pragma once
#include "Math.h"

/*Simple class to store variables about a light source*/
class Light {

private:
	Vec3 m_position;
	Vec3 m_color;
	Vec3 m_attenuation; //this is used to dim the light the source emits the further you are from it
	//TODO: I think the formula is a*x^2 + b*x + c where a,b,c are the elements of the vec3 attenuation

public:
	Light(const Vec3& t_position, const Vec3& t_color, const Vec3& t_attenuation) {
		m_position = t_position;
		m_color = t_color;
		m_attenuation = t_attenuation;
	}

	Vec3 GetPosition() const { return m_position; }
	Vec3 GetColor() const { return m_color; }
	Vec3 GetAttenuation() const { return m_attenuation; }

	void SetPosition(const Vec3& t_position) { m_position = t_position; }
	void SetColor(const Vec3& t_color) { m_color = t_color; }
	void SetAttenuation(const Vec3& t_attenuation) { m_attenuation = t_attenuation; }
};