/*
 * Copyright (c) 2023 Tracey Emery K7TLE
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>

#include "html.h"

#define DEBUG		 1
#define EEPROM_SIZE	 2

#define LED_R1		 13
#define LED_R2		 12
#define LED_A1		 14
#define LED_A2		 27
#define LED_A3		 26
#define LED_A4		 25

#define R1		 2
#define R2		 4
#define R_SWAP		 16
#define KR		 21
#define KT		 19

#define A_SWAP		 17
#define	A2_A1		 5
#define A3_A4		 18

const char		*ssid = "Bifrost-24";
const char		*pass = "0xG0@wayN1n3M0th3r$";
IPAddress		 ip(192, 168, 100, 35);
IPAddress		 gw(192, 168, 100, 1);
IPAddress		 sn(255, 255, 255, 0);
IPAddress		 dns1(192, 168, 100, 1);
IPAddress		 dns2(8, 8, 8, 8);

AsyncWebServer		 server(80);
AsyncEventSource	 events("/event");

void			 action(AsyncWebServerRequest *);
void			 init_pins(void);
void			 set_radio(void);
void			 set_antenna(void);

String			 handle_connect(const String&);
String			 radio_s, antenna_s;
int			 radio = 0, antenna = 1;

void
setup(void)
{
	if (DEBUG) {
		Serial.begin(115200);
		delay(10);
		Serial.println();
		Serial.println();
		Serial.print("Connecting to ");
		Serial.println(ssid);
	}

	WiFi.begin(ssid, pass);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		if (DEBUG) {
			Serial.print(".");
		}
	}

	WiFi.config(ip, gw, sn, dns1, dns2);
	if (DEBUG) {
		Serial.println("");
		Serial.println("WiFi connected");
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
	}

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send_P(200, "text/html", index_html, handle_connect);
	});
	server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
		String msg;
		if (request->hasParam("radio")) {
			msg = request->getParam("radio")->value();
			radio = msg.toInt();
		}
		if (request->hasParam("ant")) {
			msg = request->getParam("ant")->value();
			antenna = msg.toInt();
		}
		request->send_P(200, "text/plain", "OK");
		events.send("", "reload", millis());
	});
	server.addHandler(&events);
	server.begin();

	EEPROM.begin(EEPROM_SIZE);
	radio = EEPROM.read(0);
	antenna = EEPROM.read(1);
	if (antenna == 0) {
		EEPROM.write(1, 1);
		EEPROM.commit();
	}

	init_pins();
	set_radio();
	set_antenna();
}

void
loop(void)
{
}

String
handle_connect(const String& var)
{
	char buff[100];

	switch (antenna) {
	default:
	case 1:
		antenna_s = "ONE";
		EEPROM.write(1, 1);
		break;
	case 2:
		antenna_s = "TWO";
		EEPROM.write(1, 2);
		break;
	case 3:
		antenna_s = "THREE";
		EEPROM.write(1, 3);
		break;
	case 4:
		antenna_s = "FOUR";
		EEPROM.write(1, 4);
		break;
	}
	switch(radio) {
	default:
	case 0:
		radio_s = "DUMMY LOAD";
		EEPROM.write(0, 2);
		break;
	case 1:
		radio_s = "ONE";
		EEPROM.write(0, 1);
		break;
	case 2:
		radio_s = "TWO";
		EEPROM.write(0, 2);
		break;
	}

	set_radio();
	set_antenna();

	EEPROM.commit();
	snprintf(buff, 100, "RADIO: %s<br />ANTENNA: %s", radio_s, antenna_s);
	if (var == "SELECTION") {
		return buff;
	}
}

void
init_pins(void)
{
	pinMode(LED_R1, OUTPUT);
	pinMode(LED_R2, OUTPUT);
	pinMode(LED_A1, OUTPUT);
	pinMode(LED_A2, OUTPUT);
	pinMode(LED_A3, OUTPUT);
	pinMode(LED_A4, OUTPUT);

	pinMode(R1, OUTPUT);
	pinMode(R2, OUTPUT);
	pinMode(R_SWAP, OUTPUT);
	pinMode(KR, OUTPUT);
	pinMode(KT, OUTPUT);

	pinMode(A2_A1, OUTPUT);
	pinMode(A3_A4, OUTPUT);
	pinMode(A_SWAP, OUTPUT);

	digitalWrite(LED_R1, LOW);
	digitalWrite(LED_R2, LOW);
	digitalWrite(LED_A1, LOW);
	digitalWrite(LED_A2, LOW);
	digitalWrite(LED_A3, LOW);
	digitalWrite(LED_A4, LOW);

	digitalWrite(R1, LOW);
	digitalWrite(R2, LOW);
	digitalWrite(R_SWAP, LOW);
	digitalWrite(KR, LOW);
	digitalWrite(KT, LOW);

	digitalWrite(A2_A1, LOW);
	digitalWrite(A3_A4, LOW);
	digitalWrite(A_SWAP, LOW);
}

void
set_radio(void)
{
	switch(radio) {
	default:
	case 0:
		digitalWrite(R1, LOW);
		digitalWrite(R2, LOW);
		digitalWrite(R_SWAP, LOW);
		digitalWrite(KR, LOW);
		digitalWrite(KT, LOW);
		digitalWrite(LED_R1, LOW);
		digitalWrite(LED_R2, LOW);
		break;
	case 1:
		digitalWrite(R1, HIGH);
		digitalWrite(R2, LOW);
		digitalWrite(R_SWAP, LOW);
		digitalWrite(KR, LOW);
		digitalWrite(KT, LOW);
		digitalWrite(LED_R1, HIGH);
		digitalWrite(LED_R2, LOW);
		break;
	case 2:
		digitalWrite(R1, LOW);
		digitalWrite(R2, HIGH);
		digitalWrite(R_SWAP, HIGH);
		digitalWrite(KR, HIGH);
		digitalWrite(KT, HIGH);
		digitalWrite(LED_R1, LOW);
		digitalWrite(LED_R2, HIGH);
		break;
	}
}

void
set_antenna(void)
{
	switch(antenna) {
	default:
	case 1:
		digitalWrite(A2_A1, LOW);
		digitalWrite(A3_A4, LOW);
		digitalWrite(A_SWAP, LOW);
		digitalWrite(LED_A1, HIGH);
		digitalWrite(LED_A2, LOW);
		digitalWrite(LED_A3, LOW);
		digitalWrite(LED_A4, LOW);
		break;
	case 2:
		digitalWrite(A2_A1, HIGH);
		digitalWrite(A3_A4, LOW);
		digitalWrite(A_SWAP, LOW);
		digitalWrite(LED_A1, LOW);
		digitalWrite(LED_A2, HIGH);
		digitalWrite(LED_A3, LOW);
		digitalWrite(LED_A4, LOW);
		break;
	case 3:
		digitalWrite(A2_A1, LOW);
		digitalWrite(A3_A4, HIGH);
		digitalWrite(A_SWAP, HIGH);
		digitalWrite(LED_A1, LOW);
		digitalWrite(LED_A2, LOW);
		digitalWrite(LED_A3, HIGH);
		digitalWrite(LED_A4, LOW);
		break;
	case 4:
		digitalWrite(A2_A1, LOW);
		digitalWrite(A3_A4, LOW);
		digitalWrite(A_SWAP, HIGH);
		digitalWrite(LED_A1, LOW);
		digitalWrite(LED_A2, LOW);
		digitalWrite(LED_A3, LOW);
		digitalWrite(LED_A4, HIGH);
		break;
	}

}
