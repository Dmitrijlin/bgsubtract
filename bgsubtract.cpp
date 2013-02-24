#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <time.h>
#include <limits.h>

#include "opencv2/video/background_segm.hpp"

int main(int argc, char** argv){
	CvCapture* capture = cvCaptureFromCAM( 0 );
	if (!capture){
		fprintf(stderr, "ERROR: Capture is null\nPress any key to exit\n");
		getchar();
		return -1;
	}
	
	int percent = 50;
	if (argv[1]){
		percent = atoi(argv[1]);
		printf("MESSAGE: Resizing to %d%% original resolution\n", percent);
	} else {
		printf("MESSAGE: Resizing to 50%% original resolution\n");
	}
	
	bool doBGS = false;
	//TODO: think of something smarter than this!
	if (argv[2] && strcmp(argv[2], "bgs") == 0){
		printf("MESSAGE: Showing background subtraction\n");
		doBGS = true;
	}
	
	// fps counter begin
	time_t start, end;
	int counter = 0;
	double sec;
	double fps;
	// fps counter end
	
	//cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Resized", CV_WINDOW_AUTOSIZE);
	
	cv::Mat foreground, image;
	cv::BackgroundSubtractorMOG2 mog;	
	if (doBGS){
		cvNamedWindow("Foreground", CV_WINDOW_AUTOSIZE);
	}
	
	// The main loop
	while(1){
		if (counter == 0){
			time(&start);
		}
		
		IplImage* srcImg = cvQueryFrame(capture);
		// source image resized
		//TODO: will it be better if this moved outside the loop?
		IplImage* srcImgRes = cvCreateImage(cvSize((int)(srcImg->width*percent/100), (int)(srcImg->height*percent/100)), srcImg->depth, srcImg->nChannels);
		cvResize(srcImg, srcImgRes);
		
		//TODO: how to move this outside the loop?
		cv::Mat frame(srcImgRes);

		if (doBGS){			
			mog(frame, foreground, -1);
			threshold(foreground,foreground,128,255,cv::THRESH_BINARY);
			//medianBlur(foreground,foreground,1);
			erode(foreground,foreground,cv::Mat());
			dilate(foreground,foreground,cv::Mat());
			cv::imshow("Foreground", foreground);
		}
		//cvShowImage("Original", srcImg);
		cvShowImage("Resized", srcImgRes);
		
		// fps counter begin
		time(&end);
		counter++;
		sec = difftime(end, start);
		fps = counter/sec;
		if (counter > 10)
			printf("%.2f fps\n", fps);
		if (counter == (INT_MAX - 1000))
			counter = 0;
		// fps counter end
		
		if ( (cvWaitKey(10) & 255) == 27 ) break;
	}
	
	cvReleaseCapture(&capture);
	//cvDestroyWindow("Original");
	cvDestroyWindow("Resized");
	if (doBGS)
		cvDestroyWindow("Foreground");
	
	return 0;
}
