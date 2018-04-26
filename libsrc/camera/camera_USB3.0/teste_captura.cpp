#include "camera.h"

using namespace std;

int main(){
  Camera cam(1);
  char key;
  cout << "q - Exit \n ENTER - Print "<<endl;

  while(true){
    cin.get(key);
    if(key == 'q')return 0;
    else if(key == '\n')
    {
      cam.captureimage();
      cam.waitforimage();
      cam.imgBruta.save("CamSaveTeste.ppm");
    }
  };


  return 0;
}
