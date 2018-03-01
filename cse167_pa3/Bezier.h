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
#include <stdio.h>
#include "Window.h"
#ifndef Bezier_h
#define Bezier_h
#define N_CURVES 149
using namespace std;

class Bezier
{
private:
    GLuint VAO, VBO;
    GLuint uProjection, uModelview;
    vector<glm::vec3> vertices;
    glm::vec3 p0, p1, p2, p3;
    glm::mat4 C; // Geometry matrix G * geometric basis B
public:
    Bezier(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
    ~Bezier();
    void approximateCurve();
    void draw(GLuint shaderProgram);
    vector<glm::vec3> getVertices();
};

#endif /* Bezier_h */
