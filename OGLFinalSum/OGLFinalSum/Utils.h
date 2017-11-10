#ifndef UTILS_H
#define UTILS_H

#include "Dependencies\soil\SOIL.h"

#include "Dependencies\glew\glew.h"

#include "Dependencies\freeglut\freeglut.h"

#include "glm\glm/glm.hpp"
#include "glm\glm/gtc/matrix_transform.hpp"
#include "glm\glm/gtc/type_ptr.hpp"
#include <vector>
#include <fstream>
#include <chrono>

enum ModelType {

	Triangle = 0,
	Actor = 1,
	Cube = 2
};

/* Some physics constants */
#define DAMPING 0.01f // how much to damp the cloth simulation each frame
#define TSTEPSIZE .1425f // how large time step each particle takes each frame
#define CONSTRAINT_ITERATIONS 10// how many iterations of constraint satisfaction each frame (more is rigid, less is soft)

struct Position	//Sets position fo something 
{
	float x, y, z;

	Position(float _x, float _y, float _z) {

		x = _x; y = _y; z = _z;
	}

	Position() {}
};
struct TexCoord	//Text Coordinates
{
	float u, v;

	TexCoord(float _u, float _v) {
		u = _u; v = _v;
	}

	TexCoord() {}
};
struct Normals
{
	float x, y, z;

	Normals(float _x, float _y, float _z) {

		x = _x; y = _y; z = _z;
	}

	Normals() {}
};
struct VertexFormat
{
	Position pos;
	TexCoord texCoord;
	Normals normal;
	VertexFormat(Position _pos, TexCoord _texCoord, Normals _normal) {

		pos = _pos;
		texCoord = _texCoord;
		normal = _normal;

	}
	VertexFormat(Position _pos, TexCoord _texCoord) {

		pos = _pos;
		texCoord = _texCoord;
	}
	VertexFormat() {}
};

struct IndexFormat
{
	int v1; int v2; int v3;

	IndexFormat(int _v1, int _v2, int _v3) {

		v1 = _v1; v2 = _v2; v3 = _v3;
	}
};


class Utils
{
public:


	//functions to set vertices for CUBE.
#pragma region StaticFuncs
	Utils();
	~Utils();

	static void setCubeA(std::vector<VertexFormat>& vertices, std::vector<GLuint>& indices);

	static void QuadData(std::vector<VertexFormat>& vertices, std::vector<GLuint>& indices);

	static void SphereData(std::vector<VertexFormat>& vertices, std::vector<GLuint>& indices, float radius, unsigned int rings, unsigned int sectors);

#pragma endregion




private:

};

#endif // !ULTILS_H
