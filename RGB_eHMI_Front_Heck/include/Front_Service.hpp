#ifndef FRONT_SERVICE_HPP
#define FRONT_SERVICE_HPP

// If not run on ARM-Powered Hardware (assuming Raspberry Pi here), we want to enter DUMMY_MODE
#ifndef __ARM_ARCH
    #define DUMMY_MODE
#endif

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

//LED-Matrix Controller helper class -> contains bild_anzeigen(string filepath) function (shows images or GIFs)
#include "RGB_Matrix_Controller.hpp"

class RGB_Front : public Service {
private:
     //static class variables MUST be initialized below
     static RGBMatrixController * rgbController; //declaration of helper class
     static bool prevDisplayStatus;
     static bool safeStopMode; //exit every state if this bool is true
     static bool otherTaskActive; //if a task is active then other tasks should not send stop-signals

     //prevStatus variables
     static bool prevLoadingStatus;
     static bool prevStartRightStatus;
     static bool prevStartLeftStatus;
     static bool prevAllowPassStatus;
     static bool prevPedCrossStatus;
     static bool prevTurningRightStatus;
     static bool prevTurningLeftStatus;
     static bool prevStopLeftStatus;
     static bool prevStopRightStatus;
     static bool prevSafeStopStatus;

    public:
        // Requirements (total: 10)
      class : public Requirement<load_sts, 5>{
            bool parameterFilter(const load_sts::Parameter *param){
                return true;
            }
        } requirement_loading_status; //Fahrzeug wird geladen

      class : public Requirement<LPRR_sts, 5>{
            bool parameterFilter(const LPRR_sts::Parameter *param){
                return true;
            }
        } requirement_start_right; //Ausparken/Anfahren geplant rechts (90°)

      class : public Requirement<LPRL_sts, 5>{
            bool parameterFilter(const LPRL_sts::Parameter *param){
                return true;
            }
        } requirement_start_left; //Ausparken/Anfahren geplant links (90°)

      class : public Requirement<APass_sts, 5>{
            bool parameterFilter(const APass_sts::Parameter *param){
                return true;
            }
        } requirement_allow_pass; //Engstelle beidsitig - Vorfahrt wird gewährt

      class : public Requirement<AlPedCr_sts, 5>{
            bool parameterFilter(const AlPedCr_sts::Parameter *param){
                return true;
            }
        } requirement_ped_cross; //Fußgänger an Zebrastreifen

      class : public Requirement<TurnRig_sts, 5>{
            bool parameterFilter(const TurnRig_sts::Parameter *param){
                return true;
            }
        } requirement_turning_right; //Wendemanöver auf Punkt (rechts)

      class : public Requirement<TurLeft_sts, 5>{
            bool parameterFilter(const TurLeft_sts::Parameter *param){
                return true;
            }
        } requirement_turning_left; //Wendemanöver auf Punkt (links)

      class : public Requirement<RPL_sts, 5>{
            bool parameterFilter(const RPL_sts::Parameter *param){
                return true;
            }
        } requirement_stop_left; //Einparken/Anfahren geplant links (90°)

       class : public Requirement<RPR_sts, 5>{
            bool parameterFilter(const RPR_sts::Parameter *param){
                return true;
              }
        } requirement_stop_right; //Einparken/Anfahren geplant rechts (90°)

      class : public Requirement<SafStop_sts, 5>{
            bool parameterFilter(const SafStop_sts::Parameter *param){
                return true;
              }
        } requirement_warning_sign; //sicheres Anhalten aktiviert


      // RPC Requirements (8 states for RPI in the back) -> static, so they are visible in onWork()
      static class : public RPCRequirement<sCuLoad_sts>{} rpc_sCuLoad_sts;
      static class : public RPCRequirement<sLPRL_sts>{} rpc_sLPRL_sts;
      static class : public RPCRequirement<sLPRR_sts>{} rpc_sLPRR_sts;
      static class : public RPCRequirement<sRPL_sts>{} rpc_sRPL_sts;
      static class : public RPCRequirement<sRPR_sts>{} rpc_sRPR_sts;
      static class : public RPCRequirement<sSafSto_sts>{} rpc_sSafSto_sts;
      static class : public RPCRequirement<sTurLef_sts>{} rpc_sTurLef_sts;
      static class : public RPCRequirement<sTurRig_sts>{} rpc_sTurRig_sts;

