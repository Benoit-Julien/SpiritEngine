#ifndef SPIRITENGINE_TEXTURE2D_HPP
#define SPIRITENGINE_TEXTURE2D_HPP

#include "Texture.hpp"

class Texture2D : public Texture {
 private:
	bool _initialised;

 protected:
	std::string _filename;
	int _width;
	int _height;
	int _channel;

 public:
	MinFilterEnum MinFilter;
	MagFilterEnum MagFilter;
	WrapEnum SWrap;
	WrapEnum TWrap;
	ColorFormatEnum ColorFormat;

 public:
	explicit Texture2D();
	virtual ~Texture2D();

	void initFromFile(const std::string &filename);

 private:
	virtual void setup();
};


#endif /* !SPIRITENGINE_TEXTURE2D_HPP */
