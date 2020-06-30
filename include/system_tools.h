#include <ctime>
#include <sys/time.h>
#include <atomic>
#include <string>

namespace tools{
  double clock(void);
  void sleep(double segundos);
  void sleep_us(unsigned int usec);

  void str2upper(std::string &str);
  //retorna uma string contendo a data e a hora atual do sistema
  std::string getData();
};
