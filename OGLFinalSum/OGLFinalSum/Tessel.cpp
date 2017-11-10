#include "Tessel.h"

CTessel::CTessel(GLuint prog, Camera* _camera)
{
	program = prog;
	m_camera = _camera;

	GLfloat points[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0, 1.0, 0.0f
	};

	glPatchParameteri(GL_PATCH_VERTICES, 4);


	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glBindVertexArray(0);

}

CTessel::~CTessel()
{
}

void CTessel::render()
{
	glUseProgram(program);

	glm::mat4 model;
	model = glm::translate(model, position);
	glm::mat4 mvp = m_camera->getProjectionMatrix() * m_camera->getViewMatrix() * model;
	GLint mvLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	float scalarvalue = GetTessLevel(m_camera->getPosition(), position);
	GLfloat Ehscale = glGetUniformLocation(program, "scalar");
	glUniform1f(1, scalarvalue);

	glBindVertexArray(vao);
	glDrawArrays(GL_PATCHES, 0, 4);
	glBindVertexArray(0);

}

float CTessel::GetTessLevel(glm::vec3 Distance0, glm::vec3 Distance1)
{
	float AvgDistance = glm::distance(Distance0, Distance1) / 2.0;

	if (AvgDistance <= 2.0) {
		return 10.0;
	}
	else if (AvgDistance <= 3.0) {
		return 8.0;
	}
	else if (AvgDistance <= 4.0) {
		return 7.0;
	}
	else if (AvgDistance <= 5.0) {
		return 5.0;
	}
	else if (AvgDistance <= 6.0) {
		return 4.0;
	}
	else {
		return 3.0;
	}
}

//getter
glm::vec3 CTessel::getPosition()
{
	return position;
}

glm::vec3 CTessel::getScale()
{
	return this->scale;
}

glm::vec3 CTessel::getRotation()
{
	return this->angle;

}

glm::vec3 CTessel::getRotationAxis()
{
	return rotationAxis;
}


//setter
void CTessel::setPosition(glm::vec3 _position)
{
	this->position = _position;
}

void CTessel::setScale(glm::vec3 _scale)
{
	this->scale = _scale;
}

glm::vec3 CTessel::setAngle(glm::vec3 & _Vec, float _Angle)
{
	_Vec;
	auto _len = glm::length(_Vec);
	_Vec.x = cosf(_Angle)*_len;
	_Vec.y = sinf(_Angle)*_len;
	_Vec.z = 0.0f;
	return _Vec;
}

void CTessel::setRotation(glm::vec3 angle)
{
	this->angle = angle;
}

void CTessel::setRotationAxis(glm::vec3 rotationAxis)
{
	this->rotationAxis = rotationAxis;
}
