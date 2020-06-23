#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Pose.h"

#include <sstream>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "hd_indoor");
    ros::NodeHandle n;

    ros::Publisher hd_pub = n.advertise<geometry_msgs::Pose>("hd_indoor/pose", 1000);
    ros::Rate loop_rate(10);

    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    float w = 0.0;

    while (ros::ok())
    {
        geometry_msgs::Pose pose;

        pose.position.x = x;
        pose.position.y = y;
        pose.position.z = z;
        pose.orientation.x = 0.022260;
        pose.orientation.y = 0.439680;
        pose.orientation.z = 0.360424;
        pose.orientation.w =  0.822363;

        // quaternion.x = x;
        // quaternion.y = y;
        // quaternion.z = z;
        // quaternion.w = w;
        // pose..x = x;
        // pose.position.y = y;

        ROS_INFO("hd_indoor pose publish" );       

        hd_pub.publish(pose);

        ros::spinOnce();

        loop_rate.sleep();
        ++x;
        ++y;
        ++z;
        ++w;
    }

    return 0;
}