#include <iostream>   
#include <unistd.h>
#include "fatctl/environment/statefact.hpp"

using namespace std;
using namespace rrobot;

int main() {
    const size_t size = 1024;
    char buffer[size];
    getcwd(buffer, size);
    cout << "Current working directory: " << buffer << endl;

    rr_state_c* state = statefact::create_state("../resources/manifests/virtual.json");
    return 0;
}