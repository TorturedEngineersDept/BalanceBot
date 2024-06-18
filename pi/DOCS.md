# Full docs for testing
### Setup

- Raspberry Pi 4B (4GB version), Ubuntu 22.04 LTS
- PiCamera V2 and V3 are available. 
- Local laptop Ubunutu 22.04 Jammy
- ROS distro: ROS 2 Humble (which is officially linked to 22.04)

### Setup steps

Guide for quickly setting up local computer to do SLAM.

1. Install ROS 2 Humble from Debian packages
https://docs.ros.org/en/humble/Installation/Ubuntu-Install-Debians.html

2. Download the listed packages in the section [[#Nodes in the system]] below under local laptop. There are references below on how to setup. (Alternatively you can just clone Kevin's most updated workspace on his GitHub branch)

Learn how to create ROS packages in the workspace in [[#How to create packages?]]

3. Everytime you make a change, `colcon build` in the `workspace/` directory, NOT the `workspace/src/` directory.

4. Always remember to `source install/setup.bash` in a workspace (in a new terminal), and `source /opt/ros/humble/setup.bash`. Alternatively add them to your .bashrc

Debugging tips: ChatGPT the missing dependencies, or the terminal will give you what to sudo-apt get / install.

### Things to test (for William)

1. Please test the PiCam V2, Hanif's group was able to make it work on the setup I'm running (Ubuntu 22.04 LTS, ROS 2 Humble). I only tested V3, which hasn't worked so far. Refer to [[#Picamera debug]] section below.

Specification: (step-by-step)
- Should be able to capture an image and save it in the ~/
- Should be run a simple colour thresholding script and detect green. 
	- The colour thresholding Python script already exists in the Raspberry Pi's ~/ as `colour_thresh.py`, or a similar name.
- Should be able to capture ONE image when it detects something green. 
	- It should not repeatedly capture image of the same object when re-detecting the same green object. 
	- You may use some hacks for the purpose of the demo.

It would be best if you could run it as a ROS node, which is trivial once it starts running.

2. Test the robot such that it is able to autonomously explore and map your bedroom (if with stabilisers, make sure the stabilisers don't affect movement).

Specifications: (step-by-step)
- Able to manually control and map out the entire bedroom. Refer to [[#SLAM tuning]]
- Able to autonomously control and map out the entire bedroom. Refer to [[#Autonomous control tuning]]

There are several other things that need to be done, but I will handle it after I come back, because this is already a lot to do - ***please spend your remaining time on the report***.

### To-do list (not for William)

>Before report deadline (Mon)

1. Following [[#Things to test]] #2, we will run it in the self-built arena and take a picture for our report's "testing section".

>Before demo (Thu)

1. Waypoint Manager

Specifications:
- Marks waypoints on the SLAM map based on the current position of the robot, when it receives a signal from the camera node (that is has detected an incident)
- After exploration is completed, this node (or another node) should download the map as a .pgm and .yaml file. The .yaml file should include coordinates of the waypoint. This will then be sent to the website server to be displayed on the UI.

Note: We need to spray paint the boxes green.

### ROS

ROS 2 is used. ROS operates on the idea of publishing and subscribing to different topics that are published by different software packages running on different hardware nodes. These topics are published as "frames".

### ROS Domain

In the same subnet under the same network, you can see all the published topics. To avoid clashes, you can export `$ROS_DOMAIN_ID` as a certain number, so different ROS systems don't interfere. For example, if you run a simulation but forget to turn off the real-life LiDAR, then your simulation will keep using the real laser_scan data and output weird stuff. However, I usually doesn't set domain ids because no one else is using ROS on his hotspot / accommodation room subnet.

### Viewing topics

Run: `ros2 topic list` 
This shows all the published topics in your subnetwork.

To see if a specific topic is actually publishing anything useful, you can run:
`ros2 topic echo /{topic}`
For example, `ros topic echo /map`

### TF tree

The flow of data can be observed in a TF (transform) tree, which shows the relationship and transformation of data between different frames in the robotic system.

To observe the flow of the data:
```bash
# Install
sudo apt-get install ros-humble-tf2-tools

# Run
ros2 run tf2_tools view_frames
```

I previously sent an image to you in dms. Check chat history.

Please note that, when configuring topic publishing, there are some frames that are not published as a topic. For example, Cartographer runs a system where it can create its own `odom` frame. You can configure whether to create / use the frame and its frame id, but if you don't publish it as a topic, it is only seen within Cartographer and does not appear in `ros2 topic list`.

Sometimes this causes issues where you don't know that there's an `odom` frame that's **hidden** and accidentally messing up your published topic. I'll explain more with regards to Cartographer in [[#Odometry tuning]].

### How to create packages?

>Creating a package

```bash
cd ~/workspace/src
ros2 pkg create --build-type ament_cmake --dependencies rclcpp std_msgs {package_name}
```

The created package will have the following structure:
```css
my_package/
├── CMakeLists.txt
├── package.xml
├── src/
└── include/
```

All the packages that are run are stored under `~/workspace/src`.

>Looking into the package

Note that you some packages use launch files (e.g. Cartographer), some don't (e.g. autonomous exploration).

There are several other possible directories and files:
```css
my_package/
├── CMakeLists.txt
├── package.xml
├── src/
├── launch/
├── resource/
├── setup.py
└── include/
```

It's typical to see the `launch.py` file under l`aunch/ `and a relevant un-editable resource file under `resource/` . The script you will launch is usually inside `src/` in this package. 

Before you `colcon build`, remember to check that your `package.xml` has the correct exec and build dependencies, and that your `setup.py` has the correct entries. For more information, please reference my files.

```bash
# Running a node (without launch file)
ros2 run my_package my_node
## For example
ros2 run autonomous_exploration control

# Running a node using launch file
ros2 run my_package my_package.launch.py
## For example
ros2 launch rf2o_laser_odometry rf2o_laser_odometry.launch.py
```

>Example installation of `rf2o_laser_odometry`

```bash
# Creating a workspace (if you haven't already)
mkdir -p ~/ros2_ws/src

# Cloning the repository
cd ~/ros2_ws/src
git clone https://github.com/MAPIRlab/rf2o_laser_odometry.git

# Installing dependencies
cd ~/ros2_ws
rosdep install --from-paths src --ignore-src -r -y

# Building the package
colcon build --packages-select rf2o_laser_odometry

# Sourcing the workspace
source ~/ros2_ws/install/setup.bash

# Launching the node
ros2 launch rf2o_laser_odometry rf2o_laser_odometry.launch.py
```

### Nodes in the system

Here are the nodes which I built in his system. Note that you may see some packages on more than one device. This is because all of the processing used to be run on the Raspberry Pi 4, before most of it was moved to the local laptop processing.

Please refer to the references for how it was setup. The instructions should be rather intuitive and easy to follow, if you understand [[#How to create packages?]]

NB: I use /scan interchangeably with /laser_scan .

Raspberry Pi
- ldlidar_stl_ros2
	- Topics: Takes in raw data from the LiDAR, and publishes 2D pointcloud data in topic /scan
	- Reference: https://github.com/ldrobotSensorTeam/ldlidar_stl_ros2
	- Secondary reference: https://www.waveshare.com/wiki/Ugv02_Using_LiDAR_Tutorial
- communication_module
	- Topics: Subscribes to /cmd_vel, packages it as JSON data (or whatever encoded format it is modified to output), and opens up a serial port to communicate to the ESP32
	- Reference: https://www.waveshare.com/wiki/Chassis_Control_Tutorial

Local
- lidar_cartographer
	- Topics: Subscribes to /scan and /odom, and publishes /map .
	- Reference: https://www.waveshare.com/wiki/Cartographer_Map_Building
	- Note: The current setup relies on rf2o_laser_odometry's /odom topic. So you need to run both this and the rf2o node for the mapping to work. Refer to [[#Odometry tuning]] for more information as to why.
- rf2o_laser_odometry
	- Topics: Subscribes to /scan and publishes /odom .
	- Reference: https://github.com/MAPIRlab/rf2o_laser_odometry
	- Note: The repo doesn't provide installation details. I have a section on how to install this in [[#How to create packages?]] above.
- autonomous_exploration
	- Topics: Subscribes to /map, /scan, /odom, and publishes /cmd_vel .
	- Reference: https://github.com/abdulkadrtr/ROS2-FrontierBaseExplorationForAutonomousRobot/tree/main
	- Note: By following the repo README you should be able to run its simulation on your laptop. In my codebase, I have configured it such that it outputs English comments in the terminal and also it runs EITHER linear.x (W,S) OR angular.z (A,D) commands.

Other nodes you might see but aren't used
- lidar_bringup
	- Topics: It runs on Nav2, and this specific design does remappings of tf and tf_static. Nav2 has multiple library packages and a variety of topic transformations that can be done. Currently it is not in use, because I am running `autonomous_exploration`, which directly provides movement commands and works with the ESP32. Nav2 however might be used for `waypoint_manager` node when after it is tested locally.
	- Reference: https://www.waveshare.com/wiki/Mapping_and_Navigation_Simultaneously_Tutorial

So as we can see, ignoring the hidden links and layers such as base_layer, base_link, laser_link, the basic idea of the data is:

LiDAR SDK: 
	LiDAR raw data -> /scan
Cartographer SLAM:
	/scan + /odom -> /map
rf2o Odometry calculations:
	/scan -> /odom
Autonomous exploration:
	/map + /scan + /odom -> /cmd_vel
Communication module:
	/cmd_vel -> encoded commands serial to ESP32

At this point you may ask, so why exactly is rf2o needed? Refer to [[#Odometry tuning]] below.

### SLAM tuning

>.lua file

The `cartographer_2d.lua` file under `lidar_cartographer/config/` provides parameters that configure the Local and Global SLAM algorithms that occur inside Cartographer, which is a SLAM library created by Google. 

The core idea is that Local SLAM builds a succession of 'submaps'. Global SLAM operates in background threads and tries to 'close loops' by matching the current scanned surrounding features to features in the locally built submaps.

There are a lot of parameters that can be tuned in the .lua file, and they are ALL LABELLED using comments. 

Documentation: https://google-cartographer-ros.readthedocs.io/en/latest/algo_walkthrough.html

- Changes I've made on Thursday include:

```lua
TRAJECTORY_BUILDER_2D.motion_filter.max_angle_radians = math.rad(0.7)
-- Changed the number from 0.1 to 0.7
-- This (according to the comments) should reduce senstivity to movement
-- Motion filter: A scan is dropped if the motion that led to it is not considered significant enough.
```

I don't think it worked because in general the turning motion was too large and jerky, so the motion filter couldn't really filter.

- Recommended tests: 

Both my recommended tests below surround distance / range. Refer to the comments and also the section below -- speed control -- for more information of why to do so.

```lua
TRAJECTORY_BUILDER_2D.motion_filter.max_distance_meters = 0.05
-- This didn't exist in the file before, but I added it. I haven't tested its effects because I ran out of time.
-- Please reference the official docs before testing.
```

```lua
TRAJECTORY_BUILDER_2D.max_range = 3.5 -- the maximum scanning range of the lidar
-- I think this is a nice test: if we reduce the max_range of the LiDAR, then when a jerky motion is applied, it might not get as confused when suddenly a lot of LiDAR data points in the pointcloud jerk. 

-- But reducing the max_range might lead to quite a lot of side effects.
```

>Speed control

From the testing session on Thursday, I realised that the reason of the huge sudden drift of the map is because the map is turning so fast, such that the POTENTIALLY the odometry gets confused, because suddenly all the pointcloud data shifted massively in the map.

My recommendation: ***Run the rotation as slowly as possible***

Currently my simulation runs `math.pi / 4` rad/s, which is 45 degrees per second. My recommendation is reducing it down even slower such that it doesn't mess up the odometry's calculated pose / orientation.

So just tell Varun to make some changes, or supply him with a smaller number - currently he configured that on a scale of 0 to 1, 1 is equivalent to ~ 45 deg/s .

### Odometry tuning

>Understanding 

This is very important but is a bit hard to understand immediately. Odometry data provides us with the current pose (i.e. position and direction with reference to original starting location) of the robot. 

It is crucial because the `autonomous_exploration` node REQUIRES /odom data to be able to calculate the needed rotational movement to navigate the environment.

>Using Cartographer's odometry

So previously I didn't know that Cartographer calculated and supplied odometry data. In fact, you can configure it in the .lua file, but you also need to make some changes to the cartographer.launch.py launch-file. 

```lua
map_builder = MAP_BUILDER,

trajectory_builder = TRAJECTORY_BUILDER,

map_frame = "map", -- map frame name

tracking_frame = "laser_link", -- tracking frame name

published_frame = "odom", -- published frame name

-- set to "odom" if it is provided by a different part of the system, otherwise set to "base_link"

odom_frame = "odom", -- name of the odometer frame

provide_odom_frame = false, -- whether to provide the odometer frame

-- if set to true, odom frame is published to "odom_frame"

publish_frame_projected_to_2d = false, -- whether to publish 2d gesture

use_pose_extrapolator = false,

use_odometry = false, -- whether use odometry

use_nav_sat = false, -- whether use the navigation satellite

use_landmarks = false, -- whether use the landmark
```

Currently, the /odom is calculated and supplied by the rf2o node. That is why Cartographer is currently dependent on the rf2o node running - it is using the rf2o node's /odom topic.

You can see the comment above for `published_frame` - currently I set it to "odom" because it is being supplied by a different part of the system (rf2o).

In fact, because previously I set `provide_odom_frame` to true and `published_frame` to base_link, the odometry data from Cartographer **CONFLICTED*** with the odometry data calculated from rf2o, resulting in that weird map jerking motion which I showed you.

In the case that you might want to use Cartographer's odometry instead of rf2o's odometry, then you would have to modify the parameters above and also make changes to the 'static transform publishers' in the launch file which I commented out. Then you would also need to make Cartographer published the odom_frame as a topic such that it can be used by other nodes. Please consult online sources. (But most likely I'll attempt these changes) 

>rf2o tuning

You can tune the frequency of rf2o from its launch file.

### Autonomous control tuning

Considerations and methods:

1. There is a possibility that the bot goes too close to and gets stuck to a wall.
- Modify the costmap calculation and make the robot do a central-line costmap navigation (stay as far away as possible from walls)

### Picamera debug

There are some issues with the v3 camera module that I'm running. Please note that it uses a different hardware than the v2 module which I bought. 

- v3: 12MP Sony IMX708 sensor
- v2 / v2.1: 8MP Sony IMX219 sensor

There are several **VERY*** useful resources:
- https://chuckmails.medium.com/enable-pi-camera-with-raspberry-pi4-ubuntu-20-10-327208312f6e
- https://github.com/raspberrypi/picamera2/issues/563
- https://forums.raspberrypi.com/viewtopic.php?t=347172
- https://gaseoustortoise.notion.site/Raspberry-Pi-Camera-bc33c733eeb4417cbd5e3db027a3a429

Useful (but not very for debugging)
- https://articulatedrobotics.xyz/tutorials/mobile-robot/hardware/camera/
- https://groups.google.com/g/hbrobotics/c/Al8wuQRHHfQ
- https://github.com/christianrauch/camera_ros/tree/main

What I attempted:
- Basically everything. I even attempted source builds of several applications like libcamera. 

If you are trying out on my Raspberry Pi:

1. Try to add to `/boot/firmwware/config.txt` the setting `camera_auto_detect=0`

If you are trying out on your own Raspberry Pi:

1. Use `sudo raspi-config` to enable legacy camera, and also I2C and SPI ports. Most tutorials teach you to do so.
2. Set `start_x = 1` in your `/boot/firmware/config.txt`. Refer to online sources for what other values to set. 
3. Use `groups` and `sudo chmod` to give user access to `video` group.
4. Your Raspberry Pi camera should usually show up as `/dev/video0`

Commands for debugging help:
```bash

# Checking video format supported by camera
v4l2-ctl --list-formats-ext -d /dev/video0

# Check device capabilities
v4l2-ctl --all -d /dev/video0

# Inspecting kernel logs
dmesg | grep video
dmesg | grep bcm2835-v4l2

# Check camera status (should output: "supported=1,detected=1,libcamera-interface=0"
vcgencmd get_camera

```

The issue I am currently facing is similar to the following website:
https://forums.raspberrypi.com/viewtopic.php?t=352011

I highly recommend reading the above webpage's answer. It mentions that you can only either use the legacy camera setting or the libcamera setting. I think legacy is better since some people were able to get it working. libcamera-dev might not be best supported to work with RaspiCam on Ubuntu 22.04 LTS.

Also, the mix-and-match now allowed explains why when I run `vcgencmd get_camera`, that libcamera-inteface = 0. It is detected by v4l2 but is not supporting for libcamera, because I am running a setup for legacy.


