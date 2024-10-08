// https://projects.drogon.net/raspberry-pi/wiringpi/serial-library/

#include <wiringSerial.h>
#include <unistd.h>
#include <iostream>

int main()
{
    int arduino = serialOpen("/dev/ttyACM0", 9600);
    // uint8_t c = static_cast<uint8_t>(serialGetchar(arduino));

    for (int i = 0; i < 80; i++)
    {
        uint8_t c;
        if (read(arduino, &c, 1) < 0)
        {
            std::cout << "something went wrong\n";
        }
        std::cout << "recieved " << (c & 0xFF) << " from arduino\n";

        if (c == '\n') 
        {
            std::cout << "got EOL marker\n";
        }
    }
    serialClose(arduino);
    return 0;
}