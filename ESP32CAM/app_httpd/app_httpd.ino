#include "esp_camera.h"
#include <WiFi.h>
#include <ArduinoOTA.h>

/* Access Point Credentials */
const char *ap_ssid = "ESP32";      // set the SSID for your access point
const char *ap_password = "12345678";         // set the password for your access point

/* define CAMERA_MODEL_AI_THINKER */
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

/* ... (existing code) */
extern int IN1 = 14;
extern int IN2 = 15;
extern int IN3 = 13;
extern int IN4 = 12;

extern int ledVal = 20;
extern int ledPin = 4;
extern int servoPin = 2;

unsigned long previousMillis = 0;
void startCameraServer();

void initServo() {
  ledcSetup(8, 50, 16); /*50 hz PWM, 16-bit resolution and range from 3250 to 6500 */
  ledcAttachPin(servoPin, 8);
}

void initLed() {
  ledcSetup(7, 5000, 8); /* 5000 hz PWM, 8-bit resolution and range from 0 to 255 */
  ledcAttachPin(ledPin, 7);
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  Serial.println("ESP32 Camera Remote Control - L298N Motor Driver");
  Serial.println("-------------------------------------------------------");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ledPin, OUTPUT); 
  pinMode(servoPin, OUTPUT); 

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(ledPin, LOW);
  digitalWrite(servoPin, LOW);

  initServo();
  initLed();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_CIF);

  char chip_id[15];
  snprintf(chip_id, 15, "%04X", (uint16_t)(ESP.getEfuseMac() >> 32));
  String hostname = "esp32cam-" + String(chip_id);

  Serial.println();
  Serial.println("Hostname: " + hostname);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);

  IPAddress myIP;
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi-STA-Mode");
    Serial.print("IP: ");
    myIP = WiFi.localIP();
    Serial.println(myIP);
    delay(2000);
  } else {
    myIP = WiFi.softAPIP();
    Serial.println("");
    Serial.println("WiFi failed connected to " + String(ap_ssid));
    Serial.println("");
    Serial.println("WiFi-AP-Mode");
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    delay(2000);
  }

  startCameraServer();
  Serial.print("Camera Ready! Use 'http://");
  Serial.print(myIP);
  Serial.println("' to connect ");

  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();
}
