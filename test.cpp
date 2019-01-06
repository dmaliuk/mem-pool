#include "Pool.h"
#include "Task.h"
#include <vector>
#include <iostream>

struct Foo
{
  Foo(){
    std::cout << "Foo()\n";
  }

  ~Foo(){
    std::cout << "~Foo()\n";
  }
};

void test1()
{
  MemPool<TaskA, Task> poolA{2};
  MemPool<TaskB, Task> poolB{2};

  auto run = [&](int n){
    std::cout << "... n = " << n << std::endl;
    std::vector<decltype(poolA.Alloc())> vec;
    for (int i = 0; i < n; ++i)
    {
      vec.push_back(poolA.Alloc(1ul));
      vec.push_back(poolB.Alloc(1ul));
    }
  };
  
  run(1);
  run(2);
  run(3);
}

void test2()
{
  MemPool<Foo> pool{1};

  auto run = [&](int n){
    std::cout << "... n = " << n << std::endl;
    std::vector<decltype(pool.Alloc())> vec;
    for (int i = 0; i < n; ++i)
      vec.push_back(pool.Alloc());
  };

  run(1);
  run(2);
  run(3);
}

int main()
{
  test1();
  
  return 0;

}
