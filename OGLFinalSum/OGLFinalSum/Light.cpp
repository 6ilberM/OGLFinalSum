#include "Light.h"

CLight::CLight(Camera* camera, GLuint program)
{

	this->camera = camera;
	this->program = program;

	LightPos = glm::vec3(0.0f, 0.0f, 0.0f);
	LightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	Utils::setCubeA(vertices, indices);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	//Attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::texCoord)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::normal)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void CLight::render() {

	glUseProgram(this->program);

	//if(bIsTextureSet)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glUniform1i(glGetUniformLocation(program, "Texture"), 0);
	//uniform vec3 lightPos;
	//uniform vec3 viewPos;
	//uniform vec3 lightColor;
	//uniform vec3 objectColor;


	GLint colorLoc = glGetUniformLocation(program, "objectColor");
	glUniform3f(colorLoc, getColor().x, getColor().y, getColor().z);

	GLuint lightColorLoc = glGetUniformLocation(program, "lightColor");
	glUniform3f(lightColorLoc, getColor().x, getColor().y, getColor().z);

	GLuint cameraPosLoc = glGetUniformLocation(program, "viewPos");
	glUniform3f(cameraPosLoc, camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);

	GLuint lightPosLoc = glGetUniformLocation(program, "lightPos");
	glUniform3f(lightPosLoc, getPosition().x, getPosition().y, getPosition().z);


	glm::mat4 model;


	//model = glm::translate(model, LightPos);

	glm::mat4 mvp = camera->getProjectionMatrix() * camera->getViewMatrix() * model;
	//glm::mat4 vp = glm::perspective(45.0f, (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);
	GLint mvpLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// lighting calculations
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void CLight::update(unsigned char keystate[]) {



	if (keystate[(unsigned char)'i'] == BUTTON_DOWN)
	{
		moveForward();
		//	printf("%2f %2f %2f ", LightPos.x, LightPos.y, LightPos.z);
	}

	if (keystate[(unsigned char)'k'] == BUTTON_DOWN)
	{
		moveBackward();
	}

	if (keystate[(unsigned char)'j'] == BUTTON_DOWN)
	{
		moveLeft();
	}

	if (keystate[(unsigned char)'l'] == BUTTON_DOWN)
	{
		moveRight();
	}


}

glm::vec3 & CLight::getPosition()
{
	return LightPos;
}

glm::vec3 & CLight::getColor()
{
	return LightColor;
}

//movement
void CLight::moveForward() {

	LightPos -= glm::vec3(0.0f, 0.0f, 1.0f) * speed;
}

void CLight::moveBackward() {

	LightPos += glm::vec3(0.0f, 0.0f, 1.0f) * speed;
}

void CLight::moveLeft() {

	LightPos -= glm::vec3(1.0f, 0.0f, 0.0f) * speed;
}

void CLight::moveRight() {

	LightPos += glm::vec3(1.0f, 0.0f, 0.0f) * speed;
}



CLight::~CLight()
{

}
