#pragma once
#include "BoundedEntity.h"
#include "TerrainCollider.h"
#include "GameObjects.h"
#include "ModelLoader.h"
#include "TextureLoader.h"

class PhysicsEngine {

private:
	TerrainCollider m_terrainCollider;
	std::shared_ptr<Line> line1 = std::make_shared<Line>(Line(Vec3(0.0f), Vec3(10.0f), Vec4(0.69f, 0.0f, 0.69f, 1.0f), Vec4(1.0f)));
	std::shared_ptr<Line> line2 = std::make_shared<Line>(Line(Vec3(0.0f), Vec3(10.0f), Vec4(0.69f, 0.0f, 0.69f, 1.0f), Vec4(1.0f)));
	std::shared_ptr<Line> line3 = std::make_shared<Line>(Line(Vec3(0.0f), Vec3(10.0f), Vec4(0.69f, 0.0f, 0.69f, 1.0f), Vec4(1.0f)));
	std::shared_ptr<Line> line4 = std::make_shared<Line>(Line(Vec3(0.0f), Vec3(10.0f), Vec4(0.69f, 0.0f, 0.69f, 1.0f), Vec4(1.0f)));

public:
	PhysicsEngine() {
		//GameObjects::Add(line1);
		//GameObjects::Add(line2);
		//GameObjects::Add(line3);
		//GameObjects::Add(line4);
		m_terrainCollider = TerrainCollider();
	}

	struct Triangle {
		Vec3 a, b, c;
		Vec3 normal;

		Triangle(Vec3 a, Vec3 b, Vec3 c) {
			normal = Vec3::Normalize(Vec3::Cross(b - a, c - a));
			if (Vec3::Dot(normal, a) > 0) {
				this->a = a;
				this->b = b;
				this->c = c;
			} else {
				this->a = a;
				this->b = c;
				this->c = b;
				normal = Vec3::Normalize(Vec3::Cross(c - a, b - a));
			}
		}

		std::vector<std::pair<Vec3, Vec3>> GetEdges() {
			std::vector<std::pair<Vec3, Vec3>> edges = std::vector<std::pair<Vec3, Vec3>>();
			edges.push_back(std::pair<Vec3, Vec3>(a, b));
			edges.push_back(std::pair<Vec3, Vec3>(b, c));
			edges.push_back(std::pair<Vec3, Vec3>(c, a));
			return edges;
		}
	};

	void Update(std::vector<std::shared_ptr<BoundedEntity>> t_boundedEntities) {
		unsigned int sizeOfEntities = t_boundedEntities.size();
		
		Vec3* correction = new Vec3[sizeOfEntities];
		Vec3* totalMoment = new Vec3[sizeOfEntities];
		Vec3 pos = t_boundedEntities[1]->GetPosition();
		Vec3 scale = t_boundedEntities[1]->GetScale();
		Quaternion rotation = t_boundedEntities[1]->GetBoundingBody()->GetRotation();

		line1->SetStartPoint(pos);
		line2->SetStartPoint(pos);
		line3->SetStartPoint(pos);
		line4->SetStartPoint(pos);
		line1->SetEndPoint(pos + Quaternion::RotatePoint(rotation, Vec3(-scale.x, -scale.y, -scale.z)));
		line2->SetEndPoint(pos + Quaternion::RotatePoint(rotation, Vec3(scale.x, -scale.y, -scale.z)));
		line3->SetEndPoint(pos + Quaternion::RotatePoint(rotation, Vec3(scale.x, -scale.y, scale.z)));
		line4->SetEndPoint(pos + Quaternion::RotatePoint(rotation, Vec3(-scale.x, -scale.y, scale.z)));

		for (int i = 0; i < sizeOfEntities; ++i) {
			for (int j = i + 1; j < sizeOfEntities; ++j) {
				Vec3 contactPoint;
				Vec3 penVector = IsColliding(contactPoint, t_boundedEntities[i]->GetBoundingBody(), t_boundedEntities[j]->GetBoundingBody());
				totalMoment[i] -= Vec3::Cross(contactPoint - t_boundedEntities[i]->GetPosition(), penVector / 2.0f);
				totalMoment[j] += Vec3::Cross(contactPoint - t_boundedEntities[j]->GetPosition(), penVector / 2.0f);
				correction[i] -= penVector / 2.0f;
				correction[j] += penVector / 2.0f;
			}
		}

		for (int i = 0; i < sizeOfEntities; ++i) {
			Vec3 momentTerrain = Vec3(0.0f);
			Vec3 penVector = m_terrainCollider.IsColliding(momentTerrain, t_boundedEntities[i]->GetBoundingBody());
			Vec3 total = correction[i] + penVector;
			t_boundedEntities[i]->SetPosition(t_boundedEntities[i]->GetPosition() + correction[i] + penVector);
			Vec3 totalAngular = t_boundedEntities[i]->GetAngularVelocity() + momentTerrain * (2.0f / 3.0f);
			//t_boundedEntities[i]->SetAngularVelocity(totalAngular);

			t_boundedEntities[i]->AddRotation(totalAngular * 30 * Time::GetDeltaTime());
			if (total.y > 0.0f) {
				t_boundedEntities[i]->SetVelocity(0.0f);
			}
		}
	}

