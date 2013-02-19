// source: http://docs.opencv.org/modules/highgui/doc/reading_and_writing_images_and_video.html
#include "opencv2/opencv.hpp"

#include <stdio.h>
#include <time.h>
#include <limits.h>

using namespace cv;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat edges;
    namedWindow("edges",1);
    
    
	// fps counter begin
	time_t start, end;
	int counter = 0;
	double sec;
	double fps;
	// fps counter end
	
    for(;;)
    {
		if (counter == 0){
			time(&start);
		}
        Mat frame;
        cap >> frame; // get a new frame from camera
        //cvtColor(frame, edges, CV_BGR2GRAY);
        //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        //Canny(edges, edges, 0, 30, 3);
        imshow("edges", frame);
        
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
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

Mat bgsubtract(Mat cur_frame, Mat prev_frame){
	
}
