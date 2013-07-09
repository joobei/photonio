#include "asset.h"

pho::Asset::Asset()
{}

pho::Asset::Asset(const std::string& filename, pho::Shader* tehShader)
{
    std::string assetpath;
    //load the asset path and store it
    if (boost::filesystem::exists("assetpath")) { //if you're not using cmake put the shaders in the same dir as the binary
        assetpath = readTextFile("assetpath");
        assetpath = assetpath.substr(0,assetpath.size()-1); //cmake puts a newline char
        assetpath.append("/"); //at the end of the string
    }

    Assimp::Importer importer;

    scene = importer.ReadFile( assetpath+filename,
            aiProcess_CalcTangentSpace       |
            aiProcess_Triangulate            |
            aiProcess_JoinIdenticalVertices  |
            aiProcess_SortByPType);

    // If the import failed, report it
    if( !scene)
    {
        std::cout << importer.GetErrorString() << std::endl;

    }
    else {
        std::cout << "imported "+filename << std::endl;

    }

    upload(tehShader);
}

void pho::Asset::upload(pho::Shader* tehShader)
{

    //log("Number of Meshes :");

    for (unsigned int n = 0; n < scene->mNumMeshes; ++n)
        {
            const struct aiMesh* mesh = scene->mMeshes[n];
            MyMesh tempMesh;

            // generate Vertex Array for mesh
            glGenVertexArrays(1,&(tempMesh.vao));
            glBindVertexArray(tempMesh.vao);

            GLuint buffer;
            if (mesh->HasPositions()) {
                CALL_GL(glGenBuffers(1, &buffer));
                CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
                CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW));
                CALL_GL(glEnableVertexAttribArray(vertexLoc));
                CALL_GL(glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0));
            }

            if (mesh->HasNormals()) {
                log("Has Normals");
                CALL_GL(glGenBuffers(1, &buffer));
                CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
                CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW));
                CALL_GL(glEnableVertexAttribArray(normalLoc));
                CALL_GL(glVertexAttribPointer(normalLoc, 3, GL_FLOAT,0, 0, 0));
            }

            tempMesh.numFaces = mesh->mNumFaces;
            //collect indices from all faces into an std::vector
            for (int i=0; i < tempMesh.numFaces; ++i)
            {
                for (int in=0; in < mesh->mFaces[i].mNumIndices; ++in)
                {
                    tempMesh.indices.push_back(mesh->mFaces[i].mIndices[in]);
                }
            }
            CALL_GL(glGenBuffers(1, &buffer));
            CALL_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer));
            CALL_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(int)*tempMesh.indices.size(),tempMesh.indices.data(),GL_STATIC_DRAW));

            // buffer for vertex texture coordinates
            if (mesh->HasTextureCoords(0)) {
                float *texCoords = (float *)malloc(sizeof(float)*2*mesh->mNumVertices);
                for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {

                    texCoords[k*2]   = mesh->mTextureCoords[0][k].x;
                    texCoords[k*2+1] = mesh->mTextureCoords[0][k].y;

                }
                glGenBuffers(1, &buffer);
                glBindBuffer(GL_ARRAY_BUFFER, buffer);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
                glEnableVertexAttribArray(texCoordLoc);
                glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, 0, 0, 0);
            }

            std::string assetpath;

            if (boost::filesystem::exists("assetpath")) { //if you're not using cmake put the shaders in the same dir as the binary
                assetpath = readTextFile("assetpath");
                assetpath = assetpath.substr(0,assetpath.size()-1); //cmake puts a newline char
                assetpath.append("/"); //at the end of the string
            }
             pho::myMaterial newMat;

             aiString path;	// filename to store path got from Assimp GetTexture

             int texIndex = 0; //for 1st texture (if many)

             //diffuse texture
             if (scene->mMaterials[scene->mMeshes[n]->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS )
             {
                 //add texture to material
                 newMat.diffuseTexture = gli::createTexture2D(assetpath+path.C_Str());
             }

            tempMesh.material = newMat;
            tempMesh.shader = tehShader;
            mMeshes.push_back(tempMesh);
    }
}


void pho::Asset::draw() {
    for (std::vector<pho::MyMesh>::size_type i = 0; i != mMeshes.size(); i++)
    {
        CALL_GL(glActiveTexture(GL_TEXTURE0));
        CALL_GL(glBindTexture(GL_TEXTURE_2D,mMeshes[i].material.diffuseTexture));
        mMeshes[i].shader->use();
        //mMeshes[i].shader["mvp"] = modelMatrix;
        CALL_GL(glBindVertexArray(mMeshes[i].vao));
        CALL_GL(glDrawElements(GL_TRIANGLES,mMeshes[i].numFaces*3,GL_UNSIGNED_INT,0));
    }
}





void pho::Asset::rotate(glm::mat4 rotationMatrix) {
        glm::vec4 tempPosition = modelMatrix[3];
        modelMatrix = rotationMatrix*modelMatrix;
        modelMatrix[3] = tempPosition;

}

void pho::Asset::setShader(pho::Shader *tehShader)
{
    for (std::vector<pho::MyMesh>::size_type i = 0; i != mMeshes.size(); i++)
    {
        mMeshes[i].shader = tehShader;
    }
}

void pho::Asset::setPosition(glm::vec3 position)
{
    modelMatrix[3] = glm::vec4(position,1);
}


pho::MyMesh::MyMesh()
{
    vao = numFaces = 0;

}
