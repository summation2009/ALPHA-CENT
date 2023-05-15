#define ENC_TYPE_NONE 7


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

	Serial.print("WiFi status = "); 
	Serial.println(state);

	Serial.println("Scan network...");

	int n = WiFi.scanComplete();

	if(n == -2){
		Serial.println("n = -2");
		WiFi.scanNetworks(true);
	} else if(n){
		Serial.println("Scan n > 0");
		for (int i = 0; i < n; ++i){
			Networks = "Found " + String(n) + " Networks<br>";
			Networks += "<table border='0' cellspacing='0' cellpadding='3' style='width:100%'>";
			Networks += "<tr bgcolor='#DDDDDD' ><td><strong>Name</strong></td><td><strong>Quality</strong></td><td><strong>Enc</strong></td><tr>";

			for (int i = 0; i < n; i++) {
				Serial.print("ib=");
				Serial.println(i);

				int quality=0;
				if(WiFi.RSSI(i) <= -100){
						quality = 0;
				} else if(WiFi.RSSI(i) >= -50){
						quality = 100;
				} else {
					quality = 2 * (WiFi.RSSI(i) + 100);
				}

				Networks += "<tr><td><a href='javascript:selssid(\""  +  String(WiFi.SSID(i))  + "\")'>"  +  String(WiFi.SSID(i))  + "</a></td><td>" +  String(quality) + "%</td><td>" +  String((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*")  + "</td></tr>";

				Serial.print("i=");
				Serial.println(i);
			
			}
			Networks += "</table>";
		}	// end for 
		WiFi.scanDelete();
		if(WiFi.scanComplete() == -2){
			WiFi.scanNetworks(true);
		}
	}

	Serial.println(Networks);

	return Networks;
	
}

/*
**
** CONFIGURATION HANDLING
**
*/



bool ConfigureWifi() {
	int count = 0;

	Serial.println("Configuring Wifi");


	//char copy[50];
 	//strcpy(copy, "Pissing away resources uselessly");    // Uncomment for string use

	 config.ssid = "sumtech 2.4G";
	 config.password = "123456aaff";

	
	//WiFi.begin (config.ssid, config.password);
	char ssid_buf[config.ssid.length()+1];
	config.ssid.toCharArray(ssid_buf, config.ssid.length()+1);
	//strcpy(buf1, config.ssid);

	Serial.println(ssid_buf);

	char pass_buf[config.password.length()+1];
	config.password.toCharArray(pass_buf, config.password.length()+1);
	Serial.println(pass_buf);
	//WiFi.begin (config.ssid, config.password);

	WiFi.mode(WIFI_STA);
	WiFi.begin (ssid_buf, pass_buf);
	

	//WiFi.begin (ssid, pass);

	if (!config.dhcp){
		WiFi.config(IPAddress(config.IP[0],config.IP[1],config.IP[2],config.IP[3] ),  IPAddress(config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3] ) , IPAddress(config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3]), IPAddress(192,168,1,1) );
	}



	while (WiFi.status() != WL_CONNECTED) {
		digitalWrite(LED_LINK, LEDON);
		delay(250);
		digitalWrite(LED_LINK, LEDOFF);
		Serial.print(".");
		delay(250);
		digitalWrite(LED_LINK, LEDON);
		delay(250);
		digitalWrite(LED_LINK, LEDOFF);
		Serial.print(".");
		delay(250);

		count++;
		if (count == 5) {
			count = 0;
			//digitalWrite(LED_LINK, LEDOFF);	// LED LINK OFF
			//Serial.print("Wifi mode WIFI_AP");
			//WiFi.mode(WIFI_AP_STA);
			/*WiFi.disconnect();
			
			//WiFi.mode(WIFI_AP_STA);
			WiFi.mode(WIFI_AP);
			WiFi.softAP( ACCESS_POINT_NAME , ACCESS_POINT_PASSWORD);
			Serial.print("IP Address");
			Serial.println(WiFi.softAPIP()); */
			return false;							// connect not OK --> enter AP mode
		}
	}	// end while (WiFi.status() != WL_CONNECTED)

	//WiFi.config(IPAddress(192,168,4,1 ),  IPAddress(config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3] ) , IPAddress(config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3] ));
	
	Serial.print("Connected router success.");

	Serial.print("IP address: ");
  	Serial.println(WiFi.localIP());

	return true;		// connect OK
}

