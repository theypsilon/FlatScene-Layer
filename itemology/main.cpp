#include "IFirstEngine.h"

#include <exception>
#include <iostream>
#include <memory>

Screen& FSDraw = Screen::I();

int main (int argc, char* argv[]) {
    using namespace std;

    cout << "Starting main" << endl;

    // constexpr auto c = Color("FFAABBCC");

    // c.setR(0xAA);
    // c.setG(0x11);
    // c.setB(0x22);
    // c.setA(0x33);

    // cout << hex << c.getHex() << '.' << 
    // (int) c.R() << (int) c.G() << (int) c.B() << (int) c.A() <<'.' << 
    // (int) c.getA() << (int) c.getR() << (int) c.getG() << (int) c.getB() << endl;

    //run the application
    Library::init(640,480,32,false,true);

    try {
        IFirstEngine e;
        e.onInit();
        e.loop();
        e.onExit();
    } catch (FlatScene::Exception &e) {
        cout << "Exception catched: " << e.what() << endl;
    } catch (std::exception& e) {
        cout << "Exception catched: " << e.what() << endl;
    } catch (...) {
        cout << "The exception catched is unknown." << endl;
    }

    cout << "Ending main" << endl;

    return 0;
}