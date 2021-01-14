#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);

WiFiServer myServer(80);

const String ssid = "Mh_Mi2N";
const String pass = "87654321";


#define ana A0
#define sw1 D5
#define buz D7
#define sw2 D6
#define red D4
#define blu D0
#define gre D3
#define rel D8

bool ac = true;

void setup() {
  pinMode (ana, OUTPUT);
  pinMode (sw1, INPUT);
  pinMode (buz, OUTPUT);
  pinMode (sw2, INPUT);
  pinMode (red, OUTPUT);
  pinMode (blu, OUTPUT);
  pinMode (gre, OUTPUT);
  pinMode (rel, OUTPUT);

  Serial.begin(115200);

  lcd.begin();
  lcd.clear();

  digitalWrite(buz, 1);
  lcd.setCursor(1, 0);
  lcd.print("Mahadi Hassan");
  lcd.setCursor(4, 1);
  lcd.print("MITHUN");
  delay(700);
  lcd.clear();
  digitalWrite(buz, 0);


  disp(500);
  buzz(100);

  lcd.setCursor(3, 0);
  lcd.print("Access Point");
  lcd.setCursor(3, 1);
  lcd.print("Client Server");

  while (!digitalRead(sw1)) {
    if (digitalRead(sw2)) {
      buzz(100);
      ac = !ac;
    }
    if (ac) {
      lcd.setCursor(0, 0);
      lcd.print("#");
      lcd.setCursor(0, 1);
      lcd.print(" ");

    }
    else {
      lcd.setCursor(0, 0);
      lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print("#");
    }
    delay(50);
  }
  budi(300);


  //--------------------------ap(setUp)---------
  if (ac) {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, pass);

    Serial.print("\nAP IP address: ");
    Serial.println(WiFi.softAPIP());
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("IP Address:");
    lcd.setCursor(2, 1);
    lcd.print(WiFi.softAPIP());

    while (!digitalRead(sw1)) {
      delay(100);
    }
    // Starting WEB-server
    server.on ( "/", HTTP_handleRoot );
    server.onNotFound ( HTTP_handleRoot );
    server.begin();
  }

  //  ------------------------cs(setUp)-------------
  else {
    WiFi.begin(ssid, pass);
    Serial.println();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wi-Fi Connecting");
    lcd.setCursor(0, 1);
    delay(500);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");

      if (i == 8) {
        i = 0;
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
      }
      lcd.print(". ");
      buzz(200);
      delay(300);
      i++;
    }

    budi(700);

    Serial.println();
    Serial.println(WiFi.localIP());

    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("IP Address:");
    lcd.setCursor(1, 1);
    lcd.print(WiFi.localIP());


    while (!digitalRead(sw1)) {
      delay(100);
    }

    myServer.begin();


  }


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Web Server Being");
  lcd.setCursor(0, 1);
  for (int i = 0; i <= 8; i++) {
    lcd.print(". ");
    buzz(30);
    delay(30);
  }
  budi(700);
  lcd.clear();

  lcd.setCursor(0, 1);
  lcd.print("Transmit: ");
  lcd.setCursor(0, 0);
  lcd.print("Received: x_xx");

}

