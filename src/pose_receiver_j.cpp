#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"

// to-do
// subscribe topic : "hd_indoor/pose", type: geomety_msgs::pose
// 단위 변환 : mm --> m 
// publish topci : "/mavros/vision_pose/pose", type: geometry_mssgs::poseStamped

// publish 노드 생성 
ros::Publisher pose_pub;

//callback 함수 구현 
void poseSubCallback(const geometry_msgs::Pose msg){

    geometry_msgs::PoseStamped receivedMsg; 
    receivedMsg.pose= msg;
    
    //단위 변환 mm -> m 
    receivedMsg.pose.position.x = receivedMsg.pose.position.x/1000;
    receivedMsg.pose.position.y = receivedMsg.pose.position.y/1000;
    receivedMsg.pose.position.z = receivedMsg.pose.position.z/1000;
    
    // 부가 정보
    receivedMsg.header.seq++;
    receivedMsg.header.stamp = ros::Time::now();

    //publish 
    pose_pub.publish(receivedMsg);
    ROS_INFO("seq: %d, publish PoseStamped to MAVROS",receivedMsg.header.seq);

}


int main(int argc, char **argv){
    //노드 초기화   
    ros::init(argc, argv, "pose_receiver");
    //노드 핸들 생성
    ros::NodeHandle nh;
    // subscribe 노드 생성 
    ros::subscriber pose_sub = nh.subscribe("hd_indoor/pose",1000, poseSubCallback);
    nh.advertise<geometry_msgs::PoseStamped>("/mavros/vision_pose/pose", 1000); 

    ros::spin();

    return 0;

}