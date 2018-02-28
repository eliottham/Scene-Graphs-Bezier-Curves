#include "Geometry.h"

Geometry::Geometry(const char* filepath)
{
    parse(filepath);
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    // Vertex colors
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
    // Vertex normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Geometry::~Geometry()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Geometry::parse(const char* filepath)
{
    FILE* fp;
    float x, y, z;
    float r, g, b;
    unsigned int v1, v2, v3, n1, n2, n3;
    int c1, c2;
    
    fp = fopen(filepath, "r");
    
    while((c1 = fgetc(fp)) != EOF) {
        if(c1 == 'v') {
            c2 = fgetc(fp);
            if(c2 == ' ') {
                fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
                vpositions.push_back(glm::vec3(x, y, z));
            }
            else if(c2 == 'n') {
                fscanf(fp, "%f %f %f", &x, &y, &z);
                vnormals.push_back(glm::vec3(x, y, z));
            }
        }
        else if(c1 == 'f') {
            c2 = fgetc(fp);
            if(c2 == ' ') {
                fscanf(fp, "%u//%u %u//%u %u//%u", &v1, &n1, &v2, &n2, &v3, &n3);
                indices.push_back(v1 -1);
                indices.push_back(v2 - 1);
                indices.push_back(v3 - 1);
            }
        }
        
        if(feof(fp)) break;
    }
    fclose(fp);
    
    for(int i = 0; i < vpositions.size(); i++) {
        Vertex v;
        v.position = vpositions[i];
        v.color = glm::normalize(vnormals[i]);// * 0.5f + 0.5f; // Normal coloring
        v.normal = vnormals[i];
        
        vertices.push_back(v);
    }
}

void Geometry::draw(glm::mat4 C)
{
    uProjection = glGetUniformLocation(Window::robotShader, "projection");
    uModelview = glGetUniformLocation(Window::robotShader, "modelview");
    
    glm::mat4 modelview = Window::V * C;
    
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
        
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
