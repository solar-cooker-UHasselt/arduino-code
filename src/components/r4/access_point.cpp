#include "WiFiS3.h"
#include <vector>

void printWiFiStatus();
void displayConnectedClients(WiFiClient client);

// Structure to hold client information
struct ClientInfo {
  IPAddress ip;
};

std::vector<ClientInfo> connectedClients;

// Network credentials
char ssid[] = "solar-cooker"; // your network SSID (name)
char pass[] = ""; // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0; // your network key index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Access Point Web Server");

  // Check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // By default the local IP address will be 192.168.4.1
  // You can override it with the following:
  WiFi.config(IPAddress(192, 168, 100, 1));

  // Print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // Wait 10 seconds for connection:
  delay(10000);

  // Start the web server on port 80
  server.begin();

  // You're connected now, so print out the status
  printWiFiStatus();
}

void loop() {
  // Compare the previous status to the current status
  if (status != WiFi.status()) {
    // It has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      // A device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // A device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

  WiFiClient client = server.available(); // Listen for incoming clients

  if (client) { // If you get a client,
    Serial.println("New client"); // Print a message out the serial port
    IPAddress clientIP = client.remoteIP();

    // Check if the client is already in the list
    bool clientExists = false;
    for (auto& c : connectedClients) {
      if (c.ip == clientIP) {
        clientExists = true;
        break;
      }
    }

    // If the client is not in the list, add it
    if (!clientExists) {
      ClientInfo newClient;
      newClient.ip = clientIP;
      connectedClients.push_back(newClient);
    }

    String currentLine = ""; // Make a String to hold incoming data from the client
    while (client.connected()) { // Loop while the client's connected
      if (client.available()) { // If there's bytes to read from the client,
        char c = client.read(); // Read a byte, then
        Serial.write(c); // Print it out to the serial monitor
        if (c == '\n') { // If the byte is a newline character

          // If the current line is blank, you got two newline characters in a row.
          // That's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<h1>Connected Clients</h1>");
            displayConnectedClients(client);
            client.println();
            // Break out of the while loop:
            break;
          } else { // If you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') { // If you got anything else but a carriage return character,
          currentLine += c; // Add it to the end of the currentLine
        }
      }
    }
    // Close the connection:
    client.stop();
    Serial.println("Client disconnected");
  }
}

void printWiFiStatus() {
  // Print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void displayConnectedClients(WiFiClient client) {
  for (auto& c : connectedClients) {
    client.print("<p>Client IP: ");
    client.print(c.ip);
    client.println("</p>");
  }
}
