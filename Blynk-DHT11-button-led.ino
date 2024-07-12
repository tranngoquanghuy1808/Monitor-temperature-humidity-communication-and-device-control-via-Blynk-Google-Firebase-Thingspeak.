#define BLYNK_TEMPLATE_ID "TMPL6boJbfsgF"
#define BLYNK_TEMPLATE_NAME "NHIET DO"
#define BLYNK_AUTH_TOKEN "q8dSW8RS10Ga4KgHesi9VFVnifkIEgUE"

#include <WiFi.h> //sử dụng hàm wifi để kết nối và làm việc với mạng wifi
#include <WiFiClient.h> //Sử dụng thư viện WiFiClient để tạo kết nối TCP/IP
#include <BlynkSimpleEsp32.h> //Sử dụng thư viện BlynkSimpleEsp32 để kết nối và giao tiếp với ứng dụng Blynk.
#include <string.h> //Sử dụng thư viện string.h để làm việc với chuỗi ký tự.

#include <DHT.h> //Sử dụng thư viện DHT để làm việc với cảm biến DHT.
#define DHTPIN 14   //Định nghĩa chân kết nối của cảm biến DHT11.
#define DHTTYPE DHT11   // there are multiple kinds of DHT sensors
DHT dht(DHTPIN, DHTTYPE); //Khởi tạo đối tượng cảm biến DHT11.

#define LED 27 //Định nghĩa chân kết nối của đèn LED.
WidgetLED LED_ON_APP(V2); //Khởi tạo đối tượng LED trên ứng dụng Blynk.
int button;//Khai báo biến button để lưu trạng thái của nút điều khiển trên ứng dụng Blynk.

char auth[] = BLYNK_AUTH_TOKEN;//Khai báo một mảng ký tự auth và gán giá trị của BLYNK_AUTH_TOKEN cho mảng này.
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Wifi 903";//Khai báo một mảng ký tự ssid và gán giá trị "dh" cho mảng này. Đây là tên của mạng WiFi.
char pass[] = "0795843079";//Khai báo một mảng ký tự pass và gán giá trị "11234566" cho mảng này. Đây là mật khẩu của mạng WiFi.


void setup() //
{
  pinMode(LED, OUTPUT);//Thiết lập chân kết nối của đèn LED là chế độ OUTPUT.
  Serial.begin(9600);//Khởi tạo giao tiếp Serial với tốc độ baud 921600.
  Serial.println("DHTxx test!");//in ra thông báo
  dht.begin(); //Khởi tạo cảm biến DHT11.
  Blynk.begin(auth, ssid, pass);//Kết nối với ứng dụng Blynk thông qua mạng WiFi.
}
BLYNK_WRITE(V3) // Hàm này được gọi khi giá trị của nút điều khiển trên ứng dụng Blynk thay đổi.

{
  button = param.asInt();//Gán giá trị của nút điều khiển cho biến button.
  if(button == 1)//kiểm tra nếu button bằng 1
   {
    digitalWrite(LED, HIGH);
    LED_ON_APP.on();
  } 
  else 
  {
    digitalWrite(LED, LOW);
    LED_ON_APP.off();
  }
}
void loop() {
  Blynk.run();
  // Wait a few seconds between measurements.
  delay(2000);
  float h = dht.readHumidity();//Đọc độ ẩm từ cảm biến DHT11 và gán giá trị cho biến h.
  float t = dht.readTemperature();//Đọc nhiệt độ từ cảm biến DHT11 và gán giá trị cho biến t.
  float f = dht.readTemperature(true);//Đọc nhiệt độ từ cảm biến DHT11 và gán giá trị cho biến f dưới định dạng Fahrenheit.

  if (isnan(h) || isnan(t) || isnan(f))//Kiểm tra nếu giá trị đọc từ cảm biến DHT11 không hợp lệ.
  {
Serial.println("Ket noi DHT sensor that bai !!!");
    return;
  }
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (Fahrenheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Blynk.virtualWrite(V0, t);//Gửi giá trị nhiệt độ tới ứng dụng Blynk.
  Blynk.virtualWrite(V1, h);//Gửi giá trị độ ẩm tới ứng dụng Blynk.
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("  Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
}

