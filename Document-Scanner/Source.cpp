#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

Mat final(800, 800, CV_8UC3);
Point2f trans[] = { Point2f(0,0), Point2f(800,0), Point2f(800,800), Point2f(0,800) };
Point2f points[4];
Mat pers;
Mat pic = imread("exampleDocument2.jpg");

int tesseractFunction(Mat image) {
    cvtColor(image, image, COLOR_BGR2GRAY);
    // Initialize Tesseract API
    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
    if (api->Init(NULL, "eng")) {
        cerr << "Failed to initialize Tesseract API" << endl;
        return 1;
    }
    // Set image data
    api->SetImage(image.data, image.cols, image.rows, 1, image.cols);
    // Recognize text
    char* outText = api->GetUTF8Text();
    // Print recognized text
    cout << outText << endl;

    //Make text file
    
    ofstream file("example.txt");
    if (file.is_open()) {  // check if file is open
        file << outText;  // write text to file
        file.close();  // close file
        cout << "File created successfully.\n";
    }
    else {
        cout << "Unable to create file.\n";
    }

    // Release resources
    api->End();
    delete[] outText;

    return 0;
    
}

void getPoints(int etkinlik, int x, int y, int flags, void* userdata) {
    static int clickCounter = 0;
	if (etkinlik == EVENT_LBUTTONDOWN) {
		if (clickCounter < 4) {
			points[clickCounter] = Point2f(x, y);
		}
		else {
			pers = getPerspectiveTransform(points, trans);
			warpPerspective(pic, final, pers, final.size());
            tesseractFunction(final);
			imshow("pers", final);
		}
		clickCounter++;

	}
}

int main() {
	namedWindow("Resim", WINDOW_FREERATIO);
	setMouseCallback("Resim", getPoints, NULL);
	imshow("Resim", pic);
	waitKey(0);
}