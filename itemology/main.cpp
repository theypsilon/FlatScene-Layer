#include "IFirstEngine.h"

#include <exception>
#include <iostream>
#include <memory>
using namespace std;


Library& FSLib = Library::I();
Screen& FSDraw = Screen::I();

int main (int argc, char* argv[]) {

    cout << "Starting main" << endl;

    //run the application
    FSLib.startLibrary(false);
    FSDraw.start(640,480,32,1.0,1.0,false);

    FSLib.processEngine(unique_ptr<IFirstEngine>(new IFirstEngine));

    cout << "Ending main" << endl;

    return 0;
}