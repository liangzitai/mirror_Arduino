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
//oled引脚定义
U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9);

//传感器引脚定义与初始化
MQ135 gasSensor = MQ135(A_AIR);
DHT dht(TP,DHT11);

//oled显示函数集
void draw(void)
{
  int A = analogRead(AIR);//空气质量传感器的数据处理与储存；
  float h = dht.readHumidity();//温度传感器的数据处理与储存；
  float t = dht.readTemperature();//湿度传感器的数据处理与储存；
  float ppm = gasSensor.getPPM();
  float P = (ppm*3.3)/4096;
  
  //oled u8g库的引用
  u8g.setFont(u8g_font_8x13);//设置字体与字体大小
  //温湿度的显示与显示位置位置
  u8g.setPrintPos(0, 10);
  u8g.print("HUMI(%) :");
  u8g.setPrintPos(73, 10);
  u8g.print(h);
  u8g.setPrintPos(0, 20);
  u8g.print("TEMP(oC):");
  u8g.setPrintPos(73, 20);
  u8g.print(t);
 
  //当前环境的空气质量好坏文字显示
  u8g.setPrintPos(0, 40);
  u8g.print("AIR:");
  u8g.setPrintPos(50, 40);
  u8g.print(P);

}
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
 
 if (u8g.getMode() == U8G_MODE_R3G3B2)
    u8g.setColorIndex(255);//设置屏幕亮度
//设置颜色
  else if (u8g.getMode() == U8G_MODE_GRAY2BIT) u8g.setColorIndex(3);
  else if (u8g.getMode() == U8G_MODE_BW) u8g.setColorIndex(1);


  u8g.setFont(u8g_font_6x10);//设置字体大小
  u8g.setFontRefHeightExtendedText();//设置字体高度
  u8g.setDefaultForegroundColor();//设置默认的前景色
  u8g.setFontPosTop();//字体起始位置湿屏幕的顶部
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
  //oled的屏幕设置
  u8g.firstPage();
  do {
    draw();
  } while (u8g.nextPage());

}
