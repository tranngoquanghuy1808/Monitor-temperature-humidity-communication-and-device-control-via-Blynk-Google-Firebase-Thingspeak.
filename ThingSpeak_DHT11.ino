//Thư viện cần thiết
#include <WiFi.h>
#include "ThingSpeak.h"//CƠ SỞ VÀ ỨNG DỤNG IOTS - ITFA436064
#include <DHT.h>

#define WIFI_SSID "dh" //Nhập tên WIFi ESP32 kết nối
#define WIFI_PASSWORD "11234566" //Nhập mật khẩu của WIFI

#define SECRET_CH_ID 2369581 //Nhập channel number trên Thingspeak của bạn
#define SECRET_WRITE_APIKEY "9GLFONVGQNTC21ES" //Nhập mã WRITE API KEY của Thingspeak channel
//Nut an
#define BUTTON_PIN 14 
 int input_state = HIGH;
 bool ledState = 0;
//OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Khởi tạo DHT11
#define DHTPIN 4
#define DHTTYPE DHT11
  DHT dht(DHTPIN, DHTTYPE);
#define LED_RED 2

  WiFiClient client;
  unsigned long myChannelNumber = SECRET_CH_ID;
  const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

  unsigned long dataMillis = 0; //biến dùng điều khiển delay

void setup() {
  Serial.begin(9600); //khởi tạo giá trị baund
  pinMode(LED_RED, OUTPUT); // Đặt led đỏ làm chân đầu ra

  delay(1000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //kết nối với WIFI
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) //Đợi khi chưa kết nối được
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  dht.begin();

  ThingSpeak.begin(client); // Khởi tạo Thinkspeak
}

void loop() {
         

  float h = dht.readHumidity(); //Đọc độ ẩm
  float t = dht.readTemperature(); // Đọc nhiệt độ theo độ C
  if (isnan(h) || isnan(t)) { //Kiểm tra nếu như không đọc được 1 trong 2 dữ liệu nhiệt độ và độ ẩm
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
//Ghi dữ liệu đọc được trên Termial
  Serial.print("Nhiet do: ");
  Serial.print(t);
  Serial.print("*C ");
  Serial.print("Do am: ");
  Serial.print(h);
  Serial.println("% ");
  if (t>10 && t<30) { // nếu nhiệt độ dưới ngưỡng mức cao
    digitalWrite(LED_RED, HIGH); // led đỏ được đưa lên mức 1
  }
  
  ThingSpeak.setField(1, t); //Gửi dữ liệu về Field 1 trên Thingspeak
  ThingSpeak.setField(2, h); //Gửi dữ liệu về Field 2 trên Thinkspeak
  if (millis() - dataMillis > 20000){ //Delay cập nhật dữ liệu 20s (Thingspeak chỉ cho phép cập nhật mỗi 15s)
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); //Ghi dữ liệu lên Thingspeak
  if(x == 200){ //Nếu trả về code 200 == Gửi thành công
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x)); //Gửi không thành công khichưa đủ delay 20s
    }
  }

}
