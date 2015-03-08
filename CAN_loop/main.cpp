#include "mbed.h"
Serial pc(USBTX, USBRX);
Ticker ticker;
DigitalOut led1(LED1);
DigitalOut led2(LED2);
CAN can1(p30, p29);
CAN can2(p9, p10);
char counter1[8] = { 0, 0, 0, 0, 0, 0, 0, 1};
char counter2[8] = { 0, 0, 0, 0, 0, 0, 0, 0};
char c = ' ';
int sent = 0;

void send_cr_forever()
{
    printf("send()\r\n");
    while(can1.write(CANMessage(0x111, counter1))) {
        //wait (0.5);
        printf("wloop()\r\n");
        //counter++;
        printf("Message sent: %d\r\n", counter1);


        //can1.monitor(sent);
    }

    led1 = !led1;
}

void handle_msg()
{
    CANMessage msg;
    printf("handle_mag is called\r\n");
    if(can1.read(msg)) {
        /*if (msg.id == 0x111) {
            printf("received\r\n");

        }
        */

        if(msg.id <= 32 && msg.id>= 0x1) {
            printf("ID: %x\r\n", msg.id);
            printf("Message received: ");
            for (int i = 0; i < 8; i++)
                printf("%d ", msg.data[i]);
            printf("\r\n");
        }//check bms_status
        led2 = !led2;

    }
}



int main()
{
    printf("main()\r\n");
    can1.frequency(250000);
    pc.printf("Press 'c' to close contactor");
    c = pc.getc();
    
    can1.attach(&handle_msg);//interrupt trigger function

    if (c == 'c') {
        send_cr_forever();
 
    }
    while(1);
}
