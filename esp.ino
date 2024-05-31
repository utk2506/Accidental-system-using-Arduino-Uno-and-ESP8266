#include <ESP8266WiFi.h>

char buffer[100] = "";
WiFiClient client;

// Your WiFi credentials.
char ssid[] = "DESKTOP-JMTJ8LU 7086";
char pass[] = "Utk25062001";

String MakerIFTTT_Key = "b5hcyIEEPXr_mBY02anXfRyMiy7Y_Z1AtrgiVlXZo3C";
String MakerIFTTT_Event = "Esp8266_Call";

char* append_str(char* here, String s) {
  int i = 0;
  while (*here++ = s[i++]);
  return here - 1;
}

char* append_ul(char* here, unsigned long u) {
  char buf[20];
  return append_str(here, ultoa(u, buf, 10));
}

char post_rqst[256];
char* p;
char* content_length_here;
char* json_start;
int compi;


void setup() {
  Serial.begin(9600);

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Main code to run repeatedly
   
  if (Serial.available() > 0) {
    Serial.readBytesUntil('\n', buffer, 100);
    Serial.println(buffer);

    // Trigger IFTTT event with received data
    if (client.connect("maker.ifttt.com", 80)) {
      p = post_rqst;
      p = append_str(p, "POST /trigger/");
      p = append_str(p, MakerIFTTT_Event);
      p = append_str(p, "/with/key/");
      p = append_str(p, MakerIFTTT_Key);
      p = append_str(p, " HTTP/1.1\r\n");
      p = append_str(p, "Host: maker.ifttt.com\r\n");
      p = append_str(p, "Content-Type: application/json\r\n");
      p = append_str(p, "Content-Length: ");
      content_length_here = p;
      p = append_str(p, "NN\r\n");
      p = append_str(p, "\r\n");
      json_start = p;
      p = append_str(p, "{\"value1\":\"");
      p = append_str(p, buffer);  // Append the received data
      p = append_str(p, "\"}");

      compi = strlen(json_start);
      content_length_here[0] = '0' + (compi / 10);
      content_length_here[1] = '0' + (compi % 10);
      client.print(post_rqst);
      Serial.println("IFTTT Event Triggered");
    } else {
      Serial.println("IFTTT Connection Failed");
    }
  }
}
