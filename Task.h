#include <cstddef>

class Task
{
public:
  virtual void Run() = 0;
  virtual ~Task(){};
};

class TaskA : public Task
{
public:
  TaskA(std::size_t sleepT)
      : sleepT(sleepT)
  {}

  ~TaskA() = default;

  void Run() override;

private:
  std::size_t sleepT;
};

  
