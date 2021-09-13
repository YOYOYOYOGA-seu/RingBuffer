/*
 * @Author Shi Zhangkun
 * @Date 2021-02-04 11:14:24
 * @LastEditTime 2021-09-13 15:54:34
 * @LastEditors Shi Zhangkun
 * @Description none
 * @FilePath /RingBuffer/main.cpp
 */
#include <vector>
#include "main.h"
#include "ringBuffer.hpp"
int main()
{
  RingBuffer<int> buf(1024);
  int test[10] = {0};
  std::vector<int> test2(2,10);
  buf.push(100);
  buf.push_n(test,10);
  buf.push_n(test2);
  buf.pop();
  buf.pop(2);

  auto ret = buf[0];
  
  return 0;
}