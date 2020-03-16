#pragma once

#include <string>

using uchar = unsigned char;

enum class ImageType {
	Unknow,
	PNG,
	JPG,
	BMP
};

struct ImageSize {
	size_t width;
	size_t height;

	ImageSize(size_t w = 0, size_t h = 0);
};


// TODO: подумать над элегантным способом хранения компонент rgb и rgba,
// чтобы все было полиморфно и удобно.
// Сейчас при чтении bmp и jpg скорее всего придется преобразовывать массив 
// размера 3 * ширина * высота в массив 4 * ширина * высота.
#pragma pack(push, 1)
struct Pixel {
	uchar red;
	uchar green;
	uchar blue;
	uchar alpha;

	Pixel(uchar r = 0, uchar g = 0, uchar b = 0, uchar alpha = 0);

	size_t metric() const;
	size_t colorAvg() const;
};
#pragma pack(pop)

class AbstractImage;

class PixelMatrix {
public:
	friend AbstractImage;

	PixelMatrix(size_t w = 0, size_t h = 0);
	PixelMatrix(Pixel* data, size_t w = 0, size_t h = 0);

	Pixel* operator[](size_t i) const;

	// для продвинутых - гуглить move-семантику 
	// данный метод будет вызываться в случае, когда аргумент — rvalue (временный объект)
	// суть: если после приравнивания больше не нужен объект mv, нам не нужно копировать
	// данные, а можно просто перекинуть указатели из одного объекта в другой, при этом
	// объект-аргумент следует обязательно привести в целостное начальное состояние
	PixelMatrix& operator=(PixelMatrix&& mv) noexcept;

	void moveFrom(PixelMatrix& mv);

	~PixelMatrix();
private:
	Pixel* _data;
	size_t _height;
	size_t _width;
};

class AbstractImage
{
public:
	virtual ~AbstractImage();

	ImageType	type() const;
	size_t		height() const;
	size_t		width() const;
	ImageSize	size() const;

	static AbstractImage* create(const std::string& fileName, ImageType type = ImageType::Unknow);	// фабричная штука

	// мутация текущего изменения
	void resize(const ImageSize& size);						// изменить размер
	void resize(const size_t w, const size_t h);			// перегрузка изменения размера
	void widthResize(const size_t width);					// изменить ширину
	void heightResize(const size_t height);					// изменить высоту
	void toBlackWhite();									// превратить в ЧБ
	void toColorInversion();								// инверсия цветов
	void toSepia();											// сепия

	// получение нового изображения с сохранением формата
	AbstractImage* getResizeImage(const ImageSize& size) const;  
	AbstractImage* getBlackWhite() const;
	AbstractImage* getBlackWhite() const;
	AbstractImage* getColorInversion() const;
	AbstractImage* getSepia() const;
	
	// use your imagination: имея матрицу пикселей, вы можете реализовать любые фильтры

	virtual void save(const std::string& output) = 0;
	virtual std::string stringType() const = 0;		// ".png" и т.д.
protected:
	AbstractImage();
	PixelMatrix		_pixelData;
	ImageType		_type;
	size_t			_dataSize;
	size_t			_channelCount;

	Pixel* pixelLine(size_t i);
	Pixel& pixelAt(size_t i, size_t j);

	uchar* rawData();
};

