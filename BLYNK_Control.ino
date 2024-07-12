//Define Blynk
#define BLYNK_TEMPLATE_ID "TMPL6P30d4_T7"
#define BLYNK_TEMPLATE_NAME "THEODOINHIETDO"
#define BLYNK_AUTH_TOKEN "JX5G58a8GriY07X448pMqLHehnsYPeZQ"
char auth[] = BLYNK_AUTH_TOKEN;//Khai báo một mảng ký tự auth và gán giá trị của BLYNK_AUTH_TOKEN cho mảng này.

#include <WiFi.h> //sử dụng hàm wifi để kết nối và làm việc với mạng wifi
#include <WiFiClient.h> //Sử dụng thư viện WiFiClient để tạo kết nối TCP/IP
#include <BlynkSimpleEsp32.h> //Sử dụng thư viện BlynkSimpleEsp32 để kết nối và giao tiếp với ứng dụng Blynk.
#include <string.h> //Sử dụng thư viện string.h để làm việc với chuỗi ký tự.

//Lib DHT11 and define DHT11
#include <DHT.h> //Sử dụng thư viện DHT để làm việc với cảm biến DHT.
#define DHTPIN 14   //Định nghĩa chân kết nối của cảm biến DHT11.
#define DHTTYPE DHT11   // there are multiple kinds of DHT sensors
DHT dht(DHTPIN, DHTTYPE); //Khởi tạo đối tượng cảm biến DHT11.

// Define LEDs
#define LED_RED 2
#define LED_GREEN 15
#define LED_BLUE 4
WidgetLED LED_RED_ON_APP(V1); // Khởi tạo một đối tượng LED ảo trong thư viện Blynk
WidgetLED LED_GREEN_ON_APP(V3); // Khởi tạo một đối tượng LED ảo trong thư viện Blynk
WidgetLED LED_BLUE_ON_APP(V2);

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Wifi 903";//Khai báo một mảng ký tự ssid và gán giá trị "dh" cho mảng này. Đây là tên của mạng WiFi.
char pass[] = "0795843079";

void setup()
{
  Serial.begin(9600);
  // Debug console
  pinMode(LED_RED, OUTPUT); // Đặt led đỏ làm chân đầu ra
  pinMode(LED_GREEN, OUTPUT); //Đặt led xanh làm chân đầu ra
  pinMode(LED_BLUE, OUTPUT);
  dht.begin(); // khởi tạo cảm biến DHT

  Blynk.begin(auth, ssid, pass); //kết nối mạng wifi và máy chủ blynk
  Serial.println("DHTxx test!");//in ra thông báo
}

void loop()
{
  Blynk.run(); // dùng để duy trì giao tiếp với máy chủ Blynk
  float h = dht.readHumidity();//Đọc độ ẩm từ cảm biến DHT11 và gán giá trị cho biến h.
  float t = dht.readTemperature();//Đọc nhiệt độ từ cảm biến DHT11 và gán giá trị cho biến t.
  float f = dht.readTemperature(true);//Đọc nhiệt độ từ cảm biến DHT11 và gán giá trị cho biến f dưới định dạng Fahrenheit.
  // Check isRead ?
 if (isnan(h) || isnan(t) || isnan(f))//Kiểm tra nếu giá trị đọc từ cảm biến DHT11 không hợp lệ.
  {
Serial.println("Ket noi DHT sensor that bai !!!");
    return;
  }

  Blynk.virtualWrite(V0, t); //Nếu việc đọc nhiệt độ thành công, nó gửi các giá trị này đến ứng dụng Blynk
  Blynk.virtualWrite(V4, h); //Nếu việc đọc độ ẩm thành công, nó gửi các giá trị này đền ứng dụng Blynk

  Serial.print("\n");
  Serial.print("Humidity: " + String(h) + "%");  //in ra độ ẩm ra màn hình
  Serial.print("\t");
  Serial.print("\t");
  Serial.print("Temperature:" + String(t) + " C"); //in ra nhiệt độ ra màn hình
  delay(1000);

  // Check temperature thresholds
  if (t >35 ) { // nếu nhiệt độ dưới ngưỡng mức cao
    digitalWrite(LED_RED, HIGH); // led đỏ được đưa lên mức 1
    LED_RED_ON_APP.on();  // led đỏ bật
  } else {
    digitalWrite(LED_RED, LOW); // ngược lại nếu trên ngưỡng mức thấp thì đưa led đỏ về mức thấp
    LED_RED_ON_APP.off(); // led đỏ tắt
  }

  if (t<35 && t>=15) { // nếu nhiệt độ trên ngưỡng mức cao
  digitalWrite(LED_GREEN, HIGH); // led xanh được đưa lên mức 1
    LED_GREEN_ON_APP.on(); // led xanh bật
  } else {
    digitalWrite(LED_GREEN, LOW); // ngược lại nếu dưới ngưỡng mức cao thì đưa led xanh về mức thấp
    LED_GREEN_ON_APP.off(); // led xanh tắt
  }
  if (t<15) { // nếu nhiệt độ trên ngưỡng mức cao
  digitalWrite(LED_BLUE, HIGH); // led xanh được đưa lên mức 1
    LED_BLUE_ON_APP.on(); // led xanh bật
  } else {
    digitalWrite(LED_BLUE, LOW); // ngược lại nếu dưới ngưỡng mức cao thì đưa led xanh về mức thấp
    LED_BLUE_ON_APP.off(); // led xanh tắt
  }

  delay(1000);
}
