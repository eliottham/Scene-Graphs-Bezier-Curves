#include "Transform.h"

Transform::Transform(glm::mat4 transformation)
{
    this->M = transformation;
    this->original_M = transformation;
    this->degree = 0.0f;
    this->angle = 0.0f;
    this->pivot = 0.0f;
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

// This sets the initial parameters for a object we want animated
void Transform::setAnimate(float degree, float start_angle, float pivot)
{
    this->degree = degree;
    this->angle = start_angle;
    this->pivot = pivot;
}

/* Matrix transformations only occur when setAnimate has been called prior because default
   values for degree, angle, and pivot are 0. */
void Transform::update()
{
    for(auto it = children.begin(); it != children.end(); ++it) {
        (*it)->update();
    }
    
    if(angle == -45.0f || angle == 45.0f) {
        degree *= -1.0f;
    }
    
    angle += degree;
    
    this->M = this->original_M * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, pivot)) *
                        glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f)) *
                        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -pivot));
}
