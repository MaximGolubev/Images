#pragma once
#include "AbstractImage.h"
class JPGImage :
	public AbstractImage
{
public:
	JPGImage(const std::string& fileName);

	void save(const std::string& outputName) override;
	std::string stringType() const override;
};

