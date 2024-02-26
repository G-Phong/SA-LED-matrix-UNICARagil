/*
* THIS IS ONLY A DUMMY-VERSION OF BACK_SERVICE! Only for testing purposes!
*/

#ifndef BACK_SERVICE_HPP
#define BACK_SERVICE_HPP

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

//LED-Matrix Controller helper class (Dummy-Version only prints on console)
#include "RGB_Matrix_Controller_Dummy.hpp"

class RGB_Back : public Service {
public:
    //static class variables MUST be initialized below
    static DummyRGBController rgbController; //declaration of helper class
    static bool prevDisplayStatus; //status of previous display status
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
        // RPC Garantuees (total: 8)
        static class : public RPCGuarantee<sCuLoad_sts>{
        public:
            sCuLoad_sts::Return remoteProcedureCall(sCuLoad_sts::Parameter parameter){

                std::string loading_status_path = "../eHMI_symbols/eHMI_streams/AkkuLadenStream.stream"; //path to .stream file

                if(!safeStopMode){
                  if(parameter.set_currently_loading == true){
                      if(!prevLoadingStatus){
                        std::cout << "RPC-Call received: Showing Loading image" << "\n" << std::endl;
                      }

                      rgbController.setFilepath(loading_status_path);
                      rgbController.setIsDisplayActive(true);
                      prevDisplayStatus = true;
                      prevLoadingStatus = true;

                    } else{
                      rgbController.setIsDisplayActive(false);

                      if(prevDisplayStatus && prevLoadingStatus){
                        std::cout << "Stop-Signal sent" << "\n" << std::endl;
                        prevDisplayStatus = false;
                        prevLoadingStatus = false;

                        //raise(SIGINT); //stop Matrix in RGB_Thread; commented out because of Dummy mode

                      }
                    }

                    sCuLoad_sts::Return ret;
                    ret.success = true;
                    return ret;
                } else {
                  sCuLoad_sts::Return ret;
                  ret.success = false;
                  return ret;
                }
              }
          } rpc_set_currently_loading;

          static class : public RPCGuarantee<sLPRL_sts>{
          public:
              sLPRL_sts::Return remoteProcedureCall(sLPRL_sts::Parameter parameter){

                  //Back matrix -> ParkenLinks symbol
                  std::string start_left_path = "../eHMI_symbols/eHMI_streams/ParkenLinksStream.stream"; //path to .stream file//
                  if(!safeStopMode){
                    if(parameter.set_leaving_parking_rect_left_status == true){
                        if(!prevStartLeftStatus){
                          std::cout << "RPC-Call received: Showing Leaving Parking Rectangular Left." << "\n" << std::endl;
                        }

                        rgbController.setFilepath(start_left_path);
                        rgbController.setIsDisplayActive(true);
                        prevDisplayStatus = true;
                        prevStartLeftStatus = true;

                      } else{
                        rgbController.setIsDisplayActive(false);

                        if(prevDisplayStatus && prevStartLeftStatus){
                          std::cout << "Stop-Signal sent" << "\n" << std::endl;
                          prevDisplayStatus = false;
                          prevStartLeftStatus = false;

                          //raise(SIGINT); //stop Matrix in RGB_Thread; commented out because of Dummy mode

                        }
                      }
                        sLPRL_sts::Return ret;
                        ret.success = true;
                        return ret;
                    } else {
                        sLPRL_sts::Return ret;
                        ret.success = false;
                        return ret;
                    }
              }
            } rpc_set_leaving_parking_rect_left_status;

          static class : public RPCGuarantee<sLPRR_sts>{
          public:
              sLPRR_sts::Return remoteProcedureCall(sLPRR_sts::Parameter parameter){

                  //Back matrix -> ParkenRechts symbol
                  std::string start_right_path = "../eHMI_symbols/eHMI_streams/ParkenRechtsStream.stream"; //path to .stream file

                  if(!safeStopMode){
                    if(parameter.set_leaving_parking_rectangular_right == true){
                        if(!prevStartRightStatus){
                          std::cout << "RPC-Call received: Showing Leaving Parking Rectangular Right image" << "\n" << std::endl;
                        }

                        rgbController.setFilepath(start_right_path);
                        rgbController.setIsDisplayActive(true);
                        prevDisplayStatus = true;
                        prevStartRightStatus = true;
                      } else{
                        rgbController.setIsDisplayActive(false);

                        if(prevDisplayStatus && prevStartRightStatus){
                          std::cout << "Stop-Signal sent" << "\n" << std::endl;
                          prevDisplayStatus = false;
                          prevStartRightStatus = false;

                          //raise(SIGINT); //stop Matrix in RGB_Thread; commented out because of Dummy mode

                        }
                      }

                      sLPRR_sts::Return ret;
                      ret.success = true;
                      return ret;
              } else {
                  sLPRR_sts::Return ret;
                  ret.success = false;
                  return ret;
                }
              }
            } rpc_set_leaving_parking_rectangular_right;

