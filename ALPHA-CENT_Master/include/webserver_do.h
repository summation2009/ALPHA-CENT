//#define ACCESS_POINT_NAME  "SMART_CLEANER_BOX"
#define ACCESS_POINT_PASSWORD  "12345678"

#define ENC_TYPE_NONE 7

char device_id[15];


void getEfuseMac() {
  uint64_t chipid;

  chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).

  //Serial.printf("ESP32 Chip ID = %06X", chipid >> 16);
  //Serial.println();

  //sprintf(device_id, "wup-%04X", (uint16_t)(chipid >> 32));
  
  //chipid = 0x40A776;
  
  sprintf(ssid, "AP_%06X", (chipid >> 24));
  //sprintf(ssid, "AP_%04X", chipid);

  //device_id = "wup_10E97B";
  
  
  //Serial.printf("ESP32 Chip ID = %04X", (uint16_t)(chipid >> 32)); //print High 2 bytes
  //Serial.printf("%08X\n", (uint32_t)chipid); //print Low 4bytes.

   
   Serial.print("ESP32 Chip ID = ");
   //Serial.print(device_id);
   Serial.print(ssid);
   Serial.println();

}


String getMessage() {

	String  str = String(config.server_address);								// index 0
			str.concat(","); str.concat(String(config.uuid));
			str.concat(","); str.concat(String(config.out1on));
			str.concat(","); str.concat(String(config.out2on));
			str.concat(","); str.concat(String(config.out3on));
			str.concat(","); str.concat(String(config.out4on));
			str.concat(","); str.concat(String(config.out1off));
			str.concat(","); str.concat(String(config.out2off));
			str.concat(","); str.concat(String(config.out3off));
			str.concat(","); str.concat(String(config.out4off));
			str.concat(","); str.concat(String(config.SEND_DATA_EVERY));
			str.concat(","); str.concat(String(config.server_get_address));
			
			
	return str;
}

String getNetwork() {

	String str = String(config.ssid);
		str.concat(","); str.concat(String(config.password));
		str.concat(","); str.concat(String(config.IP[0]));
		str.concat(","); str.concat(String(config.IP[1]));
		str.concat(","); str.concat(String(config.IP[2]));
		str.concat(","); str.concat(String(config.IP[3]));

		str.concat(","); str.concat(String(config.Netmask[0]));
		str.concat(","); str.concat(String(config.Netmask[1]));
		str.concat(","); str.concat(String(config.Netmask[2]));
		str.concat(","); str.concat(String(config.Netmask[3]));

		str.concat(","); str.concat(String(config.Gateway[0]));
		str.concat(","); str.concat(String(config.Gateway[1]));
		str.concat(","); str.concat(String(config.Gateway[2]));
		str.concat(","); str.concat(String(config.Gateway[3]));

		str.concat(","); str.concat(String(config.DNS[0]));
		str.concat(","); str.concat(String(config.DNS[1]));
		str.concat(","); str.concat(String(config.DNS[2]));
		str.concat(","); str.concat(String(config.DNS[3]));

		str.concat(","); str.concat(String(config.dhcp));

	return str;
}


String saveNetwork() {
	//Serial.println("Save network");
	WriteConfig();
	return "Save network OK";
}

String saveMessage() {
	//Serial.println("Save network");
	WriteConfig();
	return "Save message OK.";
}

String reboot() {
	ESP.restart();
	return "OK";
}

// String RTC_Write(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
// 	//rtc.adjust(DateTime(yyyy,....)) --> DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour = (uint8_t)0U, uint8_t min = (uint8_t)0U, uint8_t sec = (uint8_t)0U)
// 	//rtc.adjust(DateTime(year, month, day, hour, minute, second));	// year format 2021
	
// 	rtc.setYear(year-2000);		// year format is only  21 (no 2021)
// 	rtc.setMonth(month);
// 	rtc.setDate(day);
// 	//rtc.setDoW(DoW);
// 	rtc.setHour(hour);
// 	rtc.setMinute(minute);
// 	rtc.setSecond(second);

// 	return "Write RTC OK.";
// }

