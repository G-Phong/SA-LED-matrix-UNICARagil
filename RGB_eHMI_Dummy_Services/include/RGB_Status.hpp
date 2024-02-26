#ifndef RGB_STATUS_HPP
#define RGB_STATUS_HPP

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class RGBStatus {

public:
  int counter;

  char GUI_dynamic_module_position[255];

  float GUI_time_distance_pedestrian;
  float GUI_distance_pedestrian;
  float GUI_allow_passing_distance;
  float GUI_distance_passing_vehicle;
  float GUI_distance_narrowing;
  float GUI_turning_left_dynamic_module_planned_t;
  float GUI_turning_right_dynamic_module_planned_t;

    json toJSON(){
        json jsonDatei;
        jsonDatei["GUI_time_distance_pedestrian"] = GUI_time_distance_pedestrian;
        jsonDatei["GUI_distance_pedestrian"] = GUI_distance_pedestrian;
        jsonDatei["GUI_distance_passing_vehicle"] = GUI_distance_passing_vehicle;
        jsonDatei["GUI_distance_narrowing"] = GUI_distance_narrowing;
        jsonDatei["GUI_turning_left_dynamic_module_planned_t"] = GUI_turning_left_dynamic_module_planned_t;
        jsonDatei["GUI_turning_right_dynamic_module_planned_t"] = GUI_turning_right_dynamic_module_planned_t;

        return jsonDatei;
    }

    std::string toString(){
        return toJSON().dump();
    }
};

#endif
