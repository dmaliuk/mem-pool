#include <memory>
#include <type_traits>
#include <cassert>
#include <iostream>

#define CACHE_ALIGNMENT 64
//#define TRACE_METHOD() std::cout << this << " " << __PRETTY_FUNCTION__ << "\n";
#define TRACE_METHOD()

template <class T>
union alignas(CACHE_ALIGNMENT) Item
{
  Item(){}
  ~Item(){}
  
  T data;
  Item * next;
};

template <class T>
class Block
{
 public:
  Block(std::size_t const size = 1024)
      : size(size)
      , storage(new Item<T>[size])
  {
    TRACE_METHOD();
    for (std::size_t i = 1; i < size; ++i)
      storage[i - 1].next = &storage[i];

    storage[size - 1].next = nullptr;
  }

  void LinkNextBlock(std::unique_ptr<Block> && nextBlock)
  {
    next = std::move(nextBlock);
  }

  decltype(auto) operator[](std::size_t const k) const
  {
    return storage[k];
  }

  decltype(auto) operator[](std::size_t const k)
  {
    return storage[k];
  }

 private:
  std::size_t size{};
  std::unique_ptr<Item<T>[]> storage{};
  std::unique_ptr<Block> next{};
};

template <class T>
class MemPool;

template <class T>
struct ItemDeleter
{
  ItemDeleter(MemPool<T> * home)
      : home(home)
  {}

  void operator()(T * ptr)
  {
    home->Release(ptr);
  }

  MemPool<T> * home;
};
  
  
template <class T>
class MemPool
{
 public:
  using BlockType = Block<T>;
  using ItemType = Item<T>;
  using ReturnType = std::unique_ptr<T, ItemDeleter<T>>;
  
  MemPool(int blockSize)
      : block(std::make_unique<Block<T>>(blockSize))
      , blockSize(blockSize)
  {
    nextFree = &(*block)[0];
  }

  template <class... Args>
  ReturnType Alloc(Args&&... args)
  {
    TRACE_METHOD();
    if (!nextFree)
      AddBlock();

    auto const ptr = nextFree;
    nextFree = nextFree->next;
    auto const tp = &ptr->data;
    new (tp) T{std::forward<Args>(args)...};
    assert((std::size_t)tp % CACHE_ALIGNMENT == 0);
    return ReturnType{tp, ItemDeleter<T>{this}};
  }

  void Release(T * ptr)
  {
    TRACE_METHOD();
    if (!std::is_trivially_destructible_v<T>)
      ptr->~T();

    Item<T> * item = reinterpret_cast<Item<T>*>(ptr);
    item->next = nextFree;
    nextFree = item;
  }

 private:
  void AddBlock()
  {
    TRACE_METHOD();
    auto newBlock = std::make_unique<BlockType>(blockSize);
    newBlock->LinkNextBlock(std::move(block));
    block = std::move(newBlock);
    nextFree = &(*block)[0];
  }
  
  Item<T> * nextFree{};
  std::unique_ptr<Block<T>> block{};
  std::size_t const blockSize{};
};