String scanNetwork() {
	//WiFi.mode(WIFI_AP_STA);
	//WiFi.softAP( ACCESS_POINT_NAME , ACCESS_POINT_PASSWORD);
	//WiFi.disconnect();


	String state = "N/A";
	String Networks = "";

	if (WiFi.status() == 0) state = "Idle";
	else if (WiFi.status() == 1) state = "NO SSID AVAILBLE";
	else if (WiFi.status() == 2) state = "SCAN COMPLETED";
	else if (WiFi.status() == 3) state = "CONNECTED";
	else if (WiFi.status() == 4) state = "CONNECT FAILED";
	else if (WiFi.status() == 5) state = "CONNECTION LOST";
	else if (WiFi.status() == 6) state = "DISCONNECTED";

	//Serial.print("WiFi status = "); 
	//Serial.println(state);

	//Serial.println("Scan network...");

	int n = WiFi.scanComplete();

	if(n == -2){
		//Serial.println("n = -2");
		WiFi.scanNetworks(true);
	} else if(n){
		//Serial.println("Scan n > 0");
		for (int i = 0; i < n; ++i){
			Networks = "Found " + String(n) + " Networks<br>";
			Networks += "<table border='0' cellspacing='0' cellpadding='3' style='width:100%'>";
			Networks += "<tr bgcolor='#DDDDDD' ><td><strong>Name</strong></td><td><strong>Quality</strong></td><td><strong>Enc</strong></td><tr>";

			for (int i = 0; i < n; i++) {
				//Serial.print("ib=");
				//Serial.println(i);

				int quality=0;
				if(WiFi.RSSI(i) <= -100){
						quality = 0;
				} else if(WiFi.RSSI(i) >= -50){
						quality = 100;
				} else {
					quality = 2 * (WiFi.RSSI(i) + 100);
				}

				Networks += "<tr><td><a href='javascript:selssid(\""  +  String(WiFi.SSID(i))  + "\")'>"  +  String(WiFi.SSID(i))  + "</a></td><td>" +  String(quality) + "%</td><td>" +  String((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*")  + "</td></tr>";

				//Serial.print("i=");
				//Serial.println(i);
			
			}
			Networks += "</table>";
		}	// end for 
		WiFi.scanDelete();
		if(WiFi.scanComplete() == -2){
			WiFi.scanNetworks(true);
		}
	}

	//Serial.println(Networks);

	return Networks;
	
}

bool ConfigureWifi() {
	int count = 0;

	//Serial.println("Configuring Wifi");


	//char copy[50];
 	//strcpy(copy, "Pissing away resources uselessly");    // Uncomment for string use

	
	//WiFi.begin (config.ssid, config.password);
	char ssid_buf[config.ssid.length()+1];
	config.ssid.toCharArray(ssid_buf, config.ssid.length()+1);
	//strcpy(buf1, config.ssid);

	//Serial.println(ssid_buf);

	char pass_buf[config.password.length()+1];
	config.password.toCharArray(pass_buf, config.password.length()+1);
	//Serial.println(pass_buf);
	//WiFi.begin (config.ssid, config.password);

	WiFi.begin (ssid_buf, pass_buf);
	

	//WiFi.begin (ssid, pass);

	if (!config.dhcp){
		WiFi.config(IPAddress(config.IP[0],config.IP[1],config.IP[2],config.IP[3] ),  IPAddress(config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3] ) , IPAddress(config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3]), IPAddress(192,168,1,1) );
	}



	while (WiFi.status() != WL_CONNECTED) {
		digitalWrite(BUILTIN_LED, LED_ON);
		delay(500);
		digitalWrite(BUILTIN_LED, LED_OFF);
		Serial.print(".");
		delay(500);

		count++;
		if (count == 10) {
			count = 0;
			digitalWrite(BUILTIN_LED, LED_OFF);	// LED LINK OFF
			//Serial.print("Wifi mode WIFI_AP");
			//WiFi.mode(WIFI_AP_STA);
			WiFi.disconnect();

			//WiFi.mode(WIFI_AP_STA);
			//WiFi.mode(WIFI_AP);
			//WiFi.softAP( ACCESS_POINT_NAME , ACCESS_POINT_PASSWORD);
			//Serial.print("IP Address");
			//Serial.println(WiFi.softAPIP());


			return true;
		}
	}

	//WiFi.config(IPAddress(192,168,4,1 ),  IPAddress(config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3] ) , IPAddress(config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3] ));
	
	//Serial.print("IP address: ");
  	//Serial.println(WiFi.localIP());
	
	digitalWrite(BUILTIN_LED, LED_ON);	// LED LINK ON

	return false;
}

