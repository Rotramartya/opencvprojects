/* This is sample from the OpenCV book. The copyright notice is below */

/* *************** License:**************************
   Oct. 3, 2008
   Right to use this code in any way you want without warranty, support or any guarantee of it working.

   BOOK: It would be nice if you cited it:
   Learning OpenCV: Computer Vision with the OpenCV Library
     by Gary Bradski and Adrian Kaehler
     Published by O'Reilly Media, October 3, 2008

   AVAILABLE AT:
     http://www.amazon.com/Learning-OpenCV-Computer-Vision-Library/dp/0596516134
     Or: http://oreilly.com/catalog/9780596516130/
     ISBN-10: 0596516134 or: ISBN-13: 978-0596516130

   OPENCV WEBSITES:
     Homepage:      http://opencv.org
     Online docs:   http://docs.opencv.org
     Q&A forum:     http://answers.opencv.org
     Issue tracker: http://code.opencv.org
     GitHub:        https://github.com/opencv/opencv/
   ************************************************** */

#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/core.hpp"

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iomanip> 

using namespace cv;
using namespace std;

static int print_help()
{
    cout <<
            " Given a list of chessboard images, the number of corners (nx, ny)\n"
            " on the chessboards, and a flag: useCalibrated for \n"
            "   calibrated (0) or\n"
            "   uncalibrated \n"
            "     (1: use cvStereoCalibrate(), 2: compute fundamental\n"
            "         matrix separately) stereo. \n"
            " Calibrate the cameras and display the\n"
            " rectified results along with the computed disparity images.   \n" << endl;
    cout << "Usage:\n ./stereo_calib -w=<board_width default=9> -h=<board_height default=6> -s=<square_size default=1.0> <image list XML/YML file default=../data/stereo_calib.xml>\n" << endl;
    return 0;
}


