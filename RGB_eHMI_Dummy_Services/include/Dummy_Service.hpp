#ifndef DUMMY_SERVICE_HPP
#define DUMMY_SERVICE_HPP

#include <asoa/core/runtime.hpp>
#include <asoa/driver/rtps.h>
#include <asoa/core/task.hpp>

//Include Interface Definitions
#include <idl/AlPedCr_sts.hpp>
#include <idl/APass_sts.hpp>
#include <idl/load_sts.hpp>
#include <idl/LPRL_sts.hpp>
#include <idl/LPRR_sts.hpp>
#include <idl/RPL_sts.hpp>
#include <idl/RPR_sts.hpp>
#include <idl/SafStop_sts.hpp>
#include <idl/TurLeft_sts.hpp>
#include <idl/TurnRig_sts.hpp>

//Include RPC Interface Definitions
#include <idl/sCuLoad_sts.hpp>
#include <idl/sLPRL_sts.hpp>
#include <idl/sLPRR_sts.hpp>
#include <idl/sRPL_sts.hpp>
#include <idl/sRPR_sts.hpp>
#include <idl/sSafSto_sts.hpp>
#include <idl/sTurLef_sts.hpp>
#include <idl/sTurRig_sts.hpp>

#include <string.h>

#include <server_socket.hpp>
#include <RGB_Status.hpp>
//#include "RGB_Matrix_Controller.hpp" //auskommentieren weil Dummy-Service auf VM läuft

class DummyService : public Service, SimpleMessageSubscriber {
//Functions for Threads
private:
  void checkPedCrossDistance(){
    std::cout << "DummyService -> GUI Thread PedCrossDistance" << std::endl;
    while(true){
      if(DummyService::GUI_time_distance_pedestrian > -0.5){
        while(DummyService::GUI_time_distance_pedestrian > 0.0){
            DummyService::GUI_time_distance_pedestrian -= 0.05;
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
            std::cout << "[Zebrastreifen] Fußgänger-Distanz (s): " << DummyService::GUI_time_distance_pedestrian << "\n" << std::endl;
          }
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(5));
      //std::cout << "[PedCrossDist] Thread loops one more time" << "\n" << std::endl;
    }
  }

private:
  void checkAllowPassDistance(){
    std::cout << "DummyService -> GUI Thread AllowPassDistance" << std::endl;
    while(true){
      while(DummyService::GUI_distance_passing_vehicle > -0.5){
          DummyService::GUI_distance_passing_vehicle -= 0.25;
          std::this_thread::sleep_for(std::chrono::milliseconds(25));
          std::cout << "[Engstelle] Fahrzeug-Distanz: " << DummyService::GUI_distance_passing_vehicle << "\n" << std::endl;
        }


      std::this_thread::sleep_for(std::chrono::milliseconds(5));
      //std::cout << "[Engstelle] Thread loops one more time" << std::endl;
    }
  }

  private:
    void checkTurningRightPlannedIn(){
      std::cout << "DummyService -> GUI Thread TurningRightPlannedIn" << std::endl;
      while(true){
        while(DummyService::GUI_turning_right_dynamic_module_planned_t > -0.5){
            DummyService::GUI_turning_right_dynamic_module_planned_t -= 0.05;
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
            std::cout << "Wende rechts geplant in (s): " << DummyService::GUI_turning_right_dynamic_module_planned_t << "\n" << std::endl;
          }


        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        //std::cout << "[TurningRight] Thread loops one more time" << std::endl;
      }
    }

    private:
      void checkTurningLeftPlannedIn(){
        std::cout << "DummyService -> GUI Thread checkTurningLeftPlannedIn" << std::endl;
        while(true){
          while(DummyService::GUI_turning_left_dynamic_module_planned_t > -0.5){
              DummyService::GUI_turning_left_dynamic_module_planned_t -= 0.05;
              std::this_thread::sleep_for(std::chrono::milliseconds(25));
              std::cout << "Wende links geplant in (s): " << DummyService::GUI_turning_left_dynamic_module_planned_t << "\n" << std::endl;
            }


          std::this_thread::sleep_for(std::chrono::milliseconds(5));
          //std::cout << "[TurningLeft] Thread loops one more time" << std::endl;
        }
      }

private:
    static ServerSocket serverSocket;
    static RGBStatus RGB_LED_Status;

    public:
         //static class variables MUST be initialized below; GUI_variables set by GUI -> they control the PeriodicTasks
        static bool GUI_currently_loading;
        static bool GUI_allow_pedestrian_crossing;
        //static float GUI_allow_pedestrian_crossing_distance; //zu viel
        static float GUI_time_distance_pedestrian;
        static float GUI_distance_pedestrian;

