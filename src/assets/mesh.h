namespace pho {

class Mesh {
public:
    Asset();
    Asset(std::string name);
    Asset(std::vector<glm::vec3> vertices, std::vector<GLushort> indices, std::vector<glm::vec3> colors, std::string name);
    Asset(std::vector<glm::vec3> vertices, std::vector<GLushort> indices, std::vector<glm::vec2> texcoords, std::string name);
    GLuint getVaoId();
    void render();
    glm::mat4 modelMatrix;
    GLuint* shader;
    GLuint vaoId,iboId,vertexVboId,colorVboId;
    std::vector<glm::vec3> vertices;
    std::vector<GLushort> indices;
    std::vector<glm::vec3> colors;
    std::vector<glm::vec2> texcoords;
    GLuint texIndex;
    GLuint uniformBlockIndex;
    int numFaces;
    Box aabb;
    bool selected;
    std::string name;
    int numfaces;
    glm::vec3 getPosition();
};

}
