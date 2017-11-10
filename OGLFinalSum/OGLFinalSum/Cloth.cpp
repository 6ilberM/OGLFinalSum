#include "Cloth.h"
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School.
//
// File Name    :cloth.cpp
// Description    : cpp manager for cloth
// Author        :Gilberto Moreno & Gibson Hickey
// Mail            :gilberto.mor6945@mediadesign.school.nz
Ccloth::~Ccloth() {

}
float howmuch = 1.0;
void Ccloth::update(unsigned char keyState[255]) {

	if (keyState[(unsigned char)'r'] == BUTTON_DOWN)
	{
		std::vector<Particle>::iterator particle;
		for (particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).reset();
			//SetText
	
		}
		//placement
		for (int i = 0; i < m_hooks; i++)
		{//This is relative

			getParticle(0 + i, 0)->makeUnmovable();

			getParticle(num_particles_width - 1 - i, 0)->makeUnmovable();

		}
	}

	//placement with unmovable
	if (keyState[(unsigned char)'R'] == BUTTON_DOWN)
	{
		for (int i = 0; i < m_hooks; i++)
		{//This is relative
			getParticle(0 + i, 0)->makeMovabble();
			getParticle(num_particles_width - 1 - i, 0)->makeMovabble();

			//SetText

		}
	}

	//placement with unmovable
	if (keyState[(unsigned char)'K'] == BUTTON_DOWN)
	{
		for (int i = 0; i < 2; i++)
		{//This is relative

			getParticle(num_particles_width - 1 - i, 0)->ShiftStatic(howmuch);
			

		}
	}


}

void Ccloth::render() {

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
#pragma endregion

	//glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, f_numparticles);
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vPositions.size(), &vPositions[0], GL_STATIC_DRAW);

	glBindVertexArray(0);


	//glUseProgram(program);


	//glDrawArrays(GL_POINTS, 0, f_numparticles);
	//glBindVertexArray(0);
	//glDisable(GL_BLEND);
}

void Ccloth::moveLeft() {

	position -= glm::vec3(1.0f, 0.0f, 0.0f) * speed;

}

void Ccloth::moveRight() {

	position += glm::vec3(1.0f, 0.0f, 0.0f) * speed;

}

void Ccloth::moveUp() {

	position += glm::vec3(0.0f, 1.0f, 0.0f) * speed;
}

void Ccloth::moveDown() {

	position -= glm::vec3(0.0f, 1.0f, 0.0f) * speed;

}

void Ccloth::moveForward() {

	position -= glm::vec3(0.0f, 0.0f, -1.0f) * speed;

}
void Ccloth::moveBackward() {

	position -= glm::vec3(0.0f, 0.0f, 1.0f) * speed;

}

void Ccloth::rotate(glm::vec3 axis) {

	this->angle.x += axis.x * speed * 20;
	this->angle.y += axis.y * speed * 20;
	this->angle.z += axis.z * speed * 20;
}

#pragma region Tuto

void Ccloth::makeConstraint(Particle * p1, Particle * p2)
{
	constraints.push_back(Constraint(p1, p2));
}
void Ccloth::shredConstraint(float _epx, float _epy)
{

}

glm::vec3 Ccloth::calcTriangleNormal(Particle * p1, Particle * p2, Particle * p3)
{
	glm::vec3 pos1 = p1->getPos();
	glm::vec3 pos2 = p2->getPos();
	glm::vec3 pos3 = p3->getPos();

	glm::vec3 v1 = pos2 - pos1;
	glm::vec3 v2 = pos3 - pos1;

	return glm::cross(v1, v2);
}

void Ccloth::addWindForcesForTriangle(Particle * p1, Particle * p2, Particle * p3, const glm::vec3 direction)
{
	glm::vec3 normal = calcTriangleNormal(p1, p2, p3);
	glm::vec3 d = glm::normalize(normal);
	glm::vec3 force = normal*(glm::dot(d, direction));
	p1->addForce(force);
	p2->addForce(force);
	p3->addForce(force);
}