        //loading_status
        class : public ConditionalTask {
            public:
                Requirement<load_sts, 5>::AccessHandle* access_handle_loading_status;

                void onWork(){
                    load_sts::Data data;
                    access_handle_loading_status->pullData(data);

                    //Define filepath as string -> use stream-Files for faster loading
                    std::string loading_status_path = "../eHMI_symbols/eHMI_streams/AkkuLadenStream.stream";

                    sCuLoad_sts::Parameter param;
                    sCuLoad_sts::Return ret;

                    if(!safeStopMode){
                      if(data.currently_loading){

                        // if(!prevLoadingStatus){
                        //   std::cout << "COND_IF: DISPLAY SET TO ACTIVE (now waiting for thread) \n" << std::endl;
                        // }

                        prevLoadingStatus = true;
                        prevDisplayStatus = true;
                        rgbController->setFilepath(loading_status_path);
                        rgbController->setIsDisplayActive(true);

                        //Immediate RPC-Call to eHMI-Back-Service -> set to true to start behind matrix
                        param.set_currently_loading = true;
                        rpc_sCuLoad_sts.call(param, ret);

                      } else{

                        if(prevDisplayStatus == true && prevLoadingStatus == true){
                          rgbController->setIsDisplayActive(false);
                          std::cout << "Stop-Signal sent" << std::endl;
                          prevDisplayStatus = false;
                          prevLoadingStatus = false;
                          raise(SIGINT); //stop Matrix in RGB_Thread

                          //Immediate RPC-Call to eHMI-Back-Service -> unter if() verschoben
                          param.set_currently_loading = false;
                          rpc_sCuLoad_sts.call(param, ret);

                        }
                      }
                  }
                }
              } conditional_task_loading_status;


        //start_right
        class : public ConditionalTask {
            public:
                Requirement<LPRR_sts, 5>::AccessHandle* access_handle_start_right;

                void onWork(){
                    LPRR_sts::Data data;
                    access_handle_start_right->pullData(data);
                //Functionality
                //Eingang: leaving_parking_rectangular_right=True
                //entry: start_right.png anzeigen vorne/hinten
                //exit: deactivate all matrices
                //exit1: parking= True or leaving_parking_rectangular_right=False
                //exit1: automatic_driving=True && leaving_parking_rectangular_right=False && leaving_parking_rectangular_left=false


                std::string start_right_path = "../eHMI_symbols/eHMI_streams/ParkenLinksStream.stream";

                sLPRR_sts::Parameter param;
                sLPRR_sts::Return ret;

                if(!safeStopMode){
                  if(data.leaving_parking_rectangular_right == true){

                    prevStartRightStatus = true;
                    prevDisplayStatus = true;
                    rgbController->setFilepath(start_right_path);
                    rgbController->setIsDisplayActive(true);


                    //Immediate RPC-Call to eHMI-Back-Service -> set to true to start behind matrix
                    param.set_leaving_parking_rectangular_right = true;
                    rpc_sLPRR_sts.call(param, ret);

                  } else{

                    if(prevDisplayStatus == true && prevStartRightStatus == true){
                      rgbController->setIsDisplayActive(false);
                      std::cout << "Stop-Signal sent" << std::endl;
                      prevStartRightStatus = false;
                      prevDisplayStatus = false;
                      raise(SIGINT); //stop Matrix in RGB_Thread

                      //Immediate RPC-Call to eHMI-Back-Service -> set to false to stop behind matrix
                      param.set_leaving_parking_rectangular_right = false;
                      rpc_sLPRR_sts.call(param, ret);
                    }
                  }
                }
              }
        } conditional_task_start_right;

        //start_left
        class : public ConditionalTask {
            public:
                Requirement<LPRL_sts, 5>::AccessHandle* access_handle_start_left;

