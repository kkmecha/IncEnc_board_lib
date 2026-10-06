# IncEnc_board_lib  
AMT102-CAN変換基盤からのデータを処理するライブラリです  
CANデータの受け取り処理はmain関数内でしてください。このメンバ関数でデータ変換ができます。
  
注意  
もしencoder_reset_all()ですべてのnodeにリセットコマンドを送信できない不具合が起きた場合、IncEnc_board.cpp内のコメントアウトした部分を戻してあげるとすべてにリセットがかかると思います　だたこれは_can.write(msg)を確実に行うための応急処置的なもののため、main関数の処理中にThisThread::sleep_for()が入ってしまうことに注意してください

  
  
サンプルコード  
  
main.cpp
~~~main.cpp
#include "mbed.h"
#include "IncEnc_board.h"

CAN can(PD_0, PD_1, 1000000);
UnbufferedSerial pc(USBTX, USBRX, 9600);
IncEnc_board encoder(can, 1);

int main() {
    encoder.encoder_reset_all();
    while(true) {
        int64_t received_angle;
        encoder.conv_data_all(&received_angle);
        
        if(pc.readable()){
            char key = 0;
            pc.read(&key, 1);
            switch(key){
                case 'r': 
                    printf("Sending reset command to node 1...\r\n");
                    encoder.encoder_reset_all();
                    break;
            }
        }
        printf("Received data: %lld\r\n", received_angle);
        
        ThisThread::sleep_for(1ms);
~~~
    }
}
