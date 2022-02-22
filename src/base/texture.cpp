
#include "texture.h"



template<typename TMem, typename TReturn>
std::map<TexInfo, std::unique_ptr<MipMap<TMem>>> ImageTexture<TMem, TReturn>::textures;




template<typename TMem, typename TReturn>
ImageTexture<TMem, TReturn>::ImageTexture(std::string fileName)
{
    mipmap = GetTexture(fileName);
}


template<typename TMem, typename TReturn>
MipMap<TMem>* ImageTexture<TMem, TReturn>::GetTexture(std::string fileName)
{
    TexInfo texInfo;
    texInfo.name = fileName;
    if(textures.find(texInfo) != textures.end())
    {
        return textures[texInfo].get();
    }
    
    
    
    return nullptr;
}
