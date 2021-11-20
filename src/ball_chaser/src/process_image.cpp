#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
 
  ROS_INFO_STREAM("Driving the robot toward the ball");

   // Request velocities [lin_x,ang_z]
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

  // Call the DriveToTarget service with the requested velocities
    if (!client.call(srv))
        ROS_ERROR("failed to call the DriveToTarget service"); 

}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;
    bool white_ball_in_camera = false;
    ROS_INFO_STREAM(white_ball_in_camera);
  

    // TODO: Loop through each pixel in the image and check if there's a bright white one

for (int i = 0 ; i < img.height*img.step; i++)
  {
      if (img.data[i] - white_pixel == 0)
      {

// Depending on the white ball position, call the drive_bot function and pass velocities to it

          {
           if (i < trunc(img.step/3)) 
           {drive_robot(0.5, 0.5);
           white_ball_in_camera = true;}
          }

          { 
           if (trunc(img.step/3) + 1 < i < trunc(img.step/3)*2) 
           {drive_robot(0.5, 0.5);
            white_ball_in_camera = true;}
           }
        
          { 
           if (trunc(img.step/3)*2 + 1 < i) 
           {drive_robot(0.5, -0.5);
            white_ball_in_camera = true;}
          }
 
       }
      
   }
  
  if (white_ball_in_camera == false)
  {drive_robot(0, 0);}   
  ROS_INFO_STREAM(white_ball_in_camera);   
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/drive_bot/DriveToTarget");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
