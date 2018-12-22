#define INTERVAL_SENSOR   40000             //定义传感器采样时间间隔  597000
#define INTERVAL_NET      40000             //定义发送时间
//传感器部分================================   
#include <Wire.h>                                  //调用库  
#include "./ESP8266.h"
#include "I2Cdev.h"                                //调用库  
//温湿度   
#include "./SHT2x.h"
//光照
#define  sensorPin_1  D6
#define  sensorPin_2  D8
#define  sensorPin_3  D10

#define SSID           "DABAOSHENG"                   // cannot be longer than 32 characters!
#define PASSWORD       "YUbaosheng"

#define IDLE_TIMEOUT_MS  3000      // Amount of time to wait (in milliseconds) with no data 
                                   // received before closing the connection.  If you know the server
                                   // you're accessing is quick to respond, you can reduce this value.

// WEBSITE

#define HOST_NAME   "api.heclouds.com"

// 修改成自己的ID
#define DEVICEID   "505040475"
#define PROJECTID "183400"
#define HOST_PORT   (80)

// 修改成自己在OneNet上面的apikey
// 大概长这样: VeFI0HZ44Qn5dZO14AuLbWSlSlI=
String apiKey="F5eDYem6rDzxPrxILebj21vprxk=";
char buf[10];

#define INTERVAL_sensor 2000
unsigned long sensorlastTime = millis();



#define INTERVAL_OLED 1000

String mCottenData;
String jsonToSend;

//3,传感器值的设置 
float sensor_tem, sensor_hum, sensor_pirSupermarket,sensor_pirBarberShop,sensor_pirCopyShop,densityS=0,densityC=0,densityB=0;                    //传感器温度、湿度、光照   
char  sensor_tem_c[7], sensor_hum_c[7], sensor_pirSupermarket_c[7],sensor_pirBarberShop_c[7],sensor_pirCopyShop_c[7] ;    //换成char数组传输

#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D3, TX:D2 */
ESP8266 wifi(mySerial);
//ESP8266 wifi(Serial1);                                      //定义一个ESP8266（wifi）的对象
unsigned long net_time1 = millis();                          //数据上传服务器时间
unsigned long sensor_time = millis();                        //传感器采样时间计时器

//int SensorData;                                   //用于存储传感器数据
String postString;                                //用于存储发送数据的字符串
//String jsonToSend;                                //用于存储发送的json格式参数

void setup(void)     //初始化函数  
{       
  //初始化串口波特率  
    Wire.begin();
    Serial.begin(115200);   
    while(!Serial);
    pinMode(sensorPin_1, INPUT);
    pinMode(sensorPin_2, INPUT);
    pinMode(sensorPin_3, INPUT);

   //ESP8266初始化
    Serial.print("setup begin\r\n");   

  Serial.print("FW Version:");
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print("to station + softap ok\r\n");
  } else {
    Serial.print("to station + softap err\r\n");
  }

  if (wifi.joinAP(SSID, PASSWORD)) {      //加入无线网
    Serial.print("Join AP success\r\n");  
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }

  if (wifi.disableMUX()) {
    Serial.print("single ok\r\n");
  } else {
    Serial.print("single err\r\n");
  }

  Serial.print("setup end\r\n");
    
  
}
void loop(void)     //循环函数  
{   sensor_pirSupermarket = digitalRead(D6);                //读取数据
    sensor_pirBarberShop = digitalRead(D8);
    sensor_pirCopyShop = digitalRead(D10);
    if(sensor_pirSupermarket==1)densityS+=1;               //如果有人，则在一定时间内持续加一，相对表达人流量
    if(sensor_pirBarberShop==1)densityB+=1;
    if(sensor_pirCopyShop==1)densityC+=1;
    delay(50);
  if (sensor_time > millis())  sensor_time = millis();  
    
  if(millis() - sensor_time > INTERVAL_SENSOR)              //传感器采样时间间隔  
  {  
    getSensorData();                                        //读串口中的传感器数据
    sensor_time = millis();
  }  

    
  if (net_time1 > millis())  net_time1 = millis();
  
  if (millis() - net_time1 > INTERVAL_NET)                  //发送数据时间间隔
  {                
    updateSensorData(); 
    densityS=0,densityC=0,densityB=0;//将数据上传到服务器的函数
    net_time1 = millis();
  }
  
}

void getSensorData(){  
    sensor_tem = SHT2x.GetTemperature() ;   
    sensor_hum = SHT2x.GetHumidity();   
    //获取光照
    
    delay(1000);
    dtostrf(sensor_tem, 2, 1, sensor_tem_c);
    dtostrf(sensor_hum, 2, 1, sensor_hum_c);
    dtostrf(densityS, 3, 1, sensor_pirSupermarket_c);
    dtostrf(densityB, 3, 1, sensor_pirBarberShop_c);
    dtostrf(densityC, 3, 1, sensor_pirCopyShop_c);
}
void updateSensorData() {
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
    Serial.print("create tcp ok\r\n");

    jsonToSend="{\"Temperature\":";
    dtostrf(sensor_tem,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+=",\"Humidity\":";
    dtostrf(sensor_hum,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+=",\"pirSupermarket\":";
    dtostrf(densityS,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+=",\"pirBarberShop\":";
    dtostrf(densityB,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+=",\"pirCopyShop\":";
    dtostrf(densityC,1,2,buf);
    jsonToSend+="\""+String(buf)+"\"";
    jsonToSend+="}";



    postString="POST /devices/";
    postString+=DEVICEID;
    postString+="/datapoints?type=3 HTTP/1.1";
    postString+="\r\n";
    postString+="api-key:";
    postString+=apiKey;
    postString+="\r\n";
    postString+="Host:api.heclouds.com\r\n";
    postString+="Connection:close\r\n";
    postString+="Content-Length:";
    postString+=jsonToSend.length();
    postString+="\r\n";
    postString+="\r\n";
    postString+=jsonToSend;
    postString+="\r\n";
    postString+="\r\n";
    postString+="\r\n";

  const char *postArray = postString.c_str();                 //将str转化为char数组
  Serial.println(postArray);
  wifi.send((const uint8_t*)postArray, strlen(postArray));    //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
  Serial.println("send success");   
     if (wifi.releaseTCP()) {                                 //释放TCP连接
        Serial.print("release tcp ok\r\n");
        } 
     else {
        Serial.print("release tcp err\r\n");
        }
      postArray = NULL;                                       //清空数组，等待下次传输数据
  
  } else {
    Serial.print("create tcp err\r\n");
  }
  
}
