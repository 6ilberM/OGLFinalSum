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
	std::string textureFileName = "Assets/Images/gr0und.JPG";
	setTexture(textureFileName);

	scale = glm::vec3(100.0f, 100.0f, 100.0f);
	position = glm::vec3(-0, -0, -0);
	color = glm::vec3(1.0f, 1.0f, 1.0f);

	ambientStrength = _ambientStrength;
	specularStrength = _specularStrength;

	mNumVertices = perlininfo.NumRows*perlininfo.NumCols;
	mNumFaces = (perlininfo.NumRows - 1)*(perlininfo.NumCols - 1) * 2;

	GenerateHMap();
	process();
	smooth();
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


}

CPerlin::~CPerlin()
{
}


double CPerlin::average(GLuint i, GLuint j)
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
				avg += m_Heightmap[m*perlininfo.NumCols + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

void CPerlin::smooth()
{
	std::vector<double> dest(m_Heightmap.size());

	for (GLuint i = 0; i < perlininfo.NumRows; ++i)
	{
		for (GLuint j = 0; j < perlininfo.NumCols; ++j)
		{
			dest[i*perlininfo.NumCols + j] = average(i, j);
		}
	}

	// Replace the old heightmap with the filtered one.
	m_Heightmap = dest;
}

//Random number
void CPerlin::GenerateHMap()
{
	m_Heightmap.resize(perlininfo.NumRows * perlininfo.NumCols, 0);


	//per vertex Rather than per point 
	for (UINT i = 0; i < perlininfo.NumRows * perlininfo.NumCols; ++i)
	{
		m_Heightmap[i] = ((double)rand() / (double)RAND_MAX)*perlininfo.HeightOffset+ perlininfo.HeightScale;
	}

}

void CPerlin::process()
{
	for (int i = 0; i < perlininfo.NumCols; i++)
	{
		for (int j = 0; j < perlininfo.NumRows; j++)
		{
			m_Heightmap[i*j] = OctavePerlin(i, j, 1, 8, 1);
		}
	}
}

void CPerlin::linInterp()
{

}


double CPerlin::OctavePerlin(double x, double y, double z, int octaves, double persistence)
{
	double total = 0;
	double frequency = 1;
	double amplitude = 1;
	for (int i = 0; i < octaves; i++) {
		total += perlin(x * frequency, y * frequency, z * frequency) * amplitude;

		amplitude *= persistence;
		frequency *= 2;
	}

	return total;
}

double CPerlin::perlin(double x, double y, double z) {
	//if (repeat > 0) {									// If we have any repeat on, change the coordinates to their "local" repetitions
	//	x = x%repeat;
	//	y = y%repeat;
	//	z = z%repeat;
	//}

	int xi = (int)x & 255;								// Calculate the "unit cube" that the point asked will be located in
	int yi = (int)y & 255;								// The left bound is ( |_x_|,|_y_|,|_z_| ) and the right bound is that
	int zi = (int)z & 255;								// plus 1.  Next we calculate the location (from 0.0 to 1.0) in that cube.
	double xf = x - (int)x;								// We also fade the location to smooth the result.
	double yf = y - (int)y;
	double zf = z - (int)z;
	double u = fade(xf);
	double v = fade(yf);
	double w = fade(zf);

	int a = p[xi] + yi;								// This here is Perlin's hash function.  We take our x value (remember,
	int aa = p[a] + zi;								// between 0 and 255) and get a random value (from our p[] array above) between
	int ab = p[a + 1] + zi;								// 0 and 255.  We then add y to it and plug that into p[], and add z to that.
	int b = p[xi + 1] + yi;								// Then, we get another random value by adding 1 to that and putting it into p[]
	int ba = p[b] + zi;								// and add z to it.  We do the whole thing over again starting with x+1.  Later
	int bb = p[b + 1] + zi;								// we plug aa, ab, ba, and bb back into p[] along with their +1's to get another set.
														// in the end we have 8 values between 0 and 255 - one for each vertex on the unit cube.
														// These are all interpolated together using u, v, and w below.

	double x1, x2, y1, y2;
	x1 = lerp(grad(p[aa], xf, yf, zf),			// This is where the "magic" happens.  We calculate a new set of p[] values and use that to get
		grad(p[ba], xf - 1, yf, zf),			// our final gradient values.  Then, we interpolate between those gradients with the u value to get
		u);										// 4 x-values.  Next, we interpolate between the 4 x-values with v to get 2 y-values.  Finally,
	x2 = lerp(grad(p[ab], xf, yf - 1, zf),			// we interpolate between the y-values to get a z-value.
		grad(p[bb], xf - 1, yf - 1, zf),
		u);										// When calculating the p[] values, remember that above, p[a+1] expands to p[xi]+yi+1 -- so you are
	y1 = lerp(x1, x2, v);								// essentially adding 1 to yi.  Likewise, p[ab+1] expands to p[p[xi]+yi+1]+zi+1] -- so you are adding
														// to zi.  The other 3 parameters are your possible return values (see grad()), which are actually
	x1 = lerp(grad(p[aa + 1], xf, yf, zf - 1),		// the vectors from the edges of the unit cube to the point in the unit cube itself.
		grad(p[ba + 1], xf - 1, yf, zf - 1),
		u);
	x2 = lerp(grad(p[ab + 1], xf, yf - 1, zf - 1),
		grad(p[bb + 1], xf - 1, yf - 1, zf - 1),
		u);
	y2 = lerp(x1, x2, v);

	return (lerp(y1, y2, w) + 1) / 2;						// For convenience we bound it to 0 - 1 (theoretical min/max before is -1 - 1)
}

double CPerlin::lerp(double a, double b, double x)
{
	return a + x * (b - a);
}

double CPerlin::fade(double t) {
	// Fade function as defined by Ken Perlin.  This eases coordinate values
	// so that they will "ease" towards integral values.  This ends up smoothing
	// the final output.
	return t * t * t * (t * (t * 6 - 15) + 10);			// 6t^5 - 15t^4 + 10t^3
}


double CPerlin::grad(int hash, double x, double y, double z)
{
	int h = hash & 15;									// Take the hashed value and take the first 4 bits of it (15 == 0b1111)
	double u = h < 8 /* 0b1000 */ ? x : y;				// If the most signifigant bit (MSB) of the hash is 0 then set u = x.  Otherwise y.

	double v;											// In Ken Perlin's original implementation this was another conditional operator (?:).  I
														 // expanded it for readability.

	if (h < 4 /* 0b0100 */)								// If the first and second signifigant bits are 0 set v = y
		v = y;
	else if (h == 12 /* 0b1100 */ || h == 14 /* 0b1110*/)// If the first and second signifigant bits are 1 set v = x
		v = x;
	else 												// If the first and second signifigant bits are not equal (0/1, 1/0) set v = z
		v = z;

	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v); // Use the last 2 bits to decide if u and v are positive or negative.  Then return their addition.
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

	//model = glm::translate(model, glm::vec3(0.0f * this->scale.x, 0.0f * scale.y, 0.0f));
	//model = glm::rotate(model, glm::radians(angle.x), glm::vec3(1.0, 0.0, 0.0));
	//model = glm::rotate(model, glm::radians(angle.y), glm::vec3(0.0, 1.0, 0.0));
	//model = glm::rotate(model, glm::radians(angle.z), glm::vec3(0.0, 0.0, 1.0));
	//model = glm::translate(model, glm::vec3(-0.0f * scale.x, -0.0f * scale.y, 0.0f));

	model = glm::scale(model, scale);

	//GLint movel = glGetUniformLocation(program, "model");

	//glUniformMatrix4fv(movel, 1, GL_FALSE, glm::value_ptr(model));

	//GLint cameraL = glGetUniformLocation(program, "CameraPos");
	//glm::vec3 CameraPos = camera->getPosition();

	//glUniform3f(cameraL, CameraPos.x, CameraPos.y, CameraPos.z);


	//glm::mat4 vp = camera->getProjectionMatrix() * camera->getViewMatrix();
	//GLint vepe = glGetUniformLocation(program, "vp");

	//glUniformMatrix4fv(vepe, 1, GL_FALSE, glm::value_ptr(vp));
	glm::mat4 mvp = camera->getProjectionMatrix() * camera->getViewMatrix() * model;

/*	GLint mvpLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp))*/;

	glm::mat4 vp = camera->getProjectionMatrix() * camera->getViewMatrix();
	GLint vpLoc = glGetUniformLocation(program, "vp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//light
	GLint colorLoc = glGetUniformLocation(program, "objectColor");
	glUniform3f(colorLoc, color.x, color.y, color.z);

	GLuint cameraPosLoc = glGetUniformLocation(program, "cameraPos");
	glUniform3f(cameraPosLoc, camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);

	GLuint lightPosLoc = glGetUniformLocation(program, "lightPos");
	glUniform3f(lightPosLoc, this->light->getPosition().x, this->light->getPosition().y, this->light->getPosition().z);

	GLuint lightColorLoc = glGetUniformLocation(program, "lightColor");
	glUniform3f(lightColorLoc, this->light->getColor().x, this->light->getColor().y, this->light->getColor().z);

	GLuint specularStrengthLoc = glGetUniformLocation(program, "specularStrength");
	glUniform1f(specularStrengthLoc, specularStrength);

	GLuint ambientStrengthLoc = glGetUniformLocation(program, "ambientStrength");
	glUniform1f(ambientStrengthLoc, ambientStrength);

	//light
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices1.size(), &indices[0], GL_STATIC_DRAW);
}