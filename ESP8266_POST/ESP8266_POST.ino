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

const int suppliesButton = 4;
const int tempHotButton = 26;
const int tempColdButton = 32;
const int LED = 5;
long timePushed;
bool disabled0;
bool disabled1;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000;
const int   daylightOffset_sec = 3600;

SMTPData smtpData;

void sendCallback(SendStatus info);

void sendEmail(int LED, int msg);

String serverName = "http://192.168.2.57:8000/tickets/main";

void setup() {
  pinMode(suppliesButton, INPUT_PULLUP);
  pinMode(tempHotButton, INPUT_PULLUP);
  pinMode(tempColdButton, INPUT_PULLUP);
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

    if (digitalRead(suppliesButton) == false && !disabled0) {
      //if it is not disabled send email and disable
      sendEmail(LED, 0);
      postDataToServer();
      disabled0 = false;
      printLocalTime();
    }

    //if it is 6 oclock and disabled enable it
    if ((timeinfo.tm_hour == 06) && disabled0) {
      disabled0 = false;
      printLocalTime();
    }

    if (digitalRead(tempHotButton) == false) {
      checkenable();
      if (!disabled1) {
        sendEmail(LED, 1);
        disabled1 = true;
        timePushed = millis();
        printLocalTime();
      }
    }
    /*
        if (digitalRead(tempColdButton) == false) {
          checkenable();
          if (!disabled1) {
            sendEmail(LED, 2);
            disabled1 = true;
            timePushed = millis();
            printLocalTime();
          }
    */
  }
}

void checkenable() {
  //if 30 min pass and disabled, enable it
  if (((millis() > timePushed + 1800000) || millis() < timePushed) && disabled1) {
    disabled1 = false;
    printLocalTime();
  }
}

void postDataToServer() {
  HTTPClient http;
  Serial.println("Posting JSON data to server...");
  String serverpath = serverName + "?room=Hudson&request=Supplies&status=Not_Complete";
  
  http.begin(serverpath.c_str());  //Specify destination for HTTP request
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<200> ticket;
  // Add values in the document
  //
  ticket["room"] = "Hudson 110";
  ticket["request"] = "Supplies";
  ticket["status"] = "Not Complete";

 

  String requestBody;
  serializeJson(ticket, requestBody);

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {

    String response = http.getString();

    Serial.println(httpResponseCode);
    Serial.println(response);

  }
  
    else {
      Serial.printf("Error occurred while sending HTTP POST: %s\n", http.errorToString(httpResponseCode).c_str());

    }
  
}

void sendEmail(int LED, int msg) {
  digitalWrite(LED, HIGH);
  Serial.println("Preparing to send email");
  Serial.println();

  // Set the SMTP Server Email host, port, account and password
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);

  // Set the sender name and Email
  smtpData.setSender("ESP32", emailSenderAccount);

  // Set Email priority or importance High, Nosrmal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");

  // Set the subject
  smtpData.setSubject(emailSubject);

  // Set the message with HTML format
  if (msg == 0) {
    smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Supplies Request</h1><p>- WE NEED MARKERS (sent from the POD)</p></div>", true);
  }
  if (msg == 1) {
    smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Temperature Request</h1><p>- ITS HOT IN HERE (sent from the POD)</p></div>", true);
  }

  if (msg == 2) {
    smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Temperature Request</h1><p>- ITS COLD IN HERE (sent from the POD)</p></div>", true);
  }

  // Add recipients, you can add more than one recipient
  smtpData.addRecipient(emailRecipient);

  smtpData.setSendCallback(sendCallback);

  //Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

  //Clear all data from Email object to free memory
  smtpData.empty();
  delay(1500);
  digitalWrite(LED, LOW);
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
