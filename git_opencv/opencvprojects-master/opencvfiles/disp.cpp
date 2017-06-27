#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace cv;
using namespace std;
int s=0;
int le[3][3];
int r[3][3];
Mat img1;
Mat img2;
 
void matrixl(int i,int j)
{   int x=0;
    int y=0;
    for(int k=i;k<i+3;k++)
{
   for(int l=j;l<j+3;l++)
{
    Scalar intensity = img1.at<uchar>(k,l);
    le[x][y]=intensity.val[0];
    x++;
    y++;
}
}
}
void matrixr(int i,int j)
{   int x=0;
    int y=0;
    for(int k=i;k<i+3;k++)
{
   for(int l=j;l<j+3;l++)
{
    Scalar intensity = img2.at<uchar>(k,l);
    r[x][y]=intensity.val[0];
    x++;
    y++;
}
}
}
int i1(int i,int j)
{

     Scalar intensity = img1.at<uchar>(i, j);
     return intensity.val[0];
}
int i2(int i,int j)
{

     Scalar intensity = img2.at<uchar>(i, j);
     return intensity.val[0];
}

void ssd()
{    s=0;
    for(int i=0;i<3;i++)
    for(int j=0;j<3;j++)
    {
        s=s+(i1(i,j)-i2(i,j))*(i1(i,j)-i2(i,j));
     }
}


int main()
{  img1 = imread("/home/shivani/Opencv/img1.bmp",CV_LOAD_IMAGE_COLOR);
   img2 = imread("/home/shivani/Opencv/img2.bmp",CV_LOAD_IMAGE_COLOR);

   int rows = img1.rows;
   int cols = img1.cols;
   
   //Mat gray_image1;
   //Mat gray_image2;
   int min=3000;
   int u=0;
 
   //cvtColor( img1, gray_image1, GRAY2COLOR_BGR );
   //cvtColor( img2, gray_image2, GRAY2COLOR_BGR);
   for(int i=0;i<256-3;i++)
  { for(int j=0;j<256-3;j++)
   {   min=3000;
        u=0;
       cout<<"Hello"<<endl ;
       matrixl(i,j);
       for(int z=0;z<cols-3;z++)
      { matrixr(i,z);
       ssd();
       if(s<min)
      { min=s;u=z;
       }
     }
    cout<<"Hello"<<endl ; 
   }
  }
}
