#include  <iostream>
#include "gwindow.h"
#include "gbufferedimage.h"
#include "gobjects.h"

using namespace std;

const int MIN_SILHOUETTE_SIZE = 100;// the minimum number of pixels required to determine the silhouette
const int BACKGROUND_COLOR = 13750737; // color for background (HEX #d1d1d1)

//function prototypes

//@param imgName - name of the input image
int countSilhouettes(string imgName);

/*
*@param bitMap - a bitmap of image
*@param imgBuffer - matrix of colored pixels
*@param imgWidth - width of the image
*@param imgHeight - height of the image
*/
void binarizationImage(int **bitMap, GBufferedImage* imgBuffer, int imgWidth, int imgHeight);

/*
*@param x, y - coordinates of the bitmap
*@param oldValue - label used for marking pixels of the silhouettes
*@param newValue - label used for separation pixels of the silhouettes
*@param silhouetteSize - the minimum size of the silhouette to be taken into account
*/
void fillRegion(int x, int y, int oldValue, int newValue, int **bitMap, int imgWidth, int imgHeight, int &silhouetteSize);

int countSilhouettes(string imgName) {
    GImage *image;
    image = new GImage(imgName);
    //object window
    GWindow gw(image->getWidth(), image->getHeight());// window size is the size of the image
    gw.add(image);
    //buffer for image pixel reading
    GBufferedImage *imgBuffer = new GBufferedImage(0, 0, image->getWidth(), image->getHeight()); //matrix size is the size of the image
    imgBuffer->load(imgName);
    //image width and height
    int imgHeight = (int)imgBuffer->getHeight();
    int imgWidth = (int)imgBuffer->getWidth();
    //create a bitmap array
    int **bitMap = new int *[imgWidth];

    for (int x = 0; x < imgWidth; x++) {
        bitMap[x] = new int[imgHeight];
    }

    binarizationImage(bitMap, imgBuffer, imgWidth, imgHeight);// binarization of the image
    int oldValue = 1; // new value of the pixel
    int newValue = 1;// old value of the pixel
    int silhouettesOnImg = 0;// silhouettes counter

    for (int x = 1; x < imgWidth; x++) {
        for (int y = 1; y < imgHeight; y++) {
            if (bitMap[x][y] == 1) {
                newValue++;
                int silhouetteSize = 0;
                fillRegion(x, y, oldValue, newValue, bitMap, imgWidth, imgHeight, silhouetteSize);

                // if the size of the selected area is less than MIN_SILHOUETTE_SIZE,
                // we do not consider this area a separate silhouette
                if (silhouetteSize >= MIN_SILHOUETTE_SIZE) {
                    silhouettesOnImg++;
                }
            }
        }
    }

    // clean memory
    for (int x = 0; x < imgWidth; x++) {
        delete[]bitMap[x];
    }

    delete []bitMap;
    delete image;
    delete imgBuffer;
    return silhouettesOnImg;
}

void binarizationImage(int **bitMap, GBufferedImage* imgBuffer, int imgWidth, int imgHeight){
    int pixelColor = 0;
    for (int y = 1; y < imgHeight; y++) {
        for (int x = 1; x < imgWidth; x++) {
            pixelColor = imgBuffer->getRGB(x, y);
            if (pixelColor > BACKGROUND_COLOR) {
                bitMap[x][y] = 0;
            } else {
                bitMap[x][y] = 1;
            }
        }
    }
}

//recursive function that found filling set in the binary plane
void fillRegion(int x, int y, int oldValue, int newValue, int **bitMap, int imgWidth, int imgHeight, int & silhouetteSize) {

    if (x >= 0 && x < imgWidth && y >= 0 && y < imgHeight && bitMap[x][y] == oldValue && bitMap[x][y] != newValue) {
        silhouetteSize++;
        bitMap[x][y] = newValue;
        fillRegion(x, y+1, oldValue, newValue, bitMap, imgWidth, imgHeight, silhouetteSize);
        fillRegion(x, y-1, oldValue, newValue, bitMap, imgWidth, imgHeight, silhouetteSize);
        fillRegion(x-1, y, oldValue, newValue, bitMap, imgWidth, imgHeight, silhouetteSize);
        fillRegion(x+1, y, oldValue, newValue, bitMap, imgWidth, imgHeight, silhouetteSize);
    }
}

int main() {
    string imgName;
    cout << "Please, enter name of the image: ";
    cin >> imgName;
    cout << "silhouette: There is/are " << countSilhouettes(imgName) << " silhouette/s on the image." << endl;
    return 0;
}