        static bool GUI_allow_passing;
        //static float GUI_allow_passing_distance; //zu viel
        static float GUI_distance_passing_vehicle;
        static float GUI_distance_narrowing;

        static bool GUI_leaving_parking_rectangular_left;
	      static bool GUI_leaving_parking_rectangular_right;
        static bool GUI_rectangular_parking_left;
    		static bool GUI_parking_aborted;
    		static bool GUI_parking;
        static bool GUI_rectangular_parking_right;
        static bool GUI_safe_stop;
        static float GUI_turning_left_dynamic_module_planned_t;
    		static bool GUI_turning_finished;
    		static char GUI_dynamic_module_position[255];
        static float GUI_turning_right_dynamic_module_planned_t;

        // Garantien (total: 10)
        static class : public Guarantee<AlPedCr_sts>{
        } guarantee_ped_cross;

        static class : public Guarantee<APass_sts>{
        } guarantee_allow_pass;

        static class : public Guarantee<load_sts>{
        } guarantee_loading_status;

        static class : public Guarantee<LPRL_sts>{
        } guarantee_start_left;

        static class : public Guarantee<LPRR_sts>{
        } guarantee_start_right;

        static class : public Guarantee<RPL_sts>{
        } guarantee_stop_left;

        static class : public Guarantee<RPR_sts>{
        } guarantee_stop_right;

        static class : public Guarantee<SafStop_sts>{
        } guarantee_warning_sign;

        static class : public Guarantee<TurLeft_sts>{
        } guarantee_turning_left;

        static class : public Guarantee<TurnRig_sts>{
        } guarantee_turning_right;

        // Periodic Tasks (total: 10)

        class : public PeriodicTask {
            public:
                void onWork(){
                    load_sts::Data data;

                    data.currently_loading = DummyService::GUI_currently_loading;

                    guarantee_loading_status.sendData(data);
                }
        } periodic_task_loading_status;

        class : public PeriodicTask {
            public:
                void onWork(){

                    AlPedCr_sts::Data data;


                    data.allow_pedestrian_crossing = DummyService::GUI_allow_pedestrian_crossing;
                		data.time_distance_pedestrian = DummyService::GUI_time_distance_pedestrian;
                		data.distance_pedestrian = DummyService::GUI_distance_pedestrian;
                    RGB_LED_Status.GUI_time_distance_pedestrian = GUI_time_distance_pedestrian;

                    guarantee_ped_cross.sendData(data);

                }
        } periodic_task_ped_cross;

        class : public PeriodicTask {
            public:
                void onWork(){

                    APass_sts::Data data;

                    data.allow_passing = DummyService::GUI_allow_passing;
                		data.distance_passing_vehicle = DummyService::GUI_distance_passing_vehicle;
                		data.distance_narrowing = DummyService::GUI_distance_narrowing;

                		data.allow_pedestrian_crossing = DummyService::GUI_allow_pedestrian_crossing;
                		data.time_distance_pedestrian = DummyService::GUI_time_distance_pedestrian;

                    RGB_LED_Status.GUI_distance_passing_vehicle = GUI_distance_passing_vehicle;

                    guarantee_allow_pass.sendData(data);

                }
        } periodic_task_allow_pass;

        class : public PeriodicTask {
            public:
                void onWork(){

                    LPRL_sts::Data data;

                    data.leaving_parking_rectangular_left = DummyService::GUI_leaving_parking_rectangular_left;

                    guarantee_start_left.sendData(data);

                }
        } periodic_task_start_left;

        class : public PeriodicTask {
            public:
                void onWork(){

                    LPRR_sts::Data data;

                    data.leaving_parking_rectangular_right = DummyService::GUI_leaving_parking_rectangular_right;

                    guarantee_start_right.sendData(data);

                }
        } periodic_task_start_right;

        class : public PeriodicTask {
            public:
                void onWork(){

                    RPL_sts::Data data;

                    data.rectangular_parking_left = DummyService::GUI_rectangular_parking_left;
                		data.parking_aborted = DummyService::GUI_parking_aborted;
                		data.parking = DummyService::GUI_parking;

                    guarantee_stop_left.sendData(data);

                }
        } periodic_task_stop_left;

        class : public PeriodicTask {
            public:
                void onWork(){

                    RPR_sts::Data data;

                    data.rectangular_parking_right = DummyService::GUI_rectangular_parking_right;
                		data.parking_aborted = DummyService::GUI_parking_aborted;
                		data.parking = DummyService::GUI_parking;

                    guarantee_stop_right.sendData(data);

                }
        } periodic_task_stop_right;