	Vec3 IsColliding(Vec3& t_contactPoint, const std::shared_ptr<BoundedBody>& t_first, const std::shared_ptr<BoundedBody>& t_second) {
		Vec3 posFirst = t_first->GetPosition();
		Vec3 posSecond = t_second->GetPosition();
		
		Vec3 scaleFirst = t_first->GetScale(); 
		Vec3 scaleSecond = t_second->GetScale();

		if (posFirst.x - scaleFirst.x < posSecond.x + scaleSecond.x &&
			posFirst.x + scaleFirst.x > posSecond.x - scaleSecond.x && 
			posFirst.y - scaleFirst.y < posSecond.y + scaleSecond.y &&
			posFirst.y + scaleFirst.y > posSecond.y - scaleSecond.y &&
			posFirst.z - scaleFirst.z < posSecond.z + scaleSecond.z &&
			posFirst.z + scaleFirst.z > posSecond.z - scaleSecond.z) {
			return GJK(t_contactPoint, t_first, t_second);
		} else {
			t_contactPoint = Vec3(0.0f);
			return Vec3(0.0f);
		}
	}

	Vec3 GJK(Vec3& t_contactPoint, const std::shared_ptr<BoundedBody>& t_first, const std::shared_ptr<BoundedBody>& t_second) {
		if (t_first->GetPosition() == t_second->GetPosition()) {
			t_contactPoint = Vec3(0.0f);
			return Vec3(0.0f, max(t_first->GetScale().y, t_second->GetScale().y), 0.0f);
		}
		Vec3 searchDir = Vec3(1.0f, 1.0f, 1.0f);
		std::vector<Vec3> simplex = std::vector<Vec3>();

		simplex.push_back(t_first->Support(searchDir) - t_second->Support(-searchDir));
		searchDir = -simplex[0];

		while (true) {	
			searchDir = Vec3::Normalize(searchDir);
			Vec3 newPoint = t_first->Support(searchDir) - t_second->Support(-searchDir);
			if (Vec3::Dot(newPoint, searchDir) < 0.0f) return Vec3(0.0f);//return false; //no collision
			float x = simplex[0].x;
			float y = simplex[0].y;
			float z = simplex[0].z;
			simplex.push_back(newPoint);
			if (DoSimplex(simplex, searchDir)) {
				return EPA(t_contactPoint, simplex, t_first, t_second);
				//return true; //collision
			}
		}
	}

