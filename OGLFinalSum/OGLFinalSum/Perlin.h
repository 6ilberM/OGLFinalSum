#ifndef PERLIN_H
#define PERLIN_H

#include "Utils.h"
#include "Light.h"
#include <vector>
#include <time.h>  
#include "camera.h"
#include <string>
#include <cstdlib>

using namespace std;
static int p[512];

const int  permutation[] = {
	151,160,137,91,90,15,																// Hash lookup table as defined by Ken Perlin.  This is a randomly
	131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,	// arranged array of all numbers from 0-255 inclusive.
	190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
	88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
	77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
	102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
	135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
	5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
	223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
	129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
	251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
	49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

static int Perlin() {
	//p = new int[513];
	for (int x = 0; x < 512; x++)
	{
		p[x] = permutation[x % 256];
	}
};

class Clight;
class Camera;

class CPerlin
{
public:
	struct InitPerlin
	{
		float HeightScale;
		float HeightOffset;
		GLuint NumRows;
		GLuint NumCols;

		float CellSpacing;
	};

	Camera* camera;
	CLight* light;

	std::vector<VertexFormat>vertices;
	std::vector<GLuint>indices1;

	std::vector<glm::vec2>PerlinMap;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 scale;
	glm::vec3 angle;
	glm::vec3 rotationAxis;
	glm::vec3 color;

	InitPerlin perlininfo;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint texture;
	GLuint program;

	float speed;
	float ambientStrength;
	float specularStrength;

	CPerlin(Camera* _camera, GLuint prog, CLight * _light, float _ambientStrength, float _specularStrength, InitPerlin _info);
	void smooth();
	double average(GLuint i, GLuint j);
	~CPerlin();

	double Noise(double _x, double _y);

	double Smooth(double _x, double _y);

	void GenerateHMap();

	void process();

	void linInterp();

	double OctavePerlin(double x, double y, double z, int octaves, double persistence);

	static double perlin(double x, double y, double z);

	static double lerp(double a, double b, double x);

	static double fade(double t);

	float width() const;
	float depth() const;

	float getHeight(float x, float z) const;

	//void loadHeightmap();
	void setTexture(std::string texFileName);

	//float average(GLuint i, GLuint j);

	bool inBounds(GLuint i, GLuint j);

	static double grad(int hash, double x, double y, double z);

	void render();

	void setScale(glm::vec3 _scale);

	glm::vec3 setAngle(glm::vec3 & _Vec, float _Angle);


	void setRotation(glm::vec3 angle);

	void setRotationAxis(glm::vec3 rotationAxis);

	void setPosition(glm::vec3 _position);

	void setColor(glm::vec3 _color);

	void setCamera(Camera * camera);

	void setSpeed(float _speed);

	void setProgram(GLuint program);

	glm::vec3 getPosition();

	glm::vec3 getColor();

	glm::vec3 getScale();

	glm::vec3 getRotation();

	glm::vec3 getRotationAxis();

	void buildVB();

	void buildIB();

private:
	GLuint mNumVertices;
	GLuint mNumFaces;

	std::vector<double> m_Heightmap;



};

#endif // !PERLIN_H