        class : public PeriodicTask {
            public:
                void onWork(){

                    SafStop_sts::Data data;

                    data.safe_stop = DummyService::GUI_safe_stop;

                    guarantee_warning_sign.sendData(data);

                }
        } periodic_task_warning_sign;

        class : public PeriodicTask {
            public:
                void onWork(){

                    TurLeft_sts::Data data;

                    data.turning_left_dynamic_module_planned_t = DummyService::GUI_turning_left_dynamic_module_planned_t;
                		data.turning_finished = DummyService::GUI_turning_finished;
                    strcpy(data.dynamic_module_position, DummyService::GUI_dynamic_module_position); //copy strings

                    RGB_LED_Status.GUI_turning_left_dynamic_module_planned_t = GUI_turning_left_dynamic_module_planned_t;

                    guarantee_turning_left.sendData(data);

                }
        } periodic_task_turning_left;

        class : public PeriodicTask {
            public:
                void onWork(){

                    TurnRig_sts::Data data;

                    data.turning_right_dynamic_module_planned_t = DummyService::GUI_turning_right_dynamic_module_planned_t;
                		data.turning_finished = DummyService::GUI_turning_finished;
                		strcpy(data.dynamic_module_position, DummyService::GUI_dynamic_module_position); //copy strings

                    RGB_LED_Status.GUI_turning_right_dynamic_module_planned_t = GUI_turning_right_dynamic_module_planned_t;

                    guarantee_turning_right.sendData(data);

                }
        } periodic_task_turning_right;

        // Periodic Tasks
        class : public PeriodicTask {
        public:
            void onWork(){ //sends actual ehmiStatus to GUI in json-format
               std::string data = RGB_LED_Status.toString(); //toString() creates String in json-format

               // Send data
               serverSocket.send(data);
            }
        } periodic_task_serverSending;


