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
    VideoCapture cap1(1);
    VideoCapture cap2(2);
                       // open the default camera
    Mat img1,img2, left,right;;
    char* method = argv[1];
   while(true)
		
    {
        cap1.read(left);
	cap2.read(right);
	 // get a new frame from camera
        // do any processing
	       
	//imwrite("/home/ashutosh/opencvprojects/image_left.png", img11);
        //imwrite("/home/ashutosh/opencvprojects/image_right.png", img2);
        //system( "gimp /home/ashutosh/opencvprojects/image_left.png");
   	Mat Q,M1,M2,P1,P2,R1,R2,D1,D2;
	
	
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
	D1=(Mat_<double>(1,8)<<  4.7345198951448973e-02, -6.4704644799383781e-01, 0., 0., 0.,
       0., 0., -8.3437885112429928e+00);
	D2=(Mat_<double>(1,8)<<  2.2021069911931975e-02, 5.4811361938338278e-02, 0., 0., 0.,
       0., 0., -4.2084612245123765e-01);
Mat cam1map1,cam1map2;
Mat cam2map1,cam2map2;
initUndistortRectifyMap(M1,D1,R1,P1,Size(640,480),CV_16SC2,cam1map1,cam1map2);
initUndistortRectifyMap(M2,D2,R2,P2,Size(640,480),CV_16SC2,cam2map1,cam2map2);

remap(left,img1,cam1map1,cam1map2,INTER_LINEAR);
remap(right,img2,cam2map1,cam2map2,INTER_LINEAR);
Mat disp,disparity,g1,g2;

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
       Q=(Mat_<double>(4,4)<< 1., 0., 0., 8.6582224788665771e+03, 0., 1., 0.,

       7.5069656372070313e+01, 0., 0., 0., -2.0918405446154462e+04, 0.,

       0., -1.2906999244682757e-01, 0. );

  	/*Q=(Mat_<double>(4,4)<< 1., 0., 0., -3.4725467300415039e+02, 0., 1., 0.,
       -2.4029708480834961e+02, 0., 0., 0., 7.8252675802400540e+02, 0.,
       0., -1.3374253614777099e-01, 0. );*/

	Q.convertTo(Q, CV_32F);

      //  cout<<Q<<endl;


 
	
       
        

          // you can increase delay to 2 seconds here
   
    // the camera will be deinitialized automatically in VideoCapture destructor
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
          

      for (int i = 0; i < image3D.rows; i++)

	{

		const cv::Vec3f* image3D_ptr = image3D.ptr<cv::Vec3f>(i);

               



		for (int j = 0; j < image3D.cols; j++)

		{      

                            image.at<float>(i,j)=image3D_ptr[j][2];

                                       //if(image.at<float>(i,j)<0)

	                                    
				  //   if(image3D_ptr[j][2] < 6000)
                                  //   cout<<image3D_ptr[j][2]*0.026<<endl;

		}

	} 

     

        normalize(image, depth,0, 255, CV_MINMAX, CV_8UC1);

        namedWindow("Depth Map",WINDOW_NORMAL);

        resizeWindow("Depth Map",600,600);

        setMouseCallback("Depth Map", mouseEvent, &image3D);

        imshow("Depth Map",depth);
        
        imshow("disp",disparity);

         if(waitKey(1) >= 0) break;
	}

    return 0;
}
