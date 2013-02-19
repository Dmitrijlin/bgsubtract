#include "cv.h"
#include "highgui.h"
#include <stdio.h>

#include <time.h>
#include <limits.h>

using namespace cv;

int main(int argc, char** argv){
	CvCapture* capture = cvCaptureFromCAM( 0 );
	if (!capture){
		fprintf(stderr, "ERROR: Capture is null\nPress any key to exit\n");
		getchar();
		return -1;
	}
	
	int percent = 50;
	//if (argv[1] != ""){
	//	percent = atoi(argv[1]);
	//}
	
	// fps counter begin
	time_t start, end;
	int counter = 0;
	double sec;
	double fps;
	// fps counter end
	
	//cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Resized", CV_WINDOW_AUTOSIZE);
	
	while(1){
		if (counter == 0){
			time(&start);
		}
		
		IplImage* srcImg = cvQueryFrame(capture);
		// source image resized
		//TODO: will it be better if this moved outside the loop?
		IplImage* srcImgRes = cvCreateImage(cvSize((int)(srcImg->width*percent/100), (int)(srcImg->height*percent/100)), srcImg->depth, srcImg->nChannels);
		cvResize(srcImg, srcImgRes);
		
		//cvShowImage("Original", srcImg);
		cvShowImage("Resized", srcImgRes);
		
		// fps counter begin
		time(&end);
		counter++;
		sec = difftime(end, start);
		fps = counter/sec;
		printf("%.2f fps\n", fps);
		if (counter == (INT_MAX - 1000)) {
			counter = 0;
		}
		// fps counter end
		
		if ( (cvWaitKey(10) & 255) == 27 ) break;
	}
	
	cvReleaseCapture(&capture);
	//cvDestroyWindow("Original");
	cvDestroyWindow("Resized");
	return 0;
}

CvMat bgsubtract(CvMat curFrame, CvMat prevFrame){
	
}
