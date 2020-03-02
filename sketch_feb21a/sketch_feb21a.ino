/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-send-email-smtp-server-arduino-ide/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
  
 
#include "ESP32_MailClient.h"
#include <WiFi.h>
#include "time.h"

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "DukeVisitor";
const char* password = "";

// To send Email using Gmail use port 465 (SSL) and SMTP Server smtp.gmail.com
// YOU MUST ENABLE less secure app option https://myaccount.google.com/lesssecureapps?pli=1
#define emailSenderAccount    "suppliesdetect@gmail.com"
#define emailSenderPassword   "missingtooth"
#define emailRecipient        "suppliesdetect@gmail.com"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "ESP32 Test"

const int suppliesButton = 13;
const int suppliesLED = 23;
const int tempButton = 26;
const int tempLED = 5;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000;
const int   daylightOffset_sec = 3600;


// The Email Sending data object contains config and data to send
SMTPData smtpData;

// Callback function to get the Email sending status
void sendCallback(SendStatus info);

void sendEmail(int button, int LED, int msg);

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup() {
  pinMode(suppliesButton, INPUT_PULLUP);
  pinMode(tempButton, INPUT_PULLUP);
  pinMode(suppliesLED, OUTPUT);
  pinMode(tempLED, OUTPUT);
  digitalWrite(23, LOW);
  digitalWrite(5, LOW);
  Serial.begin(115200);
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
  delay(1000);
  printLocalTime();
  
  if (digitalRead(suppliesButton) == false) {
    sendEmail(suppliesLED, 0);
  }
  else if (digitalRead(tempButton) == false) {
    sendEmail(tempLED, 1);
  }
}

void sendEmail(int LED, int msg) {
  Serial.println("Preparing to send email");
  Serial.println();

  // Set the SMTP Server Email host, port, account and password
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);

  // For library version 1.2.0 and later which STARTTLS protocol was supported,the STARTTLS will be
  // enabled automatically when port 587 was used, or enable it manually using setSTARTTLS function.
  //smtpData.setSTARTTLS(true);

  // Set the sender name and Email
  smtpData.setSender("ESP32", emailSenderAccount);

  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");

  // Set the subject
  smtpData.setSubject(emailSubject);

  // Set the message with HTML format
  if (msg == 0) {
    smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Supplies Request</h1><p>- WE NEED MARKERS (sent from the POD)</p></div>", true);
    // Set the email message in text format (raw)
    //smtpData.setMessage("WE NEED MARKERS! -sent from the POD", false);
  }
  if (msg == 1) {
    smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Temperature Request</h1><p>- ITS HOT IN HERE (sent from the POD)</p></div>", true);
    // Set the email message in text format (raw)
    //smtpData.setMessage("WE NEED MARKERS! -sent from the POD", false);
  }

  // Add recipients, you can add more than one recipient
  smtpData.addRecipient(emailRecipient);
  //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");

  smtpData.setSendCallback(sendCallback);

  //Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());

  //Clear all data from Email object to free memory
  smtpData.empty();
  digitalWrite(LED, HIGH);
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
