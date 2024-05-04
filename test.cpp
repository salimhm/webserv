#include <iostream>     // std::cout
#include <algorithm>    // std::remove

int main () {
  int myints[] = {10,20,30,30,20,10,10,20};      // 10 20 30 30 20 10 10 20

  std::string files = "todelete";
  // char* files = lol.data();
  // bounds of range:
  std::string::iterator pbegin = files.begin();   //Pointer to the first element of the array
  std::string::iterator pend = files.end();     //Pointer to the last element of the array

    // std::cout << "Here " << pbegin << " END " << pend << std::endl;

  std::remove(files.c_str());
                                                 // ^              ^
  std::cout << "range contains:";
  // for (char* p=pbegin; p!=pend; ++p)
  //   std::cout << ' ' << *p;
  std::cout << '\n';

  return 0;
}