bool WiFiInternetOK () {
	bool success = Ping.ping("www.google.com", 3);

	//bool success = false;

	if(!success){
		Serial.println("\nPing failed");
		//mb.Coil(REG_COIL_WIFI_OKF, 0);
		return false;
	} else {
		Serial.println("\nPing successful.");
		//mb.Coil(REG_COIL_WIFI_OKF, 1);
		return true;
	}
}


/*
void ConfigureEthernet() {
	pinMode(4, OUTPUT);
	digitalWrite(4, LOW);		// Ehternet module RESET Pin
	delay(25);
	digitalWrite(4, HIGH); 		//enable the ethernet chip after resetting



	Ethernet.init(5);  			//set Ehternet module CS pin for ethernet

	//Ethernet.begin(mac, ip); 

	
	//Ethernet.setLocalIP(IPAddress(config.IP[0],config.IP[1],config.IP[2],config.IP[3]));
	//Ethernet.setGatewayIP(IPAddress(config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3] ));
	//Ethernet.setSubnetMask(IPAddress(config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3] ));
	//Ethernet.setDnsServerIP(IPAddress(config.DNS[0],config.DNS[1],config.DNS[2],config.DNS[3] ));

	Serial.println(F("Before SET Ethernet.begin()"));

	Serial.println("IP");
	Serial.println(Ethernet.localIP());
	Serial.println("GW");
	Serial.println(Ethernet.gatewayIP());
	Serial.println("SUBNET");
	Serial.println(Ethernet.subnetMask());
	Serial.println("DNS");
	Serial.println(Ethernet.dnsServerIP());

	Ethernet.begin(mac, IPAddress(config.IP[0],config.IP[1],config.IP[2],config.IP[3]), IPAddress(config.DNS[0],config.DNS[1],config.DNS[2],config.DNS[3] ), IPAddress(config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3] ), IPAddress(config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3] ));
	//Ethernet.begin(mac);		// begin with get DHCP


	Serial.println("IP");
	Serial.println(Ethernet.localIP());
	Serial.println("GW");
	Serial.println(Ethernet.gatewayIP());
	Serial.println("SUBNET");
	Serial.println(Ethernet.subnetMask());
	Serial.println("DNS");
	Serial.println(Ethernet.dnsServerIP());

	Serial.println(F("After Ethernet.begin()"));


	Ethernet.setMACAddress(mac);
	Ethernet.setLocalIP(IPAddress(config.IP[0],config.IP[1],config.IP[2],config.IP[3]));
	Ethernet.setGatewayIP(IPAddress(config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3] ));
	Ethernet.setSubnetMask(IPAddress(config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3] ));
	Ethernet.setDnsServerIP(IPAddress(config.DNS[0],config.DNS[1],config.DNS[2],config.DNS[3] ));

	Serial.println(F("After SET Ethernet.begin()"));

	
    // Give the Ethernet shield a second to initialize
    delay(1000);

	if (Ethernet.hardwareStatus() == EthernetNoHardware) {
		Serial.println(F("Ethernet hardware not found"));
	} else {
		Serial.println(Ethernet.hardwareStatus());
	}
} // end
*/







/*
String getMessage() {

	String str = "";
			str.concat(String(config.line_token));
			str.concat(","); 
			//str.concat(","); str.concat(String(config.message[2]));
			//str.concat(","); str.concat(String(config.message[3]));
			str.concat(","); str.concat(String(config.MOTOR_SPEED));
			str.concat(","); str.concat(String(config.MOTOR_SET_MINUTE));


	return str;
}
*/

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
