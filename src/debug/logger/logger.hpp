#include <iostream>

#define DEBUG_MODE 1

#if DEBUG_MODE == 1


#define LOG(x) std::cout << x
#define LOG_WNL(x) std::cout << x << "\n"
#define LOG_NL() std::cout << "\n"

#else

#define LOG(x) 

#endif