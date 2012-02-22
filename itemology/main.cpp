#include "IFirstEngine.h"

#include <exception>
#include <iostream>
#include <memory>
using namespace std;

int main (int argc, char* argv[]) {

    cout << "Starting main" << endl;

    FSLib.startLibrary(640,480,32,false,false);

    FSLib.processEngine(unique_ptr<IFirstEngine>(new IFirstEngine()));

    cout << "Ending main" << endl;

    return 0;
}