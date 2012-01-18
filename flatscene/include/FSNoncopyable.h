#ifndef __FS_NONCOPYABLE__
#define __FS_NONCOPYABLE__

class FSNoncopyable {
    protected:
        FSNoncopyable(){}
        ~FSNoncopyable(){}
    private:
        FSNoncopyable(const FSNoncopyable&);
        const FSNoncopyable& operator=(const FSNoncopyable&);
};

#endif
