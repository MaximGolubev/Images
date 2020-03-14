#include "PNGImage.h"			// это интерфейс к нашему классу
#include "LodePNG\lodepng.h"	// а это интерфейс к нашей либе для разбора png

#include<cassert>

PNGImage::PNGImage(const std::string& fileName) : AbstractImage()
{
	unsigned char** buf = new unsigned char*(nullptr);
	unsigned width, height;
	unsigned err = lodepng::decode(buf, width, height, _dataSize, fileName, LCT_RGBA, 8u);
	PixelMatrix buffer(reinterpret_cast<Pixel*>(*buf), width, height); 
	_pixelData.moveFrom(buffer);
	if (*buf) {
		delete[] * buf;
	}
	delete buf;
	assert(err == 0);
}

void PNGImage::save(const std::string& outputName)
{
	lodepng::encode(outputName, rawData(), width(), height());
}

std::string PNGImage::stringType() const
{
	return ".png";
}
