
#include "texture.h"
#include "tool/imagehelper.h"
#include "struct/intersectpoint.h"


static void ConvertIn(dVec3& in, dVec3* out)
{
	*out = in;
}

static void ConvertOut(dVec3& in, dVec3& out)
{
	out = in;
}


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
    if(textures.find(texInfo) != textures.end()) {
        return textures[texInfo].get();
    }
	int w, h;
	dVec3* pixels = LoadImageVec(fileName, w, h);
	assert(pixels);
	std::unique_ptr<TMem[]>convertedTexels(new TMem[w*h]);
	for(int i = 0; i < w*h; ++i)
	{
		ConvertIn(pixels[i], &convertedTexels[i]);
	}
	MipMap<TMem>*mm = new MipMap<TMem>(w, h, convertedTexels.get());
	textures[texInfo].reset(mm);
	return mm;
}


template<typename TMem, typename TReturn>
TReturn ImageTexture<TMem, TReturn>::Evaluate(const IntersectPoint &is) const
{
	dVec3 val{ 215/255.0,171/255.0,100/255.0 };
	//TReturn ret;
	//convertOut(val, ret);
	return (TReturn)mipmap->Evaluate(is);
}


template<typename T>
MipMap<T>::MipMap(int w, int h, T* data)
{
	static T OneOver255 = { 1.0 / 255, 1.0 / 255,1.0 / 255 };
	width = w;
	height = h;
	T* convertedData = new T[w*h];
	for (int i = 0; i < w*h; ++i) convertedData[i] = data[i]*OneOver255;
	pixels.reset(convertedData);
}

template<typename T>
T MipMap<T>::Evaluate(const IntersectPoint& it)
{
	int u = ((int)std::floor(width*it.uv.x)) % width;
	int v = ((int)std::floor(height*(it.uv.y)))% height;
	int index = v * width + u;
	return { it.uv.x, it.uv.y,0 };
	//std::cout << pixels[index].x << " " << pixels[index].y << " " << pixels[index].z << std::endl;
	return pixels[index];
}

