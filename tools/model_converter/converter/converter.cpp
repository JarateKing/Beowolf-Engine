#include "pch.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <stack>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>
#include <set>
#include <vector>
#include <utility>

void writeTransform(std::ofstream& outfile, aiMatrix4x4 mat) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			outfile.write((char*)&(mat[j][i]), sizeof(float));
}

aiMatrix4x4 interpolateMatrices(aiMatrix4x4 a, aiMatrix4x4 b, float percent) {
	aiMatrix4x4 toret = b;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			toret[i][j] = a[i][j] * (1.0f - percent) + b[i][j] * (percent);

	return toret;
}

bool convert(std::string filename) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_GenUVCoords |
		aiProcess_SortByPType);

	if (!scene)
		return false;

	std::string outfileName = filename.substr(0, filename.length() - 3) + "bmw";
	std::ofstream outfile;
	char null = 0;
	char* nullval = &null;
	outfile.open(outfileName, std::ios::binary);
	std::string header = "Beo-Model-Wolf Format Version 1";
	outfile.write(header.c_str(), sizeof(char) * header.size());
	outfile.write(nullval, sizeof(char));

	outfile.write((char*)&scene->mNumMaterials, sizeof(unsigned int));
	for (int i = 0; i < scene->mNumMaterials; i++) {
		unsigned int count = scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);
		outfile.write((char*)&count, sizeof(unsigned int));
		for (int j = 0; j < scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE); j++) {
			aiString tex;
			scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, j, &tex);
			outfile.write(tex.C_Str(), tex.length);
			outfile.write(nullval, sizeof(char));
		}
	}

	aiMatrix4x4 globalInverse = *(&scene->mRootNode->mTransformation);
	globalInverse.Inverse();

	std::set<int> boneIDs;
	std::map<int, std::string> boneNames;
	std::vector<int> nodeOrder;
	std::vector<int> boneOrder;
	std::map<std::string, int> nodeIDs;
	std::map<int, int> nodeParentIDs;
	std::map<int, aiMatrix4x4> nodeTransforms;
	std::map<int, aiMatrix4x4> nodeOffsets;
	std::stack<aiNode* const*> idNodes;
	idNodes.push(&scene->mRootNode);
	while (idNodes.size() > 0) {
		aiNode* current = *(idNodes.top());
		idNodes.pop();
		nodeTransforms[nodeIDs.size()] = current->mTransformation;
		nodeIDs[current->mName.C_Str()] = nodeIDs.size();
		nodeOrder.push_back(nodeIDs.size());
		if (current->mParent != NULL) {
			nodeParentIDs[nodeIDs[current->mName.C_Str()]] = nodeIDs[current->mParent->mName.C_Str()];
		}
		for (int i = 0; i < current->mNumChildren; i++)
			idNodes.push(&(current->mChildren[i]));

		boneNames[nodeIDs[current->mName.C_Str()]] = current->mName.C_Str();
	}

	for (int i = 0; i < scene->mNumMeshes; i++) {
		for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++) {
			boneIDs.insert(nodeIDs[scene->mMeshes[i]->mBones[j]->mName.C_Str()]);
			boneNames[nodeIDs[scene->mMeshes[i]->mBones[j]->mName.C_Str()]] = scene->mMeshes[i]->mBones[j]->mName.C_Str();
			nodeOffsets[nodeIDs[scene->mMeshes[i]->mBones[j]->mName.C_Str()]] = scene->mMeshes[i]->mBones[j]->mOffsetMatrix;
		}
	}
	for (int i = 0; i < scene->mNumAnimations; i++) {
		for (int j = 0; j < scene->mAnimations[i]->mNumChannels; j++) {
			boneIDs.insert(nodeIDs[scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str()]);
			boneNames[nodeIDs[scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str()]] = scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();
		}
	}

	for (int i = 0; i < nodeOrder.size(); i++)
		if (boneIDs.count(nodeOrder[i]))
			boneOrder.push_back(nodeOrder[i]);

	outfile.write((char*)&scene->mNumAnimations, sizeof(unsigned int));
	for (int i = 0; i < scene->mNumAnimations; i++) {
		unsigned int duration = scene->mAnimations[i]->mDuration;
		unsigned int rate = scene->mAnimations[i]->mTicksPerSecond;
		outfile.write((char*)&duration, sizeof(unsigned int));
		outfile.write((char*)&rate, sizeof(unsigned int));
		outfile.write((char*)&(scene->mAnimations[i]->mNumChannels), sizeof(unsigned int));

		std::map<int, std::map<int, aiMatrix4x4>> animKeyframes;
		for (int v = 0; v < boneOrder.size(); v++) {
			for (int j = 0; j < scene->mAnimations[i]->mNumChannels; j++) {
				unsigned int curID = nodeIDs[scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str()];
				// make sure this is being done in order, in the hackiest way possible
				if (curID == boneOrder[v]) {
					outfile.write((char*)&(curID), sizeof(unsigned int));

					// figure out transforms for each frame
					std::vector<double> timestamps[3];
					std::vector<aiMatrix4x4> frameTransforms[3];

					for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++) {
						aiMatrix4x4 pos = aiMatrix4x4();
						aiMatrix4x4::Translation(scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue, pos);

						timestamps[0].push_back(scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime);
						frameTransforms[0].push_back(pos);
					}
					for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumRotationKeys; k++) {
						aiMatrix4x4 rot = aiMatrix4x4(scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.GetMatrix());

						timestamps[1].push_back(scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime);
						frameTransforms[1].push_back(rot);
					}
					for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumScalingKeys; k++) {
						aiMatrix4x4 scale = aiMatrix4x4();
						aiMatrix4x4::Scaling(scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue, scale);

						timestamps[2].push_back(scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mTime);
						frameTransforms[2].push_back(scale);
					}

					// interpolate frames over entire range
					std::vector<aiMatrix4x4> interpolateds[3];
					for (int l = 0; l < 3; l++) {
						int ki = 0;
						double ko = timestamps[l][ki];
						for (int k = 0; k <= ko; k++) {
							interpolateds[l].push_back(interpolateMatrices(aiMatrix4x4(), frameTransforms[l][ki], k / ko));
						}
						ki++;
						while (ki < timestamps[l].size()) {
							double kn = timestamps[l][ki];
							for (int k = ko + 1; k <= kn; k++) {
								interpolateds[l].push_back(interpolateMatrices(frameTransforms[l][ki - 1], frameTransforms[l][ki], (k - ko) / (kn)));
							}
							ko = kn;
							ki++;
						}
						for (int k = ko + 1; k <= scene->mAnimations[i]->mDuration; k++) {
							interpolateds[l].push_back(frameTransforms[l][(ki < frameTransforms[l].size()) ? ki : 0]);
						}
					}

					// get the animated parent id
					aiMatrix4x4 cumulative = aiMatrix4x4();
					int nextid = nodeParentIDs[curID];
					while (nodeParentIDs.count(nextid) != 0 && animKeyframes.count(nextid) == 0) {
						cumulative = nodeTransforms[nextid] * cumulative;
						nextid = nodeParentIDs[nextid];
					}

					// apply parent transforms and write
					for (int k = 0; k <= scene->mAnimations[i]->mDuration; k++) {
						aiMatrix4x4 parentTrans = nodeTransforms[nextid];
						if (animKeyframes.count(nextid) != 0)
							parentTrans = animKeyframes[nextid][k];

						animKeyframes[curID][k] = parentTrans * cumulative * interpolateds[0][k] * interpolateds[1][k] * interpolateds[2][k];
						writeTransform(outfile, globalInverse * animKeyframes[curID][k] * nodeOffsets[curID]);
					}
				}
			}
		}
	}
	
	outfile.write((char*)&scene->mNumMeshes, sizeof(unsigned int));
	for (int i = 0; i < scene->mNumMeshes; i++) {

		std::map<int, std::vector<std::pair<int, float>>> boneWeights;

		//outfile.write((char*)&scene->mMeshes[i]->mNumBones, sizeof(unsigned int));
		for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++) {
			//writeTransform(outfile, scene->mMeshes[i]->mBones[j]->mOffsetMatrix);
			for (int k = 0; k < scene->mMeshes[i]->mBones[j]->mNumWeights; k++)
				boneWeights[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].push_back({ nodeIDs[scene->mMeshes[i]->mBones[j]->mName.C_Str()], scene->mMeshes[i]->mBones[j]->mWeights[k].mWeight });
		}

		int boneWeightsSize = boneWeights.size();
		outfile.write((char*)&boneWeightsSize, sizeof(unsigned int));
		for (auto it : boneWeights) {
			outfile.write((char*)&it.first, sizeof(unsigned int));
			int size = it.second.size();
			outfile.write((char*)&size, sizeof(unsigned int));
			for (int j = 0; j < size; j++) {
				outfile.write((char*)&it.second[j].first, sizeof(unsigned int));
				outfile.write((char*)&it.second[j].second, sizeof(float));
			}
		}

		outfile.write((char*)&scene->mMeshes[i]->mNumVertices, sizeof(unsigned int));
		for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
			// points
			outfile.write((char*)&scene->mMeshes[i]->mVertices[j].x, sizeof(float));
			outfile.write((char*)&scene->mMeshes[i]->mVertices[j].y, sizeof(float));
			outfile.write((char*)&scene->mMeshes[i]->mVertices[j].z, sizeof(float));

			// vertex color
			if (scene->mMeshes[i]->HasVertexColors(0)) {
				int r = (int)(255 * scene->mMeshes[i]->mColors[0][j].r);
				int g = (int)(255 * scene->mMeshes[i]->mColors[0][j].g);
				int b = (int)(255 * scene->mMeshes[i]->mColors[0][j].b);
				int a = (int)(255 * scene->mMeshes[i]->mColors[0][j].a);
				outfile.write((char*)&r, sizeof(int));
				outfile.write((char*)&g, sizeof(int));
				outfile.write((char*)&b, sizeof(int));
				outfile.write((char*)&a, sizeof(int));
			}
			else {
				int rgbaval = 255;
				outfile.write((char*)&rgbaval, sizeof(int));
				outfile.write((char*)&rgbaval, sizeof(int));
				outfile.write((char*)&rgbaval, sizeof(int));
				outfile.write((char*)&rgbaval, sizeof(int));
			}

			// texture uv
			if (scene->mMeshes[i]->HasTextureCoords(0)) {
				outfile.write((char*)&scene->mMeshes[i]->mTextureCoords[0][j].x, sizeof(float));
				outfile.write((char*)&scene->mMeshes[i]->mTextureCoords[0][j].y, sizeof(float));
			}
			else {
				float defuv = 0.0f;
				outfile.write((char*)&defuv, sizeof(float));
				outfile.write((char*)&defuv, sizeof(float));
			}

			// normal mapping
			if (scene->mMeshes[i]->HasNormals()) {
				outfile.write((char*)&scene->mMeshes[i]->mNormals[j].x, sizeof(float));
				outfile.write((char*)&scene->mMeshes[i]->mNormals[j].y, sizeof(float));
				outfile.write((char*)&scene->mMeshes[i]->mNormals[j].z, sizeof(float));
			}
			else {
				int defnormal = 0.0f;
				int defnormal2 = 1.0f;
				outfile.write((char*)&defnormal, sizeof(float));
				outfile.write((char*)&defnormal2, sizeof(float));
				outfile.write((char*)&defnormal, sizeof(float));
			}
		}
		outfile.write((char*)&scene->mMeshes[i]->mNumFaces, sizeof(unsigned int));
		for (int j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
			for (int k = 0; k < 3; k++)
				outfile.write((char*)&scene->mMeshes[i]->mFaces[j].mIndices[k], sizeof(unsigned int));
	}

	std::stack<aiNode* const*> nodes;
	nodes.push(&scene->mRootNode);
	while (nodes.size() > 0) {
		aiNode* current = *(nodes.top());
		nodes.pop();

		outfile.write((char*)&nodeIDs[current->mName.C_Str()], sizeof(unsigned int));

		writeTransform(outfile, current->mTransformation);

		outfile.write((char*)&(current->mNumMeshes), sizeof(unsigned int));
		for (int i = 0; i < current->mNumMeshes; i++)
			outfile.write((char*)&(current->mMeshes[i]), sizeof(unsigned int));

		outfile.write((char*)&(current->mNumChildren), sizeof(unsigned int));
		for (int i = 0; i < current->mNumChildren; i++)
			nodes.push(&(current->mChildren[i]));
	}

	// print out list of bones with their id
	unsigned int boneFinalCount = boneNames.size();
	outfile.write((char*)&(boneFinalCount), sizeof(unsigned int));

	for (auto it : boneNames) {
		outfile.write((char*)&(it.first), sizeof(unsigned int));
		outfile.write(it.second.c_str(), sizeof(char) * (it.second.length()));
		outfile.write(nullval, sizeof(char));
	}

	outfile.close();

	return true;
}

int main(int argc, char** argv)
{
	for (int i = 1; i < argc; i++) {
		std::ifstream fbxFile(argv[i]);
		bool isGood = fbxFile.good();
		fbxFile.close();
		if (!fbxFile.good()) {
			std::cout << "Couldn't access file " << argv[i] << "\n";
		}
		else {
			std::cout << "Converting " << argv[i] << "\n";
			bool isGood = convert(std::string(argv[i]));

			if (!isGood)
				std::cout << "Failed to convert " << argv[i] << "\n";
			else
				std::cout << "Success!\n";
		}
	}
}
