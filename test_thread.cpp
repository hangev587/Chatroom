#include <iostream>
#include <thread>
#include <unistd.h>
using namespace std;

void print(){
    for(int i=1; i<=60; i++){
        cout << i << endl;
        sleep(1);
    }
}

int main(){
    thread t1(print), t2(print);
    t1.detach();
    // t2.detach();
    // t1.join();
    t2.join();

    return 0;
}
