#include <ESP8266WiFi.h>
extern "C" {
  #include "user_interface.h"
}

String alfa = "1234567890qwertyuiopasdfghjkklzxcvbnm QWERTYUIOPASDFGHJKLZXCVBNM_";
byte channel;

//Beacon帧
uint8_t packet[128] = {
  /*  0 - 3  */ 0x80, 0x00, 0x00, 0x00,                         //(子)类型：Beacon帧
  /*  4 - 9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,             //接收方：广播
  /* 10 - 15 */ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,             //发送方
  /* 16 - 21 */ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,             //发送方
  /* 22 - 23 */ 0x00, 0x00,                                     //分片&序列号(SDK自动填充这部分)

  /* 24 - 31 */ 0x83, 0x51, 0xF7, 0x8F, 0x0F, 0x00, 0x00, 0x00, //时间戳
  /* 32 - 33 */ 0x64, 0x00,                                     //发送周期: 0x64, 0x00 => 每100ms - 0xE8, 0x03 => 每1s
  /* 34 - 35 */ 0x31, 0x00,                                     //特征信息

  /* 36 - 37 */ 0x00, 0x06,                                     //元素编号: 0x00 => SSID, 长度: 0x06
  /* 38 - 43 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,             //SSID内容
  /* 44 - 48 */ 0x01, 0x08,                                     //元素编号: 0x01 => 支持速率, 长度: 0x08
  /* 49 - 53 */ 0x82, 0x84, 0x8B, 0x96, 0x24, 0x30, 0x48, 0x6C, //速率
  /* 54 - 55 */ 0x03, 0x01,
  /* 56 */      0x04                                            //频道
};

void setup() {
  Serial.begin(115200);
  delay(500);
  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(1); 
}

void loop() {
  channel = random(1, 12);//随机频道
  wifi_set_channel(channel);

  //随机源MAC地址
  packet[10] = packet[16] = random(256);
  packet[11] = packet[17] = random(256);
  packet[12] = packet[18] = random(256);
  packet[13] = packet[19] = random(256);
  packet[14] = packet[20] = random(256);
  packet[15] = packet[21] = random(256);
  //随机SSID
  packet[38] = alfa[random(65)];
  packet[39] = alfa[random(65)];
  packet[40] = alfa[random(65)];
  packet[41] = alfa[random(65)];
  packet[42] = alfa[random(65)];
  packet[43] = alfa[random(65)];
  packet[56] = channel;

  Serial.print("Creating \"");
  Serial.write(packet + 38, 6);
  Serial.println("\"");
  for(byte i = 0; i < 3; i++) {
    wifi_send_pkt_freedom(packet, 57, 0);//发包
    delay(100);
  }
}