void ConfigureServer() {
	
	// Route for root / web page
	//server.on ( "/", processExample  );


	// Route for root / web page
	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
		//Serial.println("enter home page...");
		request->send(LITTLEFS, "/configwifi.html", String());
	});
	
	server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request){
		SEC_Network = 0;
		MIN_Network = 0;
		CONFIG_WEBF = false;
		request->send(200, "text/html", reboot());
	});

	server.on("/scannetwork", HTTP_GET, [](AsyncWebServerRequest *request){
		SEC_Network = 0;
		MIN_Network = 0;
		request->send(200, "text/html", scanNetwork());
	});

	server.on("/savenetwork", HTTP_GET, [](AsyncWebServerRequest *request){
		SEC_Network = 0;
		MIN_Network = 0;

		int paramsNr = request->params();
		//Serial.println(paramsNr);
	
		for(int i=0;i<paramsNr;i++){
	
			AsyncWebParameter* p = request->getParam(i);
			//Serial.print("Param name: ");
			//Serial.println(p->name());
			//Serial.print("Param value: ");
			//Serial.println(p->value());
			//Serial.println("------");

			if (p->name() == "ssid") config.ssid = p->value();
			else if (p->name() == "password") config.password = p->value();
			else if (p->name() == "ip0") config.IP[0] = p->value().toInt();
			else if (p->name() == "ip1") config.IP[1] = p->value().toInt();
			else if (p->name() == "ip2") config.IP[2] = p->value().toInt();
			else if (p->name() == "ip3") config.IP[3] = p->value().toInt();
			else if (p->name() == "nm0") config.Netmask[0] = p->value().toInt();
			else if (p->name() == "nm1") config.Netmask[1] = p->value().toInt();
			else if (p->name() == "nm2") config.Netmask[2] = p->value().toInt();
			else if (p->name() == "nm3") config.Netmask[3] = p->value().toInt();
			else if (p->name() == "gw0") config.Gateway[0] = p->value().toInt();
			else if (p->name() == "gw1") config.Gateway[1] = p->value().toInt();
			else if (p->name() == "gw2") config.Gateway[2] = p->value().toInt();
			else if (p->name() == "gw3") config.Gateway[3] = p->value().toInt();
			else if (p->name() == "dns0") config.DNS[0] = p->value().toInt();
			else if (p->name() == "dns1") config.DNS[1] = p->value().toInt();
			else if (p->name() == "dns2") config.DNS[2] = p->value().toInt();
			else if (p->name() == "dns3") config.DNS[3] = p->value().toInt();
			else if (p->name() == "dhcp") config.dhcp = p->value().toInt();
		}
		
		request->send(200, "text/html", saveNetwork());
	});

	server.on("/savemessage", HTTP_GET, [](AsyncWebServerRequest *request){
		SEC_Network = 0;
		MIN_Network = 0;

		int paramsNr = request->params();
		//Serial.println(paramsNr);
	
		for(int i=0;i<paramsNr;i++){
	
			AsyncWebParameter* p = request->getParam(i);
	

			if (p->name() == "server_address") config.server_address = p->value();
			else if (p->name() == "uuid") config.uuid = p->value();
			else if (p->name() == "out1on") config.out1on = p->value().toFloat();
			else if (p->name() == "out2on") config.out2on = p->value().toFloat();
			else if (p->name() == "out3on") config.out3on = p->value().toFloat();
			else if (p->name() == "out4on") config.out4on = p->value().toFloat();
			else if (p->name() == "out1off") config.out1off = p->value().toFloat();
			else if (p->name() == "out2off") config.out2off = p->value().toFloat();
			else if (p->name() == "out3off") config.out3off = p->value().toFloat();
			else if (p->name() == "out4off") config.out4off = p->value().toFloat();
			else if (p->name() == "send_data_every") config.SEND_DATA_EVERY = p->value().toInt();
			else if (p->name() == "server_get_address") config.server_get_address = p->value();
			

			
		}

		request->send(200, "text/html", saveMessage());
	});

	server.on("/getmessage", HTTP_GET, [](AsyncWebServerRequest *request){
		SEC_Network = 0;
		MIN_Network = 0;
		request->send(200, "text/html", getMessage());
	});

	// server.on("/getlinehost", HTTP_GET, [](AsyncWebServerRequest *request){
	// 	SEC_Network = 0;
	// 	MIN_Network = 0;
	// 	request->send(200, "text/html", getLineHost());
	// });

	server.on("/getnetwork", HTTP_GET, [](AsyncWebServerRequest *request){
		SEC_Network = 0;
		MIN_Network = 0;
		request->send(200, "text/html", getNetwork());
	});
	
	server.on("/configwifi.html", HTTP_GET, [](AsyncWebServerRequest *request){
		SEC_Network = 0;
		MIN_Network = 0;
		request->send(LITTLEFS, "/configwifi.html", String());
	});

	server.on("/configdata.html", HTTP_GET, [](AsyncWebServerRequest *request){
		SEC_Network = 0;
		MIN_Network = 0;
		request->send(LITTLEFS, "/configdata.html", String());
	});

	// server.on("/datalog.txt", HTTP_GET, [](AsyncWebServerRequest *request){
	// 	request->send(SPIFFS, "/datalog.txt", String());
	// });

	// server.on("/savedatetime", HTTP_GET, [](AsyncWebServerRequest *request){
	// 	int paramsNr = request->params();
	// 	//Serial.println(paramsNr);

	// 	uint16_t year = 0;
	// 	uint8_t day = 0, month = 0, hour = 0, min = 0, sec = 0;
	
	// 	for(int i=0; i<paramsNr; i++){
	
	// 		AsyncWebParameter* p = request->getParam(i);
	// 		//Serial.print("Param name: ");
	// 		//Serial.println(p->name());
	// 		//Serial.print("Param value: ");
	// 		//Serial.println(p->value());
	// 		//Serial.println("------");

	// 		if (p->name() == "day") day = p->value().toInt();
	// 		else if (p->name() == "month") month = p->value().toInt();
	// 		else if (p->name() == "year") year = p->value().toInt();
	// 		else if (p->name() == "hour") hour = p->value().toInt();
	// 		else if (p->name() == "min") min = p->value().toInt();
	// 		else if (p->name() == "sec") sec = p->value().toInt();
	// 	}

		
		
	// 	request->send(200, "text/html", RTC_Write(year, month, day, hour, min, sec));
	// });


	


	// Route to load style.css file
	server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(LITTLEFS, "/bootstrap.min.css", "text/css");
	});

	server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(LITTLEFS, "/style.css", "text/css");
	});

	// Route to load js file
	server.on("/jquery-3.5.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(LITTLEFS, "/jquery-3.5.1.min.js", "text/javascript");
	});

	server.on("/bootstrap.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(LITTLEFS, "/bootstrap.bundle.min.js", "text/javascript");
	});



	// server.on("/jquery.dataTables.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
	// 	request->send(SPIFFS, "/jquery.dataTables.min.css", "text/css");
	// });

	// server.on("/buttons.dataTables.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
	// 	request->send(SPIFFS, "/buttons.dataTables.min.css", "text/css");
	// });

	// server.on("/jquery.dataTables.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
	// 	request->send(SPIFFS, "/jquery.dataTables.min.js", "text/javascript");
	// });

	// server.on("/dataTables.buttons.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
	// 	request->send(SPIFFS, "/dataTables.buttons.min.js", "text/javascript");
	// });

	server.on("/buttons.html5.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(LITTLEFS, "/buttons.html5.min.js", "text/javascript");
	});

	

	
	server.begin();
	Serial.println( "HTTP server started" );

}