void Ccloth::drawTriangle(Particle * p1, Particle * p2, Particle * p3, const glm::vec3 color)
{
	glColor3fv((GLfloat*)&color);

	glNormal3fv((GLfloat *) & (glm::normalize((p1->getNormal()))));
	//shift the other Shaded object a little further To the side!!
	glVertex3fv((GLfloat *) &(p1->getPos() + glm::vec3(.3f, 0.0f, 0.0f)));

	glNormal3fv((GLfloat *) &(glm::normalize((p2->getNormal()))));
	//shift the other Shaded object a little further To the side!!
	glVertex3fv((GLfloat *) &(p2->getPos() + glm::vec3(.3f, 0.0f, 0.0f)));

	glNormal3fv((GLfloat *) &(glm::normalize((p3->getNormal()))));
	//shift the other Shaded object a little further To the side!!
	glVertex3fv((GLfloat *) &(p3->getPos() + glm::vec3(.3f, 0.0f, 0.0f)));

}
//constructor
Ccloth::Ccloth(float width, float height, int num_particles_width, int num_particles_height, Camera * _camera, GLuint prog, int hooks) : num_particles_width(num_particles_width), num_particles_height(num_particles_height)
{
	camera = _camera;
	program = prog;
	std::string textureFileName = "Assets/Images/Red_Wool.png";
	scale = glm::vec3(20, 20, 20);
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	rotationAxis = glm::bvec3(0.0f, 0.0f, 1.0f);
	m_hooks = hooks;
	f_numparticles = num_particles_width*num_particles_height;

	particles.resize(num_particles_width*num_particles_height); //I am essentially using this vector as an array with room for num_particles_width*num_particles_height particles

																// creating particles in a grid of particles from (0,0,0) to (width,-height,0)
	for (int x = 0; x < num_particles_width; x++)
	{
		for (int y = 0; y < num_particles_height; y++)
		{
			glm::vec3 pos = glm::vec3(width * (x / (float)num_particles_width), -height * (y / (float)num_particles_height), 0);
			particles[y*num_particles_width + x] = Particle(pos); // insert particle in column x at y'throw
			vPositions.push_back(particles[y*num_particles_width + x].getPos());
		}
	}


#pragma region CONSTRAINTS
	// Connecting immediate neighbor particles with constraints (distance 1 and sqrt(2) in the grid)
	for (int x = 0; x < num_particles_width; x++)
	{
		for (int y = 0; y < num_particles_height; y++)
		{
			if (x < num_particles_width - 1) makeConstraint(getParticle(x, y), getParticle(x + 1, y));
			if (y < num_particles_height - 1) makeConstraint(getParticle(x, y), getParticle(x, y + 1));
			if (x < num_particles_width - 1 && y < num_particles_height - 1) makeConstraint(getParticle(x, y), getParticle(x + 1, y + 1));
			if (x < num_particles_width - 1 && y < num_particles_height - 1) makeConstraint(getParticle(x + 1, y), getParticle(x, y + 1));
		}
	}

	// Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
	for (int x = 0; x < num_particles_width; x++)
	{
		for (int y = 0; y < num_particles_height; y++)
		{
			if (x < num_particles_width - 2) makeConstraint(getParticle(x, y), getParticle(x + 2, y));
			if (y < num_particles_height - 2) makeConstraint(getParticle(x, y), getParticle(x, y + 2));
			if (x < num_particles_width - 2 && y < num_particles_height - 2) makeConstraint(getParticle(x, y), getParticle(x + 2, y + 2));
			if (x < num_particles_width - 2 && y < num_particles_height - 2) makeConstraint(getParticle(x + 2, y), getParticle(x, y + 2));
		}
	}


#pragma endregion




	//new VAOSTUFF

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

void Ccloth::drawShaded()

{
	// reset normals (which where written to last frame)
	std::vector<Particle>::iterator particle;
	int i = 0;
	for (particle = particles.begin(); particle != particles.end(); particle++)
	{
		(*particle).resetNormal();
		vPositions[i] = (*particle).getPos();
		i++;
	}

	//for (int i = 0; i < f_numparticles; i++)
	//{
	//	vPositions[i] = particles[i].getPos();
	//}


	//create smooth per particle normals by adding up all the (hard) triangle normals that each particle is part of
	for (int x = 0; x < num_particles_width - 1; x++)
	{
		for (int y = 0; y < num_particles_height - 1; y++)
		{
			glm::vec3 normal = calcTriangleNormal(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1));
			getParticle(x + 1, y)->addToNormal(normal);
			getParticle(x, y)->addToNormal(normal);
			getParticle(x, y + 1)->addToNormal(normal);

			normal = calcTriangleNormal(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1));
			getParticle(x + 1, y + 1)->addToNormal(normal);
			getParticle(x + 1, y)->addToNormal(normal);
			getParticle(x, y + 1)->addToNormal(normal);
		}
	}

	for (particle = particles.begin(); particle != particles.end(); particle++)
	{

		glm::vec3 that = (*particle).getPos();
	}


}

//actually drawing stuff
void Ccloth::drawShadedtwo()

