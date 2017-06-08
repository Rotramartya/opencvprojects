#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/contrib/contrib.hpp"
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace cv;
using namespace std;

 

void expand(int value)
{
    const char * const ones[20] = {"zero", "one", "two", "three","four","five","six","seven",
    "eight","nine","ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen",
    "eighteen","nineteen"};
    const char * const tens[10] = {"", "ten", "twenty", "thirty","forty","fifty","sixty","seventy",
    "eighty","ninety"};

    if(value<0)
    {
        cout<<"minus ";
        expand(-value);
    }
    else if(value>=1000)
    {
        expand(value/1000);
        cout<<" thousand";
        if(value % 1000)
        {
            if(value % 1000 < 100)
            {
                cout << " and";
            }
            cout << " " ;
            expand(value % 1000);
        }
    }
    else if(value >= 100)
    {
        expand(value / 100);
        cout<<" hundred";
        if(value % 100)
        {
            cout << " and ";
            expand (value % 100);
        }
    }
    else if(value >= 20)
    {
        cout << tens[value / 10];
        if(value % 10)
        {
            cout << " ";
            expand(value % 10);
        }
    }
    else
    {
        cout<<ones[value];
    }
	cout<<" ";
    return;
}

void conversion( float d)
		{ 	int a=d,l=2;
			expand(a);
				d-=a;
				if(d!=0)
				cout<<" point ";
				while(l--)
				{
					d*=10;
					int c=d;
				        expand(c);
					d-=c;
                                }
                               cout << " centimeters" ;
		}

	int main(int argc, char* argv[])
{ 
	VideoCapture stream1(0);
	VideoCapture stream2(1);
 
	Mat img1, img2, g1, g2;
	Mat disp, disparity;

	char* method = argv[1];
while(true){
	float min=100000.00;
	stream1.read(img1);
	stream2.read(img2);
		cvtColor(img1, g1, CV_BGR2GRAY);
		cvtColor(img2, g2, CV_BGR2GRAY);
	if (!(strcmp(method, "BM")))
	{
	   StereoBM sbm;
	   sbm.state->SADWindowSize = 21;
	   sbm.state->numberOfDisparities = 80;
	   sbm.state->preFilterSize = 5;
	   sbm.state->preFilterCap = 61;
	   sbm.state->minDisparity = -39;
	   sbm.state->textureThreshold = 507;
	   sbm.state->uniquenessRatio = 0;
	   sbm.state->speckleWindowSize = 0;
	   sbm.state->speckleRange = 8;
	   sbm.state->disp12MaxDiff = 1;
	   sbm(g1, g2, disp);
	}
 
		 normalize(disp, disparity, 0, 255, CV_MINMAX, CV_8U);


		 Mat Q;
    Q=(Mat_<double>(4,4)<< 1., 0., 0., -2.9615028381347656e+02, 0., 1., 0., -2.3373317337036133e+02, 0.
    ,0., 0., 5.6446880931501073e+02 ,0., 0., -1.1340974198400260e-01,
		4.1658568844268817e+00);

	Q.convertTo(Q, CV_32F);
	if (Q.cols != 4 || Q.rows != 4)
	{
		std::cerr << "ERROR: Q is not 4x4)" << std::endl;
		return 1;
	}
	//cv::Mat disparity = cv::imread("disp8", cv::IMREAD_GRAYSCALE);
    
	if (disparity.empty())
	{
		std::cerr << "ERROR: Could not read disparity" << std::endl;
		return 1;
	}
		disparity.convertTo(disparity, CV_32F, 1.0 / 16.0);
		cv::Mat image3D;
	cv::reprojectImageTo3D(disparity, image3D, Q, false, CV_32F);
        for (int i = 0; i < image3D.rows; i++)
	{
		const cv::Vec3f* image3D_ptr = image3D.ptr<cv::Vec3f>(i);

		for (int j = 0; j < image3D.cols; j++)
		{      
			//cout<< image3D_ptr[j][0] << " " << image3D_ptr[j][1] << " " << image3D_ptr[j][2] << endl;
			if(min > image3D_ptr[j][2] ) min = image3D_ptr[j][2] ;
		}


	}  
			   	   

		freopen ("myfile.txt","w",stdout);
		cout << "Closest object is detected at a distance of   ";
	    conversion (min) ;
		
		fclose (stdout);
		system("say -f myfile.txt"); 

						   	    namedWindow("left",WINDOW_NORMAL);
							    resizeWindow("left",600,600);

							    namedWindow("right",WINDOW_NORMAL);
						        resizeWindow("right",600,600);
						        
				               namedWindow("disp",WINDOW_NORMAL);
			           		   resizeWindow("disp",600,600);
		imshow("left", img1);
		imshow("right", img2);
		//imshow("disp", disparity);
	if (waitKey(5) >= 0)
	break;
         }
	return(0);
}
