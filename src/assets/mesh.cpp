pho::Asset::Mesh():
selected(false),
name(name)
    {
    modelMatrix = glm::mat4();
    glGenVertexArrays(1,&vaoId);
    glGenBuffers(1,&iboId);
    glGenBuffers(1,&vertexVboId);
    glGenBuffers(1,&colorVboId);
}

pho::Asset::Asset(std::string name):
selected(false),
name(name)
    {
    modelMatrix = glm::mat4();
    glGenVertexArrays(1,&vaoId);
    glGenBuffers(1,&iboId);
    glGenBuffers(1,&vertexVboId);
    glGenBuffers(1,&colorVboId);
}

pho::Asset::Asset(std::vector<glm::vec3> vertixes, std::vector<GLushort> indixes, std::vector<glm::vec3> color, std::string name):
vertices(vertixes),
indices(indixes),
colors(color),
name(name),
selected(false) {
    modelMatrix = glm::mat4();
    glGenVertexArrays(1,&vaoId);
    glGenBuffers(1,&iboId);
    glGenBuffers(1,&vertexVboId);
    glGenBuffers(1,&colorVboId);

    glBindVertexArray(vaoId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(GLushort),indices.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,iboId);
    glBindBuffer(GL_ARRAY_BUFFER,vertexVboId);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*3*sizeof(GLfloat),vertices.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc,3,GL_FLOAT,GL_FALSE,0,0);  //******* CHANGE
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER,colorVboId);
    glBufferData(GL_ARRAY_BUFFER,colors.size()*3*sizeof(GLfloat),colors.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc,3,GL_FLOAT,GL_FALSE,0,0); //******* CHANGE?
    glEnableVertexAttribArray(colorLoc);
    glBindVertexArray(0);

}

pho::Asset::Asset(std::vector<glm::vec3> vertixes, std::vector<GLushort> indixes, std::vector<glm::vec2> texcoords,std::string name):
    vertices(vertixes),
    indices(indixes),
    texcoords(texcoords),
    selected(false),
    name(name)
    {
    modelMatrix = glm::mat4();
    glGenVertexArrays(1,&vaoId);
    glGenBuffers(1,&iboId);
    glGenBuffers(1,&vertexVboId);
    glGenBuffers(1,&colorVboId);

    glBindVertexArray(vaoId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(GLushort),indices.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,iboId);
    glBindBuffer(GL_ARRAY_BUFFER,vertexVboId);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*3*sizeof(GLfloat),vertices.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER,colorVboId);
    glBufferData(GL_ARRAY_BUFFER,texcoords.size()*2*sizeof(GLfloat),texcoords.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(colorLoc);
    glBindVertexArray(0);
}


GLuint pho::Asset::getVaoId() {
    return vaoId;
}

glm::vec3 pho::Asset::getPosition() {

    return glm::vec3(modelMatrix[3][0],modelMatrix[3][1],modelMatrix[3][2]);
}
