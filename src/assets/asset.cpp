#include "asset.h"

pho::Asset::Asset()
{}

pho::Asset::Asset(const std::string& filename)
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

    upload();
}

void pho::Asset::upload()
{
    for (unsigned int n = 0; n < scene->mNumMeshes; ++n)
        {
            const struct aiMesh* mesh = scene->mMeshes[n];
            myMesh tempMesh;

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
                CALL_GL(glGenBuffers(1, &buffer));
                CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
                CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW));
                CALL_GL(glEnableVertexAttribArray(normalLoc));
                CALL_GL(glVertexAttribPointer(normalLoc, 3, GL_FLOAT,0, 0, 0));
            }

            tempMesh.numFaces = mesh->mNumFaces;

            for (int i=0; i < tempMesh.numFaces; ++i)
            {
                for (int in=0; in < mesh->mFaces[i].mNumIndices; ++in)
                {
                    tempMesh.indices.push_back(mesh->mFaces[i].mIndices[in]);
                }
            }


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
            tempMesh.materialIndex = mesh->mMaterialIndex;
            mMeshes.push_back(tempMesh);
    }
}

void pho::Asset::draw() {
    for (std::vector<pho::myMesh>::size_type i = 0; i != mMeshes.size(); i++)
    {
        CALL_GL(glBindVertexArray(mMeshes[i].vao));
        //CALL_GL(glDrawArrays(GL_TRIANGLES,0,mMeshes[i].numFaces*3));
        CALL_GL(glDrawElements(GL_TRIANGLES,mMeshes[i].numFaces*3,GL_UNSIGNED_INT,mMeshes[i].indices.data()));
    }
}

void pho::Asset::rotate(glm::mat4 rotationMatrix) {
        glm::vec4 tempPosition = modelMatrix[3];
        modelMatrix = rotationMatrix*modelMatrix;
        modelMatrix[3] = tempPosition;

}
