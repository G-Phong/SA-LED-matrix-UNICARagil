#ifndef RGB_MATRIX_CONTROLLER_DUMMY_HPP
#define RGB_MATRIX_CONTROLLER_DUMMY_HPP

#include <asoa/core/runtime.hpp>
#include <asoa/driver/rtps.h>
#include <asoa/core/task.hpp>

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <cstring>

#include <chrono>
#include <thread>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <map>
#include <string>
#include <vector>

// Dummy RGB-Controller for use when testing the software on non-Raspberry Pi hardware.
class DummyRGBController{
private:
  bool isDisplayActive;
  std::string filepath;

private:
  void RGB_Thread(){
    std::cout << "Dummy_Thread started: LED-Matrix-Handling (infinite Loop)" << std::endl;
    while(true){
      if(this->isDisplayActive == true){
        std::cout << "Dummy_Thread erkennt isDisplayActive: " << this->isDisplayActive << std::endl;
        bild_anzeigen(this->filepath); //hier soll nur auf die Konsole geprinted werden
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
  }

public:
    DummyRGBController(){
        std::cout << "Initializing DummyRGBController... " << std::endl;
        this->isDisplayActive = false;
        this->filepath = "";

        //Start Threads
        std::thread LED_Matrix_Thread(&DummyRGBController::RGB_Thread, this);
        LED_Matrix_Thread.detach();

        std::cout << "Done" << std::endl;
    }

  void bild_anzeigen(std::string s){
    //Print Befehle statt LED-Anzeige
    std::cout << "Showing animation: " << std::endl;
    std::cout << this->filepath << "\n" << std::endl;
  }

  void setIsDisplayActive(bool value){
    this->isDisplayActive = value;
  }

  void setFilepath(std::string value){
    this->filepath = value;
  }
};

#endif