{
	// reset normals (which where written to last frame)
	std::vector<Particle>::iterator particle;
	int i = 0;
	for (particle = particles.begin(); particle != particles.end(); particle++)
	{
		(*particle).resetNormal();
		vPositions[i] = (*particle).getPos();
		i++;
	}

	//for (int i = 0; i < f_numparticles; i++)
	//{
	//	vPositions[i] = particles[i].getPos();
	//}


	//create smooth per particle normals by adding up all the (hard) triangle normals that each particle is part of
	for (int x = 0; x < num_particles_width - 1; x++)
	{
		for (int y = 0; y < num_particles_height - 1; y++)
		{
			glm::vec3 normal = calcTriangleNormal(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1));
			getParticle(x + 1, y)->addToNormal(normal);
			getParticle(x, y)->addToNormal(normal);
			getParticle(x, y + 1)->addToNormal(normal);

			normal = calcTriangleNormal(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1));
			getParticle(x + 1, y + 1)->addToNormal(normal);
			getParticle(x + 1, y)->addToNormal(normal);
			getParticle(x, y + 1)->addToNormal(normal);
		}
	}



	glPointSize(10);
	glBegin(GL_TRIANGLES);

	for (int x = 0; x < num_particles_width - 1; x++)
	{
		for (int y = 0; y < num_particles_height - 1; y++)
		{
			glm::vec3 color(0, 0, 0);
			if (x % 2) // red and white color is interleaved according to which column number
				color = glm::vec3(0.6f, 0.2f, 0.2f);
			else
				color = glm::vec3(1.0f, 1.0f, 1.0f);

			drawTriangle(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1), color);
			drawTriangle(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1), color);
		}
	}
	glEnd();


}

void Ccloth::timeStep()
{
	std::vector<Constraint>::iterator constraint;
	for (int i = 0; i < CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times
	{
		for (constraint = constraints.begin(); constraint != constraints.end(); constraint++)
		{
			(*constraint).satisfyConstraint(); // satisfy constraint.
		}
	}

	std::vector<Particle>::iterator particle;
	for (particle = particles.begin(); particle != particles.end(); particle++)
	{
		(*particle).timeStep(); // calculate the position of each particle at the next time step.
	}
}

void Ccloth::addForce(const glm::vec3 direction)
{
	std::vector<Particle>::iterator particle;
	for (particle = particles.begin(); particle != particles.end(); particle++)
	{
		(*particle).addForce(direction); // add the forces to each particle
	}

}

void Ccloth::windForce(const glm::vec3 direction)
{
	for (int x = 0; x < num_particles_width - 1; x++)
	{
		for (int y = 0; y < num_particles_height - 1; y++)
		{
			addWindForcesForTriangle(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1), direction);
			addWindForcesForTriangle(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1), direction);
		}
	}
}

void Ccloth::ballCollision(const glm::vec3 center, const float radius)
{
	std::vector<Particle>::iterator particle;
	for (particle = particles.begin(); particle != particles.end(); particle++)
	{
		glm::vec3 v = (*particle).getPos() - center;
		float l = glm::length(v);
		if (glm::length(v) < radius) // if the particle is inside the ball
		{
			(*particle).offsetPos(glm::normalize(v)*(radius - l)); // project the particle to the surface of the ball
		}
	}

}
void Ccloth::ClothCollision()
{

}


#pragma endregion

//important
void Ccloth::setTexture(std::string  texFileName) {

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

//old
// setters and getters
void Ccloth::setScale(glm::vec3 _scale) {

	this->scale = _scale;
}

glm::vec3 Ccloth::setAngle(glm::vec3& _Vec, float _Angle)
{
	_Vec;
	auto _len = glm::length(_Vec);
	_Vec.x = cosf(_Angle)*_len;
	_Vec.y = sinf(_Angle)*_len;
	_Vec.z = 0.0f;
	return _Vec;
}

void Ccloth::setRotation(glm::vec3 angle) {

	this->angle = angle;
}

void Ccloth::setRotationAxis(glm::vec3 rotationAxis) {

	this->rotationAxis = rotationAxis;
}

void Ccloth::setPosition(glm::vec3 _position) {


	this->position = _position;

}

void Ccloth::setColor(glm::vec3 _color) {

	this->color = _color;
}

void Ccloth::setCamera(Camera * camera) {

}

void Ccloth::setSpeed(float _speed) {

	this->speed = _speed;

}

void Ccloth::setProgram(GLuint program) {

	this->program = program;
}

glm::vec3 Ccloth::getPosition() {

	return position;
}

glm::vec3 Ccloth::getColor() {

	return this->color;
}

glm::vec3 Ccloth::getScale() {

	return this->scale;

}

glm::vec3 Ccloth::getRotation() {

	return this->angle;
}

glm::vec3 Ccloth::getRotationAxis() {

	return rotationAxis;
}

void Ccloth::HoldaParticle(float x, float y, float lastx, float lasty)
{
	if (getParticle(x, y)->m_bmovable == true)
	{
		getParticle(x, y)->offsetPos(glm::vec3(lastx*TSTEPSIZE, lasty*TSTEPSIZE, 0));
	}
	else
	{

	}
}

int Ccloth::SetTextState(int _txtVar, int _txtState)
{
	return(_txtVar = _txtState);
}

int Ccloth::GetTextState(int _txtVar)
{
	return(_txtVar);
}