#!/usr/bin/env python
# -*- coding: utf-8 -*-
import rospy,math,random
import numpy as np
import message_filters
from std_msgs.msg import String
from geometry_msgs.msg import PointStamped
from geometry_msgs.msg import Point
from utils.log import get_logger
from std_msgs.msg import Header
from std_msgs.msg import Float32MultiArray 
def multi_callback(Left,Right):
	rate = rospy.Rate(10) 
	data_list=([Left.x,Left.y,Right.x,Right.y])        
	data=Float32MultiArray(data=data_list)
	rate.sleep()
	pub.publish(data)
print("牛")
if __name__ == '__main__':
    rospy.init_node('sync', anonymous=True) 
    left=message_filters.Subscriber("/person_tracking/bbox_center", Point, queue_size=1) 
    right=message_filters.Subscriber("/person_tracking2/bbox_center2", Point, queue_size=1)
    sync = message_filters.ApproximateTimeSynchronizer([left, right],1,0.1,allow_headerless=True)
    sync.registerCallback(multi_callback)
    pub = rospy.Publisher('uvpoint', Float32MultiArray, queue_size=1)
    rospy.spin()
