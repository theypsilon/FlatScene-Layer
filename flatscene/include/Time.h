#ifndef FS_TIME_H__
#define FS_TIME_H__

namespace FlatScene {

    class Time {
    private:

        unsigned int _msInterval;
        unsigned int _msLast;
        unsigned int _ticks;

    public:
        Time();
        ~Time();
        unsigned int setInterval(unsigned int msInterval );
        unsigned int setFPS(unsigned int fpsInterval );
        void nextFrame();
        unsigned int getTick() const;
        void reset(unsigned int tick = 0);
    };

} // flatscene

#endif
