#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "tf/transform_datatypes.h"


// 전역에 new PoseStamped 메시지 생성 
geometry_msgs::PoseStamped receivedMsg;

ros::Publisher pose_pub;

void poseSubCallback(const geometry_msgs::Pose msg)
{
    ROS_INFO("Subscribe pose");
    receivedMsg.pose = msg;

// mm --> m
    receivedMsg.pose.position.x = receivedMsg.pose.position.x/1000;
    receivedMsg.pose.position.y = receivedMsg.pose.position.y/1000;
    receivedMsg.pose.position.z = receivedMsg.pose.position.z/1000;

// 부가 정보
    receivedMsg.header.seq++;
    receivedMsg.header.stamp = ros::Time::now();

//    ROS_INFO("*** pose_receiver position ***  x : %f, y: %f, z: %f",receivedMsg.pose.position.x ,receivedMsg.pose.position.y, receivedMsg.pose.position.z);
    pose_pub.publish(receivedMsg);
    ROS_INFO("seq: %d, publish PoseStamped",receivedMsg.header.seq);   

}



int main(int argc, char **argv)
{
    ros::init(argc, argv, "pose_receiver");
    ros::NodeHandle n;

    ros::Subscriber pose_sub = n.subscribe("hd_indoor/pose", 1000, poseSubCallback);
    
    pose_pub = n.advertise<geometry_msgs::PoseStamped>("/mavros/vision_pose/pose", 1000);

    ros::spin();
    return 0;
}
