#include "Pool.h"
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

int main()
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
run(10);

  
  return 0;

}
