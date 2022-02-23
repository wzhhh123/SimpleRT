#pragma once


#include "header.h"
#include "map"


class TexInfo
{
public:
    std::string name;
	int h, w;
	bool operator<(const TexInfo &b) const
	{
		if (name != b.name) return name < b.name;
		if (w != b.w) return w < b.w;
		return h < b.w;
	}
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
public:
	MipMap(int w, int h, T* data);

	T Evaluate(const IntersectPoint& it);

private:
	int width, height;
	std::unique_ptr<T[]> pixels;
};


template<typename TMem, typename TReturn>
class ImageTexture: public Texture<TReturn>
{
public:
    
    ImageTexture(std::string fileName);
    
	TReturn Evaluate(const IntersectPoint &) const override;

private:
    
    MipMap<TMem> *mipmap;
    static std::map<TexInfo, std::unique_ptr<MipMap<TMem>>> textures;
    static MipMap<TMem>* GetTexture(std::string fileName);
};



template class ImageTexture<dVec3, dVec3>;
