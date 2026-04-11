from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
   return LaunchDescription([
      Node(
         package='destroyer',
         executable='auto_mode_destroyer',
         name='auto_mode_destroyer',
         output='screen',
      ),
      Node(
         package='destroyer',
         executable='check_move_destroyer',
         name='check_move_destroyer',
         output='screen',
      ),
   ])