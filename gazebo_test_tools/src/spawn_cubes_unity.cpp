#include <gazebo_test_tools/gazebo_cube_spawner.h>
#include <gazebo_test_tools/FakeObjectRecognizer.h>
#include <gazebo_test_tools/SpawnCubesUnity.h>

float dim = 0.05;   
float mass = 0.05;
std::string frame_id = "world";

// Used to request cubes to be spawned through a cube_spawner service provided by Jennifer Buehler's repos
bool spawn(gazebo_test_tools::SpawnCubesUnity::Request &req,
gazebo_test_tools::SpawnCubesUnity::Response &res)
{
    // Spawn the cube first
    ros::NodeHandle n;
    gazebo_test_tools::GazeboCubeSpawner spawner(n);

    spawner.spawnCube(req.name, "world", req.x, req.y, req.z, 0, 0, 0, 1, dim, dim, dim, mass);
    res.success = true;

    // Recognize the cube for MoveIt
    ros::ServiceClient client = n.serviceClient<gazebo_test_tools::RecognizeGazeboObject>("gazebo_test_tools/recognize_object");
    gazebo_test_tools::RecognizeGazeboObject srv;
    srv.request.name = req.name;
    srv.request.republish = 1;

    if (client.call(srv))
    {
        ROS_INFO("Result: ");
        std::cout<<srv.response<<std::endl;
        return true;
    }

    else
    {
        ROS_INFO("Failed to recognize object.");
        return false;
    }
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "spawn_cubes_unity_server");
    ros::NodeHandle n;
    
    // Advertise a service
    ros::ServiceServer service = n.advertiseService("spawn_cubes_unity_srv", spawn);
    ROS_INFO("Ready to spawn cubes");
    ros::spin(); 

    return 0;
}