            static class : public RPCGuarantee<sRPL_sts>{
            public:
                sRPL_sts::Return remoteProcedureCall(sRPL_sts::Parameter parameter){

                    //Back matrix -> ParkenLinks symbol
                    std::string stop_left_path = "../eHMI_symbols/eHMI_streams/ParkenLinksStream.stream"; //path to .stream file

                    if(!safeStopMode){
                      if(parameter.set_rectangular_parking_left == true){
                          if(!prevStopLeftStatus){
                            std::cout << "RPC-Call received: Showing Rectangular Parking Left image" << "\n" << std::endl;
                          }

                          rgbController.setFilepath(stop_left_path);
                          rgbController.setIsDisplayActive(true);
                          prevDisplayStatus = true;
                          prevStopLeftStatus = true;

                        } else{
                          rgbController.setIsDisplayActive(false);

                          if(prevDisplayStatus && prevStopLeftStatus){
                            std::cout << "Stop-Signal sent" << "\n" << std::endl;
                            prevDisplayStatus = false;
                            prevStopLeftStatus = false;

                            //raise(SIGINT); //stop Matrix in RGB_Thread; commented out because of Dummy mode

                          }
                        }
                          sRPL_sts::Return ret;
                          ret.success = true;
                          return ret;
                      } else {
                          sRPL_sts::Return ret;
                          ret.success = false;
                          return ret;
                      }
                }
              } rpc_set_rectangular_parking_left;

            static class : public RPCGuarantee<sRPR_sts>{
            public:
                sRPR_sts::Return remoteProcedureCall(sRPR_sts::Parameter parameter){

                    //Back matrix -> ParkenLinks symbol
                    std::string stop_right_path = "../eHMI_symbols/eHMI_streams/ParkenRechtsStream.stream"; //path to .stream file

                    if(!safeStopMode){
                      if(parameter.set_rectangular_parking_right == true){
                          if(!prevStopRightStatus){
                            std::cout << "RPC-Call received: Showing rectangular parking right image" << "\n" << std::endl;
                          }

                          rgbController.setFilepath(stop_right_path);
                          rgbController.setIsDisplayActive(true);
                          prevDisplayStatus = true;
                          prevStopRightStatus = true;

                        } else{
                          rgbController.setIsDisplayActive(false);

                          if(prevDisplayStatus && prevStopRightStatus){
                            std::cout << "Stop-Signal sent" << "\n" << std::endl;
                            prevDisplayStatus = false;
                            prevStopRightStatus = false;

                            //raise(SIGINT); //stop Matrix in RGB_Thread; commented out because of Dummy mode

                          }
                        }

                          sRPR_sts::Return ret;
                          ret.success = true;
                          return ret;
                  } else{
                    sRPR_sts::Return ret;
                    ret.success = false;
                    return ret;
                  }
                }
              } rpc_set_rectangular_parking_right;

            static class : public RPCGuarantee<sTurLef_sts>{
            public:
                sTurLef_sts::Return remoteProcedureCall(sTurLef_sts::Parameter parameter){

                    //Back matrix -> gegenUZS symbol
                    std::string turning_left_path = "../eHMI_symbols/eHMI_streams/WendemanöverGegenUZSStream.stream"; //path to .stream file

                    if(!safeStopMode){
                      if(parameter.set_turning_left_status == true){
                          if(!prevTurningLeftStatus){
                            std::cout << "RPC-Call received: Showing Turning Left GIF animation" << "\n" << std::endl;
                          }

                          rgbController.setFilepath(turning_left_path);
                          rgbController.setIsDisplayActive(true);
                          prevDisplayStatus = true;
                          prevTurningLeftStatus = true;

                        } else{
                          rgbController.setIsDisplayActive(false);

                          if(prevDisplayStatus && prevTurningLeftStatus){
                            std::cout << "Stop-Signal sent" << "\n" << std::endl;
                            prevDisplayStatus = false;
                            prevTurningLeftStatus = false;

                            //raise(SIGINT); //stop Matrix in RGB_Thread; commented out because of Dummy mode

                          }
                        }
                          sTurLef_sts::Return ret;
                          ret.success = true;
                          return ret;
                      } else {
                        sTurLef_sts::Return ret;
                        ret.success = false;
                        return ret;
                      }
                    }
              } rpc_set_turning_left_status;

