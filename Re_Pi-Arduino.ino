
//库的引用

#include <DHT.h>
#include <SoftwareSerial.h>
#include<MQ135.h>
//语音控制模块的引脚设置
SoftwareSerial mySerial(4,8);//rx，tx
//各传感器引脚定义
#define ledpin 6
#define button 3
#define AIR 2
#define TP 5
#define DHTTYPE DHT11

String value;
const int A_AIR = 0;
//按键初始化
int ledstate = LOW;
int lastbutton = LOW;
int currenbutton = LOW;



//传感器引脚定义与初始化
MQ135 gasSensor = MQ135(A_AIR);
DHT dht(TP,DHT11);



//语音控制灯光
void controlLED(String command)
{
   if (command == "LED ON") {
    
    digitalWrite(ledpin, HIGH);
    lastbutton = HIGH;
    currenbutton = LOW;
    ledstate = !ledstate;

    
  } else if (command == "LED close") {
    digitalWrite(ledpin, LOW);
    lastbutton = HIGH;
    currenbutton = LOW;
    ledstate = !ledstate;
  }

}

void setup() {
//初始化
  dht.begin();
Serial.begin(9600);
 mySerial.begin(9600);
   while(!Serial)
 {;}//等待连接
 //引脚状态设定
  pinMode(ledpin, OUTPUT);
  pinMode(button,INPUT);
 
 

  
}

void loop() {
  //接受Asrpro的串口数据
  if(mySerial.available())
{
   value = mySerial.readString();
  Serial.println(value);
  controlLED(value);
}
  //按键控制灯光
 currenbutton= digitalRead(button);
  if(currenbutton == HIGH && lastbutton == LOW)
  {
    ledstate = !ledstate; // 切换灯的状态
    digitalWrite(ledpin ,ledstate);
    delay(50); // 延迟一段时间，防止按钮抖动影响
  }
  lastbutton = currenbutton;
  int A = analogRead(AIR);//空气质量传感器的数据处理与储存；
  float h = dht.readHumidity();//温度传感器的数据处理与储存；
  float t = dht.readTemperature();//湿度传感器的数据处理与储存；
  float ppm = gasSensor.getPPM();
  float P = (ppm*3.3)/4096;
  Serial.print(h);
  Serial.print(t);
  Serial.print(ppm);
 
}
