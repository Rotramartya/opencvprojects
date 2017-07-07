#include <iostream>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <opencv2/contrib/contrib.hpp>

using namespace cv;

using namespace std;

Mat Q;

Mat img;

void mouseEvent(int evt, int x, int y, int flags, void* param) 

{                   int avg =0;   

    if (evt == CV_EVENT_LBUTTONDOWN) 

    {   Vec3f intensity = img.at<Vec3f>(y,x);

       float blue = intensity.val[0];

       float green = intensity.val[1];

       float red = intensity.val[2];

        // if(red<300)

         { //avg = avg +red;

        cout<< red<<endl;

          }

    }         

}

int main(int argc, char* argv[])

{
Mat img1, img2, g1, g2;
Mat disp, disparity;
char* method = argv[3];
img1 = imread(argv[1]);
img2 = imread(argv[2]);
cvtColor(img1, g1, CV_BGR2GRAY);
cvtColor(img2, g2, CV_BGR2GRAY);
//Mat fundamental_matrix= findFundamentalMat(img1,img2,FM_RANSAC,3.,0.99);
if (!(strcmp(method, "BM")))
{
   StereoBM sbm;
   sbm.state->SADWindowSize =9;
   sbm.state->numberOfDisparities = 96;
   sbm.state->preFilterSize = 5;
   sbm.state->preFilterCap = 61;
   sbm.state->minDisparity = -127 ;
   sbm.state->textureThreshold = 507;
   sbm.state->uniquenessRatio = 0;
   sbm.state->speckleWindowSize = 0;
   sbm.state->speckleRange = 8;
   sbm.state->disp12MaxDiff = 1;
   sbm(g1, g2, disp);
   //disp.convertTo(disp8,CV_8U,255/128*16.0);
}
      /* Q=(Mat_<double>(4,4)<< 1., 0., 0., 8.6582224788665771e+03, 0., 1., 0.,

       7.5069656372070313e+01, 0., 0., 0., -2.0918405446154462e+04, 0.,

       0., -1.2906999244682757e-01, 0. );*/

  	/*Q=(Mat_<double>(4,4)<< 1., 0., 0., , 0., 1., 0.,
       -2.3995620155334473e+02, 0., 0., 0., 7.892441484481073235e+02, 0.,
       0., -3.4020142603281389e-01, 0. );*/
     
	/*Q=(Mat_<double>(4,4)<< 1., 0., 0., -3.3608328628540039e+02, 0., 1., 0.,
       -2.3995620155334473e+02, 0., 0., 0., 7.892441484481073235e+02, 0.,
       0., -3.4020142603281389e-02, 0. );*/

	Q=(Mat_<double>(4,4)<< 1., 0., 0., -2.8189150238037109e+02, 0., 1., 0.,
       -2.5036193847656250e+02, 0., 0., 0., 7.0685879462929302e+02, 0.,
       0., -1.3561515014349265e-01, 0.);


	Q.convertTo(Q, CV_32F);

        cout<<Q<<endl;

	if (Q.cols != 4 || Q.rows != 4)

	{

		std::cerr << "ERROR: Q is not 4x4)" << std::endl;

		return 1;

	}



	// Loading disparity image

	//cv::Mat disparity = cv::imread("19848899_319182118526944_162660282_n.jpg", cv::IMREAD_GRAYSCALE);

	/*if (disparity.empty())

	{

		std::cerr << "ERROR: Could not read disparity-image.pgm" << std::endl;

		return 1;

	}



	// Conversion of the disparity map to 32F before reprojecting to 3D

	// NOTE: also take care to do not scale twice the disparity

       // namedWindow("Disparity Map",WINDOW_NORMAL);

       // resizeWindow("Disparity Map",400,400);

       // imshow("Disparity Map",disparity);

        //cout<<disparity<<endl;

    /*    int max=0;

        for(int i=0;i<disparity.rows;i++)

        {

          for(int j=0;j<disparity.cols;j++)

          {

               if(disparity.at<uchar>(j,i)>max)

                  max=disparity.at<uchar>(i,j);

        }

}*/

        //  cout<<max <<endl;
	//imshow("disp1",disp);
	
        
	normalize(disp, disparity, 0, 255, CV_MINMAX, CV_8U);
        // Reproject image to 3D by OpenCV

     	cout<<disparity.rows << " "<<disparity.cols<<endl;
            cout<<disp.type()<<endl;
        Mat newdisp(disp.rows,disp.cols,3);

        for (int i = 0; i < disp.rows; i++)

{       for (int j = 0; j < disp.cols; j++)

		{      

                            Scalar Intensity=disp.at<short>(i,j);
                            newdisp.at<short>(i,j)=Intensity.val[0]+2048;
                            Scalar Intensity2=((newdisp.at<short>(i,j)/16)*16)/16;
                            cout<<Intensity2.val[0]<<" ";

                              //  cout<<image3D_ptr[j][2]<<endl;

		}

	cout<<endl;

	}

        

	cv::Mat image3D;
        newdisp.convertTo(newdisp, CV_32F);
	cv::reprojectImageTo3D(newdisp, image3D, Q,true, CV_32F);
        cv::Mat image(image3D.rows,image3D.cols,5);
        cv::Mat image1(image3D.rows,image3D.cols,5);
        cv::Mat image2(image3D.rows,image3D.cols,5);
        cv::Mat depth,x,y;
        float z[image3D.rows][image3D.cols];
               img=image3D;
           int k=0;
            float min=500.0;
         int f=1;

        for (int i = 0; i < image3D.rows; i++)

	{

		const cv::Vec3f* image3D_ptr = image3D.ptr<cv::Vec3f>(i);

               



		for (int j = 0; j < image3D.cols; j++)

		{      

                            image.at<float>(i,j)=image3D_ptr[j][2];

                                       if(image.at<float>(i,j)<0)

	                                       f=0;
				  //   if(image3D_ptr[j][2] < 6000)
                                  //   cout<<image3D_ptr[j][2]*0.026<<endl;

		}

	}

     

        normalize(image, depth,0, 255, CV_MINMAX, CV_8UC1);

        namedWindow("Depth Map",WINDOW_NORMAL);

        resizeWindow("Depth Map",400,400);

        setMouseCallback("Depth Map", mouseEvent, &image3D);

        imshow("Depth Map",depth);
        
        imshow("disp",disparity);
        

       /* normalize(image1, y, 0, 255, CV_MINMAX, CV_8U);

        namedWindow("Y Map",WINDOW_NORMAL);

        resizeWindow("Y Map",400,400);

        imshow("Y Map",y);

        normalize(image2, x, 0, 255, CV_MINMAX, CV_8U);

        namedWindow("X Map",WINDOW_NORMAL);

        resizeWindow("X Map",400,400);

        imshow("X Map",x);*/

        

        waitKey(0);



	return 0;

}