                void onWork(){
                    LPRL_sts::Data data;
                    access_handle_start_left->pullData(data);
                    //Functionality
                    //Eingang: leaving_parking_rectangular_left=True
                    //entry: start_left.png anzeigen vorne/hinten
                    //exit: deactivate all matrices
                    //exit1: parking= True or leaving_parking_rectangular_left=False
                    //exit1: automatic_driving=True && leaving_parking_rectangular_right=False && leaving_parking_rectangular_left=false

                    std::string start_left_path = "../eHMI_symbols/eHMI_streams/ParkenRechtsStream.stream";

                    sLPRL_sts::Parameter param;
                    sLPRL_sts::Return ret;

                    if(!safeStopMode){
                      if(data.leaving_parking_rectangular_left == true){
                        prevStartLeftStatus = true;
                        rgbController->setFilepath(start_left_path);
                        rgbController->setIsDisplayActive(true);
                        prevDisplayStatus = true;

                        //Immediate RPC-Call to eHMI-Back-Service -> set to true to start behind matrix
                        param.set_leaving_parking_rect_left_status = true;
                        rpc_sLPRL_sts.call(param, ret);

                      } else{

                        if(prevDisplayStatus == true && prevStartLeftStatus == true){
                          rgbController->setIsDisplayActive(false);
                          std::cout << "Stop-Signal sent" << std::endl;
                          prevDisplayStatus = false;
                          prevStartLeftStatus = false;
                          raise(SIGINT); //stop Matrix in RGB_Thread

                          //Immediate RPC-Call to eHMI-Back-Service -> set to false to stop behind matrix
                          param.set_leaving_parking_rect_left_status = false;
                          rpc_sLPRL_sts.call(param, ret);
                        }
                      }
                    }
                }
        } conditional_task_start_left;

        //allow_pass
        class : public ConditionalTask {
            public:
                Requirement<APass_sts, 5>::AccessHandle* access_handle_allow_pass;

                void onWork(){
                    APass_sts::Data data;
                    access_handle_allow_pass->pullData(data);
                    //Functionality
                    //Eingang: allow_passing=True && distance_passing_vehicle <70m
                    //entry: allow_pass.png anzeigen vorne
                    //exit: deactivate all matrices
                    //exit1: allow_passing=False or distance_narrowing < 0
                    //exit1: allow_pedestrian_crossing = True && time_distance_pedestrian <5s

                    std::string allow_pass_path = "../eHMI_symbols/eHMI_streams/EngstelleStream.stream";

                    if(!safeStopMode && !prevPedCrossStatus){ // WECHSEL IN DEN ZUSTAND FUSSGÄNGER AN ZEBRASTREIFEN BERÜCKSICHTIGT
                      if(data.allow_passing == true && (data.distance_passing_vehicle < 70) && (data.distance_passing_vehicle >= 0) && (data.distance_narrowing >= 0)){

                        prevDisplayStatus = true;
                        prevAllowPassStatus = true;
                        rgbController->setFilepath(allow_pass_path);
                        rgbController->setIsDisplayActive(true);
                      }
                      else{

                      if(prevDisplayStatus == true && prevAllowPassStatus == true){
                        rgbController->setIsDisplayActive(false);
                        std::cout << "Stop-Signal sent" << std::endl;
                        prevDisplayStatus = false;
                        prevAllowPassStatus = false;
                        raise(SIGINT); //stop Matrix in RGB_Thread

                        //NO RPC Calls for that state
                    }
                  }
              }
        }
      } conditional_task_allow_pass;

        //ped_cross
        class : public ConditionalTask {
            public:
                Requirement<AlPedCr_sts, 5>::AccessHandle* access_handle_ped_cross;

