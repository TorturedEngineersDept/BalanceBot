from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import TimerAction

def generate_launch_description():
    return LaunchDescription([
 
        Node(
            period=20.0,
            actions=[
                Node(
                    package='autonomous_exploration',
                    executable='control',
                    name='exploration_node',
                    output='screen',
                    parameters=[{'use_sim_time': False}],
                )
            ]
        ),
    ])
