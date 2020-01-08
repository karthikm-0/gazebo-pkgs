#include <gazebo_test_tools/gazebo_cube_spawner.h>
#include </home/karthikm/Desktop/json.hpp>

#include <iostream>
#include <sstream>
#include <fstream>

using json = nlohmann::json;

int main(int argc, char** argv) {
    ros::init(argc, argv, "gazebo_cube_spawner");

    ros::NodeHandle node; 
    gazebo_test_tools::GazeboCubeSpawner spawner(node);

    ROS_INFO("Running spawn cube once..");

    std::string frame_id="world";

    struct cube_spawn_parameters {
        std::string name;
        float x;
        float y;
        float z;
        float dim=0.05;   
        float mass=0.05;
    } ;

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
        spawner.spawnCube(sp->name, frame_id, sp->x, sp->y, sp->z, 0, 0, 0, 1,
        sp->dim, sp->dim, sp->dim, sp->mass);
    }

    return 0;
}