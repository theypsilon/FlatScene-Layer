#include "IFirstEngine.h"

#include <exception>
#include <iostream>
#include <memory>
using namespace std;

int main (int argc, char* argv[]) {

    cout << "Starting main" << endl;

    //run the application
    FSLib.startLibrary(false);
    FSDraw.start(640,480,32,1.0,1.0,false);

    auto text = Write.line(0,10,10,"This is the text");

    FSLib.processEngine(unique_ptr<IFirstEngine>(new IFirstEngine));

    Write.erase(text);

    cout << "Ending main" << endl;

    return 0;
}