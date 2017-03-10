#include <iostream>
#include <chrono>
#include "camera.h"

using namespace std;

int main()
{
    Camera x(CAM_MIXREAL);
    x.capturando= true;
//    char op= 'a';
//    while(true)
//    {
        auto start = chrono::high_resolution_clock::now();
        x.waitforimage();
        x.captureimage();
//        x.ImBruta.resize(640, 360, true);
        x.ImBruta.save("teste");
        auto finish = chrono::high_resolution_clock::now();
        cout << chrono::duration_cast<chrono::milliseconds>(finish-start).count() << "ms\n";
//    }
}
