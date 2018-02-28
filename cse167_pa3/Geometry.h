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
#include "Node.h"
#include "Window.h"
#ifndef Geometry_h
#define Geometry_h

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
};

class Geometry : public Node
{
private:
    std::vector<GLuint> indices;
    std::vector<glm::vec3> vpositions;
    std::vector<glm::vec3> vnormals;
    std::vector<Vertex> vertices;
    GLuint VAO, VBO, EBO;
    GLuint uProjection, uModelview;
    float degree;
    float inc;
public:
    Geometry(const char* filepath);
    ~Geometry();
    void parse(const char* filepath);
    void draw(glm::mat4 C);
    float findPivotPoint();
    void update();
};

#endif /* Geometry_h */
