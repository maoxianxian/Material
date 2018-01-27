#include "Skin.h"
#include "Tokenizer.h"

Skin::Skin(const char * filename, Skeleton* skel)
{
	this->skel = skel;
	Load(filename);
}

Skin::~Skin()
{
	for (int i = 0; i < vertices.size(); i++)
	{
		delete vertices[i];
	}
}

void Skin::Load(const char * filename)
{
	
	Tokenizer scanner;
	scanner.Open(filename);
	char buffer[64];
	scanner.GetToken(buffer);
	if (std::string(buffer) == "positions")
	{
		numberOfVertex = scanner.GetInt();
		scanner.GetToken(buffer);//{
		for (int i = 0; i < numberOfVertex; i++)
		{
			float x = scanner.GetFloat();
			float y = scanner.GetFloat();
			float z = scanner.GetFloat();
			vertices.push_back(new SkinVertex(x, y, z, i));
		}
		scanner.GetToken(buffer);//}
	}
	else
	{
		std::cout << "error reading positions" << std::endl;
		return;
	}

	//normals
	char buffer2[64];
	scanner.GetToken(buffer2);
	if (std::string(buffer2) == "normals")
	{
		scanner.GetInt();
		scanner.GetToken(buffer);//{
		for (int i = 0; i < numberOfVertex; i++)
		{
			float x = scanner.GetFloat();
			float y = scanner.GetFloat();
			float z = scanner.GetFloat();
			vertices[i]->normal = glm::vec3(x, y, z);
		}
		scanner.GetToken(buffer);//}
	}
	else
	{
		std::cout << "error reading normals" << std::endl;
		return;
	}

	//skinweights
	char buffer3[64];
	scanner.GetToken(buffer3);
	if (std::string(buffer3) == "skinweights") {
		scanner.GetInt();
		scanner.GetToken(buffer);//{
		for (int i = 0; i < numberOfVertex; i++)
		{
			int numAttach = scanner.GetInt();
			for (int j = 0; j < numAttach; j++)
			{
				int id = scanner.GetInt();
				float weight = scanner.GetFloat();
				vertices[i]->addAttach(id, weight);
			}
		}
		scanner.GetToken(buffer);//}
	}
	else
	{
		std::cout << "error reading skinweights" <<"  "<< std::endl;
		return;
	}

	//triangles
	char buffer4[64];
	scanner.GetToken(buffer4);
	if (std::string(buffer4) == "triangles") {
		numberOfTriangle = scanner.GetInt();
		scanner.GetToken(buffer);//{
		for (int i = 0; i < numberOfTriangle; i++)
		{
			int a = scanner.GetInt();
			int b = scanner.GetInt();
			int c = scanner.GetInt();
			idx.push_back(a);
			idx.push_back(b);
			idx.push_back(c);
		}
		scanner.GetToken(buffer);//{
	}
	else
	{
		std::cout << "error reading triangles" << std::endl;
		return;
	}

	//binding
	char buffer5[64];
	scanner.GetToken(buffer5);
	if (std::string(buffer5) == "bindings") {
		int bindnumber = scanner.GetInt();
		scanner.GetToken(buffer);//{

		for (int i = 0; i < bindnumber; i++)
		{
			scanner.GetToken(buffer);//matix
			scanner.GetToken(buffer);//{
			float a1 = scanner.GetFloat();
			float a2 = scanner.GetFloat();
			float a3 = scanner.GetFloat();
			float a4 = scanner.GetFloat();
			float a5 = scanner.GetFloat();
			float a6 = scanner.GetFloat();
			float a7 = scanner.GetFloat();
			float a8 = scanner.GetFloat();
			float a9 = scanner.GetFloat();
			float a10 = scanner.GetFloat();
			float a11 = scanner.GetFloat();
			float a12 = scanner.GetFloat();

			binds.push_back(
			glm::mat4(a1, a2, a3,0,
				a4, a5, a6,0,
				a7, a8, a9,0, 
				a10, a11, a12,1 ));
			scanner.GetToken(buffer);//}
			/*binds.push_back(
				glm::mat4(a1, a4, a7, a10,
					a2, a5, a8, a11,
					a3, a6, a9, a12,
					0, 0, 0, 1)); */

		}
		scanner.GetToken(buffer);//}
	}
	else
	{
		std::cout << "error reading binding mat"<< std::endl;
		return;
	}
	

	scanner.Close();
	glGenBuffers(1, &VertexBuffer);
	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(uint), &idx[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	Update();
}

void Skin::Draw(const glm::mat4 &viewProjMtx, uint shader)
{
	//std::cout << vtx.size() << " " << idx.size() << std::endl;
	//std::cout << vtx[239].Position[0]<<" "<< vtx[239].Position[1]<<" "<< vtx[239].Position[2] << std::endl;
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "ModelMtx"), 1, false, (float*)&(glm::mat4(1.0f)));
	glm::mat4 mvpMtx = viewProjMtx;
	glUniformMatrix4fv(glGetUniformLocation(shader, "ModelViewProjMtx"), 1, false, (float*)&mvpMtx);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	uint posLoc = 0;
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), 0);

	uint normLoc = 1;
	glEnableVertexAttribArray(normLoc);
	glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)12);
	glDrawElements(GL_TRIANGLES, numberOfTriangle*3, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(normLoc);
	glDisableVertexAttribArray(posLoc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);
}

void Skin::Update()
{
	vtx.clear();
	for (int i = 0; i < numberOfVertex; i++)
	{
		vtx.push_back({ vertices[i]->calculatePos(binds,skel),vertices[i]->Worldnormal });
	}
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vtx.size() * sizeof(ModelVertex), &vtx[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}