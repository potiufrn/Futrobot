#include <cstddef> //NULL
#include "system_tools.h"

double tools::clock(void)
{
  static struct timeval tbase={-1,-1};
  struct timeval t;

  gettimeofday(&t,NULL);
  if (tbase.tv_sec==-1 && tbase.tv_usec==-1)
  {
    tbase = t;
  }
  return( t.tv_sec-tbase.tv_sec + (t.tv_usec-tbase.tv_usec)/1000000.0 );
}

void tools::sleep(double segundos){
  std::atomic<double> start(tools::clock());
  while((tools::clock() - start) < segundos){};
};

void tools::sleep_us(unsigned int usec){
  std::atomic<double> start(tools::clock());
  std::atomic<double> end(usec);
  end = end/1000000.0;//nao realizar a divisao sem antes passar para atomic (teste de eficiencia)
  while( (tools::clock() - start) < end) {};
};

void tools::str2upper(std::string &str){
  for(unsigned int i = 0; i < str.size(); i++)
    str[i] = toupper(str[i]);
}

std::string tools::getData(){
  time_t rawtime;
  tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  std::string strTime = asctime(timeinfo);
  strTime.replace(strTime.find(' '), 1, "_");
  strTime.replace(strTime.find(' '), 1, "_");
  strTime.replace(strTime.find(' '), 1, "_");
  strTime.replace(strTime.find(' '), 1, "_");
  return strTime;
}
