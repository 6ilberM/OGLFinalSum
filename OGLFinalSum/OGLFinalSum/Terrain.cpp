#include "Terrain.h"

namespace
{
	struct TerrainVertex
	{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 texC;
	};
}

CTerrain::CTerrain(Camera* _camera, GLuint prog, InitInfo& _minfo)
{
	camera = _camera;
	program = prog;

	m_Info = _minfo;

	scale = glm::vec3(1.0f, 1.0f, 1.0f);

	position = glm::vec3(0.0f, -3.0f, 0.0);
	color = glm::vec3(1.0f, 0.2f, 0.4f);
	
	std::string textureFileName = "Assets/Images/gr0und.JPG";
	setTexture(textureFileName);
	mNumVertices = m_Info.NumRows*m_Info.NumCols;
	mNumFaces = (m_Info.NumRows - 1)*(m_Info.NumCols - 1) * 2;
	
	loadHeightmap();
	smooth();
	
#pragma region VAO_VBO_EBO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	buildVB();
	buildIB();
#pragma endregion
	
#pragma region ATTRIBUTES
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)(offsetof(TerrainVertex, TerrainVertex::pos)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)(offsetof(TerrainVertex, TerrainVertex::normal)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*)(offsetof(TerrainVertex, TerrainVertex::texC)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

#pragma endregion

}
CTerrain::~CTerrain()
{

}

float CTerrain::width()const
{
	return (m_Info.NumCols - 1)*m_Info.CellSpacing;
}

float CTerrain::depth()const
{
	return (m_Info.NumRows - 1)*m_Info.CellSpacing;
}

float CTerrain::getHeight(float x, float z)const
{
	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f*width()) / m_Info.CellSpacing;
	float d = (z - 0.5f*depth()) / -m_Info.CellSpacing;

	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = m_Heightmap[row*m_Info.NumCols + col];
	float B = m_Heightmap[row*m_Info.NumCols + col + 1];
	float C = m_Heightmap[(row + 1)*m_Info.NumCols + col];
	float D = m_Heightmap[(row + 1)*m_Info.NumCols + col + 1];

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

void CTerrain::loadHeightmap()
{
	//Soil to load the heightmaps 
	// A height for each vertex
	std::vector<unsigned char> in(m_Info.NumRows * m_Info.NumCols);

	// Open the file.
	std::ifstream inFile;
	inFile.open(m_Info.HeightmapFilename.c_str(), std::ios_base::binary);

	if (inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	}

	// Copy the array data into a float array, and scale and offset the heights.
	m_Heightmap.resize(m_Info.NumRows * m_Info.NumCols, 0);
	for (UINT i = 0; i < m_Info.NumRows * m_Info.NumCols; ++i)
	{
		m_Heightmap[i] = (float)in[i] * m_Info.HeightScale + m_Info.HeightOffset;
	}

}

