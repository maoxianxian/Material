#include "SkinVertex.h"
SkinVertex::SkinVertex(float x, float y, float z,int i)
{
	WorldPosition = glm::vec3(0, 0, 0);
	localPosition = glm::vec3(x, y, z);
	index = i;
}
void SkinVertex::addAttach(int id, float weight)
{
	attaches.push_back({ id,weight });
}

glm::vec3 SkinVertex::calculatePos(std::vector<glm::mat4> &binds, Skeleton* skel) {
	WorldPosition = glm::vec3(0, 0, 0);
	Worldnormal = glm::vec3(0, 0, 0);
	for (int i = 0; i < attaches.size(); i++)
	{		
		WorldPosition += attaches[i].weight*glm::vec3((skel->GetWorldMatrix(attaches[i].jointId)*glm::inverse(binds[i]))*glm::vec4(localPosition,1));
		Worldnormal += attaches[i].weight*glm::vec3(glm::mat3(skel->GetWorldMatrix(attaches[i].jointId)*glm::inverse(binds[i]))*normal);
	}
	Worldnormal = glm::normalize(Worldnormal);
	/*if (attaches.size() == 2) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				std::cout << glm::inverse(binds[1])[j][i];
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}*/
	//std::cout << WorldPosition.x << " " << WorldPosition.y << " " << WorldPosition.z << std::endl;
	return WorldPosition;
}

SkinVertex::~SkinVertex() {

}