	Vec3 EPA(Vec3& t_contactPoint, std::vector<Vec3> t_simplex, const std::shared_ptr<BoundedBody>& t_first, const std::shared_ptr<BoundedBody>& t_second) {
		//construct triangles
		std::vector<Triangle> triangles = std::vector<Triangle>();
		triangles.push_back(Triangle(t_simplex[0], t_simplex[1], t_simplex[2]));
		triangles.push_back(Triangle(t_simplex[0], t_simplex[1], t_simplex[3]));
		triangles.push_back(Triangle(t_simplex[0], t_simplex[3], t_simplex[2]));
		triangles.push_back(Triangle(t_simplex[1], t_simplex[2], t_simplex[3]));

		int iter = 0;

		while (true) {
			iter++;
			if (iter > 1000) {
				std::string a = "a";
			}
			float minPenetration = Vec3::Dot(triangles[0].normal, triangles[0].a);
			int index = 0;

			for (unsigned int i = 1; i < triangles.size(); ++i) {
				float penetration = Vec3::Dot(triangles[i].normal, triangles[i].a);
				if (penetration < minPenetration) {
					minPenetration = penetration;
					index = i;
				}
			}

			Vec3 newPoint = t_first->Support(triangles[index].normal) - t_second->Support(-triangles[index].normal);

			if (abs(Vec3::Dot(triangles[index].normal, newPoint) - minPenetration) < 0.01f) {
				//gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
				//projection of origin onto the triangle
				Vec3 p = Vec3::Dot(triangles[index].normal, triangles[index].a) * triangles[index].normal;

				Vec3 v0 = triangles[index].b - triangles[index].a;
				Vec3 v1 = triangles[index].c - triangles[index].a;
				Vec3 v2 = p - triangles[index].a;

				float d00 = Vec3::Dot(v0, v0);
				float d01 = Vec3::Dot(v0, v1);
				float d11 = Vec3::Dot(v1, v1);
				float d20 = Vec3::Dot(v2, v0);
				float d21 = Vec3::Dot(v2, v1);

				float denom = d00 * d11 - d01 * d01;

				float v = (d11 * d20 - d01 * d21) / denom; //how much b
				float w = (d00 * d21 - d01 * d20) / denom; //how much c
				float u = 1.0f - v - w; //how much a

				t_contactPoint = t_first->Support(triangles[index].a) * u + t_first->Support(triangles[index].b) * v + t_first->Support(triangles[index].c) * w;

				return triangles[index].normal * (minPenetration + 0.01f);
			}

			std::vector<std::pair<Vec3, Vec3>> edges = std::vector<std::pair<Vec3, Vec3>>();
			for (unsigned int i = 0; i < triangles.size(); ++i) {
				if (Vec3::Dot(triangles[i].normal, newPoint - triangles[i].a) > 0) {
					//remove face
					Triangle temp = triangles[i];
					triangles.erase(triangles.begin() + i);
					--i;
					for each(std::pair<Vec3, Vec3> edge in temp.GetEdges()) {
						bool isReverseInEdges = false;
						int index = 0;
						for (int j = 0; j < edges.size(); ++j) {
							if ((edges[j].first - edge.second).Length() < 0.001f && (edges[j].second - edge.first).Length() < 0.001f) {
								isReverseInEdges = true;
								index = j;
								break;
							}
						}
						if (isReverseInEdges) {
							edges.erase(edges.begin() + index);
						} else {
							edges.push_back(edge);
						}
					}
				}
			}

			for each(std::pair<Vec3, Vec3> edge in edges) {
				triangles.push_back(Triangle(edge.first, edge.second, newPoint));
			}
		}
	}

