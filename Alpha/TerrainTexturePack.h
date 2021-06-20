#pragma once
#include "Texture.h"

/*Class used to store the textures for the terrain used in a blend map.*/
class TerrainTexturePack {

private:
	Texture m_backgroundTexture;
	Texture m_rTexture;
	Texture m_gTexture;
	Texture m_bTexture;

public:
	TerrainTexturePack(Texture t_backgroundTexture, Texture t_rTexture, Texture t_gTexture, Texture t_bTexture)
	: m_backgroundTexture(t_backgroundTexture), m_rTexture(t_rTexture), m_gTexture(t_gTexture), m_bTexture(t_bTexture) { }

	Texture& GetBackgroundTexture() { return m_backgroundTexture; }
	Texture& GetRTexture() { return m_rTexture; }
	Texture& GetGTexture() { return m_gTexture; }
	Texture& GetBTexture() { return m_bTexture; }
};