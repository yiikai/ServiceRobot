#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_listener.h>
uint32_t shape = visualization_msgs::Marker::CUBE;
//drop off
//x = 0.05 y = -1.83
#define PICK_UP_X -7.3
#define PICK_UP_Y -5.73
#define DROP_OFF_X 1.08
#define DROP_OFF_Y 4.55
//pick up
//x = -0.03 y = 0.99

class Marker
{
  public:
	Marker(ros::NodeHandle n){
	  m_n = n;
	  m_r = new ros::Rate(1);
	  marker_pub = m_n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
          m_s = PICK;
	}
        ~Marker(){}
        void odom_cb(const nav_msgs::Odometry::ConstPtr &message);
        void add_mark(double x, double y);
        void delete_mark();
  private:
	typedef enum{
		PICK = 0,
		DROP
	}STATUS;	
  private:
        STATUS m_s;
	ros::Rate *m_r;
	ros::NodeHandle m_n;
	ros::Publisher marker_pub;
	tf::TransformListener listener;
        visualization_msgs::Marker marker;
};

void Marker::add_mark(double x, double y)
{
	//visualization_msgs::Marker marker;
	// Set the frame ID and timestamp.  See the TF tutorials for information on these.
	marker.header.frame_id = "/map";
	marker.header.stamp = ros::Time::now();

	// Set the namespace and id for this marker.  This serves to create a unique ID
	// Any marker sent with the same namespace and id will overwrite the old one
	marker.ns = "basic_shapes";
	marker.id = 0;

	// Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
	marker.type = shape;

	// Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
	marker.action = visualization_msgs::Marker::ADD;

	// Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
	marker.pose.position.x = x;
	marker.pose.position.y = y;
	marker.pose.position.z = 0;
	marker.pose.orientation.x = 0.0;
	marker.pose.orientation.y = 0.0;
	marker.pose.orientation.z = 0.0;
	marker.pose.orientation.w = 1.0;

	// Set the scale of the marker -- 1x1x1 here means 1m on a side
	marker.scale.x = 1;
	marker.scale.y = 1;
	marker.scale.z = 1;

	// Set the color -- be sure to set alpha to something non-zero!
	marker.color.r = 0.0f;
	marker.color.g = 1.0f;
	marker.color.b = 0.0f;
	marker.color.a = 1.0;

	marker.lifetime = ros::Duration();

	// Publish the marker
	while (marker_pub.getNumSubscribers() < 1)
	{
		if (!ros::ok())
		{
			return ;
		}
		ROS_WARN_ONCE("Please create a subscriber to the marker");
		sleep(1);
	}
	marker_pub.publish(marker);

}

void Marker::delete_mark()
{
    marker.action = visualization_msgs::Marker::DELETE;
    marker_pub.publish(marker);
}

void Marker::odom_cb(const nav_msgs::Odometry::ConstPtr &message)
{
  double x = -message->pose.pose.position.x;
  double y = message->pose.pose.position.y;
  if(m_s == PICK)
  {
    if (((x < -4) && (x > -8)) && ((y < -4) && (y > -8)))
    {
       printf("delete mark!!!!!!!!!!!!!\n");
       delete_mark();
       m_s = DROP;
    }
  }
  else
  {
    printf("x : %.2f, Y: %.2f\n",x,y);
    if (((x < 3) && (x>0)) && ((y < 6) && (y > 3)))
    {
       add_mark(DROP_OFF_X,DROP_OFF_Y);
       m_s = PICK;
    }
  }
  
}

int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");

  ros::NodeHandle n;
  Marker marker(n);
  marker.add_mark(-7.3,-5.73);
  ros::Subscriber sub = n.subscribe("/odom",1000,&Marker::odom_cb,&marker);
  ros::Rate r(10.0); // 10 Hz
  while (ros::ok()) {
      ros::spinOnce();
      r.sleep();
  }
  return 0;
}
