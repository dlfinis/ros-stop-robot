#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Bool.h"
#include "actionlib_msgs/GoalID.h"

ros::Publisher twistPublisher;
ros::Publisher movePublisher;
geometry_msgs::Twist stop;
actionlib_msgs::GoalID mstop;


void cancelAllGoals()
{
    actionlib_msgs::GoalID cancel_msg;
    // CancelAll policy encoded by stamp=0, id=0
    cancel_msg.stamp = ros::Time(0,0);
    cancel_msg.id = "";
    movePublisher.publish(cancel_msg);
    	// ROS_INFO("Cancel Goal.");   	
}



void twistCallback(const geometry_msgs::Twist& twist){

		// ROS_INFO("Twist Publish.");   	
	twistPublisher.publish(twist);

}



void cbExec(const ros::TimerEvent&)
{
  ROS_INFO("Callback  triggered");

  		cancelAllGoals();
		twistCallback(stop);
	
}

int main(int argc, char **argv)
{

    /// Receives Twist messages for the base.
	ros::Subscriber baseCommandSubscriber;
	/// Publishes Odometry messages
	ros::Publisher baseOdometryPublisher;

    ros::init(argc, argv, "stop_robot");
	ros::NodeHandle n;

    ros::Rate loop_rate(20);
	/* setup input/output communication */
	
	// emergency.data = false;

	stop.linear.x = 0;
	stop.linear.y = 0;
	stop.linear.z = 0;
	stop.angular.z = 0;

    // ros::Subscriber twistSubscriber = n.subscribe("move_base/cmd_vel", 1000, &twistCallback);
    // ros::Subscriber emergencySubscriber = n.subscribe("emergency_stop", 1000, &emergencyCallback);
	twistPublisher = n.advertise<geometry_msgs::Twist>("cmd_vel",1);


	//Create move_base/cancel Subscriber
   	// ros::Subscriber move_base_cancel_sub  = nh.subscribe<actionlib_msgs::GoalID>("/move_base/cancel",1,move_base_cancel_callback);
   	movePublisher  = n.advertise<actionlib_msgs::GoalID>("/move_base/cancel",1);

   	  int count = 1;

	ROS_INFO("Init stop");  

	// cancelAllGoals();
	// twistCallback(stop);

	// /* coordination */
	while (count < 30){

		ROS_INFO("Sending messages.");

		cancelAllGoals();
		twistCallback(stop);
	
		 
		// ros::spinOnce();
		ros::spinOnce();
		loop_rate.sleep();
		++count;
		// return true;
	}

	ROS_INFO("Stop complete. --End");   
  return 0;
}

