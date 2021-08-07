# px4-omnidirectional-avoidance-under-certain-conditions-
A ROS simulation using stereo, px4 avoidance, deepsort, YOLOv3 and LS. Condition:only "person" are trained and only for uniform linear motion.
environment setup please follow:
https://github.com/PX4/PX4-Avoidance
https://github.com/khayliang/person_tracking_ros

Principle introduction：
![image](https://user-images.githubusercontent.com/68857748/128591008-d3dd80b7-47fa-419b-9026-7fdec664d27a.png)
TO START
RUN:
roslaunch local_planner local_planner_stereo.launch
roslaunch person_tracking person_tracker.launch
roslaunch transform transform.launch

Then you can start the whole node:
rqtgraph:
![rosgraph2](https://user-images.githubusercontent.com/68857748/128590767-d5ad1ddc-9984-4fa8-95e1-188ff2cb8ec1.png)


The drone will remember the uniform linear motion of person, when person comes from behind, the drone can still avoidance.

