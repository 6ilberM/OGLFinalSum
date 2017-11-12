
#include "Skybox.h"
#include "Camera.h"



Skybox::Skybox(ModelType modelType, Camera* _camera, std::vector<std::string>cubeMapTexture, GLuint prog) {
	camera = _camera;
	program = prog;

	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};


	// Setup skybox VAO
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);


	// Cubemap (Skybox)
	std::vector<const GLchar*> faces;
	for (int i = 0; i < cubeMapTexture.size(); i++)
	{
		faces.push_back(cubeMapTexture[i].c_str());
	}
	texture = loadCubeMap(faces);


}

Skybox::~Skybox() {

}

void Skybox::update(GLfloat time) {


}

void Skybox::render() {	
	glDisable(GL_CULL_FACE);
	glUseProgram(program);
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(program, "cubeSampler"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glm::mat4 world;
	//change far plane
	world = glm::scale(world, glm::vec3(250.0f, 250.0f, 250.0f));
	glm::mat4 mvp;
	mvp = camera->getProjectionMatrix() * camera->getViewMatrix() * world;
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

}

GLuint Skybox::loadCubeMap(std::vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);

		printf("fileName %s \n", faces[i]);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}


void Skybox::setScale(glm::vec3 _scale) {


	this->scale = _scale;
}

void Skybox::setRotation(glm::vec3 angle) {

	this->angle = angle;
}

void Skybox::setRotationAxis(glm::vec3 rotationAxis) {

	this->rotationAxis = rotationAxis;
}

void Skybox::setPosition(glm::vec3 _position) {


	this->position = _position;

}

void Skybox::setColor(glm::vec3 _color) {

	this->color = _color;
}

void Skybox::setCamera(Camera * camera) {

}

void Skybox::setSpeed(float _speed) {

	this->speed = _speed;

}

void Skybox::setProgram(GLuint program) {

	this->program = program;
}

glm::vec3 Skybox::getPosition() {

	return position;
}

glm::vec3 Skybox::getColor() {

	return this->color;
}

glm::vec3 Skybox::getScale() {

	return this->scale;

}

glm::vec3 Skybox::getRotation() {

	return this->angle;
}

glm::vec3 Skybox::getRotationAxis() {

	return rotationAxis;
}

