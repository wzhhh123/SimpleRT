#pragma once


#include "header.h"
#include "map"


class TexInfo
{
public:
    std::string name;
};

template <typename T>
class Texture {
  public:
    // Texture Interface
    virtual T Evaluate(const IntersectPoint &) const = 0;
    virtual ~Texture() {}
};


template <typename T>
class MipMap{
    
};


template<typename TMem, typename TReturn>
class ImageTexture: public Texture<TReturn>
{
public:
    
    ImageTexture(std::string fileName);
    
private:
    
    MipMap<TMem> *mipmap;
    static std::map<TexInfo, std::unique_ptr<MipMap<TMem>>> textures;
    static MipMap<TMen>* GetTexture(std::string fileName);
};



template class ImageTexture<dVec3, dVec3>;
