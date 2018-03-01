#include "Group.h"

Group::Group()
{
    M = glm::mat4(1.0f);
    spacing = 75.0f;
    scale = 0.1f;
    index = 0;
}

void Group::update()
{
    for(auto it = children.begin(); it != children.end(); ++it) {
        (*it)->update();
    }
    
    // Update M matrix to follow path of Bezier curve
    this->M = glm::translate(glm::mat4(1.0f),
                             glm::vec3(path[index].x - (2 * spacing * scale),
                                       path[index].y, // x/z coords to allign center of 5 x 5 grid on curve
                                       path[index].z - (2 * spacing * scale))) *
              glm::scale(glm::mat4(1.0f),
                         glm::vec3(scale));
    index++;
    
    if(index == path.size() - 1) index = 0;
}

void Group::draw(glm::mat4 mx)
{
    for(auto it = children.begin(); it != children.end(); ++it) {
        (*it)->draw(M);
    }
}

void Group::addChild(Node* child)
{
    children.push_back(child);
}

void Group::removeChild(Node* child)
{
    children.remove(child);
}

void Group::makePath(vector<glm::vec3> bezier)
{
    for(auto it = bezier.begin(); it != bezier.end(); ++it) {
        path.push_back(*it);
    }
}

void Group::changeScale(float s)
{
    if(scale + s <= 0) {
        scale = 0.1f;
    }
    else {
        scale += s;
    }
}
