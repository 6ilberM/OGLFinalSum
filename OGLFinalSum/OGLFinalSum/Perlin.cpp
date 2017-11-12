#include "Perlin.h"
namespace {

	struct TerrainVertex
	{
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec3 texC;
	};
}

CPerlin::CPerlin(Camera * _camera, GLuint prog, CLight * _light, float _ambientStrength, float _specularStrength, InitPerlin _info)
{
	camera = _camera;
	program = prog;
	light = _light;
	perlininfo = _info;

	srand(time(NULL));

	scale = glm::vec3(0.5f, 0.5f, 0.5f);
	position = glm::vec3(-50.0, -40.0, -50.0);
	color = glm::vec3(1.0f, 1.0f, 1.0f);

	ambientStrength = _ambientStrength;
	specularStrength = _specularStrength;

	mNumVertices = perlininfo.NumRows*perlininfo.NumCols;
	mNumFaces = (perlininfo.NumRows - 1)*(perlininfo.NumCols - 1) * 2;


}

CPerlin::~CPerlin()
{
}

double CPerlin::Noise(double _x, double _y)
{
	double temp = m_Heightmap[_x*_y];

	return temp;
}
double CPerlin::Smooth(double x, double y)
{

	double corners = (Noise(x - 1, y - 1) + Noise(x + 1, y - 1) + Noise(x - 1, y + 1) + Noise(x + 1, y + 1)) / 16;
	double sides = (Noise(x - 1, y) + Noise(x + 1, y) + Noise(x, y - 1) + Noise(x, y + 1)) / 8;
	double center = Noise(x, y) / 4;

	return corners + sides + center;

}

//Random number
void CPerlin::GenerateHMap()
{
	m_Heightmap.resize(perlininfo.NumRows * perlininfo.NumCols, 0);


	//per vertex Rather than per point 
	for (UINT i = 0; i < perlininfo.NumRows * perlininfo.NumCols; ++i)
	{
		m_Heightmap[i] = (double)rand() / (double)RAND_MAX;
	}
	process();
}

void CPerlin::process()
{
	for (int i = 0; i < perlininfo.NumCols; i++)
	{
		for (int j = 0; j < perlininfo.NumRows; j++)
		{
			m_Heightmap[i*j] = Smooth(i, j);
		}
	}

}

void CPerlin::render()
{
	glUseProgram(program);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1i(glGetUniformLocation(program, "Texture"), 0);


	UINT stride = sizeof(TerrainVertex);
	UINT offset = 0;

	glm::mat4 model;
	model = glm::translate(model, position);

	model = glm::translate(model, glm::vec3(0.0f * this->scale.x, 0.0f * scale.y, 0.0f));
	model = glm::rotate(model, glm::radians(angle.x), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(angle.y), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(angle.z), glm::vec3(0.0, 0.0, 1.0));
	model = glm::translate(model, glm::vec3(-0.0f * scale.x, -0.0f * scale.y, 0.0f));

	model = glm::scale(model, scale);

	GLint movel = glGetUniformLocation(program, "model");

	glUniformMatrix4fv(movel, 1, GL_FALSE, glm::value_ptr(model));

	GLint cameraL = glGetUniformLocation(program, "CameraPos");
	glm::vec3 CameraPos = camera->getPosition();

	glUniform3f(cameraL, CameraPos.x, CameraPos.y, CameraPos.z);


	glm::mat4 vp = camera->getProjectionMatrix() * camera->getViewMatrix();
	GLint vepe = glGetUniformLocation(program, "vp");

	glUniformMatrix4fv(vepe, 1, GL_FALSE, glm::value_ptr(vp));
	glm::mat4 mvp = camera->getProjectionMatrix() * camera->getViewMatrix() * model;

	GLint mvpLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLE_STRIP, indices1.size(), GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_QUADS, indices1.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

float CPerlin::width()const
{
	return (perlininfo.NumCols - 1)*perlininfo.CellSpacing;
}

float CPerlin::depth()const
{
	return (perlininfo.NumRows - 1)*perlininfo.CellSpacing;
}

float CPerlin::getHeight(float x, float z)const
{
	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f*width()) / perlininfo.CellSpacing;
	float d = (z - 0.5f*depth()) / -perlininfo.CellSpacing;

	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = m_Heightmap[row*perlininfo.NumCols + col];
	float B = m_Heightmap[row*perlininfo.NumCols + col + 1];
	float C = m_Heightmap[(row + 1)*perlininfo.NumCols + col];
	float D = m_Heightmap[(row + 1)*perlininfo.NumCols + col + 1];

	// Where we are relative to the cell.
	float s = c - (float)col;
	float t = d - (float)row;

	// If upper triangle ABC.
	if (s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s*uy + t*vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f - s)*uy + (1.0f - t)*vy;
	}

}

