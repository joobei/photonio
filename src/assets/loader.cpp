#include "loader.h"
#include "boost/filesystem.hpp"

MeshLoader::MeshLoader()
{
    //load the asset path and store it
    if (boost::filesystem::exists("assetpath")) { //if you're not using cmake put the shaders in the same dir as the binary
        assetpath = readTextFile("assetpath");
        assetpath = assetpath.substr(0,assetpath.size()-1); //cmake puts a newline char
        assetpath.append("/"); //at the end of the string
    }
}

aiScene* pho::MeshLoader::loadFile(std::string filename)
{
    const aiScene* scene = importer.ReadFile( assetpath+filename,
            aiProcess_CalcTangentSpace       |
            aiProcess_Triangulate            |
            aiProcess_JoinIdenticalVertices  |
            aiProcess_SortByPType);

    // If the import failed, report it
    if( !scene)
    {
        DoTheErrorLogging( importer.GetErrorString());
        return false;
    }

    return scene;
}
