/* opticalFlow.cpp */
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

static void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
                    double, const Scalar& color)
{
    for(int y = 0; y < cflowmap.rows; y += step)
        for(int x = 0; x < cflowmap.cols; x += step)
        {
            const Point2f& fxy = flow.at<Point2f>(y, x);
            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                 color);
            circle(cflowmap, Point(x,y), 2, color, -1);
        }
}

int main(int, char**)
{
    // Reading video and find it's propterties
    VideoCapture cap("bee_1secc.avi");
    if( !cap.isOpened() ) {
        std::cout << "File not opened" << endl;
        return -1;
    }

    int frame_width=   cap.get(CV_CAP_PROP_FRAME_WIDTH);
    int frame_height=   cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    // Open VideoWriter object to write the video.
    VideoWriter video("out.avi",CV_FOURCC('M','P','4','2'),10, Size(frame_width,frame_height),true);

    // Variabls initalization.
    Mat prevgray, gray, flow, cflow, frame;

    // Loop through each video frame.
    for(;;)
    {
        cap >> frame;
        if (frame.empty()) break;

        cvtColor(frame, gray, COLOR_BGR2GRAY);

        if( prevgray.data )
        {
            calcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
            cvtColor(prevgray, cflow, COLOR_GRAY2BGR);
            drawOptFlowMap(flow, cflow, 16, 1.5, Scalar(0, 255, 0));

            // Writer out video.
            video.write(cflow);
        }
        std::swap(prevgray, gray);
    }

    return 0;

}
