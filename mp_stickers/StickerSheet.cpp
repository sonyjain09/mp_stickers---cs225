#include "StickerSheet.h"
using namespace std;

//sticker sheet constructor. sets private variables to parameters passed in and empties vectors
StickerSheet::StickerSheet (const Image &picture, unsigned max){
    img = picture;
    maxS = max;
    emptyVectors();
}

//function that goes through three private member variable vectors and clears them
void StickerSheet::emptyVectors(){
    for(int i = 0; i < int(this->stickers.size()); i++){
        this->stickers.pop_back();
        this->xcords.pop_back();
        this->ycords.pop_back();
    }
}

//calls empty vectors function. destorys all data in sticker sheet object
StickerSheet::~StickerSheet (){
    this->emptyVectors();
}

//copy constructor that sets all private member variables from passed in paramter to self
StickerSheet::StickerSheet (const StickerSheet &other){
    this->img = other.img;
    this->maxS = other.maxS;
    this->emptyVectors();
    for(int i = 0; i < int(other.stickers.size()); i++){
        (this->stickers).push_back(other.stickers.at(i));
        (this->xcords).push_back(other.xcords.at(i));
        (this->ycords).push_back(other.ycords.at(i));
    }
}

//overloaded operator that sets all private member variables from passed in paramter to self
const StickerSheet & StickerSheet::operator= (const StickerSheet &other){
    this->img = other.img;
    this->maxS = other.maxS;
    this->emptyVectors();
    for(int i = 0; i < int(other.stickers.size()); i++){
        (this->stickers).push_back(other.stickers.at(i));
        (this->xcords).push_back(other.xcords.at(i));
        (this->ycords).push_back(other.ycords.at(i));
    }
    return *this;
}

//sets the max number of possible stickers to passed in parameter. if the passed in max value is less than 
//the current max removes elements from end of all vectors till the current max equals the new max
void StickerSheet::changeMaxStickers (unsigned max){
    if(int(max) < maxS){
        while(max < stickers.size()){
            stickers.pop_back();
            xcords.pop_back();
            ycords.pop_back();
        }
    }
    maxS = max;
}

//adds a sticker and its location to the end of all vectors
int StickerSheet::addSticker (Image &sticker, unsigned x, unsigned y){
    if(int(stickers.size()) != maxS){
        stickers.push_back(&sticker);
        xcords.push_back(x);
        ycords.push_back(y);
        return stickers.size() - 1;
    }
    return -1;
}

//changes the x and y coordinate of sticker at specified index in coordinate vectors
bool StickerSheet::translate (unsigned index, unsigned x, unsigned y){
    if(index >= 0 and index < stickers.size()){
        xcords.at(index) = x;
        ycords.at(index) = y;
        return true;
    }
    return false;
}

//removes the element at specified index in all three vectors 
void StickerSheet::removeSticker (unsigned index){
    if(index >= 0 and index < stickers.size()){
        (this->stickers).erase((this->stickers).begin() + index);
        (this->xcords).erase((this->xcords).begin() + index);
        (this->ycords).erase((this->ycords).begin() + index);
    }
}

//returns the sticker at specified index from stickers vector
Image * StickerSheet::getSticker (unsigned index){
    if(index >= 0 and index < stickers.size()) return stickers.at(index);
    return nullptr;
}


Image StickerSheet::render () const{
    //finds the max width and max height that the end of a sticker can touch 
    int maxW = 0;
    int maxH = 0;
    for(int i = 0; i < int(this->stickers.size()); i++){
        int x = xcords.at(i);
        int y = ycords.at(i);
        int imageW = this->stickers.at(i)->width();
        int imageH = this->stickers.at(i)->height();
        if((x+imageW) > maxW) maxW = x + imageW;
        if((y+imageH) > maxH) maxH = y + imageH;
    }
    //if neccesary resizes the image in the direction needed 
    Image stickerSheet = this->img;
    if(maxW > int(this->img.width()) and maxH > int(this->img.height())) stickerSheet.resize(maxW, maxH);
    else if(maxW > int(this->img.width()))stickerSheet.resize(maxW, this->img.height());
    else if(maxH > int(this->img.height())) stickerSheet.resize(this->img.width(), maxH);
    //loops through stickers 
    for(int i = 0; i < int(stickers.size()); i++){
        int xStart = this->xcords.at(i);
        int yStart = this->ycords.at(i);
        int stickW = this->stickers.at(i)->width();
        int stickH = this->stickers.at(i)->height();
        //loops through the pixels the sticker is part of 
        for(int x = xStart; x < (xStart + stickW); x++){
            for(int y = yStart; y < (yStart + stickH); y++){
                cs225::HSLAPixel & stickPix = stickerSheet.getPixel(x, y);
                cs225::HSLAPixel & imagePix = this->stickers.at(i)->getPixel(x-xStart, y-yStart);
                //changes the stickersheet image pixel to sticker image pixel if alpha value does 
                //not equal 0
                if(imagePix.a != 0){
                    stickPix.h = imagePix.h;
                    stickPix.s = imagePix.s;
                    stickPix.l = imagePix.l;
                    stickPix.a = imagePix.a;
                }
            }
        }
    }
    return stickerSheet;
}