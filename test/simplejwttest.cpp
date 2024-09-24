#include "fatctl/ui/connection.hpp"

#include <iostream>

using namespace std;
using namespace rrobot;

int main() {

    connection_header hdr = connection_header();

    cout << hdr.to_json() << "\n";
    return 0;
}