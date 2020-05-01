#include <ArduinoJson.h>
#include "ESP32_MailClient.h"
#include <WiFi.h>
#include "time.h"
#include <HTTPClient.h>

const char* ssid = "SummitNG";
const char* password = "ewolram1";

#define emailSenderAccount    "suppliesdetect@gmail.com"
#define emailSenderPassword   "missingtooth"
#define emailRecipient        "suppliesdetect@gmail.com"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "ESP32 Test"

const int supWBMarkersBut = 4;
const int supWBErasersBut = 8;
const int supBBChalkBut = 2;
const int supBBErasersBut = 6;
const int tHotBut = 26;
const int tColdBut = 32;
const int genBut = 13;

const int LED = 5;

long timePushed;
bool disabled0;
bool disabled1;

const int supWBMarkers = 0;
const int supWBErasers = 1;
const int supBBChalk = 2;
const int supBBErasers = 3;
const int tHot = 4;
const int tCold = 5;
const int gen = 6;

String room = "Hudson%20110";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000;
const int   daylightOffset_sec = 3600;

SMTPData smtpData;

void sendCallback(SendStatus info);

void emailandGET(int LED, int msg);

void printLocalTime();

void checkenable();

String serverName = "http://192.168.2.57:8000/tickets/main";

void setup() {
  pinMode(supWBMarkersBut, INPUT_PULLUP);
  pinMode(supWBErasersBut, INPUT_PULLUP);
  //pinMode(supBBMarkersBut, INPUT_PULLUP);
  //pinMode(supBBErasersBut, INPUT_PULLUP);
  pinMode(tHotBut, INPUT_PULLUP);
  pinMode(tColdBut, INPUT_PULLUP);
  pinMode(genBut, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(5, LOW);
  disabled0 = false;
  disabled1 = false;
  Serial.begin(115200);

  delay(4000);

  Serial.println();
  Serial.print("Connecting");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println();

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
}

void loop() {
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    if ((timeinfo.tm_hour == 06) && disabled0) {
      disabled0 = false;
    }

    if (digitalRead(supWBMarkersBut) == false ){//&& !disabled0) {
      emailandGET(LED, supWBMarkers);
      disabled0 = true;
    }

    if (digitalRead(supWBErasersBut) == false && !disabled0) {
      emailandGET(LED, supWBErasers);
      disabled0 = true;
    }

    /*    if (digitalRead(supBBMarkersBut) == false && !disabled0) {
          emailandGET(LED, supBBMarkers);
          disabled0 = true;
        }
    */

    /*    if (digitalRead(supBBErasersBut) == false && !disabled0) {
          emailandGET(LED, supBBErasers);
          disabled0 = true;
        }
    */

    if (digitalRead(tHotBut) == false) {
      checkenable();
      if (!disabled1) {
        emailandGET(LED, tHot);
        disabled1 = true;
        timePushed = millis();
      }
    }
    if (digitalRead(tColdBut) == false) {
      checkenable();
      if (!disabled1) {
        emailandGET(LED, tCold);
        disabled1 = true;
        timePushed = millis();
      }
    }
    if (digitalRead(genBut) == false && !disabled0) {
      emailandGET(LED, gen);
      disabled0 = true;
    }
  }
}
void emailandGET(int LED, int msg) {
  digitalWrite(LED, HIGH);
  Serial.println("Preparing to send email");
  Serial.println();

  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
  smtpData.setSender("ESP32", emailSenderAccount);
  smtpData.setPriority("High");
  smtpData.setSubject(emailSubject);

  HTTPClient http;
  String serverpath;
  Serial.println("Posting JSON data to server...");

  if (msg == 0) {
    smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Whiteboard Supplies Request</h1><p>- We need markers for whiteboard</p></div>", true);
    serverpath = serverName + "?room=" + room + "&request=Supplies%20-%20Whiteboard&status=Not%20Complete";
  }

  if (msg == 1) {
    smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Whiteboard Supplies Request</h1><p>- We need erasers for whiteboard</p></div>", true);
    serverpath = serverName + "?room=" + room + "&request=Supplies%20-%20Whiteboard&status=Not%20Complete";
  }

  if (msg == 2) {
    smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Blackboard Supplies Request</h1><p>- We need chalk for blackboard</p></div>", true);
    serverpath = serverName + "?room=" + room + "&request=Supplies%20-%20Blackboard&status=Not%20Complete";
  }

  if (msg == 3) {
    smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Blackboard Supplies Request</h1><p>- We need erasers for blackboard</p></div>", true);
    serverpath = serverName + "?room=" + room + "&request=Supplies%20-%20Blackboard&status=Not%20Complete";
  }


  if (msg == 4) {
    smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Temperature Request</h1><p>- It is too hot in here</p></div>", true);
    serverpath = serverName + "?room=" + room + "&request=Temperature%20-%20Too%20Hot&status=Not%20Complete";
  }

  if (msg == 5) {
    smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Temperature Request</h1><p>- It is too cold in here</p></div>", true);
    serverpath = serverName + "?room=" + room + "&request=Temperature%20-%20Too%20Cold%20&status=Not%20Complete";
  }

  if (msg == 6) {
    smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>General Request</h1><p>- This is a general maintainance request</p></div>", true);
    serverpath = serverName + "?room=" + room + "&request=General&status=Not%20Complete";
  }


  http.begin(serverpath.c_str());  //Specify destination for HTTP request
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(response);
  }

  else {
    Serial.printf("Error occurred while sending HTTP GET: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  smtpData.addRecipient(emailRecipient);
  smtpData.setSendCallback(sendCallback);

  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

  smtpData.empty();
  delay(1500);
  digitalWrite(LED, LOW);
}

void checkenable() {
  //if 30 min pass and disabled, enable it
  if (((millis() > timePushed + 1800000) || millis() < timePushed) && disabled1) {
    disabled1 = false;
  }
}

void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

// Callback function to get the Email sending status
void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());

  // Do something when complete
  if (msg.success()) {
    Serial.println("----------------");
  }
}
