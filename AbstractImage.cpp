#include "AbstractImage.h"
#include "PNGImage.h"
#include "JPGImage.h"

#include <iostream>
#include <stdexcept>	// для тех, кто понимает
#include <cstring>		// memcpy
#include <cassert>

AbstractImage::~AbstractImage()
{
}

ImageType AbstractImage::type() const
{
	return _type;
}

size_t AbstractImage::height() const
{
	return _pixelData._height;
}

size_t AbstractImage::width() const
{
	return _pixelData._width;
}

ImageSize AbstractImage::size() const
{
	return ImageSize(_pixelData._width, _pixelData._height);
}

AbstractImage* AbstractImage::create(const std::string& fileName, ImageType type)
{
	// сюда добавить разбор fileName
	// в зависимости от расширения в названии файла можно вывести тип изображения
	// и создать соответствующее изображение.
	// Наивная реализация — искать точку и брать остаток строки 
	// (тогда сможем распарсить и .bmp, и .jpg, и .jpeg, и т.д.)

	switch (type) {
		case ImageType::PNG: {
			return new PNGImage(fileName);
		}
		case ImageType::JPG: {
			return new JPGImage(fileName);
		}
		// other types
		default:
			// throw std::runtime_error("Invalid type"); // вот пройдете исключения и будет хорошо
			assert(0 == 1);		// принудительно завершаем работу (можно через exit)
	}

	return nullptr;
}

void AbstractImage::resize(const ImageSize& size)
{
	if (size.width != width()) {
		widthResize(size.width);
	}

	if (size.height != height()) {
		heightResize(size.height);
	}
}

void AbstractImage::widthResize(const size_t width)
{
	size_t height = this->height();
	float ratio = this->width() * 1.f / width;
	PixelMatrix buffer(width, height);

	for (size_t i = 0; i < height; ++i) {
		for (size_t j = 0; j < width; ++j) {
			int beg = int(j * ratio);
			int end = int((j + 1) * ratio);

			unsigned maxMetric = _pixelData[i][beg].metric();
			unsigned index = beg;
			for (int k = beg; k < end; ++k) {
				if (_pixelData[i][beg].metric() > maxMetric) {
					maxMetric = _pixelData[i][beg].metric();
					index = k;
				}
			}
			buffer[i][j] = _pixelData[i][index];
		}
	}

	// _pixelData = std::move(buffer);	// тут будет вызван оператор присваивания перемещением
	_pixelData.moveFrom(buffer);		// аналогично
}

void AbstractImage::heightResize(const size_t height)
{
	// TODO: принцип тот же, что и у widthResize
}

void AbstractImage::toBlackWhite()
{
	for (size_t i = 0; i < height(); ++i) {
		for (size_t j = 0; j < width(); ++j) {
			size_t avg = _pixelData[i][j].colorAvg();
			if (avg <= 127) {
				_pixelData[i][j] = { 0, 0, 0, _pixelData[i][j].alpha };
			}
			else {
				_pixelData[i][j] = { 0xFF, 0xFF, 0xFF, _pixelData[i][j].alpha };
			}
		}
	}
}

AbstractImage::AbstractImage() 
{
	_type = ImageType::Unknow;
	_dataSize = 0;
	_channelCount = 4;
}

Pixel* AbstractImage::pixelLine(size_t i)
{
	if (i < this->height()) {
		return _pixelData[i];
	}
	else {
		return nullptr;
	}
}

Pixel& AbstractImage::pixelAt(size_t i, size_t j)
{
	if (j < width()) {
		assert(0);
	}
	return pixelLine(i)[j];
}

uchar* AbstractImage::rawData()
{
	return reinterpret_cast<uchar*>(_pixelData._data);	// reinterpret_cast<uchar*> == (uchar*)
}

inline ImageSize::ImageSize(size_t w, size_t h)
	: width(w), height(h) {}

Pixel::Pixel(uchar r, uchar g, uchar b, uchar a)
	: red(r), green(g), blue(b), alpha(a)
{
}

size_t Pixel::metric() const
{
	return red + green + blue;		// метрику можете менять, ваше пространство пикселей — ваши правила
}

size_t Pixel::colorAvg() const
{
	return (red + green + blue) / 3;
}

PixelMatrix::PixelMatrix(size_t w, size_t h)
	: _height(h), _width(w)
{
	_data = new Pixel[w * h];
}

PixelMatrix::PixelMatrix(Pixel* data, size_t w, size_t h)
{
	_data = new Pixel[w * h];
	memcpy(_data, data, w * h * sizeof(Pixel));
	_height = h;
	_width = w;
}

Pixel* PixelMatrix::operator[](size_t i) const
{
	return &_data[i * _width];
}

PixelMatrix& PixelMatrix::operator=(PixelMatrix&& mv) noexcept
{
	this->_data = mv._data;
	this->_height = mv._height;
	this->_width = mv._width;

	mv._data = nullptr;
	mv._height = mv._width = 0;

	return *this;
}

void PixelMatrix::moveFrom(PixelMatrix& mv)
{
	this->_data = mv._data;
	this->_height = mv._height;
	this->_width = mv._width;

	mv._data = nullptr;
	mv._height = mv._width = 0;
}

PixelMatrix::~PixelMatrix()
{
	delete[] _data;
}
