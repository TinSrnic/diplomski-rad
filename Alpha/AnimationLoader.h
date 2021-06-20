#pragma once
#include <glfw3.h>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "Math.h"
#include "StringManipulation.h"
#include "AnimatedModel.h"

class AnimationLoader {

private:
	std::map<std::string, AnimatedModel> m_animatedModels = std::map<std::string, AnimatedModel>();

public:

	AnimationLoader() { }
	AnimatedModel LoadDAE(std::string t_filepath, std::string t_texturePath) {
		try {
			return m_animatedModels.at(t_filepath);
		} catch (std::out_of_range ex) {
			return m_animatedModels.insert(std::pair<std::string, AnimatedModel>(t_filepath, ParseAll(t_filepath, t_texturePath))).first->second;
		}
	}

private:
	AnimatedModel ParseAll(std::string t_filePath, std::string t_texturePath) {
		std::ifstream file = std::ifstream("..\\Res\\Animation\\" + t_filePath);
		std::string line = "";

		//normal mesh stuff
		GLfloat* positionArray;
		GLfloat* textureArray;
		GLfloat* normalArray;
		GLuint* indicesArray;
		unsigned int vertexCount;
		unsigned int indicesCount;
		
		//animation stuff that has to go into model along with pos, tex and normal
		GLint* jointArray;
		GLfloat* weightArray;

		//animation stuff
		Animation* animation = nullptr;
		std::vector<Bone*> bones = std::vector<Bone*>();

		//texture stuff
		GLuint textureID = TextureLoader::LoadTexture(t_texturePath);

		//begin parse
		while (StringManipulation::RemoveStartBlank(line) != "<library_geometries>") { std::getline(file, line); }
		std::map<Vec3i, GLuint, Vec3i::Vec3iMapCompare> map = ParseModel(file, positionArray, textureArray, normalArray, indicesArray, &vertexCount, &indicesCount); //parse actual model mesh

		while (StringManipulation::RemoveStartBlank(line) != "<library_animations>") { std::getline(file, line); }
		ParseAnimation(file, bones); //parse animations

		while (StringManipulation::RemoveStartBlank(line) != "<library_controllers>") { std::getline(file, line); }
		ParseBoneWeight(file, jointArray, weightArray, bones, map); //parse bone and weight don't forget to normalize weights

		while (StringManipulation::RemoveStartBlank(line) != "<library_visual_scenes>") { std::getline(file, line); }
		ParseHierarchy(file, bones, animation); //parse hierarchy

		return AnimatedModel(positionArray, textureArray, normalArray, jointArray, weightArray, indicesArray, vertexCount, indicesCount, animation, textureID);
	}

