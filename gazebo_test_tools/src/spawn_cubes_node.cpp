#include <gazebo_test_tools/gazebo_cube_spawner.h>
#include <gazebo_test_tools/FakeObjectRecognizer.h>
#include </home/karthikm/Desktop/json.hpp>

#include <iostream>
#include <sstream>
#include <fstream>

#define DEFAULT_RECOGNIZE_OBJECT_TOPIC "/gazebo/recognize_object"

using json = nlohmann::json;

int main(int argc, char** argv) {
    ros::init(argc, argv, "gazebo_cube_spawner");

    ros::NodeHandle node; 
    gazebo_test_tools::GazeboCubeSpawner spawner(node);

    ROS_INFO("Running spawn cube once..");

    // Spawn cubes frame
    std::string frame_id="world";

    // Recognizing cubes topic
	std::string RECOGNIZE_OBJECT_TOPIC;

    node.param<std::string>("gazebo_test_tools/recognize_object_service", RECOGNIZE_OBJECT_TOPIC, DEFAULT_RECOGNIZE_OBJECT_TOPIC);
	ROS_INFO("Got recognize object service topic name: <%s>", RECOGNIZE_OBJECT_TOPIC.c_str());

    // Create service for recognizing the cubes that spawn
    ros::ServiceClient client = node.serviceClient<gazebo_test_tools::RecognizeGazeboObject>(RECOGNIZE_OBJECT_TOPIC);
    gazebo_test_tools::RecognizeGazeboObject srv;

    struct cube_spawn_parameters 
    {
        std::string name;
        float x;
        float y;
        float z;
        float dim=0.05;   
        float mass=0.05;
    } ;

    // Spawning cubes from file
    std::ifstream file("/home/karthikm/Desktop/cube_spawn_info.txt");
    std::string str; 
    cube_spawn_parameters * sp = new cube_spawn_parameters;
    while (std::getline(file, str)) 
    {
        std::stringstream iss(str);
        std::string params[4];
        int i = 0;
        int new_i = i; 
        while(iss.good() && i < 4)
        {
            iss >> params[i];
            ++i;
        }
        iss.str("");
        iss.clear();
        i = new_i;
        
        sp->name = params[0];
        sp->x = stof(params[1]);
        sp->y = stof(params[2]);
        sp->z = stof(params[3]); 

        // Spawn

        spawner.spawnCube(sp->name, frame_id, sp->x, sp->y, sp->z, 0, 0, 0, 1,
        sp->dim, sp->dim, sp->dim, sp->mass);
        
        // Recognize
        srv.request.name = sp->name;
        srv.request.republish = 1;  // Keep publishing the states
    
        if (client.call(srv))
        {
            ROS_INFO("Result:");
            std::cout<<srv.response<<std::endl;
        }
        else
        {
            ROS_ERROR("Failed to call service %s",RECOGNIZE_OBJECT_TOPIC.c_str());
            return 1;
        }
    }
    
    return 0;
}