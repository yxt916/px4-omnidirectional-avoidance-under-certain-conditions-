#include <opencv2/opencv.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/core/core_c.h>
#include <opencv2/core/core.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <string>  
#include <iostream>
#include "ros/ros.h"	
#include "std_msgs/String.h"
#include "std_msgs/Float32MultiArray.h"	
#include <sstream> 
#include <string.h>
#include <pcl/point_cloud.h> 
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h> 
#include <pcl_conversions/pcl_conversions.h> 
#include<iomanip>
#include <tf/transform_listener.h>
#include <tf/transform_datatypes.h>
#include <geometry_msgs/PointStamped.h>
#include <unistd.h>
using namespace std;
using namespace cv;
Point3f uv2xyz(Point2f uvLeft,Point2f uvRight);
std_msgs::Float32MultiArray msgg;
std_msgs::Float32MultiArray m;
ros::Publisher pub;
int i=0;
//左相机内参数矩阵
float leftIntrinsic[3][3] = {    238.351,             0,            200.5,
                                       0,       238.351,            200.5,
                                       0,             0,                1};
//左相机畸变系数
float leftDistortion[1][5] = {0, 0, 0, 0, 0};
//左相机旋转矩阵
float leftRotation[3][3] = {1,        0,         0, 
                            0,        1,         0, 
                            0,        0,         1};
//左相机平移向量
float leftTranslation[1][3] = {0, 0, 0};
 
//右相机内参数矩阵
float rightIntrinsic[3][3] = {    238.351,             0,            200.5,
                                        0,       238.351,            200.5,
                                        0,             0,                1};
//右相机畸变系数
float rightDistortion[1][5] = {0, 0, 0, 0, 0};
//右相机旋转矩阵
float rightRotation[3][3] = {1,        0,         0, 
                             0,        1,         0, 
                             0,        0,         1};
//右相机平移向量
float rightTranslation[1][3] = {-0.07, 0, 0};
 
