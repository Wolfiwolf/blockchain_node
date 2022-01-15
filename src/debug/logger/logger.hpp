#include <iostream>

#define LOG_MODE 1

#if LOG_MODE == 1


#define LOG(x) std::cout << x
#define LOG_WNL(x) std::cout << x << "\n"
#define LOG_NL() std::cout << "\n"

#else

#define LOG(x) 

#endif