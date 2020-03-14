#include <iostream>
#include "AbstractImage.h"

int main() {
	const int size = 2;
	AbstractImage** images = new AbstractImage*[size];
	images[0] = AbstractImage::create("C:/file.png", ImageType::PNG); // если сделаете красивый create, то можно не передавать PNG!
	images[1] = AbstractImage::create("C:/filejpg.jpg", ImageType::JPG);

	std::cout << "Start processing... [Total count: " << size << "]" << std::endl;
	for (size_t i = 0; i < size; ++i) {
		std::cout << ">> " << i << " image" << std::endl;
		images[i]->widthResize(10000);
		images[i]->toBlackWhite();
		images[i]->save("resizedandwb" + images[i]->stringType());
		std::cout << ">> " << i << " image done" << std::endl;
	}
	std::cout << "Processing success!";

	return 0;
}