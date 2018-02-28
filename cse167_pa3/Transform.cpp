#include "Transform.h"

Transform::Transform(glm::mat4 transformation)
{
    this->M = transformation;
}

Transform::~Transform()
{
    for(auto it = children.begin(); it != children.end(); ++it) {
        delete(*it);
    }
}

void Transform::addChild(Node* child)
{
    children.push_back(child);
}

void Transform::removeChild(Node* child)
{
    children.remove(child);
}

void Transform::draw(glm::mat4 C)
{
    glm::mat4 C_new = C * this->M;
    for(auto it = children.begin(); it != children.end(); ++it) {
        (*it)->draw(C_new);
    }
}

void Transform::changeM(glm::mat4 transformation)
{
    this->M = transformation;
}