static void
StereoCalib(const vector<string>& imagelist, Size boardSize, float squareSize, bool displayCorners = false, bool useCalibrated=true, bool showRectified=true)
{
    if( imagelist.size() % 2 != 0 )
    {
        cout << "Error: the image list contains odd (non-even) number of elements\n";
        return;
    }

    const int maxScale = 2;
    // ARRAY AND VECTOR STORAGE:

   // vector<vector<Point2f> > imagePoints[2];
    //vector<vector<Point3f> > objectPoints;
    Size imageSize;

    int i, j, k, nimages = 1;

    //imagePoints[0].resize(nimages);
    //imagePoints[1].resize(nimages);
    vector<string> goodImageList;
        for( k = 0; k < 2; k++ )
        {
            const string& filename = imagelist[k];
            Mat img = imread(filename, 0);
            if(img.empty())
                break;
            if( imageSize == Size() )
                imageSize = img.size();
            else if( img.size() != imageSize )
            {
                cout << "The image " << filename << " has the size different from the first image size. Skipping the pair\n";
                break;
            }
           
        }
        if( k == 2 )
        {
            goodImageList.push_back(imagelist[0]);
            goodImageList.push_back(imagelist[1]);
            
        }
    cout << 1 << " pairs have been successfully detected.\n";
    nimages = 2;
    if( nimages < 2 )
    {
        cout << "Error: too little pairs to run the calibration\n";
        return;
    }
    
// CALIBRATION QUALITY CHECK
// because the output fundamental matrix implicitly
// includes all the output information,
// we can check the quality of calibration using the
// epipolar geometry constraint: m2^t*F*m1=0
    
    Mat M1,M2,P1,P2,R1,R2,D1,D2,R,T,F,Q;

  Q=(Mat_<double>(4,4)<<1., 0., 0., -3.4725458145141602e+02, 0., 1., 0.,
       -2.4029708480834961e+02, 0., 0., 0., 7.8252666964915318e+02, 0.,
       0., -3.4104348782034094e-01, 0. );

M1=(Mat_<double>(3,3)<<8.2900017783565295e+02, 0., 3.2042782765977876e+02, 0.,
       8.2285596303928298e+02, 2.3994849499924541e+02, 0., 0., 1.);
M2=(Mat_<double>(3,3)<<8.2900017783565295e+02, 0., 3.1863535614317152e+02, 0.,
       8.2285596303928298e+02, 2.4031482644508694e+02, 0., 0., 1.);
P1=(Mat_<double>(3,4)<< 7.8252666964915318e+02, 0., 3.4725458145141602e+02, 0., 0.,
       7.8252666964915318e+02, 2.4029708480834961e+02, 0., 0., 0., 1.,
       0. );
P2=(Mat_<double>(3,4)<<  7.8252666964915318e+02, 0., 3.4725458145141602e+02,
       2.2945069986540311e+03, 0., 7.8252666964915318e+02,
       2.4029708480834961e+02, 0., 0., 0., 1., 0.);
R1=(Mat_<double>(3,3)<< 9.9955718702076612e-01, 9.4587928515038871e-04,
       -2.9741136284796684e-02, -7.7819761906424106e-04,
       9.9998374086297548e-01, 5.6491077308724151e-03,
       2.9745996093568827e-02, -5.6234617512030006e-03,
       9.9954167116450632e-01 );
R2=(Mat_<double>(3,3)<<  9.9929496646888738e-01, -2.3296359383598975e-02,
       -2.9442310191548936e-02, 2.3129996254717542e-02,
       9.9971458953873960e-01, -5.9785229485381551e-03,
       2.9573184867408873e-02, 5.2933073649321502e-03,
       9.9954860193686346e-01);
D1=(Mat_<double>(1,14)<<  4.7345198951448973e-02, -6.4704644799383781e-01, 0., 0., 0.,
       0., 0., -8.3437885112429928e+00, 0., 0., 0., 0., 0., 0. );
D2=(Mat_<double>(1,14)<<  2.2021069911931975e-02, 5.4811361938338278e-02, 0., 0., 0.,
       0., 0., -4.2084612245123765e-01, 0., 0., 0., 0., 0., 0. );
R= (Mat_<double>(3,3) << 9.9971414982117779e-01, 2.3908528915519603e-02,
       -2.9873321778178948e-05, -2.3906564266424214e-02,
       9.9964653278697835e-01, 1.1631236905330900e-02,
       3.0794852641279112e-04, -1.1627197945694646e-02,
       9.9993235442985673e-01);
T=(Mat_<double>(3,1) << 2.9301100949193559e+00, -6.8309057981108978e-02,
       -8.6330075908266024e-02);
F=(Mat_<double>(3,3) <<1.0316194559759827e-08, 1.0556967236038811e-07,
       -9.7647272839180763e-04, 6.8997225271633322e-07,
       1.4850337932880323e-06, -6.9148195264740767e-02,
       4.5179389665176637e-04, 6.8180877389604072e-02, 1. );
    Mat cameraMatrix[2], distCoeffs[2];
    cameraMatrix[0] = M1;
    cameraMatrix[1] = M2;
    distCoeffs[0]=D1;
    distCoeffs[1]=D2;
    Rect validRoi[2];
    bool isVerticalStereo = fabs(P2.at<double>(1, 3)) > fabs(P2.at<double>(0, 3));

// COMPUTE AND DISPLAY RECTIFICATION
    if( !showRectified )
        return;

    //Mat rmap[2][2];
// IF BY CALIBRATED (BOUGUET'S METHOD)
    if( useCalibrated )
    {
        // we already computed everything
    }
// OR ELSE HARTLEY'S METHOD
    else
 {}

    //Precompute maps for cv::remap()
    Mat canvas,limg,rimg;
    Mat cam1map1,cam1map2;
    Mat cam2map1,cam2map2;
    initUndistortRectifyMap(M1,D1,R1,P1,Size(640,480),CV_16SC2,cam1map1,cam1map2);
    initUndistortRectifyMap(M2,D2,R2,P2,Size(640,480),CV_16SC2,cam2map1,cam2map2);
    double sf;
    int w, h;
    if( !isVerticalStereo )
    {
        sf = 600./MAX(imageSize.width, imageSize.height);
        w = cvRound(imageSize.width*sf);
        h = cvRound(imageSize.height*sf);
        canvas.create(h, w*2, CV_8UC3);
}
        
    else
    {
        sf= 300./MAX(imageSize.width, imageSize.height);
        w = cvRound(imageSize.width*sf);
        h = cvRound(imageSize.height*sf);
        canvas.create(h*2, w, CV_8UC3);
        
    }
     stringstream l_name, r_name; 
     l_name << "left" << setw(2) << setfill('0')  << ".jpg";
     r_name << "right" << setw(2) << setfill('0')  << ".jpg";
            for( k = 0; k < 2; k++ )
        {   Mat cimg;
              if(k==0)
            { Mat img1 = imread(goodImageList[0], 0);
              remap(img1, limg, cam1map1, cam1map2, INTER_LINEAR);
              cvtColor(limg, cimg, COLOR_GRAY2BGR);
              imwrite( l_name.str(), cimg);
            }
            if(k==1)
            {  Mat img2 = imread(goodImageList[1], 0);
               remap(img2, rimg, cam2map1, cam2map2, INTER_LINEAR);
               cvtColor(rimg, cimg, COLOR_GRAY2BGR);
               imwrite( r_name.str(), cimg);
            }
            Mat canvasPart = !isVerticalStereo ? canvas(Rect(w*k, 0, w, h)) : canvas(Rect(0, h*k, w, h));
            resize(cimg, canvasPart, canvasPart.size(), 0, 0, INTER_AREA);
         }
      imshow("rectified", canvas);
      waitKey();
      
            
     
    
}


static bool readStringList( const string& filename, vector<string>& l )
{
    l.resize(0);
    FileStorage fs(filename, FileStorage::READ);
    if( !fs.isOpened() )
        return false;
    FileNode n = fs.getFirstTopLevelNode();
    if( n.type() != FileNode::SEQ )
        return false;
    FileNodeIterator it = n.begin(), it_end = n.end();
    for( ; it != it_end; ++it )
        l.push_back((string)*it);
    return true;
}

int main(int argc, char** argv)
{
    Size boardSize;
    string imagelistfn;
    bool showRectified;
    cv::CommandLineParser parser(argc, argv, "{w|9|}{h|6|}{s|1.0|}{nr||}{help||}{@input|../data/stereo_calib.xml|}");
    if (parser.has("help"))
        return print_help();
    showRectified = !parser.has("nr");
    imagelistfn = parser.get<string>("@input");
    boardSize.width = parser.get<int>("w");
    boardSize.height = parser.get<int>("h");
    float squareSize = parser.get<float>("s");
    if (!parser.check())
    {
        parser.printErrors();
        return 1;
    }
    vector<string> imagelist;
    bool ok = readStringList(imagelistfn, imagelist);
    if(!ok || imagelist.empty())
    {
        cout << "can not open " << imagelistfn << " or the string list is empty" << endl;
        return print_help();
    }

    StereoCalib(imagelist, boardSize, squareSize, false, true, showRectified);
    return 0;
}
