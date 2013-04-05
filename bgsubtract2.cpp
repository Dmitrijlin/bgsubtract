// source: http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"

#include <iostream>
#include <time.h>
#include <limits.h>

using namespace cv;
using namespace std;

Mat bgsubtract(Mat frame){
	Mat foreground;
	BackgroundSubtractorMOG2 mog;	
	mog(frame, foreground, -1);
	threshold(foreground, foreground, 128, 255, THRESH_BINARY);
	erode(foreground, foreground, Mat());
	return foreground;
}

/**
const char* keys = 
	"{r resize		|	1	| resize factor, default=1}"
	"{b bgs			|		| do background subtraction}"
;
*/

int main(int argc, char** argv)
{
    const char* outFile = "./out.mjpg";
	int resizeFactor = 1;
    bool doBGS = false;
    bool writeOut = true;
    bool displayWindows = false;
    
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
	
	/**
	CommandLineParser parser(argc, argv, keys);
	if (!parser.check()){
		parser.printErrors();
		return 0;
	}
	
	if (parser.has("bgs"))
		doBGS = true;
		
	if (parser.has("resize"))
		resizeFactor = parser.get<int>("resize");
	*/
	
    int imgSizeX = resizeFactor * 320;
    int imgSizeY = resizeFactor * 240;
    
	// fps counter begin
	time_t start, end;
	int counter = 0;
	double sec;
	double fps;
	// fps counter end
	
	Mat foreground;
	BackgroundSubtractorMOG2 mog;	

	if (displayWindows) namedWindow("Source",1);
	if (displayWindows && doBGS) namedWindow("Background Subtraction", 1);
    
    while(1)
    {
		if (counter == 0){
			time(&start);
		}
        Mat frame;
        cap >> frame; // get a new frame from camera
        
        Mat frameResized;
        resize(frame, frameResized, Size(imgSizeX, imgSizeY));
        
        if (doBGS){
			//Mat fg = bgsubtract(frameResized);
			mog(frameResized, foreground, -1);
			threshold(foreground, foreground, 128, 255, THRESH_BINARY);
			erode(foreground, foreground, Mat());
			
			if(displayWindows) imshow("Background Subtraction", foreground);
		}
        
        if (displayWindows) imshow("Source", frameResized);
        
        if (writeOut) {
			VideoWriter outStream(outFile, CV_FOURCC('M','J','P','G'), 2, Size(imgSizeX, imgSizeY), true);
			if (outStream.isOpened())
				outStream.write(frameResized);
        }
		// fps counter begin
		time(&end);
		counter++;
		sec = difftime(end, start);
		fps = counter/sec;
		cout.precision(2);
		cout<<fixed<<fps<<" fps\n";
		if (counter == (INT_MAX - 1000)) counter = 0;
		// fps counter end
		if ( (waitKey(10) & 255) == 27 ) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
