#include "IFirstEngine.h"

#include <exception>
#include <iostream>
#include <memory>


Library& FSLib = Library::I();
Screen& FSDraw = Screen::I();

int main (int argc, char* argv[]) {
    using namespace std;

    cout << "Starting main" << endl;

    constexpr auto c = Color("AABBCC");

    // c.setR(0xAA);
    // c.setG(0x11);
    // c.setB(0x22);
    // c.setA(0x33);

    cout << hex << c.getHex() << '.' << 
    // // (int) c.R() << (int) c.G() << (int) c.B() << (int) c.A() <<'.' << 
    (int) c.getA() << (int) c.getR() << (int) c.getG() << (int) c.getB() << endl;

    //run the application
    // FSLib.startLibrary(false);
    // FSDraw.start(640,480,32,1.0,1.0,false);

    // try {
    //     FSLib.processEngine(unique_ptr<IFirstEngine>(new IFirstEngine));
    // } catch (std::exception& e) {
    //     cout << "Exception catched: " << e.what() << endl;
    // } catch (...) {
    //     cout << "The exception catched is unknown." << endl;
    // }

    cout << "Ending main" << endl;

    return 0;
}