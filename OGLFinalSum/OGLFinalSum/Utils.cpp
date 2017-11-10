#include "Utils.h"

void Utils::setCubeA(std::vector<VertexFormat>& vertices, std::vector<GLuint>& indices)
{
	std::vector<VertexFormat> Vertices;
	//front
	Vertices.push_back(VertexFormat(Position(-0.5f, -0.5f, 0.5f), TexCoord(1.0, 1.0), Normals(0.0f, 0.0f, 1.0)));
	Vertices.push_back(VertexFormat(Position(0.5f, -0.5f, 0.5f), TexCoord(0.0, 1.0), Normals(0.0f, 0.0f, 1.0)));
	Vertices.push_back(VertexFormat(Position(0.5f, 0.5f, 0.5f), TexCoord(0.0, 0.0), Normals(0.0f, 0.0f, 1.0)));
	Vertices.push_back(VertexFormat(Position(-0.5f, 0.5f, 0.5f), TexCoord(1.0, 0.0), Normals(0.0f, 0.0f, 1.0)));

	//back
	Vertices.push_back(VertexFormat(Position(-0.5f, -0.5f, -0.5f), TexCoord(0.0, 1.0), Normals(0.0f, 0.0f, -1.0)));
	Vertices.push_back(VertexFormat(Position(-0.5f, 0.5f, -0.5f), TexCoord(0.0, 0.0), Normals(0.0f, 0.0f, -1.0)));
	Vertices.push_back(VertexFormat(Position(0.5f, 0.5f, -0.5f), TexCoord(1.0, 0.0), Normals(0.0f, 0.0f, -1.0)));
	Vertices.push_back(VertexFormat(Position(0.5f, -0.5f, -0.5f), TexCoord(1.0, 1.0), Normals(0.0f, 0.0f, -1.0)));

	//top
	Vertices.push_back(VertexFormat(Position(-0.5f, 0.5f, -0.5f), TexCoord(0.0, 1.0), Normals(0.0f, 1.0f, 0.0)));
	Vertices.push_back(VertexFormat(Position(-0.5f, 0.5f, 0.5f), TexCoord(1.0, 1.0), Normals(0.0f, 1.0f, 0.0)));
	Vertices.push_back(VertexFormat(Position(0.5f, 0.5f, 0.5f), TexCoord(1.0, 0.0), Normals(0.0f, 1.0f, 0.0)));
	Vertices.push_back(VertexFormat(Position(0.5f, 0.5f, -0.5f), TexCoord(0.0, 0.0), Normals(0.0f, 1.0f, 0.0)));

	//bottom
	Vertices.push_back(VertexFormat(Position(-0.5f, -0.5f, -0.5f), TexCoord(1.0, 1.0), Normals(0.0f, -1.0f, 0.0)));
	Vertices.push_back(VertexFormat(Position(0.5f, -0.5f, -0.5f), TexCoord(0.0, 1.0), Normals(0.0f, -1.0f, 0.0)));
	Vertices.push_back(VertexFormat(Position(0.5f, -0.5f, 0.5f), TexCoord(0.0, 0.0), Normals(0.0f, -1.0f, 0.0)));
	Vertices.push_back(VertexFormat(Position(-0.5f, -0.5f, 0.5f), TexCoord(1.0, 0.0), Normals(0.0f, -1.0f, 0.0)));

	//left
	Vertices.push_back(VertexFormat(Position(-0.5f, -0.5f, 0.5f), TexCoord(0.0, 0.0), Normals(-1.0f, 0.0f, 0.0)));
	Vertices.push_back(VertexFormat(Position(-0.5f, 0.5f, 0.5f), TexCoord(1.0, 0.0), Normals(-1.0f, 0.0f, 0.0)));
	Vertices.push_back(VertexFormat(Position(-0.5f, 0.5f, -0.5f), TexCoord(1.0, 1.0), Normals(-1.0f, 0.0f, 0.0)));
	Vertices.push_back(VertexFormat(Position(-0.5f, -0.5f, -0.5f), TexCoord(0.0, 1.0), Normals(-1.0f, 0.0f, 0.0)));

	//right
	Vertices.push_back(VertexFormat(Position(0.5f, -0.5f, -0.5f), TexCoord(1.0, 0.0), Normals(1.0f, 0.0f, 0.0)));
	Vertices.push_back(VertexFormat(Position(0.5f, 0.5f, -0.5f), TexCoord(0.0, 0.0), Normals(1.0f, 0.0f, 0.0)));
	Vertices.push_back(VertexFormat(Position(0.5f, 0.5f, 0.5), TexCoord(0.0, 1.0), Normals(1.0f, 0.0f, 0.0)));
	Vertices.push_back(VertexFormat(Position(0.5f, -0.5f, 0.5f), TexCoord(1.0, 1.0), Normals(1.0f, 0.0f, 0.0)));



	std::vector<GLuint> Indices;
	//cubeIndices.push_back(IndexFormat(0, 1, 2));
	//cubeIndices.push_back(IndexFormat(0, 2, 3));
	//cubeIndices.push_back(IndexFormat(4, 5, 6));
	//cubeIndices.push_back(IndexFormat(4, 6, 7));
	//cubeIndices.push_back(IndexFormat(8, 9, 10));
	//cubeIndices.push_back(IndexFormat(8, 10, 11));
	//cubeIndices.push_back(IndexFormat(12, 13, 14));
	//cubeIndices.push_back(IndexFormat(12, 14, 15));
	//cubeIndices.push_back(IndexFormat(16, 17, 18));
	//cubeIndices.push_back(IndexFormat(16, 18, 19));
	//cubeIndices.push_back(IndexFormat(20, 21, 22));
	//cubeIndices.push_back(IndexFormat(20, 22, 23));
	Indices.push_back(0); Indices.push_back(1); Indices.push_back(2);
	Indices.push_back(0); Indices.push_back(2); Indices.push_back(3);

	Indices.push_back(4); Indices.push_back(5); Indices.push_back(6);
	Indices.push_back(4); Indices.push_back(6); Indices.push_back(7);

	Indices.push_back(8); Indices.push_back(9); Indices.push_back(10);
	Indices.push_back(8); Indices.push_back(10); Indices.push_back(11);

	Indices.push_back(12); Indices.push_back(13); Indices.push_back(14);
	Indices.push_back(12); Indices.push_back(14); Indices.push_back(15);

	Indices.push_back(16); Indices.push_back(17); Indices.push_back(18);
	Indices.push_back(16); Indices.push_back(18); Indices.push_back(19);

	Indices.push_back(20); Indices.push_back(21); Indices.push_back(22);
	Indices.push_back(20); Indices.push_back(22); Indices.push_back(23);

	vertices.clear(); indices.clear();
	vertices = Vertices;
	indices = Indices;

}

