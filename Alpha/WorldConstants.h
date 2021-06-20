#pragma once
#include "Math.h"

class World {
	
	friend class Engine;
private:
	static Mat4 m_perspectiveMatrix;
	static Mat4 m_orthographicMatrix;

public:
	static Mat4 GetPerspectiveMatrix() { return m_perspectiveMatrix; }
	static Mat4 GetOrthographicMatrix() { return m_orthographicMatrix; }
};

//perspective matrix
const float PERSPECTIVE_FOV = 70.0f;
const float PERSPECTIVE_ASPECT = 16.0f / 9.0f;
const float PERSPECTIVE_NEAR = 0.1f;
const float PERSPECTIVE_FAR = 5000.0f;

//orthographic matrix
const float ORTHOGRAPHIC_LEFT = 0.0f;
const float ORTHOGRAPHIC_RIGHT = 16.0f;
const float ORTHOGRAPHIC_BOTTOM = 0.0f;
const float ORTHOGRAPHIC_TOP = 16.0f;
const float ORTHOGRAPHIC_NEAR = 0.1f;
const float ORTHOGRAPHIC_FAR = 5000.0f;

const float RENDER_DISTANCE = 5000.0f;
const float WATER_HEIGHT = 0.0f;

//terrain stuff
const int FLAT_SEED = 0;
const int TERRAIN_SIZE = 800;
const int TERRAIN_VERT_COUNT = 128;

//general stuff
const float GRAVITY = -10.0f;
const float DRAW_DISTANCE = 5000.0f;
const float LINE_WIDTH = 3.0f;
const int MAX_LINES = 10000;
const float SCROLL_SPEED = 1.0f;

//rendering stuff
const unsigned int VERTEX_LOCATION = 0;
const unsigned int UV_LOCATION = 1;
const unsigned int NORMAL_LOCATION = 2;

const unsigned int MAX_LIGHTS = 4;

const float SKYBOX_SIZE = 100000.0f;

//Vec3 FOG_COLOR = Vec3(0.5f, 0.5f, 0.5f);

//water stuff
const int REFLECTION_WIDTH = 320;
const int REFLECTION_HEIGHT = 180;

const int REFRACTION_WIDTH = 1280;
const int REFRACTION_HEIGHT = 720;

//camera stuff
const int INVERT_X = -1; //-1 = false, 1 = true
const int INVERT_Y = 1; //-1 = false, 1 = true
const float DEAD_ZONE = 0.9f;

//shadow stuff
const int SHADOW_MAP_WIDTH = 4096;
const int SHADOW_MAP_HEIGHT = 4096;
const float SHADOW_DISTANCE = 150.0f;
const float SHADOW_TRANSITION = 10.0f;

//marching cube
const float MARCHING_CUBE_TERRAIN_SCALE = 10.0f;

const std::string MODEL_PATH = "..\\Res\\Models\\";
const std::string TEXTURE_PATH = "..\\Res\\Textures\\";
const std::string SKYBOX_PATH = "..\\Res\\Skybox\\Textures\\";
const std::string CURSOR_PATH = "..\\Res\\Cursors\\";
const std::string FONT_PATH = "..\\Res\\Fonts\\";

const std::string ENTITY_VERT_PATH = "..\\Res\\Shaders\\entityVert.txt";
const std::string ENTITY_FRAG_PATH = "..\\Res\\Shaders\\entityFrag.txt";

const std::string TERRAIN_VERT_PATH = "..\\Res\\Shaders\\terrainVert.txt";
const std::string TERRAIN_FRAG_PATH = "..\\Res\\Shaders\\terrainFrag.txt";

const std::string SKYBOX_VERT_PATH = "..\\Res\\Shaders\\skyboxVert.txt";
const std::string SKYBOX_FRAG_PATH = "..\\Res\\Shaders\\skyboxFrag.txt";

const std::string PARTICLE_VERT_PATH = "..\\Res\\Shaders\\particleVert.txt";
const std::string PARTICLE_FRAG_PATH = "..\\Res\\Shaders\\particleFrag.txt";

const std::string GUI_VERT_PATH = "..\\Res\\Shaders\\GUIVert.txt";
const std::string GUI_FRAG_PATH = "..\\Res\\Shaders\\GUIFrag.txt";

const std::string WATER_VERT_PATH = "..\\Res\\Shaders\\waterVert.txt";
const std::string WATER_FRAG_PATH = "..\\Res\\Shaders\\waterFrag.txt";

const std::string FONT_VERT_PATH = "..\\Res\\Shaders\\textVert.txt";
const std::string FONT_FRAG_PATH = "..\\Res\\Shaders\\textFrag.txt";

const std::string SHADOW_VERT_PATH = "..\\Res\\Shaders\\shadowVert.txt";
const std::string SHADOW_FRAG_PATH = "..\\Res\\Shaders\\shadowFrag.txt";

const std::string ANIMATION_VERT_PATH = "..\\Res\\Shaders\\animationVert.txt";
const std::string ANIMATION_FRAG_PATH = "..\\Res\\Shaders\\animationFrag.txt";

const std::string LINE_VERT_PATH = "..\\Res\\Shaders\\lineVert.txt";
const std::string LINE_FRAG_PATH = "..\\Res\\Shaders\\lineFrag.txt";

const std::string RAY_MARCH_VERT_PATH = "..\\Res\\Shaders\\raymarchVert.txt";
const std::string RAY_MARCH_FRAG_PATH = "..\\Res\\Shaders\\raymarchFrag.c";

const std::string MARCHINGCUBE_VERT_PATH = "..\\Res\\Shaders\\marchingCubeVert.txt";
const std::string MARCHINGCUBE_FRAG_PATH = "..\\Res\\Shaders\\marchingCubeFrag.txt";