        // Konstruktor
        DummyService(const char *name) : Service(name){
          //Start Threads
          std::thread checkAllowPassDistance_Thread(&DummyService::checkAllowPassDistance, this);
          checkAllowPassDistance_Thread.detach();

          std::thread checkPedCrossDistance_Thread(&DummyService::checkPedCrossDistance, this);
          checkPedCrossDistance_Thread.detach();

          std::thread checkTurningLeftPlannedIn_Thread(&DummyService::checkTurningLeftPlannedIn, this);
          checkTurningLeftPlannedIn_Thread.detach();

          std::thread checkTurningRightPlannedIn_Thread(&DummyService::checkTurningRightPlannedIn, this);
          checkTurningRightPlannedIn_Thread.detach();

            // Garantuees (10)
            addGuarantee(&guarantee_ped_cross);
            addGuarantee(&guarantee_allow_pass);
            addGuarantee(&guarantee_loading_status);
            addGuarantee(&guarantee_start_left);
            addGuarantee(&guarantee_start_right);
            addGuarantee(&guarantee_stop_left);
            addGuarantee(&guarantee_stop_right);
            addGuarantee(&guarantee_warning_sign);
            addGuarantee(&guarantee_turning_left);
            addGuarantee(&guarantee_turning_right);

            // Periodic Tasks (10 + ServerSending)
            PeriodicTask::PeriodicTaskParam_t periodic_task_parameter_serverSending;
            periodic_task_parameter_serverSending.frequencyHz = 30;
            periodic_task_parameter_serverSending.start_ref = asoa::OS::time::getTime();
            initializeTask(&periodic_task_serverSending, periodic_task_parameter_serverSending);

            PeriodicTask::PeriodicTaskParam_t periodicTask_ped_cross_parameter;
            periodicTask_ped_cross_parameter.frequencyHz = 10;
            periodicTask_ped_cross_parameter.start_ref = asoa::OS::time::getTime();
            initializeTask(&periodic_task_ped_cross, periodicTask_ped_cross_parameter);
            taskWritesGuarantee(periodic_task_ped_cross, guarantee_ped_cross);

            PeriodicTask::PeriodicTaskParam_t periodicTask_allow_pass_parameter;
            periodicTask_allow_pass_parameter.frequencyHz = 10;
            periodicTask_allow_pass_parameter.start_ref = asoa::OS::time::getTime();
            initializeTask(&periodic_task_allow_pass, periodicTask_allow_pass_parameter);
            taskWritesGuarantee(periodic_task_allow_pass, guarantee_allow_pass);

            PeriodicTask::PeriodicTaskParam_t periodicTask_loading_status_parameter;
            periodicTask_loading_status_parameter.frequencyHz = 10;
            periodicTask_loading_status_parameter.start_ref = asoa::OS::time::getTime();
            initializeTask(&periodic_task_loading_status, periodicTask_loading_status_parameter);
            taskWritesGuarantee(periodic_task_loading_status, guarantee_loading_status);

            PeriodicTask::PeriodicTaskParam_t periodicTask_start_left_parameter;
            periodicTask_start_left_parameter.frequencyHz = 10;
            periodicTask_start_left_parameter.start_ref = asoa::OS::time::getTime();
            initializeTask(&periodic_task_start_left, periodicTask_start_left_parameter);
            taskWritesGuarantee(periodic_task_start_left, guarantee_start_left);

            PeriodicTask::PeriodicTaskParam_t periodicTask_start_right_parameter;
            periodicTask_start_right_parameter.frequencyHz = 10;
            periodicTask_start_right_parameter.start_ref = asoa::OS::time::getTime();
            initializeTask(&periodic_task_start_right, periodicTask_start_right_parameter);
            taskWritesGuarantee(periodic_task_start_right, guarantee_start_right);

            PeriodicTask::PeriodicTaskParam_t periodicTask_stop_left_parameter;
            periodicTask_stop_left_parameter.frequencyHz = 10;
            periodicTask_stop_left_parameter.start_ref = asoa::OS::time::getTime();
            initializeTask(&periodic_task_stop_left, periodicTask_stop_left_parameter);
            taskWritesGuarantee(periodic_task_stop_left, guarantee_stop_left);

            PeriodicTask::PeriodicTaskParam_t periodicTask_stop_right_parameter;
            periodicTask_stop_right_parameter.frequencyHz = 10;
            periodicTask_stop_right_parameter.start_ref = asoa::OS::time::getTime();
            initializeTask(&periodic_task_stop_right, periodicTask_stop_right_parameter);
            taskWritesGuarantee(periodic_task_stop_right, guarantee_stop_right);

            PeriodicTask::PeriodicTaskParam_t periodicTask_warning_sign_parameter;
            periodicTask_warning_sign_parameter.frequencyHz = 10;
            periodicTask_warning_sign_parameter.start_ref = asoa::OS::time::getTime();
            initializeTask(&periodic_task_warning_sign, periodicTask_warning_sign_parameter);
            taskWritesGuarantee(periodic_task_warning_sign, guarantee_warning_sign);

            PeriodicTask::PeriodicTaskParam_t periodicTask_turning_left_parameter;
            periodicTask_turning_left_parameter.frequencyHz = 10;
            periodicTask_turning_left_parameter.start_ref = asoa::OS::time::getTime();
            initializeTask(&periodic_task_turning_left, periodicTask_turning_left_parameter);
            taskWritesGuarantee(periodic_task_turning_left, guarantee_turning_left);

            PeriodicTask::PeriodicTaskParam_t periodicTask_turning_right_parameter;
            periodicTask_turning_right_parameter.frequencyHz = 10;
            periodicTask_turning_right_parameter.start_ref = asoa::OS::time::getTime();
            initializeTask(&periodic_task_turning_right, periodicTask_turning_right_parameter);
            taskWritesGuarantee(periodic_task_turning_right, guarantee_turning_right);

            // subscribe to server
            serverSocket.subscribe(this);
        }

