#include <stdlib.h>
#include <time.h>
#include <filesystem>
#include <thread>
#include <mutex>
#include <algorithm>
#include "Game.h"
#include "ModelLoader.h"
#include "FontLoader.h"
#include "Swarm.h"
#include "GameOfLife.h"
#include "Boids.h"
#include "Boids3D.h"
#include "SFM.h"
#include "RVO.h"

int main() {
	Mat4 perspective = Mat4::Perspective(PERSPECTIVE_FOV, PERSPECTIVE_ASPECT, PERSPECTIVE_NEAR, PERSPECTIVE_FAR);
	Mat4 orthographic = Mat4::Orthographic(ORTHOGRAPHIC_LEFT, ORTHOGRAPHIC_RIGHT, ORTHOGRAPHIC_BOTTOM, ORTHOGRAPHIC_TOP, ORTHOGRAPHIC_NEAR, ORTHOGRAPHIC_FAR);
	Camera camera = Camera(Vec3(50.0f, 75, 200), Vec3(0, -0.2, -1.0), .5f, 0.05f, Camera::FreeRoaming, Vec3(5.0f));
	Engine engine = Engine(perspective, orthographic, 1680, 1080, &camera);

	Game game = Game(engine);

	Light sun1 = Light(Vec3(10000.0f, 10000.0f, 10000.0f), Vec3(1.0f), Vec3(1, 0, 0));
	Light sun2 = Light(Vec3(0.0f, 10000.0f, 0.0f), Vec3(0.0f), Vec3(1, 0, 0));
	Light sun3 = Light(Vec3(0.0f, 10000.0f, 0.0f), Vec3(0.0f), Vec3(1, 0, 0));
	Light sun4 = Light(Vec3(0.0f, 10000.0f, 0.0f), Vec3(0.0f), Vec3(1, 0, 0));
	
	GameObjects::Add(sun1);
	GameObjects::Add(sun2);
	GameObjects::Add(sun3);
	GameObjects::Add(sun4);
	
 	Texture temp = TextureLoader::LoadTexture("grass.png", 1, 0.1, 0.1);
	Texture browTemp = Texture(TextureLoader::LoadTexture("brown.png"), 1, 0.1f, 0.1f);

	GameOfLife gl = GameOfLife();
	Boids boids = Boids(100);
	Boids3D boids3D = Boids3D(300);
	SFM sfm = SFM(100);
	Swarm swarm = Swarm(640000);
	RVO rvo = RVO(100);

	GUI mainGui = GUI(TexturedModel(ModelLoader::LoadModel("plane.obj"), gl.getTexture(), 0), 0, "mainGui", Vec3(8, 8, -10), Quaternion(0, 0, 0, 1), Vec3(8), false, "asdf");
	Skybox skybox = Skybox("Day", "Day", "Day", "Day", 0.001f);
	GameObjects::Add(std::make_shared<Skybox>(skybox));
	GameObjects::Add(std::make_shared<GUI>(mainGui));

	Texture textureSolid = Texture(TextureLoader::LoadTexture("solid.png"), 1, 0.1f, 0.1f);
	Texture textureTransparent = Texture(TextureLoader::LoadTexture("transparent.png"), 1, 0.1f, 0.1f);
	TexturedModel texturedModelSolid = TexturedModel(ModelLoader::LoadModel("plane.obj"), textureSolid, 0);
	TexturedModel texturedModelTransparent = TexturedModel(ModelLoader::LoadModel("plane.obj"), textureTransparent, 0);
	std::shared_ptr<Entity> mainBox = std::make_shared<Entity>(Entity(texturedModelSolid, 0, Vec3(50, 0, 50), Quaternion::Rotation(-90, Vec3(1, 0, 0)), Vec3(50), Vec3(0), true, false, "asdf"));
	std::shared_ptr<Entity> sideBox1 = std::make_shared<Entity>(Entity(texturedModelTransparent, 0, Vec3(50, 50, 0), Quaternion::Identity(), Vec3(50), Vec3(0), true, false, "asdf"));
	//std::shared_ptr<Entity> sideBox2 = std::make_shared<Entity>(Entity(texturedModelTransparent, 0, Vec3(50, 50, 100), Quaternion::Identity(), Vec3(50), Vec3(0), true, false, "asdf"));
	std::shared_ptr<Entity> sideBox3 = std::make_shared<Entity>(Entity(texturedModelTransparent, 0, Vec3(0, 50, 50), Quaternion::Rotation(90, Vec3(0, 1, 0)), Vec3(50), Vec3(0), true, false, "asdf"));
	std::shared_ptr<Entity> sideBox4 = std::make_shared<Entity>(Entity(texturedModelTransparent, 0, Vec3(100, 50, 50), Quaternion::Rotation(-90, Vec3(0, 1, 0)), Vec3(50), Vec3(0), true, false, "asdf"));
	std::shared_ptr<Entity> sideBox5 = std::make_shared<Entity>(Entity(texturedModelTransparent, 0, Vec3(50, 100, 50), Quaternion::Rotation(90, Vec3(1, 0, 0)), Vec3(50), Vec3(0), true, false, "asdf"));
	GameObjects::Add(sideBox1);
	//GameObjects::Add(sideBox2);
	GameObjects::Add(sideBox3);
	GameObjects::Add(sideBox4);
	GameObjects::Add(sideBox5);
	GameObjects::Add(mainBox);

	Texture textureDragon = Texture(TextureLoader::LoadTexture("sand.png"), 1, 0.1f, 0.1f);
	TexturedModel texturedModelSpehere = TexturedModel(ModelLoader::LoadModel("sphere.obj"), textureDragon, 0);
	std::shared_ptr<Entity> sphere = std::make_shared<Entity>(Entity(texturedModelSpehere, 0, Vec3(25, 75, 40), Quaternion::Identity(), Vec3(1), Vec3(0), true, false, "asdf"));
	GameObjects::Add(sphere);

	Texture textureRed = Texture(TextureLoader::LoadTexture("red.png"), 1, 0.1f, 0.1f);
	TexturedModel texturedModelTorus = TexturedModel(ModelLoader::LoadModel("torus.obj"), textureRed, 0);
	std::shared_ptr<Entity> torus1 = std::make_shared<Entity>(Entity(texturedModelTorus, 0, Vec3(25, 75, 25), Quaternion::Identity(), Vec3(10), Vec3(0), true, false, "asdf"));
	std::shared_ptr<Entity> torus2 = std::make_shared<Entity>(Entity(texturedModelTorus, 0, Vec3(75, 75, 75), Quaternion::Identity(), Vec3(10), Vec3(0), true, false, "asdf"));
	std::shared_ptr<Entity> torus3 = std::make_shared<Entity>(Entity(texturedModelTorus, 0, Vec3(50, 25, 50), Quaternion::Identity(), Vec3(10), Vec3(0), true, false, "asdf"));

	bool go = false;
	bool first = true;
	int skipper = 0;
	int scene = 0;
	bool cameraUpdate = false;

	std::vector<std::vector<float>> x = std::vector<std::vector<float>>();
	std::vector<std::vector<float>> v = std::vector<std::vector<float>>();
	std::vector<float> v_max = std::vector<float>();
	std::vector<std::vector<float>> goal = std::vector<std::vector<float>>();
	RVOModel ws_model;
	ws_model.robotRadius = 0.2f;
	ws_model.circularObstacles = std::vector<std::vector<float>>();
	ws_model.boundary = std::vector<std::vector<float>>();
	int numRvoAgents = 50;
	float radiusRvo = 7.0f;

	for (int i = 0; i < numRvoAgents; ++i) {
		float angle = (i / (float)numRvoAgents) * 2 * M_PI;
		float xCoord = cos(angle) * radiusRvo + 8;
		float yCoord = sin(angle) * radiusRvo + 8;
		x.push_back(std::vector<float>({ xCoord, yCoord }));
		xCoord = cos(angle + M_PI) * radiusRvo + 8;
		yCoord = sin(angle + M_PI) * radiusRvo + 8;
		goal.push_back(std::vector<float>({ xCoord, yCoord }));
	}
	for (int i = 0; i < x.size(); ++i) {
		v.push_back(std::vector<float>({ 0.0f, 0.0f }));
	}
	for (int i = 0; i < x.size(); ++i) {
		v_max.push_back(1.0);
	}

	float total_time = 15.0f;
	float step = 0.01f;
	int t = 0;

	Texture blackness = TextureLoader::LoadTexture("black.png", 1, 0.1, 0.1);
	std::vector<std::shared_ptr<GUI>> rvoModels = std::vector<std::shared_ptr<GUI>>();
	for (int i = 0; i < numRvoAgents; ++i) {
		float angle = (i / (float)numRvoAgents) * 360;
		float colorX = 1 - abs(fmod(angle / 60.0f, 2) - 1);
		Vec4 color = Vec4(1);
		if (angle >= 0 && angle < 60) {
			color.x = 1;
			color.y = colorX;
			color.z = 0;
		} else if (angle >= 60 && angle < 120) {
			color.x = colorX;
			color.y = 1;
			color.z = 0;
		} else if (angle >= 120 && angle < 180) {
			color.x = 0;
			color.y = 1;
			color.z = colorX;
		} else if (angle >= 180 && angle < 240) {
			color.x = 0;
			color.y = colorX;
			color.z = 1;
		} else if (angle >= 240 && angle < 300) {
			color.x = colorX;
			color.y = 0;
			color.z = 1;
		} else if (angle >= 300 && angle < 360) {
			color.x = 1;
			color.y = 0;
			color.z = colorX;
		}
		Texture texture = Texture(TextureLoader::LoadTexture("circleYellow.png"), 1, 0.1f, 0.1f);
		TexturedModel texturedModel = TexturedModel(ModelLoader::LoadModel("plane.obj"), texture, 0);
		rvoModels.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(x[i][0], x[i][1]), Quaternion::Identity(), Vec3(0.1), false, "asdf", color)));
	}

	Texture texture = Texture(TextureLoader::LoadTexture("fishhook.png"), 1, 0.1f, 0.1f);
	TexturedModel texturedModel = TexturedModel(ModelLoader::LoadModel("plane.obj"), texture, 0);
	std::shared_ptr<GUI> fishhook = std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(8, 8, -1), Quaternion::Identity(), Vec3(0.5), false, "asdf"));

	//std::shared_ptr<Line> line = std::make_shared<Line>(Line(Vec3(0, 0, -7), Vec3(1), Vec4(1), Vec4(1)));
	//GameObjects::Add(line);
	while (!engine.ShouldStop()) {
 		if (Input::GetKeyPress(GLFW_KEY_F3)) {
			cameraUpdate = !cameraUpdate;
			engine.setCameraUpdate(cameraUpdate);
		}
		//rvo.run();
		Vec2 mouseTarget = MousePickerOrthographic::GetCurrentRay();
		fishhook->SetPosition(Vec3(mouseTarget, -1));
		if (Input::GetKeyPress(GLFW_KEY_H)) go = !go;

		if (Input::GetKeyPress(GLFW_KEY_P)) {
			scene = (scene + 1) % 6;
			if (scene == 0) {
				gl = GameOfLife();
				GameObjects::ClearGUIs();
				mainGui.SetTexture(gl.getTexture());
				mainGui.SetTextureMultiple(1);
				GameObjects::Add(std::make_shared<GUI>(mainGui));
			} else if (scene == 1) {
				GameObjects::ClearGUIs();
				boids = Boids(boids.m_agents.size());
				mainGui.SetTexture(boids.getTexture());
				mainGui.SetTextureMultiple(1);
				GameObjects::Add(std::make_shared<GUI>(mainGui));
				for (int i = 0; i < boids.m_agents.size(); ++i) {
					GameObjects::Add(boids.m_agents[i].getModel());
				}
			} else if (scene == 2) {
				GameObjects::ClearGUIs();
				GameObjects::Add(mainBox);
				GameObjects::Add(sideBox1);
				//GameObjects::Add(sideBox2);
				GameObjects::Add(sideBox3);
				GameObjects::Add(sideBox4);
				GameObjects::Add(sideBox5);
				GameObjects::Add(sphere);
				GameObjects::Add(torus1);
				GameObjects::Add(torus2);
				GameObjects::Add(torus3);
				boids3D = Boids3D(boids3D.m_agents.size());
				for (int i = 0; i < boids3D.m_agents.size(); ++i) {
					GameObjects::Add(boids3D.m_agents[i].getModel());
				}
			} else if (scene == 3) {
				swarm = Swarm(swarm.getAgentCount());
				swarm.runTrail();
				GameObjects::ClearGUIs();
				mainGui.SetTexture(swarm.getTexture());
				mainGui.SetTextureMultiple(1);
				GameObjects::Add(std::make_shared<GUI>(mainGui));
			} else if (scene == 4) {
				GameObjects::ClearGUIs();
				sfm = SFM(sfm.m_agents.size());
				mainGui.SetTexture(sfm.getTexture());
				mainGui.SetTextureMultiple(3);
				for (int i = 0; i < sfm.m_agents.size(); ++i) {
					GameObjects::Add(sfm.m_agents[i].getModel());
				}
				GameObjects::Add(std::make_shared<GUI>(mainGui));
			} else if (scene == 5) {
				GameObjects::ClearGUIs();
				x = std::vector<std::vector<float>>();
				v = std::vector<std::vector<float>>();
				v_max = std::vector<float>();
				goal = std::vector<std::vector<float>>();
				RVOModel ws_model;
				ws_model.robotRadius = 0.2f;
				ws_model.circularObstacles = std::vector<std::vector<float>>();
				ws_model.boundary = std::vector<std::vector<float>>();

				for (int i = 0; i < numRvoAgents; ++i) {
					float angle = (i / (float)numRvoAgents) * 2 * M_PI;
					float xCoord = cos(angle) * radiusRvo + 8;
					float yCoord = sin(angle) * radiusRvo + 8;
					x.push_back(std::vector<float>({ xCoord, yCoord }));
					xCoord = cos(angle + M_PI) * radiusRvo + 8;
					yCoord = sin(angle + M_PI) * radiusRvo + 8;
					goal.push_back(std::vector<float>({ xCoord, yCoord }));
				}
				for (int i = 0; i < x.size(); ++i) {
					v.push_back(std::vector<float>({ 0.0f, 0.0f }));
				}
				for (int i = 0; i < x.size(); ++i) {
					v_max.push_back(1.0);
				}

				float total_time = 15.0f;
				float step = 0.01f;
				int t = 0;

				Texture blackness = TextureLoader::LoadTexture("black.png", 1, 0.1, 0.1);
				rvoModels = std::vector<std::shared_ptr<GUI>>();
				for (int i = 0; i < numRvoAgents; ++i) {
					float angle = (i / (float)numRvoAgents) * 360;
					float colorX = 1 - abs(fmod(angle / 60.0f, 2) - 1);
					Vec4 color = Vec4(1);
					if (angle >= 0 && angle < 60) {
						color.x = 1;
						color.y = colorX;
						color.z = 0;
					}
					else if (angle >= 60 && angle < 120) {
						color.x = colorX;
						color.y = 1;
						color.z = 0;
					}
					else if (angle >= 120 && angle < 180) {
						color.x = 0;
						color.y = 1;
						color.z = colorX;
					}
					else if (angle >= 180 && angle < 240) {
						color.x = 0;
						color.y = colorX;
						color.z = 1;
					}
					else if (angle >= 240 && angle < 300) {
						color.x = colorX;
						color.y = 0;
						color.z = 1;
					}
					else if (angle >= 300 && angle < 360) {
						color.x = 1;
						color.y = 0;
						color.z = colorX;
					}
					Texture texture = Texture(TextureLoader::LoadTexture("circleYellow.png"), 1, 0.1f, 0.1f);
					TexturedModel texturedModel = TexturedModel(ModelLoader::LoadModel("plane.obj"), texture, 0);
					rvoModels.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(x[i][0], x[i][1]), Quaternion::Identity(), Vec3(0.1), false, "asdf", color)));
				}
			}
		}
		
		if (scene == 0) {
			if (skipper % 300 == 0) {
				GameObjects::ClearGUIs();
				if (go) mainGui.SetTexture(gl.run());
				mainGui.SetTextureMultiple(1);
				GameObjects::Add(std::make_shared<GUI>(mainGui));
			}
			skipper++;
		} else if (scene == 1) {
			if (go) boids.run();
			GameObjects::ClearGUIs();
			mainGui.SetTexture(boids.getTexture());
			mainGui.SetTextureMultiple(1);
			GameObjects::Add(std::make_shared<GUI>(mainGui));
			GameObjects::Add(fishhook);
			for (int i = 0; i < boids.m_agents.size(); ++i) {
				GameObjects::Add(boids.m_agents[i].getModel());
			}
		} else if (scene == 2) {
			if (go) boids3D.run();
			GameObjects::ClearEntities();
			GameObjects::Add(mainBox);
			GameObjects::Add(sideBox1);
			//GameObjects::Add(sideBox2);
			GameObjects::Add(sideBox3);
			GameObjects::Add(sideBox4);
			GameObjects::Add(sideBox5);
			//GameObjects::Add(sphere);
			GameObjects::Add(torus1);
			GameObjects::Add(torus2);
			GameObjects::Add(torus3);
			for (int i = 0; i < boids3D.m_agents.size(); ++i) {
				GameObjects::Add(boids3D.m_agents[i].getModel());
			}
		} else if (scene == 3) {
			if (go) swarm.run();
		} else if (scene == 4) {
			if (go) sfm.run();
			GameObjects::ClearGUIs();
			mainGui.SetTexture(sfm.getTexture());
			for (int i = 0; i < sfm.m_barriers.size(); ++i) {
				GameObjects::Add(sfm.m_barriers[i]);
			}
			for (int i = 0; i < sfm.m_agents.size(); ++i) {
				GameObjects::Add(sfm.m_agents[i].getModel());
			}
			mainGui.SetTextureMultiple(5);
			GameObjects::Add(std::make_shared<GUI>(mainGui));
		} else if (scene == 5) {
			GameObjects::ClearGUIs();
			
			if (go) {
				//rvo.run();
				std::vector<std::vector<float>> v_des = RVO::compute_V_des(x, goal, v_max);
				v = RVO::RVO_update(x, v_des, v, ws_model);
				for (int i = 0; i < x.size(); ++i) {
					x[i][0] += v[i][0] * step;
					x[i][1] += v[i][1] * step;
				}
			}
			mainGui.SetTexture(rvo.getTexture());
			mainGui.SetTextureMultiple(1);
			GameObjects::Add(std::make_shared<GUI>(mainGui));
			for (int i = 0; i < rvoModels.size(); ++i) {
				rvoModels[i]->SetPosition(Vec3(x[i][0], x[i][1], -5));
				GameObjects::Add(rvoModels[i]);
			}
		}
		
		game.PushToEngine();
		std::cout << Time::GetFrameCount() << "; " <<  Time::GetDeltaTime() << std::endl;
		game.UpdateWindow();
	}

	engine.Stop();

	return 0;
}