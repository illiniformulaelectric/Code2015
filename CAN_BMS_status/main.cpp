#include "mbed.h"
Serial pc(USBTX, USBRX);
LocalFileSystem local("local");
Ticker ticker;
DigitalOut led1(LED1);
DigitalOut led2(LED2);
CAN can1(p30, p29);
CAN can2(p9, p10);

char counter2[8] = { 0, 0, 0, 0, 0, 0, 0, 0};
char c = ' ';
int sent = 0;

void send_cr()
{
    char counter1[8] = { 0x1, 0x2, 0x3, 0x4, 0, 0, 0, 0x1};
    led1 = !led1;
    //printf("send()\r\n");
    CANMessage cr = CANMessage(0x632, counter1);
    if(can1.write(cr)) { 
        printf("wloop()\r\n");
        for(int i=0; i<8; ++i)
        {
            printf("%x, ", cr.data[i]);
        }
        printf("\r\n");
        wait(1);
        led1 = !led1;
    }
    //wait (0.1);
}

void handle_msg()
{
    led2 = !led2;
  //printf("Message received: ");
    CANMessage msg;
        if(can1.read(msg)) {
            if (msg.id >= 0x1&& msg.id <= (0x1+20)) {
                printf("cell voltage from cell %d to %d \r\n", (msg.id-1)*8,(msg.id-1)*8+8 );
                for (int i = 0; i < 8; i++) {
                    double voltage = msg.data[i]*0.01+2.0;
                    printf("%6.3f ", voltage);
                }
                printf("\r\n");
            }

            if(msg.id == 0x623) {
                printf("Pack voltage %d\r\n", msg.data[1] );
                printf("min voltage %6.3f and %3d cells are of min \r\n", msg.data[2]*0.1, msg.data[3]);
                printf("max voltage %6.3f and %3d cells are of max \r\n", msg.data[4]*0.1, msg.data[5]);
            }

            if(msg.id >= 0x620)
            {
                printf("ID: %x\r\n", msg.id);
                printf("Message received: ");
                for (int i = 0; i < 8; i++)
                    printf("%d ", msg.data[i]);
                printf("\r\n");
            }
            
            // wait(0.1)
        }//
    }

    int main() {
        //printf("main()\r\n");
        led2 = !led2;
        can1.frequency(250000);
        pc.printf("Press 'c' to close contactor");
        //c = pc.getc();
        can1.attach(&handle_msg); //interrupt trigger function
        //if (c == 'c') {
        wait(4);
        send_cr();
        //}
        while (1);
        //while(1) {
//        printf("loop()\n");
//        CANMessage msg;
//        if(can1.read(msg)) {
//            printf("Message received: %x\n", msg.id);
//            led2 = !led2;
//        } 
//        wait(0.2);
//    }
    }


