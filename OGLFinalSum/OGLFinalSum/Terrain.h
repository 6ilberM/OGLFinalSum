#ifndef TERRAIN_H
#define TERRAIN_H

#include "Utils.h"
#include <vector>
#include <time.h>  
#include "camera.h"
#include <string>
using namespace std;
class CTerrain
{
public:

	struct InitInfo
	{
		std::string HeightmapFilename;
		std::string LayerMapFilename0;
		std::string LayerMapFilename1;
		std::string LayerMapFilename2;
		std::string LayerMapFilename3;
		std::string LayerMapFilename4;
		std::string BlendMapFilename;
		float HeightScale;
		float HeightOffset;
		GLuint NumRows;
		GLuint NumCols;
		float CellSpacing;
	};

	Camera* camera;

	std::vector<VertexFormat>vertices;
	std::vector<GLuint>indices1;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 scale;
	glm::vec3 angle;
	glm::vec3 rotationAxis;
	glm::vec3 color;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint texture;
	GLuint program;

	float speed;


	CTerrain(Camera* _camera, GLuint prog, InitInfo& _minfo);
	~CTerrain();
	float width() const;
	float depth() const;
	float getHeight(float x, float z) const;
	void loadHeightmap();
	void setTexture(std::string texFileName);

	float average(GLuint i, GLuint j);

	void smooth();

	bool inBounds(GLuint i, GLuint j);

	void render();

	void setScale(glm::vec3 _scale);

	glm::vec3 setAngle(glm::vec3 & _Vec, float _Angle);

private:
	InitInfo m_Info;
	GLuint mNumVertices;
	GLuint mNumFaces;

	std::vector<float> m_Heightmap;

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

};

#endif // !TERRAIN_H
