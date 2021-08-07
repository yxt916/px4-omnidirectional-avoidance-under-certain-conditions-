# PX4 omnidirectional avoidance

A ROS simulation using stereo, px4 avoidance, deepsort, YOLOv3 and LS. 

Condition:only "person" are trained and only for uniform linear motion.

# 1.Environment setup please follow:

https://github.com/PX4/PX4-Avoidance

https://github.com/khayliang/person_tracking_ros

Set up environment is very nasty, it will take time.


# 2.Principle introduction：


![image](https://user-images.githubusercontent.com/68857748/128591008-d3dd80b7-47fa-419b-9026-7fdec664d27a.png)
![new](https://user-images.githubusercontent.com/68857748/128595097-4595614c-ef4a-413c-99ab-49be4e01392f.gif)










# 3.TO START:
You can simply download the stable release zip package v1.0, make sure environment &path &virable all is well.

RUN:






roslaunch local_planner local_planner_stereo.launch

roslaunch person_tracking person_tracker.launch

roslaunch transform transform.launch


These 3 command will start the whole avoidance node. With 10 fps.(i7 10th,RTX2060)


To fly in Gazebo, i recommand MISSION mode by using QGC, it's automantic and easy to use.

You can find and download linux version of QGC release package:  1.The whole ROS workspace zip.  2.The whole px4 firmware v1.11.0 zip.  3.QGC.



![2021-01-19 11-30-09 的屏幕截图](https://user-images.githubusercontent.com/68857748/128594676-048c3bd6-3a82-4bbe-8e8a-e66d8920c02c.png)
![2021-08-05 13-24-05 的屏幕截图](https://user-images.githubusercontent.com/68857748/128594679-ba38924c-3e5a-4be3-b4fd-8e8c52591f43.png)




NOTE:The drone will remember the uniform linear motion of person, when person comes from behind, the drone can still avoid obstancle.




The whole rqtgraph:


![rosgraph2](https://user-images.githubusercontent.com/68857748/128590767-d5ad1ddc-9984-4fa8-95e1-188ff2cb8ec1.png)


NOTICE:It's originally designed by "yxt916". Protected by copyright.
