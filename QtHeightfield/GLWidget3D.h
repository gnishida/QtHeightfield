#pragma once

#include "WaterSimulator.h"
#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include "Camera.h"
#include <QVector3D>
#include <QBasicTimer>
#include <vector>
#include "Vertex.h"


using namespace std;

class GLWidget3D : public QGLWidget
{
public:
	void loadOBJ(const char* filename);
	void createDenseMesh();
	void initVAO();

protected:
	void initializeGL();
	void timerEvent(QTimerEvent *e);
	void resizeGL(int width, int height);
	void paintGL();    
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

private:
	static enum{VERTEX,NORMAL,COLOR,TOTAL_VBO_ID};

	Camera camera;
	vector<Vertex> vertices;
	vector<Vertex> vertices2;

	QBasicTimer timer;
	WaterSimulator* sim;
	int time;

	GLuint vao[2];
	GLuint vbo[2];
	GLuint verticesBuf;
};

