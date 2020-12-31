#ifndef PIPER_H
#define PIPER_H
#endif

#ifndef PIPER_H
#define PIPER_H
#endif

#include <cstdio>
#include <iostream>
#include <sstream>

namespace drv{

    struct pendl {
        char dummy;
    } endl;

    struct pflush{
        char dummy;
    } flush;

    class pStream{
        private:
            FILE* tx;
            FILE* rx;
        public:
            pStream(const char* progname);
            ~pStream(void);

    };
    
}