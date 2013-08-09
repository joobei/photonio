#include "asset.h"

pho::Asset::Asset():receiveShadow(false)
{}

pho::Asset::Asset(const std::string& filename, pho::Shader* tehShader, sharedResources* shared) :receiveShadow(false), beingIntersected(false)
{
    res = shared;
    this->shader = tehShader;
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

            //store vertices for physics
            for (int i=0;i<mesh->mNumVertices;++i) {
                vertices.push_back(glm::vec3(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z));
            }

            if (mesh->HasNormals()) {

                CALL_GL(glGenBuffers(1, &buffer));
                CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
                CALL_GL(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW));
                CALL_GL(glEnableVertexAttribArray(normalLoc));
                CALL_GL(glVertexAttribPointer(normalLoc, 3, GL_FLOAT,0, 0, 0));
            }
            else {
                log("No Normals!!!");
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
            else {
                log("No Texture Coordinates!!!");
            }


            std::string assetpath;

            if (boost::filesystem::exists("assetpath")) { //if you're not using cmake put the shaders in the same dir as the binary
                assetpath = readTextFile("assetpath");
                assetpath = assetpath.substr(0,assetpath.size()-1); //cmake puts a newline char
                assetpath.append("/"); //at the end of the string
            }

             aiString path;	// filename to store path got from Assimp GetTexture

             int texIndex = 0; //for 1st texture (if many)

             //diffuse texture
             if (scene->mMaterials[scene->mMeshes[n]->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS )
             { tempMesh.material.diffuseTexture = gli::createTexture2D(assetpath+path.C_Str()); }

             //normal map texture
             if (scene->mMaterials[scene->mMeshes[n]->mMaterialIndex]->GetTexture(aiTextureType_HEIGHT, texIndex, &path) == AI_SUCCESS )
             {
                 log("**** BUMP MAP!!");
                 tempMesh.material.hasBumpMap = true;
                 tempMesh.material.normalTexture = gli::createTexture2D(assetpath+path.C_Str());
             }

             aiColor3D tempColor;
             aiMaterial* tempMat = scene->mMaterials[scene->mMeshes[n]->mMaterialIndex];
             if(AI_SUCCESS != tempMat->Get(AI_MATKEY_COLOR_DIFFUSE,tempColor)) {
                 log("----> FAILED Diffuse Color");
             }
             else {
                 tempMesh.material.diffuseColor.r = tempColor.r;
                 tempMesh.material.diffuseColor.g = tempColor.g;
                 tempMesh.material.diffuseColor.b = tempColor.b;
             }

             if(AI_SUCCESS != tempMat->Get(AI_MATKEY_COLOR_SPECULAR,tempMesh.material.specularColor)) {
                 log("----> FAILED Specular Color");
             }
             else {
                 tempMesh.material.specularColor.r = tempColor.r;
                 tempMesh.material.specularColor.g = tempColor.g;
                 tempMesh.material.specularColor.b = tempColor.b;
             }
             tempMat->Get(AI_MATKEY_SHININESS,tempMesh.material.shininess);
             mMeshes.push_back(tempMesh);
    }
}


void pho::Asset::draw() {   

    if (beingIntersected) {

        CALL_GL(glDisable(GL_DEPTH_TEST));

        scaleMatrix = glm::scale(glm::mat4(),glm::vec3(1.05,1.05,1.05));
        res->flatShader.use();
        res->flatShader["mvp"] = res->projectionMatrix*res->viewMatrix*modelMatrix*scaleMatrix;
        res->flatShader["color"] = glm::vec4(1,0,0,1);

        for (std::vector<pho::MyMesh>::size_type i = 0; i != mMeshes.size(); i++)
        {
            CALL_GL(glBindVertexArray(mMeshes[i].vao));
            CALL_GL(glDrawElements(GL_TRIANGLES,mMeshes[i].numFaces*3,GL_UNSIGNED_INT,0));
        }

        CALL_GL(glEnable(GL_DEPTH_TEST));
        beingIntersected = false;
    }


    shader->use();
    shader[0]["model"] = modelMatrix;
    shader[0]["modelview"] = res->viewMatrix*modelMatrix;
    shader[0]["mvp"] = res->projectionMatrix*res->viewMatrix*modelMatrix;
    shader[0]["shadowMatrix"] = res->biasMatrix*res->projectionMatrix*res->light.viewMatrix*modelMatrix;
    shader[0]["receiveShadow"] = receiveShadow;
    shader[0]["light_position"] = glm::vec4(res->light.position,1);
    shader[0]["light_diffuse"] = glm::vec4(1,1,1,1);
    shader[0]["light_specular"] = glm::vec4(1,1,1,1);
    for (std::vector<pho::MyMesh>::size_type i = 0; i != mMeshes.size(); i++)
    {

        //if there's a bump map bind it
        if (mMeshes[i].material.hasBumpMap) {
            CALL_GL(glActiveTexture(GL_TEXTURE1));
            CALL_GL(glBindTexture(GL_TEXTURE_2D,mMeshes[i].material.normalTexture));
        }
        else {
            CALL_GL(glActiveTexture(GL_TEXTURE1));
            CALL_GL(glBindTexture(GL_TEXTURE_2D,0));
        }

        CALL_GL(glActiveTexture(GL_TEXTURE0));
        CALL_GL(glBindTexture(GL_TEXTURE_2D,mMeshes[i].material.diffuseTexture));


        shader[0]["material_diffuse"] = glm::vec4(mMeshes[i].material.diffuseColor,1);
        shader[0]["material_specular"] = glm::vec4(mMeshes[i].material.specularColor,1);
        shader[0]["material_shininess"] = mMeshes[i].material.shininess;


        CALL_GL(glBindVertexArray(mMeshes[i].vao));
        CALL_GL(glDrawElements(GL_TRIANGLES,mMeshes[i].numFaces*3,GL_UNSIGNED_INT,0));
    }
}

void pho::Asset::drawFlat()
{
    res->flatShader.use();
    res->flatShader["mvp"] = res->projectionMatrix*res->viewMatrix*modelMatrix;
    res->flatShader["color"] = glm::vec4(1,1,1,1);

    for (std::vector<pho::MyMesh>::size_type i = 0; i != mMeshes.size(); i++)
    {
        CALL_GL(glBindVertexArray(mMeshes[i].vao));
        CALL_GL(glDrawElements(GL_TRIANGLES,mMeshes[i].numFaces*3,GL_UNSIGNED_INT,0));
    }
}

void pho::Asset::scale()
{
    glm::vec4 tempPosition = modelMatrix[3];
    modelMatrix[3] = glm::vec4(0);
    modelMatrix = modelMatrix*scaleMatrix;
    modelMatrix[3] = tempPosition;
}


void pho::Asset::rotate(glm::mat4 rotationMatrix) {
        glm::vec4 tempPosition = modelMatrix[3];
        modelMatrix = rotationMatrix*modelMatrix;
        modelMatrix[3] = tempPosition;

}

void pho::Asset::setPosition(glm::vec3 position)
{
    modelMatrix[3] = glm::vec4(position,1);
}

void pho::Asset::setScale(float scaleFactor)
{
    scaleMatrix = glm::scale(glm::mat4(1),glm::vec3(scaleFactor,scaleFactor,scaleFactor));
}


pho::MyMesh::MyMesh()
{
    vao = numFaces = 0;

}
