#include "Image.h"
#include "StickerSheet.h"

int main() {

  //
  // Reminder:
  //   Before exiting main, save your creation to disk as myImage.png
  //
  Image java;     
  java.readFromFile("tests/baseimage.png");
  java.scale(0.5);
  Image i;        
  i.readFromFile("tests/s1.png");
  Image j;
  j.readFromFile("tests/s2.png");
  j.scale(0.5);
  j.writeToFile("test.png");
  Image k;
  k.readFromFile("tests/s3.png");
  k.scale(0.5);
  StickerSheet sheet(java, 3);
  sheet.addSticker(i, 25, 25);
  sheet.addSticker(j, 600, 600);
  sheet.addSticker(k, 1200, 1000);
  Image out = sheet.render();
  out.writeToFile("myImage.png");
  return 0;
}
