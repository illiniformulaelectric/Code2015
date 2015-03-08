
#include "mbed.h"
#include <iostream>

Serial pc(USBTX, USBRX);
//Setup I2C on pins 9 and 10
I2C i2c(p28, p27);

//Setup LEDs
DigitalOut message_received(LED1);
DigitalOut message_invalid(LED3);

void interpret_message(char data[4]);   //Function to interpret the received message

const char address1 = 0x02;  //Address of first slave

int main()
{
    char msg[] = {0x20,0x11, 0x11, 0x11}; //Currently initialized for debug, could be zero, or anything at all really.
    i2c.frequency(1000);
    //Every 200ms, poll the atmega
    while(1)
    {
        i2c.read(address1, msg, 4); //Read 4 bytes (disassembled float)
        interpret_message(msg); //Reassemble the float and interpret it.
        wait_ms(200); //Wait 200ms before trying to read again. This will be removed in the final code.
        //Best would be to replace the wait with some timer that runs this function every 200ms.
        message_received = 1;
        message_invalid = 1;
    }   
}

union makeMeAFloat
{
    float result;
    struct
    {
        char b1, b2, b3, b4;
    };
};

//For now all we do in this is grab the data reassemble it and spit it out to console.
void interpret_message(char data[4])
{
    makeMeAFloat *converter = new makeMeAFloat();
    converter->b1 = data[0];
    converter->b2 = data[1];
    converter->b3 = data[2];
    converter->b4 = data[4];
    std::cout << "Temperature: " << converter->result << std::endl;
}
//STL - Clock signal