                void onWork(){
                    AlPedCr_sts::Data data;
                    access_handle_ped_cross->pullData(data);
                    //Functionality
                    //Eingang: allow_pedestrian_crossing = True && time_distance_pedestrian < 5s
                    //Eingang2: allow_pedestrian_crossing = True && time_distance_pedestrian <5s (VON "Vorfahrt gewähren"-Zustand)
                    //entry: ped_cross.png anzeigen vorne
                    //exit: deactivate all matrices
                    //exit1: allow_pedestrian_crossing = False or distance_pedestrian <0m

                    std::string ped_cross_path = "../eHMI_symbols/eHMI_streams/ZebrastreifenStream.stream";

                    if(!safeStopMode){
                      //Multiple conditions and state transitions here
                      if(data.allow_pedestrian_crossing == true && data.time_distance_pedestrian < 5 && data.time_distance_pedestrian > 0){

                        //Zustandsübergang von "Engstelle" zu "Zebrastreifen"
                        if(prevPedCrossStatus == false && prevAllowPassStatus == true){
                          raise(SIGINT); //stop Matrix in RGB_Thread only once
                        }

                        prevPedCrossStatus = true;
                        prevDisplayStatus = true;

                        rgbController->setFilepath(ped_cross_path);
                        rgbController->setIsDisplayActive(true);

                      }
                      else { //in allen anderen Fällen soll das Display aus sein

                        if(prevDisplayStatus == true && prevPedCrossStatus == true){
                          rgbController->setIsDisplayActive(false);
                          std::cout << "Stop-Signal sent" << std::endl;
                          prevDisplayStatus = false;
                          prevPedCrossStatus = false;
                          raise(SIGINT); //stop Matrix in RGB_Thread

                          //NO RPC Calls for that state
                        }
                      }
                    }
                }
        } conditional_task_ped_cross;

        //turning_right
        class : public ConditionalTask {
            public:
                Requirement<TurnRig_sts, 5>::AccessHandle* access_handle_turning_right;

                void onWork(){
                    TurnRig_sts::Data data;
                    access_handle_turning_right->pullData(data);
                    //Functionality
                    //Eingang: turning_right_dynamic_module_planned_t<5s
                    //entry: turning_right.png anzeigen vorne
                    //entry2: turning_right.png anzeigen hinten (hintere Version)
                    //exit: deactivate all matrices
                    //exit2: null

                    std::string turning_right_path = "../eHMI_symbols/eHMI_streams/WendemanöverGegenUZSStream.stream"; //für Back-Service dann mit Uhrzeigersinn (UZS)!

                    sTurRig_sts::Parameter param;
                    sTurRig_sts::Return ret;

                    if(!safeStopMode){
                      if((!data.turning_finished) && (data.turning_right_dynamic_module_planned_t < 5) && data.turning_right_dynamic_module_planned_t > 0){
                        prevTurningRightStatus = true;
                        prevDisplayStatus = true;
                        rgbController->setFilepath(turning_right_path);
                        rgbController->setIsDisplayActive(true);

                        //Immediate RPC-Call to eHMI-Back-Service -> set to true to start behind matrix
                        param.set_turning_right_status = true;
                        rpc_sTurRig_sts.call(param, ret);

                      } else if (data.turning_finished && (strcmp(data.dynamic_module_position, "driving_position") == 0)){



                        if(prevDisplayStatus == true && prevTurningRightStatus == true){
                          rgbController->setIsDisplayActive(false);
                          std::cout << "Stop-Signal sent" << std::endl;
                          prevDisplayStatus = false;
                          prevTurningRightStatus = false;
                          raise(SIGINT); //stop Matrix in RGB_Thread

                          //Immediate RPC-Call to eHMI-Back-Service -> set to false to stop behind matrix
                          param.set_turning_right_status = false;
                          rpc_sTurRig_sts.call(param, ret);
                        }
                      }
                      else{
                        if(prevDisplayStatus == true && prevTurningRightStatus == true){
                          rgbController->setIsDisplayActive(false);
                          std::cout << "Stop-Signal sent" << std::endl;
                          prevDisplayStatus = false;
                          prevTurningRightStatus = false;
                          raise(SIGINT); //stop Matrix in RGB_Thread

                          //Immediate RPC-Call to eHMI-Back-Service -> set to false to stop behind matrix
                          param.set_turning_right_status = false;
                          rpc_sTurRig_sts.call(param, ret);
                      }
                    }
                }
        }
      } conditional_task_turning_right;

        //turning_left
        class : public ConditionalTask {
            public:
                Requirement<TurLeft_sts, 5>::AccessHandle* access_handle_turning_left;