void CPerlin::setTexture(std::string  texFileName) {

	//** load texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//** loadImage and create texture
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(texFileName.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	if (!image)
	{
		printf("fileName: %s wasnt loaded\n", texFileName.c_str());
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

}
bool CPerlin::inBounds(GLuint i, GLuint j)
{
	// True if ij are valid indices; false otherwise.
	return
		i >= 0 && i < perlininfo.NumRows && j >= 0 && j < perlininfo.NumCols;
}

// setters and getters
void CPerlin::setScale(glm::vec3 _scale) {

	this->scale = _scale;
}

glm::vec3 CPerlin::setAngle(glm::vec3& _Vec, float _Angle)
{
	_Vec;
	auto _len = glm::length(_Vec);
	_Vec.x = cosf(_Angle)*_len;
	_Vec.y = sinf(_Angle)*_len;
	_Vec.z = 0.0f;
	return _Vec;
}

void CPerlin::setRotation(glm::vec3 angle) {

	this->angle = angle;
}

void CPerlin::setRotationAxis(glm::vec3 rotationAxis) {

	this->rotationAxis = rotationAxis;
}

void CPerlin::setPosition(glm::vec3 _position) {


	this->position = _position;

}

void CPerlin::setColor(glm::vec3 _color) {

	this->color = _color;
}

void CPerlin::setCamera(Camera * camera) {

}

void CPerlin::setSpeed(float _speed) {

	this->speed = _speed;
}

void CPerlin::setProgram(GLuint program) {

	this->program = program;
}

glm::vec3 CPerlin::getPosition() {

	return position;
}

glm::vec3 CPerlin::getColor() {

	return this->color;
}

glm::vec3 CPerlin::getScale() {

	return this->scale;

}

glm::vec3 CPerlin::getRotation() {

	return this->angle;
}

glm::vec3 CPerlin::getRotationAxis() {

	return rotationAxis;
}

void CPerlin::buildVB()
{
	std::vector<TerrainVertex> vertices(mNumVertices);

	float halfWidth = (perlininfo.NumCols - 1)*perlininfo.CellSpacing*0.5f;
	float halfDepth = (perlininfo.NumRows - 1)*perlininfo.CellSpacing*0.5f;

	float du = 1.0f / (perlininfo.NumCols - 1);
	float dv = 1.0f / (perlininfo.NumRows - 1);
	for (UINT i = 0; i < perlininfo.NumRows; ++i)
	{
		float z = halfDepth - i*perlininfo.CellSpacing;
		for (UINT j = 0; j < perlininfo.NumCols; ++j)
		{
			float x = -halfWidth + j*perlininfo.CellSpacing;

			float y = m_Heightmap[i*perlininfo.NumCols + j];
			vertices[i*perlininfo.NumCols + j].pos = glm::vec3(x, y, z);
			vertices[i*perlininfo.NumCols + j].normal = glm::vec3(0.0f, 1.0f, 0.0f);

			// Stretch texture over grid.
			vertices[i*perlininfo.NumCols + j].texC.x = j*du;
			vertices[i*perlininfo.NumCols + j].texC.y = i*dv;
		}
	}

	// Estimate normals for interior nodes using central difference.
	float invTwoDX = 1.0f / (2.0f*perlininfo.CellSpacing);
	float invTwoDZ = 1.0f / (2.0f*perlininfo.CellSpacing);
	for (GLuint i = 2; i < perlininfo.NumRows - 1; ++i)
	{
		for (GLuint j = 2; j < perlininfo.NumCols - 1; ++j)
		{
			float t = m_Heightmap[(i - 1)*perlininfo.NumCols + j];
			float b = m_Heightmap[(i + 1)*perlininfo.NumCols + j];
			float l = m_Heightmap[i*perlininfo.NumCols + j - 1];
			float r = m_Heightmap[i*perlininfo.NumCols + j + 1];

			glm::vec3 tanZ(0.0f, (t - b)*invTwoDZ, 1.0f);
			glm::vec3 tanX(1.0f, (r - l)*invTwoDX, 0.0f);

			glm::vec3 N;
			//glm::cross(&N, &tanZ, &tanX);
			glm::vec3 _templ;
			_templ = glm::cross(N, tanZ);
			_templ = glm::cross(_templ, tanX);
			//D3DXVec3Normalize(&N, &N);
			N = glm::normalize(N);

			vertices[i*perlininfo.NumCols + j].normal = N;
		}
	}
	////FIX

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TerrainVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
}

void CPerlin::buildIB()
{
	std::vector<GLuint> indices(mNumFaces * 3); // 3 indices per face

												// Iterate over each quad and compute indices.
	int k = 0;
	for (GLuint i = 0; i < perlininfo.NumRows - 1; ++i)
	{
		for (GLuint j = 0; j < perlininfo.NumCols - 1; ++j)
		{
			indices[k] = i*perlininfo.NumCols + j;
			indices[k + 1] = i*perlininfo.NumCols + j + 1;
			indices[k + 2] = (i + 1)*perlininfo.NumCols + j;

			indices[k + 3] = (i + 1)*perlininfo.NumCols + j;
			indices[k + 4] = i*perlininfo.NumCols + j + 1;
			indices[k + 5] = (i + 1)*perlininfo.NumCols + j + 1;

			k += 6; // next quad

		}

	}

	indices1 = indices;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);
}