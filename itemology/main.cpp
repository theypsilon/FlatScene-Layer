#include "IFirstEngine.h"

#include <exception>
#include <iostream>
#include <memory>


Library& FSLib = Library::I();
Screen& FSDraw = Screen::I();

int main (int argc, char* argv[]) {
    using namespace std;

    cout << "Starting main" << endl;

    //run the application
    FSLib.startLibrary(false);
    FSDraw.start(640,480,32,1.0,1.0,false);

    try {
        FSLib.processEngine(unique_ptr<IFirstEngine>(new IFirstEngine));
    } catch (std::exception& e) {
        cout << "Exception catched: " << e.what() << endl;
    } catch (...) {
        cout << "The exception catched is unknown." << endl;
    }

    cout << "Ending main" << endl;

    return 0;
}