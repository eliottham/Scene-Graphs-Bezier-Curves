#ifndef Skybox_h
#define Skybox_h

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "Window.h"
#include "assert.h"
using namespace std;

class Skybox
{
private:
    GLuint VBO, VAO, cubeMapTexture;
    GLuint uProjection, uView;
    
public:
    Skybox(vector<string> faces);
    ~Skybox();
    void draw(GLuint shaderProgram);
    GLuint loadCubeMap(vector<string> faces);
};

// Cube vertices

const GLfloat skybox_vertices[] = {
    -500.0f,  500.0f, -500.0f,
    -500.0f, -500.0f, -500.0f,
     500.0f, -500.0f, -500.0f,
     500.0f, -500.0f, -500.0f,
     500.0f,  500.0f, -500.0f,
    -500.0f,  500.0f, -500.0f,
    
    -500.0f, -500.0f,  500.0f,
    -500.0f, -500.0f, -500.0f,
    -500.0f,  500.0f, -500.0f,
    -500.0f,  500.0f, -500.0f,
    -500.0f,  500.0f,  500.0f,
    -500.0f, -500.0f,  500.0f,
    
     500.0f, -500.0f, -500.0f,
     500.0f, -500.0f,  500.0f,
     500.0f,  500.0f,  500.0f,
     500.0f,  500.0f,  500.0f,
     500.0f,  500.0f, -500.0f,
     500.0f, -500.0f, -500.0f,
    
    -500.0f, -500.0f,  500.0f,
    -500.0f,  500.0f,  500.0f,
     500.0f,  500.0f,  500.0f,
     500.0f,  500.0f,  500.0f,
     500.0f, -500.0f,  500.0f,
    -500.0f, -500.0f,  500.0f,
    
    -500.0f,  500.0f, -500.0f,
     500.0f,  500.0f, -500.0f,
     500.0f,  500.0f,  500.0f,
     500.0f,  500.0f,  500.0f,
    -500.0f,  500.0f,  500.0f,
    -500.0f,  500.0f, -500.0f,
    
    -500.0f, -500.0f, -500.0f,
    -500.0f, -500.0f,  500.0f,
     500.0f, -500.0f, -500.0f,
     500.0f, -500.0f, -500.0f,
    -500.0f, -500.0f,  500.0f,
     500.0f, -500.0f,  500.0f
};
 
#endif /* Skybox_h */
