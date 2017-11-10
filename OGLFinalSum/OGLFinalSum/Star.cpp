#include "Star.h"

GeoStar::GeoStar(GLuint prog, Camera* _camera)
{
	program = prog;
	m_camera = _camera;

	scale = glm::vec3(2.0f, 2.0f, 2.0f);
	position = glm::vec3(0.0f, 1.0f, 0.0);
	color = glm::vec3(1.0f, 1.0f, 1.0f);

	rotationAxis = glm::bvec3(0.0f, 0.0f, 1.0f);


	GLfloat points[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, //passing in 1 point
	};

	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
	
}

GeoStar::~GeoStar()
{
}

void GeoStar::render()
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
	glm::mat4 mvp = m_camera->getProjectionMatrix() * m_camera->getViewMatrix() * model;
	GLint vpLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}


//getter
glm::vec3 GeoStar::getPosition()
{
	return position;
}

glm::vec3 GeoStar::getScale()
{
	return this->scale;
}

glm::vec3 GeoStar::getRotation()
{
	return this->angle;

}

glm::vec3 GeoStar::getRotationAxis()
{
	return rotationAxis;
}

void GeoStar::update()
{
	this->setRotation(glm::vec3(this->getRotation().x, this->getRotation().y + 1.2f,this->getRotation().z));
}


//setter
void GeoStar::setPosition(glm::vec3 _position)
{
	this->position = _position;
}

void GeoStar::setScale(glm::vec3 _scale)
{
	this->scale = _scale;
}

glm::vec3 GeoStar::setAngle(glm::vec3 & _Vec, float _Angle)
{
	_Vec;
	auto _len = glm::length(_Vec);
	_Vec.x = cosf(_Angle)*_len;
	_Vec.y = sinf(_Angle)*_len;
	_Vec.z = 0.0f;
	return _Vec;
}

void GeoStar::setRotation(glm::vec3 angle)
{
	this->angle = angle;
}

void GeoStar::setRotationAxis(glm::vec3 rotationAxis)
{
	this->rotationAxis = rotationAxis;
}
