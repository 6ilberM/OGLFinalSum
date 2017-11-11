#include "Rain.h"

CRain::CRain(glm::vec3 origin, float _numparticles, Camera * _camera, GLuint prog)
{
	f_numparticles = _numparticles;
	camera = _camera;
	program = prog;
	std::string textureFileName = "Assets/Images/Watah.png";

	setTexture(textureFileName);

	for (int i = 0; i < f_numparticles; i++)
	{
		vPositions.push_back(glm::vec3(0.0)); //initialize position vector
		RParticle p = RParticle(origin, glm::vec3(randFloat(-0.010, 0.010), randFloat(0.01f, 0.15f), randFloat(-0.010, .010)), _camera);
		particles.push_back(p); // add 
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenVertexArrays(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vPositions.size(), &vPositions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

CRain::~CRain()
{
}

void CRain::update()
{

}

void CRain::render()
{
	glUseProgram(program);

	glm::mat4 model;
	model = glm::translate(model, position);

	model = glm::translate(model, glm::vec3(0.0f * this->scale.x, 0.0f * scale.y, 0.0f));
	model = glm::rotate(model, glm::radians(angle.x), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(angle.y), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(angle.z), glm::vec3(0.0, 0.0, 1.0));
	model = glm::translate(model, glm::vec3(-0.0f * scale.x, -0.0f * scale.y, 0.0f));
	model = glm::scale(model, scale);


	model = glm::scale(model, scale);
	glm::mat4 mvp = camera->getProjectionMatrix() * camera->getViewMatrix() * model;
	//GLint vpLoc = glGetUniformLocation(program, "mvp");

#pragma region otherstuff
	glm::mat4 viewMat = camera->getViewMatrix();
	glm::vec3 vQuad1, vQuad2;
	glm::vec3 vView = camera->getFront();
	glm::mat4 vp = camera->getProjectionMatrix() * camera->getViewMatrix();
	vView = glm::normalize(vView);


	vQuad1 = glm::cross(vView, camera->getUp());
	vQuad1 = glm::normalize(vQuad1);
	vQuad2 = glm::cross(vView, vQuad1);
	vQuad2 = glm::normalize(vQuad2);

	glUniform3f(glGetUniformLocation(program, "vQuad1"), vQuad1.x, vQuad1.y, vQuad1.z);
	glUniform3f(glGetUniformLocation(program, "vQuad2"), vQuad2.x, vQuad2.y, vQuad2.z);

	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, glm::value_ptr((mvp)));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1i(glGetUniformLocation(program, "Texture"), 0);

	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, f_numparticles);
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vPositions.size(), &vPositions[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void CRain::render(float dt)
{
	glUseProgram(program);
	//lets see
	glm::mat4 model;
	model = glm::translate(model, position);

	model = glm::translate(model, glm::vec3(0.0f * this->scale.x, 0.0f * scale.y, 0.0f));
	model = glm::rotate(model, glm::radians(angle.x), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(angle.y), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(angle.z), glm::vec3(0.0, 0.0, 1.0));
	model = glm::translate(model, glm::vec3(-0.0f * scale.x, -0.0f * scale.y, 0.0f));
	model = glm::scale(model, scale);


	//model = glm::scale(model, scale);
	//glm::mat4 mvp = camera->getProjectionMatrix() * camera->getViewMatrix() * model;
	//hm
	for (int i = 0; i < f_numparticles; i++)
	{
		particles[i].update(.0167);
		vPositions[i] = particles[i].getPos();
	}
	glm::mat4 viewMat = camera->getViewMatrix();
	glm::vec3 vQuad1, vQuad2;
	glm::vec3 vView = camera->getFront();
	glm::mat4 vp = camera->getProjectionMatrix() * camera->getViewMatrix();
	vView = glm::normalize(vView);

	vQuad1 = glm::cross(vView, camera->getUp());
	vQuad1 = glm::normalize(vQuad1);
	vQuad2 = glm::cross(vView, vQuad1);
	vQuad2 = glm::normalize(vQuad2);

	glUniform3f(glGetUniformLocation(program, "vQuad1"), vQuad1.x, vQuad1.y, vQuad1.z);
	glUniform3f(glGetUniformLocation(program, "vQuad2"), vQuad2.x, vQuad2.y, vQuad2.z);

	glUniformMatrix4fv(glGetUniformLocation(program, "vp"), 1, GL_FALSE, glm::value_ptr((vp)));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1i(glGetUniformLocation(program, "Texture"), 0);

	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, f_numparticles);
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vPositions.size(), &vPositions[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	glDisable(GL_BLEND);

}

#pragma region Setters&getters
void CRain::setScale(glm::vec3 _scale) {

	this->scale = _scale;
}

glm::vec3 CRain::setAngle(glm::vec3& _Vec, float _Angle)
{
	_Vec;
	auto _len = glm::length(_Vec);
	_Vec.x = cosf(_Angle)*_len;
	_Vec.y = sinf(_Angle)*_len;
	_Vec.z = 0.0f;
	return _Vec;
}

void CRain::setRotation(glm::vec3 angle) {

	this->angle = angle;
}

void CRain::setRotationAxis(glm::vec3 rotationAxis) {

	this->rotationAxis = rotationAxis;
}

void CRain::setPosition(glm::vec3 _position) {


	this->position = _position;

}

void CRain::setColor(glm::vec3 _color) {

	this->color = _color;
}

void CRain::setCamera(Camera * camera) {

}

void CRain::setSpeed(float _speed) {

	this->speed = _speed;

}

void CRain::setProgram(GLuint program) {

	this->program = program;
}

glm::vec3 CRain::getPosition() {

	return position;
}

glm::vec3 CRain::getColor() {

	return this->color;
}

glm::vec3 CRain::getScale() {

	return this->scale;

}

glm::vec3 CRain::getRotation() {

	return this->angle;
}

glm::vec3 CRain::getRotationAxis() {

	return rotationAxis;
}

void CRain::rotate(glm::vec3 axis) {

	this->angle.x += axis.x * speed * 20;
	this->angle.y += axis.y * speed * 20;
	this->angle.z += axis.z * speed * 20;
}



void CRain::setTexture(std::string  texFileName) {

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

#pragma endregion

//void CRain::windForce(const glm::vec3 direction)
//{
//	for (int x = 0; x < num_particles_width - 1; x++)
//	{
//		for (int y = 0; y < num_particles_height - 1; y++)
//		{
//			addWindForcesForTriangle(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1), direction);
//			addWindForcesForTriangle(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1), direction);
//		}
//	}
//}
//
//void CRain::addWindForcesForTriangle(Particle * p1, Particle * p2, Particle * p3, const glm::vec3 direction)
//{
//	glm::vec3 normal = calcTriangleNormal(p1, p2, p3);
//	glm::vec3 d = glm::normalize(normal);
//	glm::vec3 force = normal*(glm::dot(d, direction));
//	p1->addForce(force);
//	p2->addForce(force);
//	p3->addForce(force);
//}
//
//glm::vec3 CRain::calcTriangleNormal(Particle * p1, Particle * p2, Particle * p3)
//{
//	glm::vec3 pos1 = p1->getPos();
//	glm::vec3 pos2 = p2->getPos();
//	glm::vec3 pos3 = p3->getPos();
//
//	glm::vec3 v1 = pos2 - pos1;
//	glm::vec3 v2 = pos3 - pos1;
//
//	return glm::cross(v1, v2);
//}