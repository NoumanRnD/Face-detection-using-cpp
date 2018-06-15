/* videoFaceDetection.cpp */
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

 /** Function Headers */
 Mat detectFace (Mat frame,
                 CascadeClassifier face_cascade,
                 CascadeClassifier eyes_cascade);



int main(){

    Mat frame;

    /* Loading haar cascade */
    String face_cascade_name = "haarcascade_frontalface_alt.xml";
    String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
    CascadeClassifier face_cascade;
    CascadeClassifier eyes_cascade;

    //-- 1. Load the cascades
    if (!face_cascade.load(face_cascade_name)) {
        cout << "--(!)Error loading\n";
        return -1;
    };

    if (!eyes_cascade.load(eyes_cascade_name)) {
        cout << "--(!)Error loading\n";
        return -1;
    };

    VideoCapture vcap("faces.avi");
    if (!vcap.isOpened()) {
        cout << "Error video not found";
    }

   int frame_width  =  vcap.get(CV_CAP_PROP_FRAME_WIDTH);
   int frame_height =  vcap.get(CV_CAP_PROP_FRAME_HEIGHT);

   VideoWriter video("out.avi",
                     CV_FOURCC('M','J','P','G'), 10,
                     Size(frame_width,frame_height),
                     true);

   for(;;) {
       Mat frame, faceFrame;

       vcap >> frame;

       if (frame.empty()) break;

       faceFrame = detectFace(frame, face_cascade, eyes_cascade);
       video.write(faceFrame);
   }

  return 0;
}

/** @function detectAndDisplay */
Mat detectFace( Mat frame, CascadeClassifier face_cascade,  CascadeClassifier eyes_cascade)
{
  std::vector<Rect> faces;
  Mat frame_gray;

  cvtColor( frame, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  for( size_t i = 0; i < faces.size(); i++ )
  {
    Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
    ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

    Mat faceROI = frame_gray( faces[i] );
    std::vector<Rect> eyes;

    //-- In each face, detect eyes
    eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    for( size_t j = 0; j < eyes.size(); j++ )
     {
       Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
       int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
       circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
     }
  }
  //-- Show what you got
  return frame;
 }
