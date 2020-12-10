#include <iostream>
#include <string.h>
#include "log.h"

using namespace drv;
using namespace std;

int main(void){
    const char* str = "can you read me?";
    cout << str << endl;

    Log log1 = Log();
    Log log2 = log1.waitFor(2000);
    cout << log2.getDurationfrom(log1) << endl;
    Note note1 = Note("start!",true);
    usleep(50);
    Note note2 = note1.waitFor(5);
    cout << note2.getDurationfrom(note1) << endl;
    cout << note2.getDurationfrom(log1) << endl;
    
    return 0;
}