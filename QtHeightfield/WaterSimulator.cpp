#include "WaterSimulator.h"
#include <iostream>
#include <fstream>

WaterSimulator::WaterSimulator(int w, int h) : _initialized(0), _width(w), _height(h)
{
    // Create a texture to store the framebuffer
    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGB, GL_FLOAT, 0);

    _programId = glCreateProgram();

	// Load fragment shader which will be used as computational kernel
	std::string vertexSource;
	loadShader("vertex.glsl", vertexSource);

    // Create the vertex program
    _vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* source1 = vertexSource.c_str();
    glShaderSource(_vertexShader, 1, &source1, NULL);
    glCompileShader(_vertexShader);
    glAttachShader(_programId, _vertexShader);

	// Load fragment shader which will be used as computational kernel
	std::string fragSource;
	loadShader("fragment.glsl", fragSource);

    // Create the fragment program
    _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* source2 = fragSource.c_str();
    glShaderSource(_fragmentShader, 1, &source2, NULL);
    glCompileShader(_fragmentShader);
    glAttachShader(_programId, _fragmentShader);

    // Link the shader into a complete GLSL program.
    glLinkProgram(_programId);

	// Check program
	{
		int infologLength = 0;
		glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength > 0) {
			char *infoLog = (char *)malloc(infologLength);
			glGetProgramInfoLog(_programId, infologLength, NULL, infoLog);
			printf("%s\n",infoLog);
			free(infoLog);
		}
	}

    // Get location of the uniform variables
    _mvpMatrixLoc = glGetUniformLocation(_programId, "mvpMatrix");
    _typeLoc = glGetUniformLocation(_programId, "type");
    _texUnitLoc = glGetUniformLocation(_programId, "texUnit");
	_tLoc = glGetUniformLocation(_programId, "t");
	_centerLoc = glGetUniformLocation(_programId, "center");
	_frequencyLoc = glGetUniformLocation(_programId, "frequency");
	_amplitudeLoc = glGetUniformLocation(_programId, "amplitude");
	_islandLoc = glGetUniformLocation(_programId, "island");
	_lightDirLoc = glGetUniformLocation(_programId, "lightDir");
	_cameraPosLoc = glGetUniformLocation(_programId, "cameraPos");
}

void WaterSimulator::draw(float t, int type)
{
	glUseProgram(_programId);

    // Set the uniform variables
    glUniform1i(_texUnitLoc, 0);
	float center[] = {-0.2, -0.2, 0.3, 0.2, -0.1, 0.4};
	float frequency[] = {200.0, 160.0, 100.0};
	float amplitude[] = {0.0015, 0.00125, 0.0015};
	float lightDir[] = {0.0, 0.0, 1.0};
	float island[] = {0.0, 0.0};
	float cameraPos[] = {0.0, -1.0, 1.0};
	glUniform1i(_typeLoc, type);
	glUniform2fv(_centerLoc, 3, center);
	glUniform1fv(_frequencyLoc, 3, frequency);
	glUniform1fv(_amplitudeLoc, 3, amplitude);
	glUniform3fv(_lightDirLoc, 1, lightDir);
	glUniform2fv(_islandLoc, 1, island);
	glUniform3fv(_cameraPosLoc, 1, cameraPos);
	glUniform1f(_tLoc, t);
}


int WaterSimulator::loadShader(char* filename, std::string& text)
{
	std::ifstream ifs;
	ifs.open(filename, std::ios::in);

	std::string line;
	while (ifs.good()) {
        getline(ifs, line);

		text += line + "\n";
    }

	return 0;
}