#ifndef Group_h
#define Group_h
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
#include <list>
#include "Node.h"
using namespace std;


class Group : public Node
{
private:
    glm::mat4 M;
    list<Node*> children;
    vector<glm::vec3> path;
    int index;
    
public:
    Group();
    float spacing; // Spacing between each robot in the 5 x 5 grid
    float scale; // Scale factor for the entire grid
    void changeM(glm::mat4 transformation);
    void update();
    void draw(glm::mat4 mx);
    void addChild(Node* child);
    void removeChild(Node* child);
    void makePath(vector<glm::vec3> bezier);
    void changeScale(float s);
};


#endif /* Group_h */
