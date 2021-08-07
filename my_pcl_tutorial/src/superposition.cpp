#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include "std_msgs/String.h"
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/conversions.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <iostream>
#include <sstream>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <message_filters/time_synchronizer.h>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace message_filters;
ros::Publisher pubc;
int multi_callback (const sensor_msgs::PointCloud2::ConstPtr& msga, const sensor_msgs::PointCloud2::ConstPtr& msgb)
{
ros::Rate loop_rate(10);

sensor_msgs::PointCloud2 cloudc_ros;
pcl::PointCloud<pcl::PointXYZI> clouda;
pcl::PointCloud<pcl::PointXYZI> cloudb;
pcl::PointCloud<pcl::PointXYZI> cloudc;

pcl::fromROSMsg(*msga, clouda);
pcl::fromROSMsg(*msgb, cloudb);

cloudc=clouda+cloudb;//点云叠加
pcl::toROSMsg (cloudc, cloudc_ros);
cloudc_ros.header.stamp = ros::Time::now();
cloudc_ros.header.frame_id = "local_origin";

loop_rate.sleep();
pubc.publish(cloudc_ros);


}
int main (int argc, char** argv)
{
	ros::init (argc, argv, "superposition");
	ros::NodeHandle nh;
	pubc = nh.advertise<sensor_msgs::PointCloud2> ("final_pointcloud", 1);
	message_filters::Subscriber<sensor_msgs::PointCloud2> sub(nh,"/predicted_pointcloud",1,ros::TransportHints().tcpNoDelay());
	message_filters::Subscriber<sensor_msgs::PointCloud2> subb(nh,"/local_pointcloud",1,ros::TransportHints().tcpNoDelay());
	typedef sync_policies::ApproximateTime<sensor_msgs::PointCloud2, sensor_msgs::PointCloud2> syncPolicy;
	Synchronizer<syncPolicy> sync(syncPolicy(10), sub, subb);  
	sync.registerCallback(boost::bind(&multi_callback, _1, _2));
	ros::spin();  
	return 0;
}
