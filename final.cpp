#include <iostream>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <opencv2/contrib/contrib.hpp>
#include <stdlib.h>
#include <fstream>
#include <iomanip>

using namespace cv;
using namespace std;


Mat img;
Mat imgs;

void mouseEvent(int evt, int x, int y, int flags, void* param) 

{                   int avg =0;   

    if (evt == CV_EVENT_LBUTTONDOWN) 

    {   Vec3f intensity = imgs.at<Vec3f>(y,x);

       float blue = intensity.val[0];

       float green = intensity.val[1];

       float red = intensity.val[2];

        // if(red<300)

         { //avg = avg +red;

        cout<< -red<<endl;

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
	       
	//imwrite("/home/ashutosh/opencvprojects/image_left.png", left);
        //imwrite("/home/ashutosh/opencvprojects/image_right.png", right);
        //system( "gimp /home/ashutosh/opencvprojects/image_left.png");
   	Mat Q,M1,M2,P1,P2,R1,R2,D1,D2;
	
	
	M1=(Mat_<double>(3,3)<<8.3076829895180879e+02, 0., 3.1818353441884472e+02, 0.,
       8.2580003914714064e+02, 2.4655735791715523e+02, 0., 0., 1. );
	M2=(Mat_<double>(3,3)<<8.3076829895180879e+02, 0., 3.1584102268688747e+02, 0.,
       8.2580003914714064e+02, 2.4793549392850724e+02, 0., 0., 1.);
	P1=(Mat_<double>(3,4)<<8.0069469675622236e+02, 0., 3.6789365005493164e+02, 0., 0.,
       8.0069469675622236e+02, 2.4675359916687012e+02, 0., 0., 0., 1.,
       0.  );
	P2=(Mat_<double>(3,4)<< 8.0069469675622236e+02, 0., 3.6789365005493164e+02,
       5.6547768300775742e+03, 0., 8.0069469675622236e+02,
       2.4675359916687012e+02, 0., 0., 0., 1., 0. );
	R1=(Mat_<double>(3,3)<< 9.9907400118344369e-01, 7.2747367832395778e-03,
       -4.2405404891818536e-02, -7.0830645392157693e-03,
       9.9996401703193227e-01, 4.6684941999342502e-03,
       4.2437841085967015e-02, -4.3638109601696643e-03,
       9.9908957896570316e-01 );
	R2=(Mat_<double>(3,3)<< 9.9790014922784898e-01, 6.6406080961048765e-05,
       -6.4771041069828450e-02, -3.5914276395721477e-04,
       9.9998977474671125e-01, -4.5079284041489674e-03,
       6.4770079415669160e-02, 4.5217244779229369e-03,
       9.9788996929532947e-01  );
	D1=(Mat_<double>(1,8)<< 5.3400795984806119e-02, -2.6573577503750628e-01, 0., 0., 0.,
       0., 0., -3.1728827434547626e+00);
	D2=(Mat_<double>(1,8)<< 2.3561746011197478e-02, 1.8691006815030847e-01, 0., 0., 0.,
       0., 0., -2.9612865609933825e-01  );
Mat cam1map1,cam1map2;
Mat cam2map1,cam2map2;
initUndistortRectifyMap(M1,D1,R1,P1,Size(640,480),CV_16SC2,cam1map1,cam1map2);
initUndistortRectifyMap(M2,D2,R2,P2,Size(640,480),CV_16SC2,cam2map1,cam2map2);

remap(left,img1,cam1map1,cam1map2,INTER_LINEAR);
remap(right,img2,cam2map1,cam2map2,INTER_LINEAR);
Mat disp,disparity,g1,g2,disps,gs1,gs2;

cvtColor(img1, g1, CV_BGR2GRAY);
cvtColor(img2, g2, CV_BGR2GRAY);
cvtColor(left, gs1, CV_BGR2GRAY);
cvtColor(right, gs2, CV_BGR2GRAY);
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
   //sbm(gs1,gs2,disps);
   //disp.convertTo(disp8,CV_8U,255/128*16.0);
}
      Q=(Mat_<double>(4,4)<< 1., 0., 0., -3.6789365005493164e+02, 0., 1., 0.,
       -2.4675359916687012e+02, 0., 0., 0., 8.0069469675622236e+02, 0.,
       0., -1.4159616211507292e-01, 0.);

  	/*Q=(Mat_<double>(4,4)<< 1., 0., 0., -3.4725467300415039e+02, 0., 1., 0.,
       -2.4029708480834961e+02, 0., 0., 0., 7.8252675802400540e+02, 0.,
       0., -1.3374253614777099e-01, 0 );1., 0., 0., 8.6582224788665771e+03, 0., 1., 0.,

       7.5069656372070313e+01, 0., 0., 0., -2.0918405446154462e+04, 0.,

       0., -1.2906999244682757e-01, 0. */
                                
        

	Q.convertTo(Q, CV_32F);

      //  cout<<Q<<endl;
      // you can increase delay to 2 seconds here
   
      // the camera will be deinitialized automatically in VideoCapture destructor
	normalize(disp, disparity, 0, 255, CV_MINMAX, CV_8U);
        // Reproject image to 3D by OpenCV
       Mat newdisp(disp.rows,disp.cols,3);
       //Mat newdisps(disps.rows,disps.cols,3);

        for (int i = 0; i < disp.rows; i++)

		{       for (int j = 0; j < disp.cols; j++)

			{      
	
                            Scalar Intensity=disp.at<short>(i,j);
                            newdisp.at<short>(i,j)=Intensity.val[0]+2048;
                            Scalar Intensity2=((newdisp.at<short>(i,j)/16)*16)/16;
                            //cout<<Intensity2.val[0]<<" ";

                              //  cout<<image3D_ptr[j][2]<<endl;

			}

							//cout<<endl;

		}

	 
        

	cv::Mat image3D;
        newdisp.convertTo(newdisp, CV_32F);
	cv::reprojectImageTo3D(newdisp, image3D, Q,true, CV_32F);
        cv::Mat image(image3D.rows,image3D.cols,5);
        cv::Mat depth;
        for (int i = 0; i < image3D.rows; i++)

	{

		const cv::Vec3f* image3D_ptr = image3D.ptr<cv::Vec3f>(i);

               



		for (int j = 0; j < image3D.cols; j++)

		{      

                            image.at<float>(i,j)=image3D_ptr[j][2];

                                      
		}

	} 

        normalize(image, depth,0, 255, CV_MINMAX, CV_8UC1);

        namedWindow("Depth Map",WINDOW_NORMAL);

        resizeWindow("Depth Map",600,600);

        //setMouseCallback("Depth Map", mouseEvent, &image3D);

        imshow("Depth Map",depth);
        
        imshow("disp",disparity);
        char choice=char(waitKey(1));
        if(choice == 'c')
      {
     //save files at proper locations if user presses 'c'  
        stringstream l_name, r_name; 
        l_name << "left" << setw(2)<< ".jpg";
        r_name << "right" << setw(2)<< ".jpg";
        imwrite( l_name.str(), left);
        imwrite( r_name.str(), right); 
        cout << "Saved set " << endl; 
        initUndistortRectifyMap(M1,D1,R1,P1,Size(640,480),CV_16SC2,cam1map1,cam1map2);
        initUndistortRectifyMap(M2,D2,R2,P2,Size(640,480),CV_16SC2,cam2map1,cam2map2);

        remap(left,img1,cam1map1,cam1map2,INTER_LINEAR);
        remap(right,img2,cam2map1,cam2map2,INTER_LINEAR);
        cvtColor(img1, gs1, CV_BGR2GRAY);
        cvtColor(img2, gs2, CV_BGR2GRAY);
        {   StereoBM sbm;
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
            sbm(gs1, gs2, disps);
        }
             Mat newdisps(disps.rows,disps.cols,3);
             for (int i = 0; i < disps.rows; i++)

		{       for (int j = 0; j < disps.cols; j++)

			{      
	
                            Scalar Intensity=disps.at<short>(i,j);
                            newdisps.at<short>(i,j)=Intensity.val[0]+2048;
                            Scalar Intensity2=((newdisps.at<short>(i,j)/16)*16);
                            //cout<<Intensity2.val[0]<<" ";

                              

			}

							//cout<<endl;

		}
            cv::Mat image3Ds;
        newdisps.convertTo(newdisps, CV_32F,1.f/16.f);
	cv::reprojectImageTo3D(newdisps, image3Ds, Q,true, CV_32F);
        cv::Mat images(image3Ds.rows,image3Ds.cols,5);
        cv::Mat depths;
        imgs=image3Ds;
      for (int i = 0; i < image3Ds.rows; i++)

	{

		const cv::Vec3f* image3D_ptr = image3Ds.ptr<cv::Vec3f>(i);
                 for (int j = 0; j < image3Ds.cols; j++)

		{      

                            images.at<float>(i,j)=image3D_ptr[j][2];

                                       
		}

	} 

     

        normalize(images, depths,0, 255, CV_MINMAX, CV_8UC1);

        namedWindow("Depths Map",WINDOW_NORMAL);

        resizeWindow("Depths Map",600,600);
        

         setMouseCallback("Depths Map", mouseEvent, &image3Ds);

        imshow("Depths Map",depths);
        system(" bash yolo ");
        int leftv,rightv,top,bottom;
        ifstream infile; 
        infile.open("file path"); 
        Mat out3Ds;
        int c[14]={0};
        float s[14];
        float val;
        int max=0;
  infile>>leftv>>rightv>>top>>bottom;
{  //rectangle( left,Point( leftv, bottom ),Point( rightv,top),Scalar( 255, 0, 255 ),8,LINE_8);
   //imshow("S",left);
   waitKey(0);
  // out3Ds=image3Ds(Range(top,bottom),Range(leftv,rightv));
  // Mat z(out3Ds.size(),CV_32FC1);
     for(int i=top;i<bottom;i++)
   {
      for(int j=leftv;j<rightv;j++)
     {  val=images.at<float>(i,j);
        if(val >400)
        continue;
        else if(val<=20&&val<=40)
          {   s[0]=s[0]+val;
              c[0]++;
              } 
        else if(val<=40&&val<=60)
          {   s[1]=s[1]+val;
              c[1]++;
              }
        else if(val<=60&&val<=80)
          {   s[2]=s[2]+val;
              c[2]++;}
        else if(val<=80&&val<=100)
          {   s[3]=s[3]+val;
              c[3]++;}
        else if(val<=100&&val<=120)
          {   s[4]=s[4]+val;
              c[4]++;}
        else if(val<=120&&val<=140)
          {   s[5]=s[5]+val;
              c[5]++;
              }
        else if(val<=140&&val<=160)
          {   s[6]=s[6]+val;
              c[6]++;}
        else if(val<=160&&val<=180)
          {   s[7]=s[7]+val;
              c[7]++;}
        else if(val<=180&&val<=200)
          {   s[8]=s[8]+val;
              c[8]++;}
        else if(val<=200&&val<=220)
          {   s[9]=s[9]+val;
              c[9]++;} //20 to 40
        else if(val<=220&&val<=240)
          {   s[10]=s[10]+val;
              c[10]++;}
        else if(val<=240&&val<=260)
          {   s[11]=s[11]+val;
              c[11]++;}
        else if(val<=260&&val<=280)
          {   s[12]=s[12]+val;
              c[12]++;}
        else if(val<=280&&val<=300)
          {   s[13]=s[13]+val;
              c[13]++;}
        }
}
	int i=0;
        //check if c[] is non-zero
        for( i=0;i<14;i++)
        {
       if(c[i]>max)
        { max=c[i];
          break;
        }
        }
        cout<<s[i]/c[i]<<endl;
        }

       }
        		
		

	}

    return 0;
}