void CTerrain::setTexture(std::string  texFileName) {

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

float CTerrain::average(GLuint i, GLuint j)
{
	// Function computes the average height of the ij element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float avg = 0.0f;
	float num = 0.0f;

	for (GLuint m = i - 1; m <= i + 1; ++m)
	{
		for (GLuint n = j - 1; n <= j + 1; ++n)
		{
			if (inBounds(m, n))
			{
				avg += m_Heightmap[m*m_Info.NumCols + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

void CTerrain::smooth()
{
	std::vector<float> dest(m_Heightmap.size());

	for (GLuint i = 0; i < m_Info.NumRows; ++i)
	{
		for (GLuint j = 0; j < m_Info.NumCols; ++j)
		{
			dest[i*m_Info.NumCols + j] = average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one.
	m_Heightmap = dest;
}

bool CTerrain::inBounds(GLuint i, GLuint j)
{
	// True if ij are valid indices; false otherwise.
	return
		i >= 0 && i < m_Info.NumRows && j >= 0 && j < m_Info.NumCols;
}

void CTerrain::render() {

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

// setters and getters
void CTerrain::setScale(glm::vec3 _scale) {

	this->scale = _scale;
}

glm::vec3 CTerrain::setAngle(glm::vec3& _Vec, float _Angle)
{
	_Vec;
	auto _len = glm::length(_Vec);
	_Vec.x = cosf(_Angle)*_len;
	_Vec.y = sinf(_Angle)*_len;
	_Vec.z = 0.0f;
	return _Vec;
}

void CTerrain::setRotation(glm::vec3 angle) {

	this->angle = angle;
}

void CTerrain::setRotationAxis(glm::vec3 rotationAxis) {

	this->rotationAxis = rotationAxis;
}

void CTerrain::setPosition(glm::vec3 _position) {


	this->position = _position;

}

void CTerrain::setColor(glm::vec3 _color) {

	this->color = _color;
}

void CTerrain::setCamera(Camera * camera) {

}

void CTerrain::setSpeed(float _speed) {

	this->speed = _speed;
}

void CTerrain::setProgram(GLuint program) {

	this->program = program;
}

glm::vec3 CTerrain::getPosition() {

	return position;
}

glm::vec3 CTerrain::getColor() {

	return this->color;
}

glm::vec3 CTerrain::getScale() {

	return this->scale;

}

glm::vec3 CTerrain::getRotation() {

	return this->angle;
}

glm::vec3 CTerrain::getRotationAxis() {

	return rotationAxis;
}

void CTerrain::buildVB()
{
	std::vector<TerrainVertex> vertices(mNumVertices);

	float halfWidth = (m_Info.NumCols - 1)*m_Info.CellSpacing*0.5f;
	float halfDepth = (m_Info.NumRows - 1)*m_Info.CellSpacing*0.5f;

	float du = 1.0f / (m_Info.NumCols - 1);
	float dv = 1.0f / (m_Info.NumRows - 1);
	for (UINT i = 0; i < m_Info.NumRows; ++i)
	{
		float z = halfDepth - i*m_Info.CellSpacing;
		for (UINT j = 0; j < m_Info.NumCols; ++j)
		{
			float x = -halfWidth + j*m_Info.CellSpacing;

			float y = m_Heightmap[i*m_Info.NumCols + j];
			vertices[i*m_Info.NumCols + j].pos = glm::vec3(x, y, z);
			vertices[i*m_Info.NumCols + j].normal = glm::vec3(0.0f, 1.0f, 0.0f);

			// Stretch texture over grid.
			vertices[i*m_Info.NumCols + j].texC.x = j*du;
			vertices[i*m_Info.NumCols + j].texC.y = i*dv;
		}
	}

	// Estimate normals for interior nodes using central difference.
	float invTwoDX = 1.0f / (2.0f*m_Info.CellSpacing);
	float invTwoDZ = 1.0f / (2.0f*m_Info.CellSpacing);
	for (GLuint i = 2; i < m_Info.NumRows - 1; ++i)
	{
		for (GLuint j = 2; j < m_Info.NumCols - 1; ++j)
		{
			float t = m_Heightmap[(i - 1)*m_Info.NumCols + j];
			float b = m_Heightmap[(i + 1)*m_Info.NumCols + j];
			float l = m_Heightmap[i*m_Info.NumCols + j - 1];
			float r = m_Heightmap[i*m_Info.NumCols + j + 1];

			glm::vec3 tanZ(0.0f, (t - b)*invTwoDZ, 1.0f);
			glm::vec3 tanX(1.0f, (r - l)*invTwoDX, 0.0f);

			glm::vec3 N;
			//glm::cross(&N, &tanZ, &tanX);
			glm::vec3 _templ;
			_templ = glm::cross(N, tanZ);
			_templ = glm::cross(_templ, tanX);
			//D3DXVec3Normalize(&N, &N);
			N = glm::normalize(N);

			vertices[i*m_Info.NumCols + j].normal = N;
		}
	}
	////FIX

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TerrainVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
}

void CTerrain::buildIB()
{
	std::vector<GLuint> indices(mNumFaces * 3); // 3 indices per face

											   // Iterate over each quad and compute indices.
	int k = 0;
	for (GLuint i = 0; i < m_Info.NumRows - 1; ++i)
	{
		for (GLuint j = 0; j < m_Info.NumCols - 1; ++j)
		{
			indices[k] = i*m_Info.NumCols + j;
			indices[k + 1] = i*m_Info.NumCols + j + 1;
			indices[k + 2] = (i + 1)*m_Info.NumCols + j;

			indices[k + 3] = (i + 1)*m_Info.NumCols + j;
			indices[k + 4] = i*m_Info.NumCols + j + 1;
			indices[k + 5] = (i + 1)*m_Info.NumCols + j + 1;

			k += 6; // next quad

		}

	}

	indices1 = indices;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);
}