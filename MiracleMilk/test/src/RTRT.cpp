#include <iostream>
#include <string.h>
#include "log.h"

using namespace drv;
using namespace std;

int main(void){
    const char* str = "can you read me?";
    cout << str << endl;
    cout << sizeof(str) << endl;
    cout << sizeof(*str) << endl;
    cout << sizeof("can you hear me?") << endl;
    cout << strlen(str) << endl;
    cout << strnlen(str,128) << endl;
    Log log1 = Log();
    Log log2 = log1.waitFor(2000);
    cout << log2.getDurationfrom(log1) << endl;
    Note note1 = Note("start!",true);
    Note note2 = note1.waitFor(3000);
    cout << note2.getDurationfrom(note1) << endl;
    cout << note2.getDurationfrom(log1) << endl;
    
    return 0;
}