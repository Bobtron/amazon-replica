#include "heap.h"
#include <string>

using namespace std;

int main (int argc, char *argv[]){
  Heap<string, equal_to<string>, less<double>, hash<string>> h;
    h.push(1.0, "a");
    h.push(2.0, "b");
    h.push(3.0, "c");
    h.push(4.0, "d");
    h.push(5.0, "e");
    h.push(6.0, "f");
    h.push(7.0, "g");
    h.push(8.0, "h");
    h.push(9.0, "i");
    h.printInside();
    h.printPriority();
    h.decreaseKey(3.0, "i");
    h.printInside();
    h.printPriority();
}
