#!/usr/bin/env python
# -*- coding: utf-8 -*-
import rospy
from std_msgs.msg import String
import  numpy as np
from std_msgs.msg import Float32MultiArray		
import matplotlib.pyplot as plt
from scipy import optimize
import struct
from sensor_msgs import point_cloud2
from sensor_msgs.msg import PointCloud2, PointField
from std_msgs.msg import Header
pub=rospy.Publisher("/predicted_pointcloud",PointCloud2,queue_size=1)#发布预测的点云 pointcloud
#i=0
#声明订阅器回调函数  轨迹预测
def callback(p):
#	global i
	rate = rospy.Rate(10) 
	if (len(p.data)==30):#存有10个点数据
		i=0
		data=np.array([[p.data[0],p.data[1]],[p.data[3], p.data[4]],[p.data[6], p.data[7]],[p.data[9], p.data[10]],[p.data[12], p.data[13]],[p.data[15], p.data[16]],[p.data[18], p.data[19]],[p.data[21], p.data[22]],[p.data[24], p.data[25]],[p.data[27], p.data[28]]])
#,[p.data[30], p.data[31]],[p.data[33], p.data[34]],[p.data[36], p.data[37]],[p.data[39], p.data[40]],[p.data[42], p.data[43]],[p.data[45], p.data[46]],[p.data[48], p.data[49]],[p.data[51], p.data[52]],[p.data[54], p.data[55]],[p.data[57], p.data[58]]
		def f_1(x, A, B):
			return A*x + B
#scipy算法包封装的二维拟合函数
		X = np.array(data[:, 0])
		Y = np.array(data[:, 1])
		A1, B1 = optimize.curve_fit(f_1, X, Y)[0]#拿前10个数据拟合出轨迹	
		v=1#速度获取
			#方向正负号获取
		x=-p.data[0]#原点获取
		while(i<1000):#每一次循环生成一个点,生成1000个点（100m）,生成带有速度信息的点轨迹
#			for d in range(2):#生成点所需的材料-----------------------------------------]
			points = []
			x=x+0.1#方向正负号获取
			y=float(A1*x+B1)
			z=2
			pt = [x, y, z, 255]
			r = 255
			g = 255
			b = 255
			a = 100
			rgb = struct.unpack('I', struct.pack('BBBB', b, g, r, a))[0]
			pt[3] = rgb
			points.append(pt)#点动累计---↑生成点，↓生成点云---------------------------------tab]
			fields = [PointField('x', 0, PointField.FLOAT32, 1),PointField('y', 4, PointField.FLOAT32, 1),PointField('z', 8, PointField.FLOAT32, 1),PointField('rgb', 12, PointField.UINT32, 1)]
			header = Header()
			header.stamp = rospy.Time.now()
			header.frame_id = "local_origin"
			pc2 = point_cloud2.create_cloud(header, fields, points)
			rate.sleep()
			pub.publish(pc2)#----------发 布----------当前点
			points=[]#清零，等待生成下一个点
			i=i+1

#没有一段10个点数据时创建空预测点(一个标识点)
	else:
		points = []
		pt = [0, 0, -1, 255]
		r = 255
		g = 255
		b = 255
		a = 0
		rgb = struct.unpack('I', struct.pack('BBBB', b, g, r, a))[0]
		pt[3] = rgb
		points.append(pt)
		fields = [PointField('x', 0, PointField.FLOAT32, 1),PointField('y', 4, PointField.FLOAT32, 1),PointField('z', 8, PointField.FLOAT32, 1),PointField('rgb', 12, PointField.UINT32, 1)]
		header = Header()
		header.stamp = rospy.Time.now()
		header.frame_id = "local_origin"
		pc2 = point_cloud2.create_cloud(header, fields, points)
		pc2.header.stamp = rospy.Time.now()
		rate.sleep()
		pub.publish(pc2)#----------发 布-----------标志点


def listener():
	rospy.init_node('prediction',anonymous=True)
	rospy.Subscriber('worldpoint',Float32MultiArray,callback,queue_size=1)
	rospy.spin()

if __name__ == '__main__':
    listener()