                void onWork(){
                    TurLeft_sts::Data data;
                    access_handle_turning_left->pullData(data);
                    //Functionality
                    //Eingang: turning_left_dynamic_module_planned_t<5s
                    //entry: turning_left.png anzeigen vorne
                    //entry2: turning_left.png anzeigen hinten (hintere Version)
                    //exit: deactivate all matrices
                    //Ausgang: null

                    std::string turning_left_path = "../eHMI_symbols/eHMI_streams/WendemanöverMitUZSStream.stream"; //für Back-Service dann gegen Uhrzeigersinn (UZS)!

                    sTurLef_sts::Parameter param;
                    sTurLef_sts::Return ret;

                    if(!safeStopMode){
                      if((!data.turning_finished) && (data.turning_left_dynamic_module_planned_t < 5) && (data.turning_left_dynamic_module_planned_t > 0)){
                        prevTurningLeftStatus = true;
                        prevDisplayStatus = true;
                        rgbController->setFilepath(turning_left_path);
                        rgbController->setIsDisplayActive(true);

                        //Immediate RPC-Call to eHMI-Back-Service -> set to true to start behind matrix
                        param.set_turning_left_status = true;
                        rpc_sTurLef_sts.call(param, ret);

                      } else if (data.turning_finished && (strcmp(data.dynamic_module_position, "driving_position") == 0)){

                        if(prevDisplayStatus == true && prevTurningLeftStatus == true){
                          rgbController->setIsDisplayActive(false);
                          std::cout << "Stop-Signal sent" << std::endl;
                          prevDisplayStatus = false;
                          prevTurningLeftStatus = false;
                          raise(SIGINT); //stop Matrix in RGB_Thread

                          //Immediate RPC-Call to eHMI-Back-Service -> set to false to stop behind matrix
                          param.set_turning_left_status = false;
                          rpc_sTurLef_sts.call(param, ret);
                        }
                      }
                      else{
                        if(prevDisplayStatus == true && prevTurningLeftStatus == true){
                          rgbController->setIsDisplayActive(false);
                          std::cout << "Stop-Signal sent" << std::endl;
                          prevDisplayStatus = false;
                          prevTurningLeftStatus = false;
                          raise(SIGINT); //stop Matrix in RGB_Thread

                          //Immediate RPC-Call to eHMI-Back-Service -> set to false to stop behind matrix
                          param.set_turning_left_status = false;
                          rpc_sTurLef_sts.call(param, ret);
                        }
                      }
                    }
                }
        } conditional_task_turning_left;

        //stop_left
        class : public ConditionalTask {
            public:
                Requirement<RPL_sts, 5>::AccessHandle* access_handle_stop_left;

                void onWork(){
                    RPL_sts::Data data;
                    access_handle_stop_left->pullData(data);
                    //Functionality
                    //Eingang: rectangular_parking_left=True
                    //entry: stop_left.png anzeigen vorne
                    //entry2: stop_left.png anzeigen hinten (hintere Version)
                    //exit: deactivate all matrices
                    //Ausgang: null

                    std::string stop_left_path = "../eHMI_symbols/eHMI_streams/ParkenRechtsStream.stream"; //im Back-Service dann Parken_links

                    sRPL_sts::Parameter param;
                    sRPL_sts::Return ret;
                    if(!safeStopMode){
                      if(data.rectangular_parking_left && !data.parking_aborted && !data.parking){
                        prevStopLeftStatus = true;
                        prevDisplayStatus = true;
                        rgbController->setFilepath(stop_left_path);
                        rgbController->setIsDisplayActive(true);

                        //Immediate RPC-Call to eHMI-Back-Service -> set to true to start behind matrix
                        param.set_rectangular_parking_left = true;
                        rpc_sRPL_sts.call(param, ret);

                      } else{
                        if(prevDisplayStatus == true && prevStopLeftStatus == true){
                          rgbController->setIsDisplayActive(false);
                          std::cout << "Stop-Signal sent" << std::endl;
                          prevDisplayStatus = false;
                          prevStopLeftStatus = false;
                          raise(SIGINT); //stop Matrix in RGB_Thread

                          //Immediate RPC-Call to eHMI-Back-Service -> set to false to stop behind matrix
                          param.set_rectangular_parking_left = false;
                          rpc_sRPL_sts.call(param, ret);
                        }
                      }
                    }
                }
        } conditional_task_stop_left;

        //stop_right
        class : public ConditionalTask {
            public:
                Requirement<RPR_sts, 5>::AccessHandle* access_handle_stop_right;