	std::vector<Vec3> TestGJK(Vec3& t_contactPoint, const std::shared_ptr<BoundedBody>& t_first, const std::shared_ptr<BoundedBody>& t_second) {
		if (t_first->GetPosition() == t_second->GetPosition()) {
			t_contactPoint = Vec3(0.0f);
			return std::vector<Vec3>();
			//return Vec3(0.0f, max(t_first->GetScale().y, t_second->GetScale().y), 0.0f);
		}

		Vec3 searchDir = Vec3(1.0f, 1.0f, 1.0f);
		std::vector<Vec3> simplex = std::vector<Vec3>();

		simplex.push_back(t_first->Support(searchDir) - t_second->Support(-searchDir));
		searchDir = -simplex[0];

		while (true) {
			searchDir = Vec3::Normalize(searchDir);
			Vec3 newPoint = t_first->Support(searchDir) - t_second->Support(-searchDir);
			if (Vec3::Dot(newPoint, searchDir) < 0.0f) return std::vector<Vec3>();//return false; //no collision
			float x = simplex[0].x;
			float y = simplex[0].y;
			float z = simplex[0].z;
			simplex.push_back(newPoint);
			if (DoSimplex(simplex, searchDir)) {
				return simplex;
				//return true; //collision
			}
		}
	}
	Vec3 TestEPA(std::vector<Triangle>& triangles, Vec3& t_contactPoint, std::vector<Vec3> t_simplex, const std::shared_ptr<BoundedBody>& t_first, const std::shared_ptr<BoundedBody>& t_second) {
		//construct triangles
		//triangles.push_back(Triangle(t_simplex[0], t_simplex[1], t_simplex[2]));
		//triangles.push_back(Triangle(t_simplex[0], t_simplex[1], t_simplex[3]));
		//triangles.push_back(Triangle(t_simplex[0], t_simplex[3], t_simplex[2]));
		//triangles.push_back(Triangle(t_simplex[1], t_simplex[2], t_simplex[3]));

		
		float minPenetration = Vec3::Dot(triangles[0].normal, triangles[0].a);
		int index = 0;
		Vec3 smallest = minPenetration * triangles[0].normal;

		for (unsigned int i = 1; i < triangles.size(); ++i) {
			float penetration = Vec3::Dot(triangles[i].normal, triangles[i].a);
			if (penetration < minPenetration) {
				minPenetration = penetration;
				index = i;
				smallest = minPenetration * triangles[i].normal;
			}
		}

		Vec3 newPoint = t_first->Support(triangles[index].normal) - t_second->Support(-triangles[index].normal);

		if (abs(Vec3::Dot(triangles[index].normal, newPoint) - minPenetration) < 0.1f) {
			//gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
			//projection of origin onto the triangle
			Vec3 p = Vec3::Dot(triangles[index].normal, triangles[index].a) * triangles[index].normal;

			Vec3 v0 = triangles[index].b - triangles[index].a;
			Vec3 v1 = triangles[index].c - triangles[index].a;
			Vec3 v2 = p - triangles[index].a;

			float d00 = Vec3::Dot(v0, v0);
			float d01 = Vec3::Dot(v0, v1);
			float d11 = Vec3::Dot(v1, v1);
			float d20 = Vec3::Dot(v2, v0);
			float d21 = Vec3::Dot(v2, v1);

			float denom = d00 * d11 - d01 * d01;

			float v = (d11 * d20 - d01 * d21) / denom; //how much b
			float w = (d00 * d21 - d01 * d20) / denom; //how much c
			float u = 1.0f - v - w; //how much a

			t_contactPoint = t_first->Support(triangles[index].a) * u + t_first->Support(triangles[index].b) * v + t_first->Support(triangles[index].c) * w;

			return triangles[index].normal * (minPenetration + 0.01f);
		}

		std::vector<std::pair<Vec3, Vec3>> edges = std::vector<std::pair<Vec3, Vec3>>();
		for (unsigned int i = 0; i < triangles.size(); ++i) {
			if (Vec3::Dot(triangles[i].normal, newPoint - triangles[i].a) > 0) {
				//remove face
				Triangle temp = triangles[i];
				triangles.erase(triangles.begin() + i);
				--i;
				for each(std::pair<Vec3, Vec3> edge in temp.GetEdges()) {
					bool isReverseInEdges = false;
					int index = 0;
					for (int j = 0; j < edges.size(); ++j) {
						if ((edges[j].first - edge.second).Length() < 0.001f && (edges[j].second - edge.first).Length() < 0.001f) {
							isReverseInEdges = true;
							index = j;
							break;
						}
						if ((edges[j].first - edge.first).Length() < 0.001f && (edges[j].second - edge.second).Length() < 0.001f) {
							isReverseInEdges = true;
							index = j;
							break;
						}
					}
					if (isReverseInEdges) {
						edges.erase(edges.begin() + index);
					} else {
						edges.push_back(edge);
					}
				}
			}
		}

		for each(std::pair<Vec3, Vec3> edge in edges) {
			triangles.push_back(Triangle(edge.first, edge.second, newPoint));
		}

		return smallest;
	}



private:
	//a is always the newly added point
	bool DoSimplex(std::vector<Vec3>& t_simplex, Vec3& t_searchDir) {
		unsigned int simplexSize = t_simplex.size();

		if (simplexSize == 2) {
			return DoSimplex2(t_simplex, t_searchDir);
		} else if (simplexSize == 3) {
			return DoSimplex3(t_simplex, t_searchDir);
		} else {
			return DoSimplex4(t_simplex, t_searchDir);
		}
	}

