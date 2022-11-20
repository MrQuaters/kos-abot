#ifndef _NAVIGATION_NAVIGATION_CTR_H
#define _NAVIGATION_NAVIGATION_CTR_H

#include <thread>
#include <mutex>

#include "pwm_controller.h"

enum NavigationTasks {
    GO_FORWARD,
    GO_BACKWARD,
    GO_LEFT,
    GO_RIGHT
};

class NavigationController {
    std::thread activeTask;
    std::mutex runMut;
    std::unique_ptr<pwm_controller> pwmController;
    bool isRunning;
    bool stop;

    void handleManualTask(NavigationTasks task, int durationMs, int speed);

public:
    NavigationController(GpioHandle *handle);
    int startManualTask(NavigationTasks task, int durationMs, int speed);
    int stopTask();
};

#endif