	std::map<Vec3i, GLuint, Vec3i::Vec3iMapCompare> ParseModel(std::ifstream& t_file, GLfloat*& positionArray, GLfloat*& textureArray, GLfloat*& normalArray, GLuint*& indicesArray, unsigned int* vertexCount, unsigned int* indicesCount) {
		std::map<Vec3i, GLuint, Vec3i::Vec3iMapCompare> map;
		std::vector<Vec3*> vertices;
		std::vector<Vec2*> textures;
		std::vector<Vec3*> normals;
		std::vector<GLuint> indices;

		std::vector<Vec3i> faces;
		unsigned int currentIndCount = 0;
		std::string line = "";
		std::string blankless = "";

		//get verts
		while ((blankless = StringManipulation::RemoveStartBlank(line)).substr(0, 43) != "<float_array id=\"Cube-mesh-positions-array\"") { std::getline(t_file, line);  }
		std::string vertText;
		for (unsigned int x = 0; x < blankless.size(); x++) {
			if (blankless[x] == '>') {
				vertText = blankless.substr(x + 1, blankless.size());
				vertText = vertText.substr(0, vertText.size() - 14);
				break;
			}
		}
		std::vector<std::string> vertValues = StringManipulation::SplitString(vertText, ' ');
		for (unsigned int i = 0; i < vertValues.size(); i += 3) {
			float x = std::stof(vertValues[i + 0]);
			float y = std::stof(vertValues[i + 1]);
			float z = std::stof(vertValues[i + 2]);

			Vec3 position = Vec3(x, y, z);
			vertices.push_back(new Vec3(Mat4::Rotation(-90, Vec3(1.0f, 0.0f, 0.0f)) * position));
			//vertices.push_back(new Vec3(x, z, -y)); //must switch y and z because in blender z is up and here y is up
			//-y because rotation just put your fingers in that funny pose, you'll get it
		}


		//get normals
		while ((blankless = StringManipulation::RemoveStartBlank(line)).substr(0, 41) != "<float_array id=\"Cube-mesh-normals-array\"") { std::getline(t_file, line); }
		std::string normalText;
		for (unsigned int x = 0; x < blankless.size(); x++) {
			if (blankless[x] == '>') {
				normalText = blankless.substr(x + 1, blankless.size());
				normalText = normalText.substr(0, normalText.size() - 14);
				break;
			}
		}
		std::vector<std::string> normalValues = StringManipulation::SplitString(normalText, ' ');
		for (unsigned int i = 0; i < normalValues.size(); i += 3) {
			float x = std::stof(normalValues[i + 0]);
			float y = std::stof(normalValues[i + 1]);
			float z = std::stof(normalValues[i + 2]);
			normals.push_back(new Vec3(x, z, -y)); //must switch y and z because in blender z is up and here y is up
		}


		//get uv coordinates
		while ((blankless = StringManipulation::RemoveStartBlank(line)).substr(0, 39) != "<float_array id=\"Cube-mesh-map-0-array\"") { std::getline(t_file, line); }
		std::string textureText;
		for (unsigned int x = 0; x < blankless.size(); x++) {
			if (blankless[x] == '>') {
				textureText = blankless.substr(x + 1, blankless.size());
				textureText = textureText.substr(0, textureText.size() - 14);
				break;
			}
		}
		std::vector<std::string> textureValues = StringManipulation::SplitString(textureText, ' ');
		for (unsigned int i = 0; i < textureValues.size(); i += 2) {
			float x = std::stof(textureValues[i + 0]);
			float y = std::stof(textureValues[i + 1]);
			textures.push_back(new Vec2(x, y));
		}


		//get faces
		while ((blankless = StringManipulation::RemoveStartBlank(line)).substr(0, 3) != "<p>") { std::getline(t_file, line); }
		//TODO: make better by taking vcount into account
		std::string faceText;
		for (unsigned int x = 0; x < blankless.size(); x++) {
			if (blankless[x] == '>') {
				faceText = blankless.substr(x + 1, blankless.size());
				faceText = faceText.substr(0, faceText.size() - 4);
				break;
			}
		}
		unsigned int currentCount = 0;
		std::vector<std::string> faceValues = StringManipulation::SplitString(faceText, ' ');
		for (unsigned int i = 0; i < faceValues.size(); i += 4) {
			unsigned int x = std::stoi(faceValues[i + 0]);
			unsigned int y = std::stoi(faceValues[i + 1]);
			unsigned int z = std::stoi(faceValues[i + 2]);
			//color unused
			faces.push_back(Vec3i(x, z, y)); // z and y reversed because tex and normal are reverse in dae
		}

		//order up
		for (unsigned int x = 0; x < faces.size(); ++x) {
			Vec3i data = faces.at(x);

			if (map.find(data) != map.end()) {
				indices.push_back(map.at(data));
			} else {
				map.insert(std::pair<Vec3i, GLuint>(data, currentIndCount));
				indices.push_back(currentIndCount);
				currentIndCount++;
			}
		}

		indicesArray = (GLuint*)malloc(sizeof(GLuint) * indices.size());
		positionArray = (GLfloat*)malloc(sizeof(GLfloat) * map.size() * 3);
		textureArray = (GLfloat*)malloc(sizeof(GLfloat) * map.size() * 2);
		normalArray = (GLfloat*)malloc(sizeof(GLfloat) * map.size() * 3);

		for each(std::pair<Vec3i, GLuint> pair in map) {
			positionArray[pair.second * 3 + 0] = vertices.at(pair.first.x)->x;
			positionArray[pair.second * 3 + 1] = vertices.at(pair.first.x)->y;
			positionArray[pair.second * 3 + 2] = vertices.at(pair.first.x)->z;

			textureArray[pair.second * 2 + 0] = textures.at(pair.first.y)->x;
			textureArray[pair.second * 2 + 1] = textures.at(pair.first.y)->y;

			normalArray[pair.second * 3 + 0] = normals.at(pair.first.z)->x;
			normalArray[pair.second * 3 + 1] = normals.at(pair.first.z)->y;
			normalArray[pair.second * 3 + 2] = normals.at(pair.first.z)->z;
		}

		for (unsigned int x = 0; x < indices.size(); x++) indicesArray[x] = indices.at(x);
		*vertexCount = map.size();
		*indicesCount = indices.size();

		return map;
	}
	void ParseAnimation(std::ifstream& t_file, std::vector<Bone*>& t_bones) {
		std::string line = "";
		std::string blankless = "";
		std::string blanklessTop = "";
		
		int boneIndex = 0;

		while ((blanklessTop = StringManipulation::RemoveStartBlank(line)) != "</library_animations>") {
			std::getline(t_file, line);
			if (blanklessTop.substr(0, 15) != "<animation id=\"") continue;

			//go to time stamps
			while ((blankless = StringManipulation::RemoveStartBlank(line)).substr(0, 17) != "<float_array id=\"") { std::getline(t_file, line); }
			Bone* bone = new Bone(boneIndex, "", Mat4::Identity());
			++boneIndex;

			std::string timeStampText;
			for (unsigned int x = 0; x < blankless.size(); x++) {
				if (blankless[x] == '>') {
					timeStampText = blankless.substr(x + 1, blankless.size());
					timeStampText = timeStampText.substr(0, timeStampText.size() - 14);
					break;
				}
			}

			std::vector<std::string> timeStampsVector = StringManipulation::SplitString(timeStampText, ' ');
			for each(std::string timeStamp in timeStampsVector) {
				bone->AddKeyFrame(KeyFrame(std::stof(timeStamp), Vec3(0.0f), Quaternion(0.0f)));
			}

			std::getline(t_file, line);
			//go to transforms for each timestamp
			while ((blankless = StringManipulation::RemoveStartBlank(line)).substr(0, 17) != "<float_array id=\"") { std::getline(t_file, line); }

			std::string matrixText;
			for (unsigned int x = 0; x < blankless.size(); x++) {
				if (blankless[x] == '>') {
					matrixText = blankless.substr(x + 1, blankless.size());
					matrixText = matrixText.substr(0, matrixText.size() - 14);
					break;
				}
			}

			std::vector<std::string> matrixVector = StringManipulation::SplitString(matrixText, ' ');
			std::vector<float> matrixInfo;
			for each(std::string element in matrixVector) matrixInfo.push_back(std::stof(element));

			for (unsigned int i = 0; i < (int)(matrixInfo.size() / 16); ++i) {
				Mat4 matrix = Mat4();
				for (unsigned int j = 0; j < 16; j++) matrix[j] = matrixInfo[i * 16 + j];
				matrix = Mat4::Transpose(matrix);
				if (bone->GetIndex() == 0) { //if is root
					matrix = Mat4::Rotation(-90, Vec3(1.0f, 0.0f, 0.0f)) * matrix;
				}

				Vec3 position = Vec3(matrix[12], matrix[13], matrix[14]);
				Quaternion rotation = matrix.ConvertToQuaternion();

				bone->GetKeyFrame(i)->SetPosition(position);
				bone->GetKeyFrame(i)->SetRotation(rotation);
			}

			t_bones.push_back(bone);
		}
	}
	void ParseBoneWeight(std::ifstream& t_file, GLint*& t_jointArray, GLfloat*& t_weightArray, std::vector<Bone*>& t_bones, std::map<Vec3i, GLuint, Vec3i::Vec3iMapCompare>& t_map) {

		std::string line = "";
		std::string blankless = "";

		while ((blankless = StringManipulation::RemoveStartBlank(line)).substr(0, 16) != "<Name_array id=\"") { std::getline(t_file, line); }

		std::string BoneText;
		for (unsigned int x = 0; x < blankless.size(); x++) {
			if (blankless[x] == '>') {
				BoneText = blankless.substr(x + 1, blankless.size());
				BoneText = BoneText.substr(0, BoneText.size() - 13);
				break;
			}
		}

		std::vector<std::string> boneVector = StringManipulation::SplitString(BoneText, ' ');
		for (unsigned int x = 0; x < t_bones.size(); ++x) {
			t_bones.at(x)->SetName(boneVector.at(x));
		}

		//invbind transform
		while ((blankless = StringManipulation::RemoveStartBlank(line)).substr(0, 17) != "<float_array id=\"") { std::getline(t_file, line); }

		std::getline(t_file, line);

		//weights
		while ((blankless = StringManipulation::RemoveStartBlank(line)).substr(0, 17) != "<float_array id=\"") { std::getline(t_file, line); }

		std::string weightText;
		for (unsigned int x = 0; x < blankless.size(); x++) {
			if (blankless[x] == '>') {
				weightText = blankless.substr(x + 1, blankless.size());
				weightText = weightText.substr(0, weightText.size() - 14);
				break;
			}
		}

		std::vector<float> weights;
		std::vector<std::string> weightVector = StringManipulation::SplitString(weightText, ' ');

		for each(std::string weight in weightVector) weights.push_back(std::stof(weight));

		std::vector<unsigned int> BoneAmount;
		std::string BoneAmountText;
		while ((blankless = StringManipulation::RemoveStartBlank(line)).substr(0, 8) != "<vcount>") { std::getline(t_file, line); }

		BoneAmountText = blankless.substr(8, blankless.size());
		BoneAmountText = BoneAmountText.substr(0, BoneAmountText.size() - 10);
		std::vector<std::string> BoneAmountVector = StringManipulation::SplitString(BoneAmountText, ' ');

		for each(std::string BoneAmountValue in BoneAmountVector) BoneAmount.push_back(std::stoi(BoneAmountValue));

		std::string vertexInfoText;
		while ((blankless = StringManipulation::RemoveStartBlank(line)).substr(0, 3) != "<v>") { std::getline(t_file, line); }

		vertexInfoText = blankless.substr(3, blankless.size());
		vertexInfoText = vertexInfoText.substr(0, vertexInfoText.size() - 4);
		std::vector<std::string> vertexInfoVector = StringManipulation::SplitString(vertexInfoText, ' ');

		std::vector<unsigned int> vertexValues;
		for each(std::string vertexInfo in vertexInfoVector) vertexValues.push_back(std::stoi(vertexInfo));


		std::vector<GLint> jointVector;
		std::vector<GLfloat> jointWeightVector;

		unsigned int boneProgress = 0;
		for (unsigned int x = 0; x < BoneAmount.size(); ++x) {
			std::vector<GLfloat> nonNormalizedWeightValues = std::vector<GLfloat>();
			std::vector<GLint> unorderedJoints = std::vector<GLint>();
			for (unsigned int y = 0; y < BoneAmount[x] * 2; y += 2) {
				unsigned int boneID = vertexValues[boneProgress * 2 + y + 0];
				float weight = weights[vertexValues[boneProgress * 2 + y + 1]];
				nonNormalizedWeightValues.push_back(weight);
				unorderedJoints.push_back(boneID);
			}
			
			if (nonNormalizedWeightValues.size() > 3) {
				float max1 = 0.0f;
				float max2 = 0.0f;
				float max3 = 0.0f;

				GLint joint1 = -1;
				GLint joint2 = -1;
				GLint joint3 = -1;

				for (unsigned int j = 0; j < nonNormalizedWeightValues.size(); ++j) {
					if(nonNormalizedWeightValues.at(j) > max1) {
						joint3 = joint2;
						joint2 = joint1;
						joint1 = unorderedJoints.at(j);

						max3 = max2;
						max2 = max1;
						max1 = nonNormalizedWeightValues.at(j);
					} else if (nonNormalizedWeightValues.at(j) > max2) {
						joint3 = joint2;
						joint2 = unorderedJoints.at(j);
						
						max3 = max2;
						max2 = nonNormalizedWeightValues.at(j);
					} else if (nonNormalizedWeightValues.at(j) > max3) {
						joint3 = unorderedJoints.at(j);

						max3 = nonNormalizedWeightValues.at(j);
					}
				}

				nonNormalizedWeightValues.clear();
				unorderedJoints.clear();

				nonNormalizedWeightValues.push_back(max1);
				nonNormalizedWeightValues.push_back(max2);
				nonNormalizedWeightValues.push_back(max3);

				jointVector.push_back(joint1);
				jointVector.push_back(joint2);
				jointVector.push_back(joint3);
			} else if (nonNormalizedWeightValues.size() <= 3) {
				for (unsigned int j = 0; j < nonNormalizedWeightValues.size(); ++j) {
					jointVector.push_back(unorderedJoints.at(j));
				}

				unsigned int filler = nonNormalizedWeightValues.size();
				for (unsigned int j = 0; j < 3 - filler; ++j) {
					nonNormalizedWeightValues.push_back(0.0f);
					jointVector.push_back(-1);
				}
			}

			float vectorLength = 0;
			for (unsigned int y = 0; y < nonNormalizedWeightValues.size(); y++) {
				vectorLength += nonNormalizedWeightValues.at(y);
			}

			for (unsigned int y = 0; y < nonNormalizedWeightValues.size(); y++) {
				jointWeightVector.push_back(nonNormalizedWeightValues.at(y) / vectorLength);
			}

			int size1 = jointWeightVector.size();
			int size2 = jointVector.size();
			boneProgress += BoneAmount[x];
		}

		t_jointArray = (GLint*)malloc(sizeof(GLint) * t_map.size() * 3);
		t_weightArray = (GLfloat*)malloc(sizeof(GLfloat) * t_map.size() * 3);

		for each(std::pair<Vec3i, GLuint> pair in t_map) {
			t_jointArray[pair.second * 3 + 0] = jointVector.at(pair.first.x * 3 + 0);
			t_jointArray[pair.second * 3 + 1] = jointVector.at(pair.first.x * 3 + 1);
			t_jointArray[pair.second * 3 + 2] = jointVector.at(pair.first.x * 3 + 2);

			t_weightArray[pair.second * 3 + 0] = jointWeightVector.at(pair.first.x * 3 + 0);
			t_weightArray[pair.second * 3 + 1] = jointWeightVector.at(pair.first.x * 3 + 1);
			t_weightArray[pair.second * 3 + 2] = jointWeightVector.at(pair.first.x * 3 + 2);
		}
	}

