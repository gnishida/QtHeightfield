#include <iostream>
#include "GLWidget3D.h"
#include "MainWindow.h"
#include <GL/GLU.h>
#include "OBJLoader.h"

/**
 * This event handler is called when the mouse press events occur.
 */
void GLWidget3D::mousePressEvent(QMouseEvent *e)
{
	camera.mouseDown(e->x(), e->y());
}

/**
 * This event handler is called when the mouse release events occur.
 */
void GLWidget3D::mouseReleaseEvent(QMouseEvent *e)
{
	camera.mouseUp();

	updateGL();
}

/**
 * This event handler is called when the mouse move events occur.
 */
void GLWidget3D::mouseMoveEvent(QMouseEvent *e)
{
	if (e->buttons() & Qt::LeftButton) {
		camera.rotate(e->x(), e->y());
	} else if (e->buttons() & Qt::RightButton) {
		camera.zoom(e->x(), e->y());
	}

	updateGL();
}

/**
 * This function is called once before the first call to paintGL() or resizeGL().
 */
void GLWidget3D::initializeGL()
{
    // Initialize glew library
    glewInit();

	glEnable(GL_DEPTH_TEST);

    // Create the gpgpu object
    sim = new WaterSimulator(512, 512);
	time = 0;

	timer.start(16, this);

	//loadOBJ("models/cube.obj");
	createDenseMesh();

	initVAO();
}

void GLWidget3D::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);
    updateGL();
}

/**
 * This function is called whenever the widget has been resized.
 */
void GLWidget3D::resizeGL(int width, int height)
{
	height = height?height:1;
	
	delete sim;
	sim = new WaterSimulator(width, height);

	camera.setWindowSize(width, height);

	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)width/(GLfloat)height, 0.1f, 10000);
	glMatrixMode(GL_MODELVIEW);
}

/**
 * This function is called whenever the widget needs to be painted.
 */
void GLWidget3D::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set model view projection matrix
	float mvpMatrixArray[16];
	for(int i=0;i<16;i++){
		mvpMatrixArray[i]=camera.mvpMatrix.data()[i];
	}

	glBindVertexArray(vao[0]);
	sim->draw((float)time * 0.1, 0);
	glUniformMatrix4fv(sim->_mvpMatrixLoc, 1, false, (float*)&mvpMatrixArray[0]);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glBindVertexArray(vao[1]);
	sim->draw((float)time * 0.1, 1);
	glUniformMatrix4fv(sim->_mvpMatrixLoc, 1, false, (float*)&mvpMatrixArray[0]);
	glDrawArrays(GL_TRIANGLES, 0, vertices2.size());

	glUseProgram(0);

	time++;
}

/**
 * Load an OBJ file and create the corresponding VAO.
 */
void GLWidget3D::loadOBJ(const char* filename)
{
	OBJLoader::load(filename, vertices);
}

void GLWidget3D::createDenseMesh()
{
	vertices.clear();
	vertices2.clear();

	int N = 300;

	for (int i = 0; i < N; ++i) {
		float y1 = i / (float)N - 0.5f;
		float y2 = (i + 1) / (float)N - 0.5f;

		for (int j = 0; j < N; ++j) {
			float x1 = j / (float)N - 0.5f;
			float x2 = (j + 1) / (float)N - 0.5f;

			vertices.push_back(Vertex(Vector3f(x1, y1, 0.0f), Vector3f(0, 0, 1), Vector3f(0, 0, 1), Vector3f(1, 0, 0)));
			vertices.push_back(Vertex(Vector3f(x2, y1, 0.0f), Vector3f(0, 0, 1), Vector3f(0, 0, 1), Vector3f(1, 0, 0)));
			vertices.push_back(Vertex(Vector3f(x2, y2, 0.0f), Vector3f(0, 0, 1), Vector3f(0, 0, 1), Vector3f(1, 0, 0)));

			vertices.push_back(Vertex(Vector3f(x1, y1, 0.0f), Vector3f(0, 0, 1), Vector3f(0, 0, 1), Vector3f(1, 0, 0)));
			vertices.push_back(Vertex(Vector3f(x2, y2, 0.0f), Vector3f(0, 0, 1), Vector3f(0, 0, 1), Vector3f(1, 0, 0)));
			vertices.push_back(Vertex(Vector3f(x1, y2, 0.0f), Vector3f(0, 0, 1), Vector3f(0, 0, 1), Vector3f(1, 0, 0)));
		}
	}

	for (int i = N * 0.2; i < N * 0.8; ++i) {
		float y1 = i / (float)N - 0.5f;
		float y2 = (i + 1) / (float)N - 0.5f;

		for (int j = N * 0.2; j < N * 0.8; ++j) {
			float x1 = j / (float)N - 0.5f;
			float x2 = (j + 1) / (float)N - 0.5f;

			vertices2.push_back(Vertex(Vector3f(x1, y1, 0.0f), Vector3f(0, 1, 0), Vector3f(0, 0, 1), Vector3f(1, 0, 0)));
			vertices2.push_back(Vertex(Vector3f(x2, y1, 0.0f), Vector3f(0, 1, 0), Vector3f(0, 0, 1), Vector3f(1, 0, 0)));
			vertices2.push_back(Vertex(Vector3f(x2, y2, 0.0f), Vector3f(0, 1, 0), Vector3f(0, 0, 1), Vector3f(1, 0, 0)));

			vertices2.push_back(Vertex(Vector3f(x1, y1, 0.0f), Vector3f(0, 1, 0), Vector3f(0, 0, 1), Vector3f(1, 0, 0)));
			vertices2.push_back(Vertex(Vector3f(x2, y2, 0.0f), Vector3f(0, 1, 0), Vector3f(0, 0, 1), Vector3f(1, 0, 0)));
			vertices2.push_back(Vertex(Vector3f(x1, y2, 0.0f), Vector3f(0, 1, 0), Vector3f(0, 0, 1), Vector3f(1, 0, 0)));
		}
	}
}

void GLWidget3D::initVAO()
{
	// Setup vertex array object
	glGenVertexArrays(2, vao);
	glGenBuffers(2, vbo);

	// setup the vertices
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	// setup the vertices2
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices2.size(), vertices2.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
}
