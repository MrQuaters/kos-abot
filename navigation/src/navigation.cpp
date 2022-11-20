#include "navigation.h"
#include <iostream>
#include <unistd.h>
#include <gpio/gpio.h>


#define MAX_MANUAL_SPEED 30

NavigationController::NavigationController(GpioHandle *handle) {
    pwmController = std::make_unique<pwm_controller>(handle);
}

void NavigationController::handleManualTask(NavigationTasks task, int durationMs, int speed) {
    std::cerr << "[NavigationController] STarting task" << std::endl;
    int speed_ = (speed < 0 || speed > 100) ? MAX_MANUAL_SPEED : speed;

    switch(task){
        case GO_LEFT:
            pwmController->goLeft(speed_);
            break;
        case GO_FORWARD:
            pwmController->goForward(speed_);
            break;
        case GO_BACKWARD:
            pwmController->goBack(speed_);
            break;
        case GO_RIGHT:
            pwmController->goRight(speed_);
            break;
    }

    // sleep 10 ms than check if get stop...
    int sleepTimes = durationMs / 100;
    for (int i=0; i<sleepTimes && !stop; ++i) {
        usleep(100000);
    }

    pwmController->stop();
    runMut.lock();
    isRunning = false;
    runMut.unlock();
    std::cerr << "[NavigationController] Task finished...." << std::endl;
}

int NavigationController::startManualTask(NavigationTasks task, int durationMs, int speed) {
    std::cerr << "[NavigationController] Requested to start manual Task: " << task << " duration " << durationMs << "ms..." << std::endl;
    runMut.lock();
    if (isRunning) {
        runMut.unlock();
        std::cerr << "[NavigationController] another task is ative now..." << std::endl;
        return 0;
    }

    if (activeTask.joinable()) {
        activeTask.join();
    }
    
    isRunning = true;
    stop = false;
    
    activeTask = std::thread(&NavigationController::handleManualTask, this, task, durationMs, speed);
    runMut.unlock();

    return 1;
}

int NavigationController::stopTask() {
    stop = true;
    
    return 1;
}