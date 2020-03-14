#include "JPGImage.h"

#include"JpegCompressor\jpgd.h"
#include"JpegCompressor\jpge.h"

JPGImage::JPGImage(const std::string& fileName) : AbstractImage()
{
	_channelCount = 4;
	int width, height, actualComps;		// в actualComps хранится реальное кол-во каналов, в данном случае 3, хотя массив мы забиваем так, будто бы 4
	unsigned char* data = jpgd::decompress_jpeg_image_from_file(fileName.c_str(), &width, &height, &actualComps, _channelCount);
	PixelMatrix buffer(reinterpret_cast<Pixel*>(data), width, height);
	_pixelData.moveFrom(buffer);
	if (data) {
		delete[] data;
	}
}

void JPGImage::save(const std::string& outputName)
{
	jpge::compress_image_to_jpeg_file(outputName.c_str(), width(), height(), _channelCount, rawData());
}

std::string JPGImage::stringType() const
{
	return ".jpeg";
}