                void onWork(){
                    RPR_sts::Data data;
                    access_handle_stop_right->pullData(data);
                    //Functionality
                    //Eingang: rectangular_parking_right=True
                    //entry: stop_right.png anzeigen vorne
                    //entry2: stop_right.png anzeigen hinten (hintere Version)
                    //exit: deactivate all matrices
                    //Ausgang: null

                    std::string stop_right_path = "../eHMI_symbols/eHMI_streams/ParkenLinksStream.stream"; //im Back-Service dann Parken_rechts

                    sRPR_sts::Parameter param;
                    sRPR_sts::Return ret;
                    if(!safeStopMode){
                      if(data.rectangular_parking_right && !data.parking_aborted && !data.parking){
                        prevStopRightStatus = true;
                        prevDisplayStatus = true;
                        rgbController->setFilepath(stop_right_path);
                        rgbController->setIsDisplayActive(true);

                        //Immediate RPC-Call to eHMI-Back-Service -> set to true to start behind matrix
                        param.set_rectangular_parking_right = true;
                        rpc_sRPR_sts.call(param, ret);

                      } else{
                        if(prevDisplayStatus == true && prevStopRightStatus == true){
                          rgbController->setIsDisplayActive(false);
                          std::cout << "Stop-Signal sent" << std::endl;
                          prevDisplayStatus = false;
                          prevStopRightStatus = false;
                          raise(SIGINT); //stop Matrix in RGB_Thread

                          //Immediate RPC-Call to eHMI-Back-Service -> set to false to stop behind matrix
                          param.set_rectangular_parking_right = false;
                          rpc_sRPR_sts.call(param, ret);
                        }
                      }
                    }
                }
        } conditional_task_stop_right;

        //warning_sign
        class : public ConditionalTask {
            public:
                Requirement<SafStop_sts, 5>::AccessHandle* access_handle_warning_sign;

                void onWork(){
                    SafStop_sts::Data data;
                    access_handle_warning_sign->pullData(data);
                    //Functionality
                    //Eingang: safe_stop=True
                    //entry: deactivale all matrices
                    //entry2: warning_sign.png anzeigen vorne/hinten
                    //exit: null
                    //Ausgang: null

                    std::string warning_sign_path = "../eHMI_symbols/eHMI_streams/AusrufezeichenWarnungStream.stream";

                    sSafSto_sts::Parameter param;
                    sSafSto_sts::Return ret;

                    if(data.safe_stop == true){
                      if(prevSafeStopStatus == false){
                        raise(SIGINT); //stop Matrix in RGB_Thread only once
                      }

                      safeStopMode = true; //activating safe stop in class -> this will stop every active state
                      prevSafeStopStatus = true;
                      prevDisplayStatus = true;

                      rgbController->setFilepath(warning_sign_path);
                      rgbController->setIsDisplayActive(true);

                      //Immediate RPC-Call to eHMI-Back-Service -> set to true to start behind matrix
                      param.set_safe_stop_status = true;
                      rpc_sSafSto_sts.call(param, ret);

                    } else{

                      safeStopMode = false; //exit safe stop mode

                      if(prevDisplayStatus == true && prevSafeStopStatus == true){
                        rgbController->setIsDisplayActive(false);
                        std::cout << "Stop-Signal sent" << std::endl;
                        prevDisplayStatus = false;
                        prevSafeStopStatus = false;
                        raise(SIGINT); //stop Matrix in RGB_Thread

                        //Immediate RPC-Call to eHMI-Back-Service -> set to false to stop behind matrix
                        param.set_safe_stop_status = false;
                        rpc_sSafSto_sts.call(param, ret);
                      }
                    }
                }
        } conditional_task_warning_sign;

