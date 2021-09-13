/*
 * @Author Shi Zhangkun
 * @Date 2021-09-10 18:55:28
 * @LastEditTime 2021-09-13 14:03:58
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath /server/inc/ringBuffer.hpp
 */

#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H
#include <memory>
#include <vector>
#include <cstring>
template<typename T> 
class RingBuffer
{
private:
  std::size_t total = 0;
  std::size_t curSize = 0;
  std::size_t start = 0;
  std::size_t end = 0;
  std::unique_ptr<T[]> pData = nullptr;
  /**
   * @brief  环形缓冲末端指针增长
   * @note  
   * @param {size_t} n ： 增长个数
   * @retval 增长后的末端位置指针
   */
  std::size_t end_add(std::size_t n = 1)
  {
    if(curSize + n <= total)
    {
      end = (end + n) % total;
      curSize += n;
    }
    return end;
  }
  /**
   * @brief  环形缓冲首端指针增长
   * @note  
   * @param {size_t} n ：增长个数
   * @retval 增长后的首位置指针
   */
  std::size_t start_add(std::size_t n = 1)
  {
    if(curSize >= n)
    {
      start = (start + n) % total;
      curSize -= n;
    }
    return start;
  }

public:
  RingBuffer(){};
  RingBuffer(std::size_t _total)
  {
    start = 0;
    end = 0;
    curSize = 0;
    total = _total;
    pData = std::make_unique<T[]>(total);
  }
  RingBuffer(const RingBuffer& ring)
  :total(ring.total), curSize(ring.curSize), start(ring.start), end(ring.end)
  {
    pData = std::make_unique<T[]>(total);
    for (decltype(curSize) i = 0; i < curSize; i++)
    {
      at(i) = ring.at(i);
    }
  }
  RingBuffer(RingBuffer&& ring)
  :total(ring.total), start(ring.start), end(ring.end), curSize(ring.curSize)
  {
    pData = std::move(ring.pData);
    ring.start = 0;
    ring.end = 0;
    ring.curSize = 0;
  }

  ~RingBuffer() = default;
  /**
   * @brief  压入一个元素
   * @note  
   * @param {const} T
   * @retval none
   */
  std::size_t push(const T& _element)
  {
    if(curSize < total)
    {
      pData[end] = _element;
      end_add();
      return true;
    }
    return false;
    
  }

  /**
   * @brief  压入一个元素
   * @note  
   * @param {const} T
   * @retval none
   */
  bool push(const T&& _element)
  {
    if(curSize < total)
    {
      pData[end] = _element;
      end_add();
      return true;
    }
    return false;
  }

  /**
   * @brief  压入n个元素
   * @note  
   * @param {const} std
   * @retval none
   */
  std::size_t push_n(const std::vector<T>& _elements)
  {
    int pushed = 0;
    for(auto element:_elements)
    {
      if(!push(element)) break;
      pushed++;
    }
    return pushed;
  }

  /**
   * @brief  压入n个元素
   * @note  
   * @param {const} std
   * @retval none
   */
  std::size_t push_n(const T* _elements, std::size_t _n)
  {
    std::size_t pushed = 0;
    for(pushed = 0; pushed < _n; pushed++)
      if(!push(_elements[pushed])) break;
    return pushed;
  }

  /**
   * @brief  弹出n个元素
   * @note  
   * @param {size_t} _n
   * @retval none
   */
  bool pop(std::size_t _n = 1)
  {
    if(curSize >= _n)
    {
      start_add(_n);
      return true;
    }
    return false;
  }

  /**
   * @brief  随机访问
   * @note  
   * @param {size_t} _index
   * @retval none
   */
  T& at(std::size_t _index) const
  {
    if(_index >= curSize) throw std::out_of_range("ringBuffer : out of range.");
    std::size_t _real = (start + _index) % total;
    return pData[_real];
  } 

  /**
   * @brief  获取当前缓冲区内数据size
   * @note  
   * @param {*}
   * @retval none
   */
  std::size_t size(void) const {return curSize;} 
  std::size_t remain(void) const {return total - curSize;} 
  T& operator [](std::size_t _index) const {return at(_index);}  

  /**
   * @brief  清空
   * @note  
   * @param {*}
   * @retval none
   */  
  void clear()
  {
    start = 0;
    end = 0;
    curSize = 0;
  }

  /**
   * @brief  拷贝复制
   * @note  
   * @param {*}
   * @retval none
   */  
  RingBuffer& operator=(const RingBuffer& ring)
  {
    start = ring.start;
    end = ring.end;
    curSize = ring.curSize;
    total = ring.total;
    pData = std::make_unique<T[]>(total);
    for (decltype(curSize) i = 0; i < curSize; i++)
    {
      at(i) = ring.at(i);
    }
    return *this;
  }

  /**
   * @brief  移动复制
   * @note  
   * @param {*}
   * @retval none
   */  
  RingBuffer& operator=(RingBuffer&& ring)
  {
    start = ring.start;
    end = ring.end;
    curSize = ring.curSize;
    total = ring.total;
    pData = std::move(ring.pData);
    return *this;
  }
};

#endif