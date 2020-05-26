#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Pose.h"


void chatterCallback(const geometry_msgs::Pose::ConstPtr& msg)
{
  ROS_INFO("\nI heard:\n x:%f\ny:%f\nz:%f\nw:%f\n-----", msg->position.x,msg->position.y,msg->orientation.z,msg->orientation.w );
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "hd_indoor_sub");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("hd_indoor/pose", 1000, chatterCallback);
    ros::spin();
    return 0;
}