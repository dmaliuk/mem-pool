#include "Task.h"
#include <chrono>
#include <thread>

void TaskA::Run()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(sleepT));
}