	inline bool DoSimplex2(std::vector<Vec3>& t_simplex, Vec3& t_searchDir) {
		//simplex is: b, a
		Vec3 ab = t_simplex[0] - t_simplex[1];
		Vec3 ao = -t_simplex[1];

		if (Vec3::Dot(ab, ao) > 0) {
			t_searchDir = Vec3::Cross(Vec3::Cross(ab, ao), ab);
		} else {
			t_simplex.clear();
			t_simplex.push_back(-ao);
			t_searchDir = ao;
		}

		return false;
	}
	inline bool DoSimplex3(std::vector<Vec3>& t_simplex, Vec3& t_searchDir) {
		//simplex is: b, c, a
		Vec3 ab = t_simplex[0] - t_simplex[2];
		Vec3 ac = t_simplex[1] - t_simplex[2];
		Vec3 abc = Vec3::Cross(ab, ac);
		Vec3 ao = -t_simplex[2];

		if (Vec3::Dot(Vec3::Cross(abc, ac), ao) > 0) {
			if (Vec3::Dot(ac, ao) > 0) {
				//ac is closest
				Vec3 oc = t_simplex[1];
				t_simplex.clear();
				t_simplex.push_back(-ao);
				t_simplex.push_back(oc);
				t_searchDir = Vec3::Cross(Vec3::Cross(ac, ao), ac);
			} else {
				if (Vec3::Dot(ab, ao) > 0){
					//ab is closest
					Vec3 ob = t_simplex[0];
					t_simplex.clear();
					t_simplex.push_back(-ao);
					t_simplex.push_back(ob);
					t_searchDir = Vec3::Cross(Vec3::Cross(ab, ao), ab);
				} else {
					//a is closest
					t_simplex.clear();
					t_simplex.push_back(-ao);
					t_searchDir = ao;
				}
			}
		} else if (Vec3::Dot(Vec3::Cross(ab, abc), ao) > 0) {
			if (Vec3::Dot(ab, ao) > 0) {
				//ab is closest
				Vec3 ob = t_simplex[0];
				t_simplex.clear();
				t_simplex.push_back(-ao);
				t_simplex.push_back(ob);
				t_searchDir = Vec3::Cross(Vec3::Cross(ab, ao), ab);
			} else {
				//a is closest
				t_simplex.clear();
				t_simplex.push_back(-ao);
				t_searchDir = ao;
			}
		} else { //up, down
			if (Vec3::Dot(abc, ao) > 0) {
				t_searchDir = abc; //abc is closest from above
			} else {
				Vec3 temp = t_simplex[1];
				t_simplex[1] = t_simplex[2];
				t_simplex[2] = temp;
				t_searchDir = -abc; //abc is closest from below
			}
		}

		return false;
	}
	inline bool DoSimplex4(std::vector<Vec3>& t_simplex, Vec3& t_searchDir) {
		//simplex is: b, c, d, a
		Vec3 nullVec = Vec3(0.0f);
		Vec3 ao = -t_simplex[3];
		Vec3 ab = t_simplex[0] - t_simplex[3];
		Vec3 ac = t_simplex[1] - t_simplex[3];
		Vec3 ad = t_simplex[2] - t_simplex[3];

		Vec3 abc = Vec3::Normalize(Vec3::Cross(ab, ac));
		Vec3 adb = Vec3::Normalize(Vec3::Cross(ad, ab));
		Vec3 acd = Vec3::Normalize(Vec3::Cross(ac, ad));

		float abcDistance = Vec3::Dot(abc, ao);
		float adbDistance = Vec3::Dot(adb, ao);
		float acdDistance = Vec3::Dot(acd, ao);

		if (abcDistance > adbDistance) {
			if (abcDistance > acdDistance) {
				//abc
				if (abcDistance < 0.0f) return true;
				t_simplex[2] = t_simplex[1];
				t_simplex[1] = t_simplex[0];
				t_simplex[0] = t_simplex[3];
				t_simplex.pop_back();
				return DoSimplex3(t_simplex, t_searchDir);
			} else {
				//acd
				if (acdDistance < 0.0f) return true;
				t_simplex[0] = t_simplex[3];
				t_simplex.pop_back();
				return DoSimplex3(t_simplex, t_searchDir);
			}
		} else {
			if (adbDistance > acdDistance) {
				//adb
				if (adbDistance < 0.0f) return true;
				t_simplex[1] = t_simplex[2];
					t_simplex[2] = t_simplex[0];
					t_simplex[0] = t_simplex[3];
					t_simplex.pop_back();
					return DoSimplex3(t_simplex, t_searchDir);
			} else {
				//acd
				if (acdDistance < 0.0f) return true;
				t_simplex[0] = t_simplex[3];
				t_simplex.pop_back();
				return DoSimplex3(t_simplex, t_searchDir);
			}
		}
	}
};