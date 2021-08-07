# px4-omnidirectional-avoidance-under-certain-conditions-

A ROS simulation using stereo, px4 avoidance, deepsort, YOLOv3 and LS. Condition:only "person" are trained and only for uniform linear motion.

1.Environment setup please follow:

https://github.com/PX4/PX4-Avoidance

https://github.com/khayliang/person_tracking_ros

Set up environment is very nasty, it will take time.


2.Principle introduction：


![image](https://user-images.githubusercontent.com/68857748/128591008-d3dd80b7-47fa-419b-9026-7fdec664d27a.png)










3.TO START, you can simply download the stable release zip package v1.0, make sure all environment&path&virable all is well, make sure your ROS workspace name is "catkin_ws"and, 
RUN:






roslaunch local_planner local_planner_stereo.launch

roslaunch person_tracking person_tracker.launch

roslaunch transform transform.launch


These 3 command will start the whole avoidance node. With 10 fps.(i7 10th,RTX2060)

The whole rqtgraph:


![rosgraph2](https://user-images.githubusercontent.com/68857748/128590767-d5ad1ddc-9984-4fa8-95e1-188ff2cb8ec1.png)


The drone will remember the uniform linear motion of person, when person comes from behind, the drone can still avoidance.


It's design is original by "yxt916", protected by copyrigh.
