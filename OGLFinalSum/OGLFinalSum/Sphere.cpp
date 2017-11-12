#include "Sphere.h"
#include "camera.h"

CSphere::CSphere(Camera* _camera, GLuint prog,float _Radius, CLight * _light, float _ambientStrength, float _specularStrength) {

	std::string textureFileName = "Assets/Images/Red_Wool.png";
	camera = _camera;
	program = prog;
	Radius = _Radius;
	light = _light;

	ambientStrength = _ambientStrength;
	specularStrength = _specularStrength;
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	setTexture(textureFileName);

	scale = glm::vec3(2.0f, 2.0f, 2.0f);
	position = glm::vec3(0.0f, 1.0f, 0.0);
	color = glm::vec3(1.0f, 1.0f, 1.0f);

	speed = .25f;
	rotationAxis = glm::bvec3(0.0f, 0.0f, 1.0f);

	Utils::SphereData(vertices, indices, .6,55, 55);


#pragma region VAO_VBO_EBO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

#pragma endregion

#pragma region ATTRIBUTES
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::pos)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::texCoord)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

#pragma endregion

}


CSphere::~CSphere() {

}

void CSphere::update(unsigned char keyState[255],glm::vec3 v_Ballpos) {

	if (keyState[(unsigned char)'J'] == BUTTON_DOWN)
	{

	}
	position = v_Ballpos;

}

void CSphere::render() {

	/*glUseProgram(program);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(program, "Texture"), 0);

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
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);*/



	glUseProgram(program);

	//if(bIsTextureSet)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(program, "Texture"), 0);

	glm::mat4 model;
	model = glm::translate(model, position);

	//model = glm::translate(model, glm::vec3(0.0f * this->scale.x, 0.0f * scale.y, 0.0f));
	//model = glm::rotate(model, glm::radians(angle.x), glm::vec3(1.0, 0.0, 0.0));
	//model = glm::rotate(model, glm::radians(angle.y), glm::vec3(0.0, 1.0, 0.0));
	//model = glm::rotate(model, glm::radians(angle.z), glm::vec3(0.0, 0.0, 1.0));
	//model = glm::translate(model, glm::vec3(-0.0f * scale.x, -0.0f * scale.y, 0.0f));
	model = glm::scale(model, glm::vec3(3, 3, 3));

	glm::mat4 vp = camera->getProjectionMatrix() * camera->getViewMatrix();
	GLint vpLoc = glGetUniformLocation(program, "vp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(vp));

	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// lighting calculations

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

	glBindVertexArray(vao);
	//indices[0] = indices[0];
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}

void CSphere::moveLeft() {

	position -= glm::vec3(1.0f, 0.0f, 0.0f) * speed;

}

void CSphere::moveRight() {

	position += glm::vec3(1.0f, 0.0f, 0.0f) * speed;

}

void CSphere::moveUp() {

	position += glm::vec3(0.0f, 1.0f, 0.0f) * speed;
}

void CSphere::moveDown() {

	position -= glm::vec3(0.0f, 1.0f, 0.0f) * speed;

}

void CSphere::moveForward() {

	position -= glm::vec3(0.0f, 0.0f, -1.0f) * speed;

}
void CSphere::moveBackward() {

	position -= glm::vec3(0.0f, 0.0f, 1.0f) * speed;

}

void CSphere::rotate(glm::vec3 axis) {

	this->angle.x += axis.x * speed * 20;
	this->angle.y += axis.y * speed * 20;
	this->angle.z += axis.z * speed * 20;
}

void CSphere::setTexture(std::string  texFileName) {

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

// setters and getters
void CSphere::setScale(glm::vec3 _scale) {

	this->scale = _scale;
}

glm::vec3 CSphere::setAngle(glm::vec3& _Vec, float _Angle)
{
	_Vec;
	auto _len = glm::length(_Vec);
	_Vec.x = cosf(_Angle)*_len;
	_Vec.y = sinf(_Angle)*_len;
	_Vec.z = 0.0f;
	return _Vec;
}

void CSphere::setRotation(glm::vec3 angle) {

	this->angle = angle;
}

void CSphere::setRotationAxis(glm::vec3 rotationAxis) {

	this->rotationAxis = rotationAxis;
}

void CSphere::setPosition(glm::vec3 _position) {


	this->position = _position;

}

void CSphere::setColor(glm::vec3 _color) {

	this->color = _color;
}

void CSphere::setCamera(Camera * camera) {

}

void CSphere::setSpeed(float _speed) {

	this->speed = _speed;

}

void CSphere::setProgram(GLuint program) {

	this->program = program;
}

glm::vec3 CSphere::getPosition() {

	return position;
}

glm::vec3 CSphere::getColor() {

	return this->color;
}


glm::vec3 CSphere::getScale() {

	return this->scale;

}

glm::vec3 CSphere::getRotation() {

	return this->angle;
}

glm::vec3 CSphere::getRotationAxis() {

	return rotationAxis;
}

