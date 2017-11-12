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
	~CPerlin();

	double Noise(double _x, double _y);

	double Smooth(double _x, double _y);

	void GenerateHMap();

	void process();



	float width() const;
	float depth() const;

	float getHeight(float x, float z) const;

	//void loadHeightmap();
	void setTexture(std::string texFileName);

	//float average(GLuint i, GLuint j);

	bool inBounds(GLuint i, GLuint j);

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
