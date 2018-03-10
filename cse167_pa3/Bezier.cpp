#include "Bezier.h"

Bezier::Bezier(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
    this->p0 = p0;
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    
    // Geometry matrix * geometric basis for cubic Bezier curve
    this->C = glm::mat4(glm::vec4(p0, 0.0f),
                        glm::vec4(p1, 0.0f),
                        glm::vec4(p2, 0.0f),
                        glm::vec4(p3, 0.0f)) *
              glm::mat4(glm::vec4(-1.0f, 3.0f, -3.0f, 1.0f),
                        glm::vec4(3.0f, -6.0f, 3.0f, 0.0f),
                        glm::vec4(-3.0f, 3.0f, 0.0f, 0.0f),
                        glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    
    approximateCurve();
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Bezier::~Bezier()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
void Bezier::approximateCurve()
{
    // Approximate curve with 150 lines
    for(unsigned int i = 0; i < N_CURVES; i++) {
        float t = (float)i / (float)N_CURVES;
        glm::vec4 poly_basis = glm::vec4(pow(t, 3), pow(t, 2), t, 1.0f);
        vertices.push_back(glm::vec3(C * poly_basis));
    }
}

void Bezier::draw(GLuint shaderProgram)
{
    uProjection = glGetUniformLocation(shaderProgram, "projection");
    uModelview = glGetUniformLocation(shaderProgram, "modelview");

    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &Window::V[0][0]);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, N_CURVES);
    glBindVertexArray(0);
}

vector<glm::vec3> Bezier::getVertices()
{
    return vertices;
}
