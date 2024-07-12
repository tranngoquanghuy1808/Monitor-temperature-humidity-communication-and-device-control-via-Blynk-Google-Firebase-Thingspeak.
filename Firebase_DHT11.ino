//------Thư viện cho Firebase, WiFi, DHT11------
#include <FirebaseESP32.h>
#include <WiFi.h>
#include "DHT.h"

#define FIREBASE_HOST "https://dht11-5d064-default-rtdb.firebaseio.com/"  //Phần này ta sẽ dán đường Link ở RTDB vào để esp32 kết nói với Firebase.
#define FIREBASE_AUTH "wrebF4jhIeNU4xS9ZdkG6fXFudie15CfwhdQ1OpY"          //Phần này ta dán mã AUTH (data secrets) vào.
#define WIFI_SSID "Wifi 903"                                              //Thay đổi tên WIFI của bạn.
#define WIFI_PASSWORD "0795843079"                                         //Thay đổi phần mật khẩu WIFI.
//---------Setup chân cho cảm biến DHT11----------
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); 

FirebaseData fbdo;  //Đặt tên cho dữ liệu là fbdo.

void setup() {
  Serial.begin(9600);  //Thiết lập tốc độ truyền nhận dữ liệu
  // -------Phần code kiểm tra kết nối Wifi---------
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  dht.begin();
  Serial.println("/n Da ket noi Wifi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  //Lệnh xuất địa chỉ IP của Wifi sau khi kết nối thành công.
                                   //-------Khởi động giao tiếp với GG Firebase với đường link và mã AUTH tương ứng-------
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
}

void loop() {
  //-----Đọc nhiệt độ độ ẩm từ cảm biến------
  float h = dht.readHumidity();//Đọc độ ẩm từ cảm biến DHT11 và gán giá trị cho biến h.
  float t = dht.readTemperature();//Đọc nhiệt độ từ cảm biến DHT11 và gán giá trị cho biến t.
  float f = dht.readTemperature(true);//Đọc nhiệt độ từ cảm biến DHT11 và gán giá trị cho biến f dưới định dạng Fahrenheit.

  if (isnan(h) || isnan(t) || isnan(f))//Kiểm tra nếu giá trị đọc từ cảm biến DHT11 không hợp lệ.
  {
  Serial.println("Ket noi DHT sensor that bai !!!");
  delay(1000);
    return;
  }
  //------Xuất kết nhiệt độ độ ẩm đã đo được ra màn hình Serial--------
 Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.println();
  //------Xuất giá trị đã đo được lên GG Firebase-------
  Firebase.setFloat(fbdo, "Nhiet Do", t);
  Firebase.setFloat(fbdo, "Do Am", h);
  delay(2000);
}
