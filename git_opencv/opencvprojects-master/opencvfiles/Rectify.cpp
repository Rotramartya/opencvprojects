#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/contrib/contrib.hpp"
#include <stdio.h>
#include <string.h>
using namespace cv;
using namespace std;int main(int argc, char* argv[])
{
Mat img1, img2, g1, g2;
Mat disp, disp8;
char* method = argv[3];
img1 = imread(argv[1]);
img2 = imread(argv[2]);
M1=(Mat_<double>(3,3)<< 5.3392448961951084e+02, 0., 3.2882415435785850e+02, 0.,
       5.2857636949090272e+02, 2.3724236734128323e+02, 0., 0., 1.);
M2=(Mat_<double>(3,3)<<5.3392448961951084e+02, 0., 3.1406466834897714e+02, 0.,
       5.2857636949090272e+02, 2.4222537580607866e+02, 0., 0., 1.);
P1=(Mat_<double>(3,4)<<  4.3982083946015695e+02, 0., 3.1934153747558594e+02, 0., 0.,
       4.3982083946015695e+02, 2.3984252166748047e+02, 0., 0., 0., 1.,
       0. );
P2=(Mat_<double>(3,4)<< 4.3982083946015695e+02, 0., 3.1934153747558594e+02,
       -1.4702727800788757e+03, 0., 4.3982083946015695e+02,
       2.3984252166748047e+02, 0., 0., 0., 1., 0.);
R1=(Mat_<double>(3,3)<< 9.9994358114548987e-01, -9.2247799505541378e-03,
       5.2666840418636555e-03, 9.1944071478160510e-03,
       9.9994112739199537e-01, 5.7623456357127614e-03,
       -5.3195303489270637e-03, -5.7135964933729038e-03,
       9.9996952824172491e-01);
R2=(Mat_<double>(3,3)<< 9.9990216576701263e-01, -1.3972140149140718e-02,
       -6.6196230278436213e-04, 1.3968111650935405e-02,
       9.9988594945828868e-01, -5.7428157556737068e-03,
       7.4212623221410088e-04, 5.7330075483450612e-03,
       9.9998329079695436e-01 );
D1=(Mat_<double>(1,14)<<  -2.5847769061902259e-01, -1.2969238877538974e-01, 0., 0., 0.,
       0., 0., -4.0469049000857821e-01, 0., 0., 0., 0., 0., 0.);
D2=(Mat_<double>(1,14)<< -2.6275799293489871e-01, -1.3243259842577040e-02, 0., 0., 0.,
       0., 0., -1.9692462348904799e-01, 0., 0., 0., 0., 0., 0.);
Mat cam1map1,cam1map2;
Mat cam2map1,cam2map2;
initUndistortRectifyMap(M1,D1,R1,P1,Size(640,480),CV_16SC2,cam1map1,cam1map2);
initUndistortRectifyMap(M2,D2,R2,P2,Size(640,480),CV_16SC2,cam2map1,cam2map2);
Mat left,right;
remap(img1,left,cam1map1,cam1map2,INTER_LINEAR);
remap(img2,right,cam2map1,cam2map2,INTER_LINEAR);
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
else if (!(strcmp(method, "SGBM")))
{
   StereoSGBM sbm;
   sbm.SADWindowSize = 3;
   sbm.numberOfDisparities = 80;
   sbm.preFilterCap = 63;
   sbm.minDisparity = -39;
   sbm.uniquenessRatio = 10;
   sbm.speckleWindowSize = 100;
   sbm.speckleRange = 32;
   sbm.disp12MaxDiff = 1;
   sbm.fullDP = true;
   sbm.P1 = 216;
   sbm.P2 = 864;
   sbm(g1, g2, disp);
}
cout<< disp <<endl ;
normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);
imshow("left", img1);
imshow("right", img2);
imshow("disp", disp8);waitKey(0);return(0);
}
