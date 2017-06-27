#include "opencv2/objdetect/objdetect.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;
 
CascadeClassifier face_cascade;
std::vector<Rect> faces;
int main() {
VideoCapture stream1(1);
VideoCapture stream2(2); 
  //0 is the id of video device.0 if you have only one camera.
 
if (!stream1.isOpened()) { //check if video device has been initialised
cout << "cannot open camera";
}

//unconditional loop
while (true) {
Mat cameraFrame1;
stream1.read(cameraFrame1);
    face_cascade.load( "/home/nihir/OpenCV/data/haarcascades/haarcascade_frontalface_xml.alt" );

Mat cameraFrame2;
stream2.read(cameraFrame2);
    face_cascade.load( "/home/nihir/OpenCV/data/haarcascades/haarcascade_frontalface_xml.alt" );
 
 
    // Detect faces
    
    face_cascade.detectMultiScale( cameraFrame1, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
 
    // Draw circles on the detected faces
    for( int i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
        ellipse( cameraFrame1, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
    }
// Detect faces
    std::vector<Rect> faces;
    face_cascade.detectMultiScale( cameraFrame2, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
 
    // Draw circles on the detected faces
    for( int i = 0; i < faces.size(); i++ )
    {
        Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
        ellipse( cameraFrame2, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
    }
imshow("cam1", cameraFrame1);
imshow("cam2", cameraFrame2);
if (waitKey(10) >= 0)
break;
}
return 0;
}