void loop() {
  String str = "";

  while (ac) { //------------------ap----------------------

    server.handleClient();
    if (server.hasArg("m2n")) {
      str = server.arg("m2n");
      Serial.println(str);

      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("Received: " + str);
      delay(100);

      execute(str);
    }

    lcd.setCursor(0, 1);
    lcd.print("\t\t\t\t");
    lcd.setCursor(0, 1);
    lcd.print("Transmit: " + (String) digitalRead(sw1) + " " + (String) digitalRead(sw2) + " " + (String) map(analogRead(ana), 0, 1024, 10, 99));
    delay(100);

  }
  while (!ac) { //-------------client server---------------

    WiFiClient client = myServer.available();
    if (client) {
      String str = client.readStringUntil('\n');
      str = str.substring(5, str.length() - 10);
      if (str == "favicon.ico") {
        return;
      }
      Serial.println(str);

      client.println("<!DOCTYPE HTML>");
      client.println("<HTML>");
      client.println("<H1 style='color:blue;FONT-SIZE: 50px;'> LED CONTROL Develop By Mithun </H1>");
      client.println("<br />");

      if (digitalRead(sw1)) {
        client.println("<H2> M Switch ON </H2>");

      }
      else {
        client.println("<H2> M Switch OFF </H2>");
      }
      
      if (digitalRead(sw2)) {
        client.println("<H2> N Switch ON </H2>");

      }
      else {
        client.println("<H2> N Switch OFF </H2>");
      }

      client.println("<H2> Pot: "+(String)analogRead(ana)+"</H2>");
      
      
      client.println("<a href=\"/a\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px;WIDTH: 300px; 126px; Margin-right: 10px; TOP: 200px;'> LED ON </button> </a>");
      client.println("<a href=\"/x\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px; WIDTH: 300px; 126px; Margin-left: 100px; TOP: 200px;'> LED OFF </button> </a>");
      client.println("<a href=\"/b\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px;WIDTH: 300px; 126px; Margin-right: 10px; TOP: 200px;'> LED ON </button> </a>");
      client.println("<a href=\"/y\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px; WIDTH: 300px; 126px; Margin-left: 100px; TOP: 200px;'> LED OFF </button> </a>");
      client.println("<a href=\"/c\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px;WIDTH: 300px; 126px; Margin-right: 10px; TOP: 200px;'> LED ON </button> </a>");
      client.println("<a href=\"/z\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px; WIDTH: 300px; 126px; Margin-left: 100px; TOP: 200px;'> LED OFF </button> </a>");
      client.println("</html>");

      delay(1);

      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("Received: " + str);
      delay(100);

      execute(str);
    }

    lcd.setCursor(0, 1);
    lcd.print("\t\t\t\t");
    lcd.setCursor(0, 1);
    lcd.print("Transmit: " + (String) digitalRead(sw1) + " " + (String) digitalRead(sw2) + " " + (String) map(analogRead(ana), 0, 1024, 10, 99));
    delay(100);

  }

}


//----------------------------function--------------------------------

void buzz(int d) {
  digitalWrite(buz, HIGH);
  delay(d);
  digitalWrite(buz, LOW);
}
void disp(int d) {
  lcd.noDisplay();
  delay(d);
  lcd.display();
}
void budi(int d) {
  digitalWrite(buz, 1);
  lcd.noDisplay();
  delay(d);
  lcd.display();
  digitalWrite(buz, 0);
}

void HTTP_handleRoot(void) {
  //server.send (200, "<html>");
  //server.send (200, "<h>Mahadi Hassan</h>");
  //server.send (200, "</html>");

  server.send (200, "<!DOCTYPE HTML>");
  server.send (200, "<HTML>");
  server.send (200, "<H1 style='color:blue;FONT-SIZE: 50px;'> LED CONTROL Develop By Mithun </H1>");
  server.send (200, "<br />");

  if (digitalRead(sw1))
    server.send (200, "<H2> LED ON </H2>");
  else
    server.send (200, "<H2> LED OFF </H2>");

  server.send (200, "<a href=\"/a\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px;WIDTH: 300px; 126px; Margin-right: 10px; TOP: 200px;'> LED ON </button> </a>");
  server.send (200, "<a href=\"/x\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px; WIDTH: 300px; 126px; Margin-left: 100px; TOP: 200px;'> LED OFF </button> </a>");
  server.send (200, "<a href=\"/b\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px;WIDTH: 300px; 126px; Margin-right: 10px; TOP: 200px;'> LED ON </button> </a>");
  server.send (200, "<a href=\"/y\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px; WIDTH: 300px; 126px; Margin-left: 100px; TOP: 200px;'> LED OFF </button> </a>");
  server.send (200, "<a href=\"/c\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px;WIDTH: 300px; 126px; Margin-right: 10px; TOP: 200px;'> LED ON </button> </a>");
  server.send (200, "<a href=\"/z\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px; WIDTH: 300px; 126px; Margin-left: 100px; TOP: 200px;'> LED OFF </button> </a>");
  server.send (200, "</html>");

  delay(1);
}

void execute(String s) {
  if (s == "a") {
    digitalWrite(red, 1);
  }
  else if (s == "x") {
    digitalWrite(red, 0);
  }
  if (s == "b") {
    digitalWrite(gre, 1);
  }
  else if (s == "y") {
    digitalWrite(gre, 0);
  }
  if (s == "c") {
    digitalWrite(blu, 1);
  }
  else if (s == "z") {
    digitalWrite(blu, 0);
  }
}