	void ParseHierarchy(std::ifstream& t_file, std::vector<Bone*>& t_bones, Animation*& t_animation) {
		
		std::string line = "";
		std::string blankless = "";
		while ((blankless = StringManipulation::RemoveStartBlank(line)).substr(0, 19) != "<node id=\"Armature\"") { std::getline(t_file, line); }
		std::getline(t_file, line);
		while ((blankless = StringManipulation::RemoveStartBlank(line)).substr(0, 10) != "<node id=\"") { std::getline(t_file, line); }
		blankless = blankless.substr(10, blankless.size());
		for (unsigned int x = 0; x < blankless.size(); ++x) {
			if (blankless[x] == '"') {
				blankless = blankless.substr(0, x);
				break;
			}
		}

		for (unsigned int x = 0; x < t_bones.size(); ++x) {
			if (t_bones.at(x)->GetName() == blankless) {
				t_animation = new Animation(t_bones.at(x), 0.8333333f, "myBrandNewAnimation");
				ParseHierarchyRecursive(t_file, t_animation->GetRootBone(), nullptr, t_bones);
				break;
			}
		}
	}
	void ParseHierarchyRecursive(std::ifstream& t_file, Bone* t_parent, Bone* t_grandparent, std::vector<Bone*>& t_bones) {

		Mat4 matrix = Mat4();
		std::string name;
		std::vector<Bone*> children;
		std::string line;
		std::string blankless;

		//get start transform
		while ((blankless = StringManipulation::RemoveStartBlank(line)).substr(0, 24) != "<matrix sid=\"transform\">") { std::getline(t_file, line); }
		std::string transformText = blankless.substr(24, blankless.size());
		transformText = transformText.substr(0, transformText.size() - 9);

		std::vector<std::string> transformVector = StringManipulation::SplitString(transformText, ' ');
		for (unsigned int i = 0; i < transformVector.size(); ++i) { matrix[i] = std::stof(transformVector.at(i)); }
		matrix = Mat4::Transpose(matrix);

		if(t_grandparent == nullptr) t_parent->SetStartTransform(Mat4::Rotation(-90.0f, Vec3(1.0f, 0.0f, 0.0f)) * matrix);
		else t_parent->SetStartTransform(t_grandparent->GetStartTransform() * matrix);

		//get children
		while ((blankless = StringManipulation::RemoveStartBlank(line)).substr(0, 10) != "</node>") {
			std::getline(t_file, line);
			if ((blankless = StringManipulation::RemoveStartBlank(line)).substr(0, 10) != "<node id=\"") continue;

			std::string childName = blankless.substr(10, blankless.size());
			for (unsigned int x = 0; x < childName.size(); x++) {
				if (childName[x] == '"') {
					childName = childName.substr(0, x);
					break;
				}
			}

			for each(Bone* child in t_bones) {
				if (child->GetName() == childName) {
					t_parent->AddChild(child);
					ParseHierarchyRecursive(t_file, child, t_parent, t_bones);
					break;
				}
			}
		}
	}
};