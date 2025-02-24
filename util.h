#ifndef UTIL_H
#define UTIL_H

#include <vector>

using namespace std;

class ColorType
{
public:
    ColorType(unsigned char r, unsigned char g, unsigned char b) {
        mRed=r;
        mGreen=g;
        mBlue=b;
    };
    unsigned char mRed;
    unsigned char mGreen;
    unsigned char mBlue;
};

unsigned int getUniqueColors(unsigned int count, std::vector<ColorType>& colors, const std::vector<ColorType>& excludeColors);
#endif // UTIL_H
