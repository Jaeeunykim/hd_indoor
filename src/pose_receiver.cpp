#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "tf/transform_datatypes.h"
#include <tf/LinearMath/Matrix3x3.h>
#include <math.h>
// #include "gazebo_msgs/ModelStates.h"

// 전역에 new PoseStamped 메시지 생성 
geometry_msgs::PoseStamped receivedMsg;
geometry_msgs::PoseStamped alreadySentMsg;

ros::Publisher pose_pub;

double RadianToDegree(double radian) {
    double degree =  radian * 180.0 / M_PI;
    return degree;
}

double RPY_RadianToDegree(double roll, double pitch, double yaw) {
    ROS_INFO("Covert Qt to RPY ==> roll: %f, pitch: %f, yaw: %f", RadianToDegree(roll), RadianToDegree(pitch), RadianToDegree(yaw));
}


void poseSubCallback(const geometry_msgs::Pose msg)
{
    ROS_INFO("Subscribe pose");
    receivedMsg.pose = msg;

//Position 변환 (x, y, z --> x, -y, -z)
    receivedMsg.pose.position.y = -receivedMsg.pose.position.y;
    receivedMsg.pose.position.z = -receivedMsg.pose.position.z;

//msg의 orientation을 Qt로 변환 
    tf::Quaternion myQuaternion;
    myQuaternion[0] = receivedMsg.pose.orientation.x;
    myQuaternion[1] = receivedMsg.pose.orientation.y;
    myQuaternion[2] = receivedMsg.pose.orientation.z;
    myQuaternion[3] = receivedMsg.pose.orientation.w;

//Qt를 RPY로 변환 
    double roll, pitch, yaw;
    tf::Matrix3x3(myQuaternion).getRPY(roll, pitch, yaw);

    RPY_RadianToDegree(roll, pitch, yaw);
    //ROS_INFO("Covert Qt to RPY ==> roll: %f, pitch: %f, yaw: %f", roll, pitch, yaw);

// RPY의 x,y,z --> x,-y,-z로 변환 
    roll = roll;
    pitch = -pitch;
    yaw = -yaw;

// RPY를 Qt로 변환 
    myQuaternion.setRPY( roll, pitch, yaw ); 
    myQuaternion.normalize();

// Qt를 receivedMsg의 orientation에 반영 
    receivedMsg.pose.orientation.x = myQuaternion[0];
    receivedMsg.pose.orientation.y = myQuaternion[1];
    receivedMsg.pose.orientation.z = myQuaternion[2];
    receivedMsg.pose.orientation.w = myQuaternion[3];

    tf::Matrix3x3(myQuaternion).getRPY(roll, pitch, yaw);
    RPY_RadianToDegree(roll, pitch, yaw);

// 부가 정보
    receivedMsg.header.seq++;
    receivedMsg.header.stamp = ros::Time::now();

// Publish to MAVROS
    if(alreadySentMsg.header.seq != receivedMsg.header.seq)
    {
        pose_pub.publish(receivedMsg);
        alreadySentMsg = receivedMsg;
        ROS_INFO("seq: %d, publish PoseStamped",receivedMsg.header.seq);
    }   

}



int main(int argc, char **argv)
{
    ros::init(argc, argv, "pose_receiver");
    ros::NodeHandle n;

    alreadySentMsg = receivedMsg;
    ros::Subscriber pose_sub = n.subscribe("hd_indoor/pose", 1000, poseSubCallback);
    
    pose_pub = n.advertise<geometry_msgs::PoseStamped>("/mavros/vision_pose/pose", 1000);

    ros::spin();
    return 0;
}