              static class : public RPCGuarantee<sTurRig_sts>{
              public:
                  sTurRig_sts::Return remoteProcedureCall(sTurRig_sts::Parameter parameter){

                      //Back matrix -> mitUZS symbol
                      std::string turning_right_path = "../eHMI_symbols/eHMI_streams/WendemanöverMitUZSStream.stream"; //path to .stream file

                      if(!safeStopMode){
                        if(parameter.set_turning_right_status == true){
                            if(!prevTurningRightStatus){
                              std::cout << "RPC-Call received: Showing Turning Right GIF animation" << "\n" << std::endl;
                            }

                            rgbController.setFilepath(turning_right_path);
                            rgbController.setIsDisplayActive(true);
                            prevDisplayStatus = true;
                            prevTurningRightStatus = true;

                          } else{
                            rgbController.setIsDisplayActive(false);

                            if(prevDisplayStatus && prevTurningRightStatus){
                              std::cout << "Stop-Signal sent" << "\n" << std::endl;
                              prevDisplayStatus = false;
                              prevTurningRightStatus = false;

                              //raise(SIGINT); //stop Matrix in RGB_Thread; commented out because of Dummy mode

                            }
                          }

                            sTurRig_sts::Return ret;
                            ret.success = true;
                            return ret;
                        } else {
                          sTurRig_sts::Return ret;
                          ret.success = false;
                          return ret;
                        }
                      }
                } rpc_set_turning_right_status;

                static class : public RPCGuarantee<sSafSto_sts>{
                public:
                    sSafSto_sts::Return remoteProcedureCall(sSafSto_sts::Parameter parameter){

                        std::string warning_sign_path = "../eHMI_symbols/eHMI_streams/AusrufezeichenWarnungStream.streams"; //path to .stream file

                          if(parameter.set_safe_stop_status == true){
                              if(prevSafeStopStatus == false){
                                std::cout << "RPC-Call received: SAFE STOP MODE ACTIVATED!!" << "\n" << std::endl;

                                //raise(SIGINT); //stop Matrix in RGB_Thread; stop Matrix in RGB_Thread only once; commented out because of Dummy mode

                              }

                              safeStopMode = true; //activating safe stop in class; this will stop every active state
                              prevDisplayStatus = true;
                              prevSafeStopStatus = true;

                              rgbController.setFilepath(warning_sign_path);
                              rgbController.setIsDisplayActive(true);

                            } else{
                              rgbController.setIsDisplayActive(false);
                              safeStopMode = false; //exit safe stop mode

                              if(prevDisplayStatus && prevSafeStopStatus){
                                std::cout << "Stop-Signal sent" << "\n" << std::endl;
                                prevDisplayStatus = false;
                                prevSafeStopStatus = false;

                                //raise(SIGINT); //stop Matrix in RGB_Thread; commented out because of Dummy mode

                              }
                            }
                              sSafSto_sts::Return ret;
                              ret.success = true;
                              return ret;
                          }
                  } rpc_set_safe_stop_status;

        // Konstruktor
        RGB_Back(const char *name) : Service(name){
            // RPC Guarantees (total: 8)
            addRPCGuarantee(&rpc_set_currently_loading);
            addRPCGuarantee(&rpc_set_leaving_parking_rect_left_status);
            addRPCGuarantee(&rpc_set_leaving_parking_rectangular_right);
            addRPCGuarantee(&rpc_set_rectangular_parking_left);
            addRPCGuarantee(&rpc_set_rectangular_parking_right);
            addRPCGuarantee(&rpc_set_safe_stop_status);
            addRPCGuarantee(&rpc_set_turning_left_status);
            addRPCGuarantee(&rpc_set_turning_right_status);
        }

        // Destruktor
        ~RGB_Back() = default;

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


//Initializations and Instantiations
//RGBMatrixController * RGB_Back::rgbController; //reference pointer not needed here (only in classes with inheritance)
DummyRGBController RGB_Back::rgbController;
bool RGB_Back::prevDisplayStatus = false;
bool RGB_Back::safeStopMode = false;

//status variables
bool RGB_Back::prevLoadingStatus = false;
bool RGB_Back::prevStartRightStatus = false;
bool RGB_Back::prevStartLeftStatus = false;
bool RGB_Back::prevAllowPassStatus = false;
bool RGB_Back::prevPedCrossStatus = false;
bool RGB_Back::prevTurningRightStatus = false;
bool RGB_Back::prevTurningLeftStatus = false;
bool RGB_Back::prevStopLeftStatus = false;
bool RGB_Back::prevStopRightStatus = false;
bool RGB_Back::prevSafeStopStatus = false;

// Instantiate RPC-Guarantees (total: 8)
decltype(RGB_Back::rpc_set_currently_loading) RGB_Back::rpc_set_currently_loading;
decltype(RGB_Back::rpc_set_leaving_parking_rect_left_status) RGB_Back::rpc_set_leaving_parking_rect_left_status;
decltype(RGB_Back::rpc_set_leaving_parking_rectangular_right) RGB_Back::rpc_set_leaving_parking_rectangular_right;
decltype(RGB_Back::rpc_set_rectangular_parking_left) RGB_Back::rpc_set_rectangular_parking_left;
decltype(RGB_Back::rpc_set_rectangular_parking_right) RGB_Back::rpc_set_rectangular_parking_right;
decltype(RGB_Back::rpc_set_safe_stop_status) RGB_Back::rpc_set_safe_stop_status;
decltype(RGB_Back::rpc_set_turning_left_status) RGB_Back::rpc_set_turning_left_status;
decltype(RGB_Back::rpc_set_turning_right_status) RGB_Back::rpc_set_turning_right_status;

#endif
