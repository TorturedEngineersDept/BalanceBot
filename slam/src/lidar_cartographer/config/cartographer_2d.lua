include "map_builder.lua"  
include "trajectory_builder.lua"  

options = {
  map_builder = MAP_BUILDER,  
  trajectory_builder = TRAJECTORY_BUILDER,  

  map_frame = "map",  -- map frame name 
  tracking_frame = "laser_link",  -- tracking frame name

  published_frame = "odom",  -- published frame name
  -- set to "odom" if it is provided by a different part of the system, otherwise set to "base_link"

  odom_frame = "odom",  -- name of the odometer frame
  provide_odom_frame = false,  -- whether to provide the odometer frame
  -- if set to true, odom frame is published to "odom_frame"

  publish_frame_projected_to_2d = false,  -- whether to publish 2d gesture  
  use_pose_extrapolator = false,
  use_odometry = false,  -- whether use odometry
  use_nav_sat = false,  -- whether use the navigation satellite 
  use_landmarks = false,  -- whether use the landmark
  num_laser_scans = 1,  -- LiDAR number  
  num_multi_echo_laser_scans = 0,  -- number of multi-echo LiDAR  
  num_subdivisions_per_laser_scan = 1,  -- number of subdivisions for each laser scan
  num_point_clouds = 0,  -- number of cloud points
  lookup_transform_timeout_sec = 0.2,  -- timeout for finding transformations (seconds)  
  submap_publish_period_sec = 0.3,  -- submap release cycle (seconds)
  pose_publish_period_sec = 5e-3,  -- attitude release period (seconds)
  trajectory_publish_period_sec = 30e-3,  -- trajectory release period (seconds)
  rangefinder_sampling_ratio = 1.,  -- rangefinder sampling ratio
  odometry_sampling_ratio = 1.,  -- odometer sampling rate
  fixed_frame_pose_sampling_ratio = 1.,  -- fixed frame attitude sampling ratio  
  imu_sampling_ratio = 1.,  -- IMU sampling ratio
  landmarks_sampling_ratio = 1.,  -- landmarks sampling ratio
}
 
MAP_BUILDER.use_trajectory_builder_2d = true  -- whether use 2D SLAM
TRAJECTORY_BUILDER_2D.submaps.num_range_data = 35  -- Number of range data for submaps in the 2D track builder  
TRAJECTORY_BUILDER_2D.min_range = 0.1  -- ignore anything smaller than the robot radius, limiting it to the minimum scan range of the lidar
TRAJECTORY_BUILDER_2D.max_range = 3.5  -- the maximum scanning range of the lidar
TRAJECTORY_BUILDER_2D.missing_data_ray_length = 5.  -- Restricted to maximum LiDAR scanning range  
TRAJECTORY_BUILDER_2D.use_imu_data = false  -- whether use IMU data
TRAJECTORY_BUILDER_2D.use_online_correlative_scan_matching = true  -- Whether to scan for matches using real-time loopback detection

TRAJECTORY_BUILDER_2D.motion_filter.max_angle_radians = math.rad(0.1)  -- Modify 1.0 to 0.1, increased sensitivity to movement
POSE_GRAPH.constraint_builder.min_score = 0.65  -- Modify 0.55 to 0.65, the minium score of Fast csm, can be optimized above this score 
POSE_GRAPH.constraint_builder.global_localization_min_score = 0.7  -- Modify 0.6 as 0.7, Minimum global positioning score below which global positioning is considered currently inaccurate

return options