        // Konstruktor
        RGB_Front(const char *name) : Service(name){
            // Requirements (total: 10)
            addRequirement(&requirement_loading_status);
            addRequirement(&requirement_start_right);
            addRequirement(&requirement_start_left);
            addRequirement(&requirement_allow_pass);
            addRequirement(&requirement_ped_cross);
            addRequirement(&requirement_turning_right);
            addRequirement(&requirement_turning_left);
            addRequirement(&requirement_stop_left);
            addRequirement(&requirement_stop_right);
            addRequirement(&requirement_warning_sign);

            // RPC Requirements (total: 8)
            addRPCRequirement(&rpc_sCuLoad_sts);
            addRPCRequirement(&rpc_sLPRL_sts);
            addRPCRequirement(&rpc_sLPRR_sts);
            addRPCRequirement(&rpc_sRPL_sts);
            addRPCRequirement(&rpc_sRPR_sts);
            addRPCRequirement(&rpc_sSafSto_sts);
            addRPCRequirement(&rpc_sTurLef_sts);
            addRPCRequirement(&rpc_sTurRig_sts);

            // Conditional Tasks are initialized below:
            // conditional_task_loading_status
            ConditionalTask::ConditionalTaskParam_t conditionalTask_loading_status_parameter;
            initializeTask(&conditional_task_loading_status, conditionalTask_loading_status_parameter);
            taskReadsFromRequirement(conditional_task_loading_status, requirement_loading_status, &conditional_task_loading_status.access_handle_loading_status);
            taskAddDataTrigger(conditional_task_loading_status, requirement_loading_status, conditional_task_loading_status.access_handle_loading_status);

            // conditional_task_start_right
            ConditionalTask::ConditionalTaskParam_t conditionalTask_start_right_parameter;
            initializeTask(&conditional_task_start_right, conditionalTask_start_right_parameter);
            taskReadsFromRequirement(conditional_task_start_right, requirement_start_right, &conditional_task_start_right.access_handle_start_right);
            taskAddDataTrigger(conditional_task_start_right, requirement_start_right, conditional_task_start_right.access_handle_start_right);

            // conditional_task_start_left
            ConditionalTask::ConditionalTaskParam_t conditionalTask_start_left_parameter;
            initializeTask(&conditional_task_start_left, conditionalTask_start_left_parameter);
            taskReadsFromRequirement(conditional_task_start_left, requirement_start_left, &conditional_task_start_left.access_handle_start_left);
            taskAddDataTrigger(conditional_task_start_left, requirement_start_left, conditional_task_start_left.access_handle_start_left);

            // conditional_task_allow_pass
            ConditionalTask::ConditionalTaskParam_t conditionalTask_allow_pass_parameter;
            initializeTask(&conditional_task_allow_pass, conditionalTask_allow_pass_parameter);
            taskReadsFromRequirement(conditional_task_allow_pass, requirement_allow_pass, &conditional_task_allow_pass.access_handle_allow_pass);
            taskAddDataTrigger(conditional_task_allow_pass, requirement_allow_pass, conditional_task_allow_pass.access_handle_allow_pass);

            // conditional_task_ped_cross
            ConditionalTask::ConditionalTaskParam_t conditionalTask_ped_cross_parameter;
            initializeTask(&conditional_task_ped_cross, conditionalTask_ped_cross_parameter);
            taskReadsFromRequirement(conditional_task_ped_cross, requirement_ped_cross, &conditional_task_ped_cross.access_handle_ped_cross);
            taskAddDataTrigger(conditional_task_ped_cross, requirement_ped_cross, conditional_task_ped_cross.access_handle_ped_cross);

            // conditional_task_turning_right
            ConditionalTask::ConditionalTaskParam_t conditionalTask_turning_right_parameter;
            initializeTask(&conditional_task_turning_right, conditionalTask_turning_right_parameter);
            taskReadsFromRequirement(conditional_task_turning_right, requirement_turning_right, &conditional_task_turning_right.access_handle_turning_right);
            taskAddDataTrigger(conditional_task_turning_right, requirement_turning_right, conditional_task_turning_right.access_handle_turning_right);

            // conditional_task_turning_left
            ConditionalTask::ConditionalTaskParam_t conditionalTask_turning_left_parameter;
            initializeTask(&conditional_task_turning_left, conditionalTask_turning_left_parameter);
            taskReadsFromRequirement(conditional_task_turning_left, requirement_turning_left, &conditional_task_turning_left.access_handle_turning_left);
            taskAddDataTrigger(conditional_task_turning_left, requirement_turning_left, conditional_task_turning_left.access_handle_turning_left);

            // conditional_task_stop_left
            ConditionalTask::ConditionalTaskParam_t conditionalTask_stop_left_parameter;
            initializeTask(&conditional_task_stop_left, conditionalTask_stop_left_parameter);
            taskReadsFromRequirement(conditional_task_stop_left, requirement_stop_left, &conditional_task_stop_left.access_handle_stop_left);
            taskAddDataTrigger(conditional_task_stop_left, requirement_stop_left, conditional_task_stop_left.access_handle_stop_left);

            // conditional_task_stop_right
            ConditionalTask::ConditionalTaskParam_t conditionalTask_stop_right_parameter;
            initializeTask(&conditional_task_stop_right, conditionalTask_stop_right_parameter);
            taskReadsFromRequirement(conditional_task_stop_right, requirement_stop_right, &conditional_task_stop_right.access_handle_stop_right);
            taskAddDataTrigger(conditional_task_stop_right, requirement_stop_right, conditional_task_stop_right.access_handle_stop_right);

            // conditional_task_warning_sign
            ConditionalTask::ConditionalTaskParam_t conditionalTask_warning_sign_parameter;
            initializeTask(&conditional_task_warning_sign, conditionalTask_warning_sign_parameter);
            taskReadsFromRequirement(conditional_task_warning_sign, requirement_warning_sign, &conditional_task_warning_sign.access_handle_warning_sign);
            taskAddDataTrigger(conditional_task_warning_sign, requirement_warning_sign, conditional_task_warning_sign.access_handle_warning_sign);

        }

