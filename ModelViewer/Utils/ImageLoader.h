#pragma once
#ifndef _image_loader_
#define _image_loader_
#include "stb_image.h"
#include <glcorearb.h>
#include <map>
#include <vector>

struct ImageStruct {
    GLuint tbo;
    int width;
    int height;
    int channel;
};
class ImageLoader
{
private:
    static std::map<std::string, ImageStruct> textureGroup;

    GLuint tbo = -1; GLenum texture_type;
    int width = 0, height = 0, channel;
    bool have_release;
public:
    ImageLoader();
    ImageLoader(GLenum type, GLuint itbo);
    ~ImageLoader();
    void Release();
    void loadTextureFromFile(const char* path, bool filpUpsideDown = false);

    int getTextureWidth();
    int getTextureHeight();
    static void* getTexturePointer(const char* imagePath, int &width, int &height, int &channels);
    GLuint getTextureBufferObjectHandle();

    static unsigned int loadCubeMap(std::vector<std::string> faces);
};
#endif 
