#ifndef Transform_h
#define Transform_h

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
#include <list>
#include "Node.h"

using namespace std;

class Transform : public Node
{
private:
    glm::mat4 M;
    glm::mat4 original_M;
    list<Node*> children;
    float degree;
    float angle;
    float pivot;
public:
    Transform(glm::mat4 transformation);
    ~Transform();
    void addChild(Node* child);
    void removeChild(Node* child);
    void draw(glm::mat4 C);
    void changeM(glm::mat4 transformation);
    void setAnimate(float degree, float start_angle, float pivot);
    void update();
};
    

#endif /* Transform_h */