        // method that handles received messages from the server socket
        void notify(std::string msg){
          //  std::cout << "Received message: " << msg;

            // Execute the received command
            if (msg == "FAHRZEUG_WIRD_GELADEN\n"){
                GUI_currently_loading = !GUI_currently_loading;

            } else if (msg == "AUSPARKEN_RECHTS\n"){
                GUI_leaving_parking_rectangular_right = !GUI_leaving_parking_rectangular_right;

            } else if (msg == "AUSPARKEN_LINKS\n"){
                GUI_leaving_parking_rectangular_left = !GUI_leaving_parking_rectangular_left;

            } else if (msg == "ENGSTELLE_BEIDSEITIG\n"){
              GUI_allow_passing = !GUI_allow_passing;
              // GUI_distance_narrowing; //TODO: Distanz einstellen in GUI per Hebel

            } else if (msg == "ENGSTELLE_BEIDSEITIG_DISTANZ\n"){
                GUI_distance_passing_vehicle = 120.0; //120m runterzählen

            } else if (msg == "ZEBRASTREIFEN\n"){
              GUI_allow_pedestrian_crossing = !GUI_allow_pedestrian_crossing;
              // float GUI_distance_pedestrian; //TODO: Distanz Fußgänger einstellen in GUI per Hebel

            } else if (msg == "ZEBRASTREIFEN_DISTANZ\n"){
              GUI_time_distance_pedestrian = 10.0; //10s runterzählen

            } else if ( msg == "WENDE_RECHTS_GEPLANT\n"){
              GUI_turning_right_dynamic_module_planned_t = 15.0; //15sek runterzählen

            } else if ( msg == "WENDE_LINKS_GEPLANT\n"){
              GUI_turning_left_dynamic_module_planned_t = 15.0; //15sek runterzählen

            } else if ( msg == "WENDE_BEENDET\n"){
              GUI_turning_finished = true;
              strcpy(GUI_dynamic_module_position, "driving_position");

            } else if ( msg == "EINPARKEN_FERTIG\n"){
              GUI_parking = !GUI_parking;

            } else if ( msg == "EINPARKEN_ABBRECHEN\n"){
              GUI_parking_aborted = !GUI_parking_aborted;

            } else if (msg == "EMERGENCY\n"){
                GUI_safe_stop = !GUI_safe_stop;

            } else if (msg == "WENDE_RECHTS\n"){
                GUI_turning_finished = !GUI_turning_finished;
                strcpy(GUI_dynamic_module_position, "turning_position");

            } else if (msg == "WENDE_LINKS\n"){
                GUI_turning_finished = !GUI_turning_finished;
                strcpy(GUI_dynamic_module_position, "turning_position");

            } else if (msg == "EINPARKEN_RECHTS\n"){
              GUI_rectangular_parking_right = !GUI_rectangular_parking_right;

            } else if (msg == "EINPARKEN_LINKS\n"){
              GUI_rectangular_parking_left = !GUI_rectangular_parking_left;

            }
        }

        // Destruktor
        ~DummyService() = default;

        bool onStartRequest(){
            std::cout << "Service wird jetzt gestartet. " << std::endl;
            return true;
        }

        bool onPrepareRequest(){
            std::cout << "Service wird in Zukunft gestartet. " << std::endl;
            return true;
        }

        bool onStopRequest(){
            std::cout << "Service wird jetzt gestoppt. " << std::endl;
            return true;
        }

        std::string name();
};

// Instanziierung der statischen Objekte
ServerSocket DummyService::serverSocket;
RGBStatus DummyService::RGB_LED_Status;

// Instanziierung der Garantien (10)
decltype(DummyService::guarantee_ped_cross) DummyService::guarantee_ped_cross;
decltype(DummyService::guarantee_allow_pass) DummyService::guarantee_allow_pass;
decltype(DummyService::guarantee_loading_status) DummyService::guarantee_loading_status;
decltype(DummyService::guarantee_start_left) DummyService::guarantee_start_left;
decltype(DummyService::guarantee_start_right) DummyService::guarantee_start_right;
decltype(DummyService::guarantee_stop_left) DummyService::guarantee_stop_left;
decltype(DummyService::guarantee_stop_right) DummyService::guarantee_stop_right;
decltype(DummyService::guarantee_warning_sign) DummyService::guarantee_warning_sign;
decltype(DummyService::guarantee_turning_left) DummyService::guarantee_turning_left;
decltype(DummyService::guarantee_turning_right) DummyService::guarantee_turning_right;

//Initialisierungen von Klassenvariablen außerhalb der Klasse:
char DummyService::GUI_dynamic_module_position[255];

bool DummyService::GUI_currently_loading = false;
bool DummyService::GUI_allow_pedestrian_crossing = false;
bool DummyService::GUI_allow_passing = false;
bool DummyService::GUI_leaving_parking_rectangular_left = false;
bool DummyService::GUI_leaving_parking_rectangular_right = false;
bool DummyService::GUI_rectangular_parking_left = false;
bool DummyService::GUI_parking_aborted = false;
bool DummyService::GUI_parking = false;
bool DummyService::GUI_rectangular_parking_right = false;
bool DummyService::GUI_safe_stop = false;
bool DummyService::GUI_turning_finished = true;

float DummyService::GUI_turning_right_dynamic_module_planned_t = -1.0;
float DummyService::GUI_time_distance_pedestrian = 0.0;
float DummyService::GUI_distance_pedestrian = 0.0;
float DummyService::GUI_turning_left_dynamic_module_planned_t = -1.0;
float DummyService::GUI_distance_passing_vehicle = -1.0;
float DummyService::GUI_distance_narrowing = 1000.0;

#endif