        // Destruktor
        ~RGB_Front() = default;

        bool onStartRequest(){
            std::cout << "Service wird jetzt gestartet. " << std::endl;
            return true;
        }

        bool onPrepareRequest(){
            std::cout << "Service wird in Zukunft gestartet. " << std::endl;
            return true; //
        }

        bool onStopRequest(){
            std::cout << "Service wird jetzt gestoppt. " << std::endl;
            return true;
        }

        std::string name();
};

#ifdef DUMMY_MODE
    RGBMatrixController * RGB_Front::rgbController = new DummyRGBController(); //Pointer* ist wichtig -> nur Textausgaben
#else
    RGBMatrixController * RGB_Front::rgbController = new hzellerController(); //Pointer ist wichtig -> richtige Bild/GIF-Anzeige
#endif

//Initialisierungen und Instanziierungen statischer Variablen/Objekte AUSSERHALB der Klasse!
//RGBMatrixController * RGB_Front::rgbController;
bool RGB_Front::prevDisplayStatus = false;
bool RGB_Front::safeStopMode = false;
bool RGB_Front::otherTaskActive = false;

//status variables
bool RGB_Front::prevLoadingStatus = false;
bool RGB_Front::prevStartRightStatus = false;
bool RGB_Front::prevStartLeftStatus = false;
bool RGB_Front::prevAllowPassStatus = false;
bool RGB_Front::prevPedCrossStatus = false;
bool RGB_Front::prevTurningRightStatus = false;
bool RGB_Front::prevTurningLeftStatus = false;
bool RGB_Front::prevStopLeftStatus = false;
bool RGB_Front::prevStopRightStatus = false;
bool RGB_Front::prevSafeStopStatus = false;

//Instanziierung der RPC-Objekte -> decltype() ist quasi der Platzhalter für den Datentyp des RPC
decltype(RGB_Front::rpc_sCuLoad_sts) RGB_Front::rpc_sCuLoad_sts;
decltype(RGB_Front::rpc_sLPRL_sts) RGB_Front::rpc_sLPRL_sts;
decltype(RGB_Front::rpc_sLPRR_sts) RGB_Front::rpc_sLPRR_sts;
decltype(RGB_Front::rpc_sRPL_sts) RGB_Front::rpc_sRPL_sts;
decltype(RGB_Front::rpc_sRPR_sts) RGB_Front::rpc_sRPR_sts;
decltype(RGB_Front::rpc_sSafSto_sts) RGB_Front::rpc_sSafSto_sts;
decltype(RGB_Front::rpc_sTurLef_sts) RGB_Front::rpc_sTurLef_sts;
decltype(RGB_Front::rpc_sTurRig_sts) RGB_Front::rpc_sTurRig_sts;


#endif