Point3f uv2xyz(Point2f uvLeft,Point2f uvRight)//双目计算三维点世界坐标函数
{
    //     [u1]      [xw]                      [u2]      [xw]
    //zc1 *|v1| = Pl*[yw]                  zc2*|v2| = P2*[yw]
    //     [ 1]      [zw]                      [ 1]      [zw]
    //               [1 ]                                [1 ]
    Mat mLeftRotation = Mat(3,3,CV_32F,leftRotation);
    Mat mLeftTranslation = Mat(3,1,CV_32F,leftTranslation);
    Mat mLeftRT = Mat(3,4,CV_32F);//左相机RT矩阵
    hconcat(mLeftRotation,mLeftTranslation,mLeftRT);
    Mat mLeftIntrinsic = Mat(3,3,CV_32F,leftIntrinsic);
    Mat mLeftP = mLeftIntrinsic * mLeftRT;
//    cout<<"左相机P矩阵 = "<<endl<<mLeftP<<endl;
 
    Mat mRightRotation = Mat(3,3,CV_32F,rightRotation);
    Mat mRightTranslation = Mat(3,1,CV_32F,rightTranslation);
    Mat mRightRT = Mat(3,4,CV_32F);//右相机RT矩阵
    hconcat(mRightRotation,mRightTranslation,mRightRT);
    Mat mRightIntrinsic = Mat(3,3,CV_32F,rightIntrinsic);
    Mat mRightP = mRightIntrinsic * mRightRT;
//    cout<<"右相机P矩阵 = "<<endl<<mRightP<<endl;
 
    //最小二乘法A矩阵
    Mat A = Mat(4,3,CV_32F);
    A.at<float>(0,0) = uvLeft.x * mLeftP.at<float>(2,0) - mLeftP.at<float>(0,0);
    A.at<float>(0,1) = uvLeft.x * mLeftP.at<float>(2,1) - mLeftP.at<float>(0,1);
    A.at<float>(0,2) = uvLeft.x * mLeftP.at<float>(2,2) - mLeftP.at<float>(0,2);
 
    A.at<float>(1,0) = uvLeft.y * mLeftP.at<float>(2,0) - mLeftP.at<float>(1,0);
    A.at<float>(1,1) = uvLeft.y * mLeftP.at<float>(2,1) - mLeftP.at<float>(1,1);
    A.at<float>(1,2) = uvLeft.y * mLeftP.at<float>(2,2) - mLeftP.at<float>(1,2);
 
    A.at<float>(2,0) = uvRight.x * mRightP.at<float>(2,0) - mRightP.at<float>(0,0);
    A.at<float>(2,1) = uvRight.x * mRightP.at<float>(2,1) - mRightP.at<float>(0,1);
    A.at<float>(2,2) = uvRight.x * mRightP.at<float>(2,2) - mRightP.at<float>(0,2);
 
    A.at<float>(3,0) = uvRight.y * mRightP.at<float>(2,0) - mRightP.at<float>(1,0);
    A.at<float>(3,1) = uvRight.y * mRightP.at<float>(2,1) - mRightP.at<float>(1,1);
    A.at<float>(3,2) = uvRight.y * mRightP.at<float>(2,2) - mRightP.at<float>(1,2);
 
    //最小二乘法B矩阵
    Mat B = Mat(4,1,CV_32F);
    B.at<float>(0,0) = mLeftP.at<float>(0,3) - uvLeft.x * mLeftP.at<float>(2,3);
    B.at<float>(1,0) = mLeftP.at<float>(1,3) - uvLeft.y * mLeftP.at<float>(2,3);
    B.at<float>(2,0) = mRightP.at<float>(0,3) - uvRight.x * mRightP.at<float>(2,3);
    B.at<float>(3,0) = mRightP.at<float>(1,3) - uvRight.y * mRightP.at<float>(2,3);
 
    Mat XYZ = Mat(3,1,CV_32F);
    //采用SVD最小二乘法求解XYZ
    solve(A,B,XYZ,DECOMP_SVD);

    //定义世界坐标系中坐标
    Point3f world;
    world.x = XYZ.at<float>(0,0);
    world.y = XYZ.at<float>(1,0);
    world.z = XYZ.at<float>(2,0);
 
    return world;
}
void chatterCallback(const std_msgs::Float32MultiArray::ConstPtr& msg)//出现！接收到话题uvpoint两对四个元素的数组消息msg，执行这个函数
{	i=i+1;
	ros::Rate loop_rate(10);
	Point2f uvLeft;
	Point2f uvRight;
	uvLeft.x=msg->data[0];
	uvLeft.y=msg->data[1];
	uvRight.x=msg->data[2];
	uvRight.y=msg->data[3];
	//已知左右相机像素坐标求空间坐标
	Point3f worldPoint;
	worldPoint = uv2xyz(uvLeft,uvRight);
//	cout<<"空间坐标为:"<<endl<<worldPoint<<endl;

//得到了一个世界坐标系下的三维点 worldpoint--(x,y,z)

	float x=0,y=0,z=0;

	x=worldPoint.x;
	y=worldPoint.y;
	z=worldPoint.z;

//有数据来时将一个三维点worldpoint压入全局变量数组msgg，并累计
	if(i<=10){msgg.data.push_back(x);msgg.data.push_back(y);msgg.data.push_back(z);}//<10个，只压10个点的数据，压入msgg
	if(i>20){msgg.data=m.data;}//够了赶紧清零防止二次使用,后续不压入
}	
int main(int argc, char **argv)
{	 
	ros::init(argc, argv, "transform");	
	ros::NodeHandle n;
	ros::Rate loop_rate(10);
	ros::Subscriber sub = n.subscribe("uvpoint", 1, chatterCallback);
	ros::Publisher pub = n.advertise<std_msgs::Float32MultiArray>("worldpoint", 1);//发布话题worldpoint，携带三个元素的数组消息msgg
	while(ros::ok())
	{
	ros::spinOnce();
	loop_rate.sleep();
	pub.publish(msgg);//----------------------------------------不论是否有内容，一直《发布》携带数组消息msgg-------------------------------------------

	}

//	ros::spin();
	return 0;
}
