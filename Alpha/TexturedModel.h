#pragma once
#include "RawModel.h"
#include "Texture.h"
#include "Math.h"

/*Class used to store data of a RawModel and Texture 
as well as a texture index specifying the position of the texture in the atlas*/
class TexturedModel {

protected:
	RawModel m_rawModel;
	Texture m_texture;
	unsigned int m_textureIndex;

public:
	TexturedModel(const RawModel& t_model, const Texture& t_texture, unsigned int t_textureIndex) 
		: m_rawModel(t_model), m_texture(t_texture), m_textureIndex(t_textureIndex) { }

	const RawModel& GetRawModel() const { return m_rawModel; }
	const Texture& GetTexture() const { return m_texture; }

	//gets offset to correct image in atlas
	Vec2 GetAtlasOffset() const {
		unsigned int numOfRows = m_texture.GetNumberOfRows();
		unsigned int column = m_textureIndex % numOfRows;
		unsigned int row = m_textureIndex / numOfRows;
		return Vec2((float)column / (float)numOfRows, (float)row / (float)numOfRows);
	}
};