#include "ros/ros.h"
#include "geometry_msgs/Pose.h"

#include "tf/tf.h"



int main(int argc, char **argv)
{
    ros::init(argc, argv, "hd_indoor"); 
    ros::NodeHandle n;
    ros::Publisher hd_pub = n.advertise<geometry_msgs::Pose>("hd_indoor/pose", 1000); 
    ros::Rate loop_rate(10);
    
    int rotation = 0; 
    float px = 0.0; 
    float py = 0.0; 
    float pz = 0.0;
    
    tf::Quaternion qt; 
    
    while (ros::ok()) { 
        geometry_msgs::Pose pose;
        px = py = pz = 0.0;
        qt[0] = 0.0; 
        qt[1] = 0.0; 
        qt[2] = 0.0; 
        qt[3] = 1.0; //w
        
        rotation = rotation %4;
        switch(rotation){ 
            case 0 : 
                break; 
            case 1 : 
                px = 1000.0; 
                qt.setRPY(0.523599,0,0); 
                qt.normalize(); 
                break; 
            case 2 : 
                py = 1000.0; 
                qt.setRPY(0,0.523599,0); 
                qt.normalize(); 
                break; 
            case 3: 
                pz = 1000.0; 
                qt.setRPY(0,0,0.523599); 
                qt.normalize(); 
                break; 
            default : 
            break; 
        }
            
        pose.position.x = px; 
        pose.position.y = py; 
        pose.position.z = pz; 
        pose.orientation.x = qt[0]; 
        pose.orientation.y = qt[1]; 
        pose.orientation.z = qt[2]; 
        pose.orientation.w = qt[3];
        
        ROS_INFO("HD_point publish" ); 

        hd_pub.publish(pose);

        ros::spinOnce();

        loop_rate.sleep();

        rotation++; 
    }

    return 0;
}