void Utils::QuadData(std::vector<VertexFormat>& vertices, std::vector<GLuint>& indices) {

	std::vector<VertexFormat> Vertices;
	Vertices.push_back(VertexFormat(Position(-1.0f, -1.0, 0.0), TexCoord(0.0, 0.0)));
	Vertices.push_back(VertexFormat(Position(-1.0, 1.0, 0.0), TexCoord(0.0, 1.0)));
	Vertices.push_back(VertexFormat(Position(1.0, 1.0, 0.0), TexCoord(1.0, 1.0)));
	Vertices.push_back(VertexFormat(Position(1.0, -1.0, 0.0), TexCoord(1.0, 0.0)));

	std::vector<GLuint> Indices;
	Indices.push_back(0); Indices.push_back(1); Indices.push_back(2);
	Indices.push_back(0); Indices.push_back(2); Indices.push_back(3);
	vertices.clear(); indices.clear();
	vertices = Vertices;
	indices = Indices;

}

void Utils::SphereData(std::vector<VertexFormat>& vertices, std::vector<GLuint>& indices, float radius, unsigned int rings, unsigned int sectors)
{

	std::vector<VertexFormat> Vertices;
	std::vector<GLuint> Indices;

	for (double latNumber = 0; latNumber <= rings; latNumber++)
	{
		double theta = latNumber * glm::pi<float>() / rings;
		double sinTheta = sin(theta);
		double cosTheta = cos(theta);

		for (double longNumber = 0; longNumber <= sectors; longNumber++) 
		{
			double phi = longNumber * glm::two_pi<float>() / sectors;
			double sinPhi = sin(phi);
			double cosPhi = cos(phi);

			Normals Nvs;
			Nvs.x = cosPhi * sinTheta;   // x
			Nvs.y = cosTheta;            // y
			Nvs.z = sinPhi * sinTheta;   // z

			TexCoord Tvs;
			Tvs.u = 1 - (longNumber / sectors); // u
			Tvs.v = 1 - (latNumber / rings);   // v

			Position Pvs;
			Pvs.x = radius * Nvs.x;
			Pvs.y = radius * Nvs.y;
			Pvs.z = radius * Nvs.z;

			Vertices.push_back(VertexFormat(Pvs, Tvs, Nvs));
		}



		for (int latNumber = 0; latNumber < rings; latNumber++)
		{
			for (int longNumber=0; longNumber < sectors; longNumber++)
			{
				int first = (latNumber * (sectors + 1)) + longNumber;
				int second = first + sectors + 1;

				Indices.push_back(first); Indices.push_back(second); Indices.push_back(first + 1);

				Indices.push_back(second); Indices.push_back(second + 1); Indices.push_back(first + 1);

			}


		}


	}

	vertices.clear(); indices.clear();
	vertices = Vertices;
	indices = Indices;
}
