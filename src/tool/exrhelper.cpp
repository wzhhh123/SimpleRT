
#include "exrhelper.h"

#include <ImfRgba.h>
#include <ImfRgbaFile.h>

namespace EXR_HELPER
{

static void WriteImageEXR(const std::string &name, const float *pixels,
                          int width, int height);

void SaveAsExrFile(int width, int height, float* pixels)
{
    //namespace IMF = OPENEXR_IMF_NAMESPACE;
    WriteImageEXR("../../assets/image/result_apple.exr", pixels, width, height);
}

static void WriteImageEXR(const std::string &name, const float *pixels,
                          int width, int height)
{
    using namespace Imf;
    using namespace Imath;

    Rgba *hrgba = new Rgba[width * height];
    for (int i = 0; i < width * height; ++i)
        hrgba[i] = Rgba(pixels[3 * i], pixels[3 * i + 1], pixels[3 * i + 2]);

    // OpenEXR uses inclusive pixel bounds.
    Box2i displayWindow(V2i(0, 0), V2i(width - 1, height - 1));
    Box2i dataWindow(V2i(0, 0),
                     V2i(width-1, height-1));

    try {
        RgbaOutputFile file(name.c_str(), displayWindow, dataWindow,
                            WRITE_RGB);
        file.setFrameBuffer(hrgba, 1, width);
        file.writePixels(height);
    } catch (const std::exception &exc) {
        
    }

    delete[] hrgba;
}

}
