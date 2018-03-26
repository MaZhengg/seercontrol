#include <ros/ros.h>
#include <QTcpSocket>
#include <seercontrol/SCHeadData.h>
#include <seercontrol/SCStatusTcp.h>
#include <geometry_msgs/Twist.h>


SCStatusTcp *pSCStatusTcp;

//callback function to control the robot
void controlCallback(const geometry_msgs::Twist::ConstPtr & cmd_input)
{

  double velx = 0.15*cmd_input->linear.x;
  double velz = 0.15*cmd_input->angular.z;

  //show the control input;
  ROS_INFO("linear=%f,angular=%f",velx,velz);

  //the control command to write with TCP
  QString sendData = QString("{\"vx\": %1,\"vy\":%2,\"w\":%3}").arg(velx)
                                                               .arg(0)
                                                               .arg(velz);
  //write the TCP command
  uint16_t sendCommand = 2010;
  QString snumber = " ";
  uint16_t number = snumber.toInt();
  pSCStatusTcp->writeTcpData(sendCommand,sendData,number);
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "seersim_node");
  ros::NodeHandle nh;
  ros::Subscriber sub;
  ros::Publisher pub;

  //connect the robot with TCP/IP
  QString ip = "192.168.192.5";
  quint16 port = 19205;
  pSCStatusTcp = new SCStatusTcp;
  pSCStatusTcp->connectHost(ip,port);

  //subscribe the the velocity command
  sub = nh.subscribe("/turtle1/cmd_vel",1,controlCallback);


  //ros::Rate loop_rate(10);
  ros::spin();
  return 0;
}
