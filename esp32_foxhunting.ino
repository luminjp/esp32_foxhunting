#include <WiFi.h>
#include <DNSServer.h>
#include <esp_wifi.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
WiFiServer server(80);

// Captive Portal Example [change here]
String responseHTML = ""
  "<!DOCTYPE html><html><head><title>CaptivePortal</title></head><body>"
  "<h1>Dear Hunter!</h1><p>You can get this Access Point password. All requests will "
  "be redirected here.</p></body></html>";

int h2i(char c) {
  if ('0' <= c && c <= '9') return c - '0';
  else if ('a' <= c && c <= 'f') return c - 'a' + 10;
  else if ('A' <= c && c <= 'F') return c - 'A' + 10;
  else return 0;
}
byte pack_hex(char *s) {
  return (h2i(s[0]) << 4) | h2i(s[1]);
}
void pack_macaddress(char *src, byte *dst) {
  for (int i = 0; i < 6; i++) {
    dst[i] = pack_hex(&src[2*i]);
  }
}

void setup() {
  //SSID [change here]
  char ssid[] = "W03_DCD916F424F2";
  byte macaddress_bytes[6];
  // MAC address [change here]
  char *macaddress="DCD916F424F2";
  pack_macaddress(macaddress, macaddress_bytes);
  //password [change here]
  char pass[] = "Password";
  byte bssid[6];
  esp_base_mac_addr_set(macaddress_bytes);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid,pass,NULL,0,4);

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print(responseHTML);
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  }
}
