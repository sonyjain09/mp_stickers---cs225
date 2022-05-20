#include "Image.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

//adds .1 to luminance of all pixels if possible
void Image::lighten(){
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            cs225::HSLAPixel & pixel = this->getPixel(x, y);
            if(pixel.l + .1 <= 1) pixel.l += .1;
            else pixel.l = 1;
        }
    }
}
//adds amount to luminance of all pixels if possible
void Image::lighten(double amount){
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            cs225::HSLAPixel & pixel = this->getPixel(x, y);
            if(pixel.l + amount <= 1) pixel.l += amount;
            else pixel.l = 1;
        }
    }
}
//subtracts .1 to luminance of all pixels if possible
void Image::darken(){
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            cs225::HSLAPixel & pixel = this->getPixel(x, y);
            if(pixel.l - .1 >= 0) pixel.l -= .1;
            else pixel.l = 0;
        }
    }
}
//adds amount to luminance of all pixels if possible
void Image::darken(double amount){
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            cs225::HSLAPixel & pixel = this->getPixel(x, y);
            if(pixel.l - amount >= 0) pixel.l -= amount;
            else pixel.l = 0;
        }
    }
}
//adds .1 to saturations of all pixels if possible
void Image::saturate(){
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            cs225::HSLAPixel & pixel = this->getPixel(x, y);
            if(pixel.s + .1 <= 1) pixel.s += .1;
            else pixel.s = 1;
        }
    }   
}
//adds amount to saturations of all pixels if possible
void Image::saturate(double amount){
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            cs225::HSLAPixel & pixel = this->getPixel(x, y);
            if(pixel.s + amount <= 1) pixel.s += amount;
            else pixel.s = 1;
        }
    }  
}
//subtracts .1 to saturations of all pixels if possible
void Image::desaturate(){
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            cs225::HSLAPixel & pixel = this->getPixel(x, y);
            if(pixel.s - .1 >= 0) pixel.s -= .1;
            else pixel.s = 0;
        }
    }
}
//subtracts amount to saturations of all pixels if possible
void Image::desaturate(double amount){
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            cs225::HSLAPixel & pixel = this->getPixel(x, y);
            if(pixel.s - amount >= 0) pixel.s -= amount;
            else pixel.s = 0;
        }
    }
}
//changes saturations of all pixels to 0 
void Image::grayscale(){
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            cs225::HSLAPixel & pixel = this->getPixel(x, y);
            pixel.s = 0;
        }
    }
}
//adds degrees to the value of hue of each pixel ensuring it is in the range 0 and 360
void Image::rotateColor(double degrees){
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            cs225::HSLAPixel & pixel = this->getPixel(x, y);
            if(pixel.h + degrees > 360) pixel.h = pixel.h + degrees - 360;
            else if(pixel.h + degrees < 0) pixel.h = pixel.h + degrees + 360;
            else pixel.h += degrees;
        }
    }   
}

//changes pixel hue to illini orange or blue depending on which hue it is originally closer to 
void Image::illinify(){
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            cs225::HSLAPixel & pixel = this->getPixel(x, y);
            int distBlue;
            int distOrange;
            if(std::abs(pixel.h - 11) > 180) distOrange = 360 - std::abs(pixel.h - 11);
            else distOrange = std::abs(pixel.h - 11);
            if(std::abs(pixel.h - 216) > 180) distBlue = 360 - std::abs(pixel.h - 216);
            else distBlue = std::abs(pixel.h - 216);
            if(distBlue < distOrange) pixel.h = 216;
            else pixel.h = 11;
        }
    }
}

//scales image by factor
void Image::scale(double factor){
    //find new dimensions and make new png image with those dimensions
    int newW = this->width() * factor;
	int newH = this->height() * factor;
    PNG* temp = new PNG(newW, newH);
    //loop through new png 
	for (int x = 0; x < newW; x++) {
		for (int y = 0; y < newH; y++) {
            //set the temporary image pixel data to the wanted original image pixel data
			cs225::HSLAPixel & pixelOrig = this->getPixel(x/factor, y/factor);
			cs225::HSLAPixel & pixelTemp = temp->getPixel(x, y);
			pixelTemp.h = pixelOrig.h;
            pixelTemp.s = pixelOrig.s;
            pixelTemp.l = pixelOrig.l;
            pixelTemp.a = pixelOrig.a;
		}
	}
    //resize the new image to new dimensions
    this->resize(newW, newH);
    //copy temp over to original image
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            cs225::HSLAPixel & pixelOrig = this->getPixel(x, y);
            cs225::HSLAPixel & pixelTemp = temp->getPixel(x, y);
            pixelOrig.h = pixelTemp.h;
            pixelOrig.s = pixelTemp.s;
            pixelOrig.l = pixelTemp.l;
            pixelOrig.a = pixelTemp.a;
        }
    } 
    // get rid of temp
    delete temp;
}

//scales image to wanted width and height while keeping original ratio between the two
void Image::scale(unsigned w, unsigned h){
    //find factor of each and go with the smaller factor (has to fit within given dimensions)
    double factorW = double(w)/this->width();
    double factorH = double(h)/this->height();
    double factor;
    if(factorW < factorH) factor = factorW;
    else factor = factorH;
    //use previous scale function
    this->scale(factor);
}