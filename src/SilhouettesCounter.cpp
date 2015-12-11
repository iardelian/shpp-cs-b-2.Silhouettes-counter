#include  <iostream>
#include "gwindow.h"
#include "gbufferedimage.h"
#include "gobjects.h"

using namespace std;

//function prototypes

int pixCounter = 100;// the minimum number of pixels required to determine the silhouette

//@param imgName - name of the input image
int countSilhouettes(string imgName);

//@param bitMap - a bitmap of image
//@param imgBuffer - matrix of colored pixels
//@param imgWidth - width of the image
//@param imgHeight - height of the image
void binarization(int **bitMap, GBufferedImage* imgBuffer, int imgWidth, int imgHeight);

//@param x, y - coordinates of the bitmap
//@param mark - label used for marking pixels of the silhouettes
//@param newMark - label used for separation pixels of the silhouettes
void marking(int x, int y, int mark, int newMark, int **bitMap, int imgWidth, int imgHeight);

int main() {
    string imgName;
    cout << "Please, enter name of the image: ";
    cin >> imgName;
    cout << "Result: There is(are) " << countSilhouettes(imgName) << " silhouette(s) on the image." << endl;
    return 0;
}

int countSilhouettes(string imgName){
    GImage* image;
    image = new GImage(imgName);
    GWindow gw(image->getWidth(),image->getHeight());// window size is the size of the image
    gw.add(image);
    GBufferedImage* imgBuffer = new GBufferedImage(0, 0, image->getWidth(), image->getHeight());//matrix size is the size of the image
    imgBuffer->load(imgName);
    //image width and height.
    int imgHeight = (int)imgBuffer->getHeight();
    int imgWidth = (int)imgBuffer->getWidth();
    //create a bitmap array
    int **bitMap = new int *[imgWidth];
    for(int x = 0; x < imgWidth; x++){
        bitMap[x] = new int[imgHeight];
    }
    // binarization of the image
    binarization(bitMap, imgBuffer, imgWidth, imgHeight);
    int mark = 1;
    int newMark = 1;
    int result = 0;// silhouettes counter
    for(int x = 1; x < imgWidth; x++){
        for(int y = 1; y < imgHeight; y++){
            if(bitMap[x][y] == 1){
                if (pixCounter >=100){ //if the object of more than 100 pixels, we assume that this is silhouette
                newMark++;// when out of recurcive marking cycle - new mark
                result++;// and result will be increased by one
                }
                pixCounter = 0;//zero out after finding each silhouette
                marking(x, y, mark, newMark, bitMap, imgWidth, imgHeight);
            }
        }
    }
    // clean memory
    for(int x = 0; x < imgWidth; x++){
        delete[]bitMap[x];
    }
    delete []bitMap;
    delete image;
    delete imgBuffer;
    return result;
}
// binarization of image
void binarization(int **bitMap, GBufferedImage* imgBuffer, int imgWidth, int imgHeight){
    int backColor = 13750737; // color for background (HEX #d1d1d1)
    int pixRGB = 0;
    for(int y = 0; y < imgHeight; y++){
        for(int x = 0; x < imgWidth; x++){
            pixRGB = imgBuffer->getRGB(x, y);
            if(pixRGB > backColor){
                bitMap[x][y] = 0;
            }else{
                bitMap[x][y] = 1;
            }
        }
    }
}
//recursive function put unique numeric layer on each found separated object on image.
void marking(int x, int y, int mark, int newMark, int **bitMap, int imgWidth, int imgHeight){
    if(x >= 0 && x < imgWidth && y >= 0 && y < imgHeight && bitMap[x][y] == mark && bitMap[x][y] != newMark){
        pixCounter++;
        bitMap[x][y] = newMark;
        marking(x, y+1, mark, newMark, bitMap, imgWidth, imgHeight);
        marking(x, y-1, mark, newMark, bitMap, imgWidth, imgHeight);
        marking(x-1, y, mark, newMark, bitMap, imgWidth, imgHeight);
        marking(x+1, y, mark, newMark, bitMap, imgWidth, imgHeight);
    }
}
