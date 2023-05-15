#include "littlefs_helper.h"


//volatile bool filesystemOK = false;

void InitFilesystem() {
  // Initialize LittleFS
  //if (!LITTLEFS.begin(false /* false: Do not format if mount failed */)) {
    //Serial.println("Failed to mount LittleFS");
    if (!LITTLEFS.begin(false /* true: format */)) {
      Serial.println("Failed to format LittleFS");
    } else {
      //Serial.println("LittleFS formatted successfully");
	  Serial.println("LITTLEFS Mount SUCCESSFUL.");
      listDir(LITTLEFS, "/", 0);
      //filesystemOK = true;
    }
  //} else { // Initial mount success
    //filesystemOK = true;
  //}
}

void WriteConfig() {

	Serial.println("Writing Configuration");

	const char * path = "/config.json";
    Serial.println("\n================================");
    Serial.printf("Writing file: %s\r\n", path);
    //File file = LITTLEFS.open(path, FILE_WRITE);

	//DynamicJsonBuffer jsonBuffer;
    DynamicJsonDocument jsonBuffer(2048);
    //JsonObject& json = jsonBuffer.createObject();

    //------------------------------------------------------------------
	jsonBuffer["ssid"] = config.ssid;
	jsonBuffer["password"] = config.password;
	jsonBuffer["dhcp"] = config.dhcp;

	jsonBuffer["ip0"] = config.IP[0];
	jsonBuffer["ip1"] = config.IP[1];
	jsonBuffer["ip2"] = config.IP[2];
	jsonBuffer["ip3"] = config.IP[3];

	jsonBuffer["nm0"] = config.Netmask[0];
	jsonBuffer["nm1"] = config.Netmask[1];
	jsonBuffer["nm2"] = config.Netmask[2];
	jsonBuffer["nm3"] = config.Netmask[3];

	jsonBuffer["gw0"] = config.Gateway[0];
	jsonBuffer["gw1"] = config.Gateway[1];
	jsonBuffer["gw2"] = config.Gateway[2];
	jsonBuffer["gw3"] = config.Gateway[3];

	jsonBuffer["dns0"] = config.Gateway[0];
	jsonBuffer["dns1"] = config.Gateway[1];
	jsonBuffer["dns2"] = config.Gateway[2];
	jsonBuffer["dns3"] = config.Gateway[3];

	jsonBuffer["server_address"] = config.server_address;
	jsonBuffer["server_get_address"] = config.server_get_address;
	jsonBuffer["uuid"] = config.uuid;

	jsonBuffer["out1on"] = config.out1on;
	jsonBuffer["out2on"] = config.out2on;
	jsonBuffer["out3on"] = config.out3on;
	jsonBuffer["out4on"] = config.out4on;

	jsonBuffer["out1off"] = config.out1off;
	jsonBuffer["out2off"] = config.out2off;
	jsonBuffer["out3off"] = config.out3off;
	jsonBuffer["out4off"] = config.out4off;


	jsonBuffer["SEND_DATAT_EVERY"] = config.SEND_DATA_EVERY;





	File configFile1 = LITTLEFS.open("/config.json", FILE_WRITE);

    if (!configFile1) {
      Serial.println("failed to open  config file for writing");
	  return;
    }


	serializeJson(jsonBuffer, configFile1);
    configFile1.close();
	


    //jsonBuffer.printTo(configFile1);
    // serializeJson(jsonBuffer, configFile1);
    // configFile1.close();


  //delay(1000);

  //-----------------------------------------------------------------------------------------------------------------------//
  //Print เพื่อเชคว่าค่าที่บันทึกไว้ยังอยู่หรือไม่
  //Serial.println("เชคว่าค่าที่บันทึกไว้ยังอยู่หรือไม่");

  //Serial.print("line_token =");
  //Serial.println(config.line_token);

}


void SetDefault() {
	// DEFAULT CONFIG
	config.ssid = "LINE_ALERT_BOX";
	config.password = "12345678";

	config.dhcp = true;
	config.IP[0] = 192; config.IP[1] = 168; config.IP[2] = 1; config.IP[3] = 222;
	config.Netmask[0] = 255;config.Netmask[1] = 255;config.Netmask[2] = 255;config.Netmask[3] = 0;
	config.Gateway[0] = 192;config.Gateway[1] = 168;config.Gateway[2] = 1;config.Gateway[3] = 1;
	config.DNS[0] = 192;config.DNS[1] = 168;config.DNS[2] = 1;config.DNS[3] = 1;


	//config.line_token = "3mJZancikmSUw3zibGL2cFBS2r8rUMl7OMlpVqGkVqU";	// khom line token for test
	config.server_address = "server for Send data address";
	config.server_get_address = "server for Get data address";
	config.uuid = "9";


	 config.out1on = 30.0;
	 config.out2on = 29.0;
	 config.out3on = 28.0;
	 config.out4on = 27.0;

	 config.out1off = 34.0;
	 config.out2off = 33.0;
	 config.out3off = 32.0;
	 config.out4off = 31.0;

	 config.SEND_DATA_EVERY = 60;


	// HOUR_COUNT = int(config.HOUR);
	// MIN_COUNT = int(config.MINUTES);
	// SEC_COUNT = int(config.SECONDS);
	// ON_DELAY_SEC = int(config.ON_DELAY_SEC);

	WriteConfig();

	Serial.println("Reset default config applied");
}


void ReadConfig() {

	Serial.println("Reading Configuration");
    

	const char * path = "/config.json";
    Serial.println("\n================================");
    Serial.printf("Reading file: %s\r\n", path);
    //File file = LITTLEFS.open("/config.json", FILE_READ);


    
	File configFile1 = LITTLEFS.open("/config.json", FILE_READ);

    // if(!file || file.isDirectory()){
    //     Serial.println("- failed to open file for reading");
    //     return;
    // }

			if (configFile1) {
				Serial.println("opened Data config file");
				size_t size = configFile1.size();
				// Allocate a buffer to store contents of the file.
				std::unique_ptr<char[]> buf1(new char[size]);

				configFile1.readBytes(buf1.get(), size);
				//DynamicJsonBuffer jsonBuffer;
				DynamicJsonDocument jsonBuffer(2048);
				//JsonObject& json = jsonBuffer.parseObject(buf1.get());
				deserializeJson(jsonBuffer, buf1.get());

				//json.printTo(Serial);
				serializeJson(jsonBuffer, Serial);

				if (!jsonBuffer.isNull()) {
					Serial.println("\nparsed  json");

					//config.line_token = jsonBuffer["line_token"].as<String>();
                    
					// config.ssid = jsonBuffer["ssid"].as<String>();
					// config.password = jsonBuffer["password"].as<String>();
					// config.dhcp = jsonBuffer["dhcp"].as<boolean>();

					config.line_token = jsonBuffer["line_token"].as<String>();
					config.ssid = jsonBuffer["ssid"].as<String>();
					config.password = jsonBuffer["password"].as<String>();

					config.IP[0] = jsonBuffer["ip0"];
					config.IP[1] = jsonBuffer["ip1"];
					config.IP[2] = jsonBuffer["ip2"];
					config.IP[3] = jsonBuffer["ip3"];

					config.Netmask[0] = jsonBuffer["nm0"];
					config.Netmask[1] = jsonBuffer["nm1"];
					config.Netmask[2] = jsonBuffer["nm2"];
					config.Netmask[3] = jsonBuffer["nm3"];

					config.Gateway[0] = jsonBuffer["gw0"];
					config.Gateway[1] = jsonBuffer["gw1"];
					config.Gateway[2] = jsonBuffer["gw2"];
					config.Gateway[3] = jsonBuffer["gw3"];

					config.DNS[0] = jsonBuffer["dns0"];
					config.DNS[1] = jsonBuffer["dns1"];
					config.DNS[2] = jsonBuffer["dns2"];
					config.DNS[3] = jsonBuffer["dns3"];
					
					config.dhcp = jsonBuffer["dhcp"].as<boolean>();

					config.server_address = jsonBuffer["server_address"].as<String>();
					config.server_get_address = jsonBuffer["server_get_address"].as<String>();
					config.uuid = jsonBuffer["uuid"].as<String>();

					config.out1on = jsonBuffer["out1on"];
					config.out2on = jsonBuffer["out2on"];
					config.out3on = jsonBuffer["out3on"];
					config.out4on = jsonBuffer["out4on"];

					config.out1off = jsonBuffer["out1off"];
					config.out2off = jsonBuffer["out2off"];
					config.out3off = jsonBuffer["out3off"];
					config.out4off = jsonBuffer["out4off"];

					
					config.SEND_DATA_EVERY = jsonBuffer["SEND_DATAT_EVERY"];

					// HOUR_COUNT = config.HOUR;
					// MIN_COUNT = config.MINUTES;
					// SEC_COUNT = config.SECONDS;
					// ON_DELAY_SEC = config.ON_DELAY_SEC;


					// config.IP[1] = jsonBuffer["ip1"];
					// config.IP[2] = jsonBuffer["ip2"];
					// config.IP[3] = jsonBuffer["ip3"];

					// config.Netmask[0] = jsonBuffer["nm0"];
					// config.Netmask[1] = jsonBuffer["nm1"];
					// config.Netmask[2] = jsonBuffer["nm2"];
					// config.Netmask[3] = jsonBuffer["nm3"];

					// config.Gateway[0] = jsonBuffer["gw0"];
					// config.Gateway[1] = jsonBuffer["gw1"];
					// config.Gateway[2] = jsonBuffer["gw2"];
					// config.Gateway[3] = jsonBuffer["gw3"];

					// config.DNS[0] = jsonBuffer["dns0"];
					// config.DNS[1] = jsonBuffer["dns1"];
					// config.DNS[2] = jsonBuffer["dns2"];
					// config.DNS[3] = jsonBuffer["dns3"];


					// config.telemetry_sec = jsonBuffer["telemetry_sec"];

	


					// Serial.print("token = ");
					// Serial.println(config.line_token);

					// Serial.print("ssid = ");
					// Serial.println(config.ssid);
					// Serial.print("password = ");
					// Serial.println(config.password);

					// Serial.print("IP0 = ");
					// Serial.println(config.IP[0]);
					// Serial.print("IP1 = ");
					// Serial.println(config.IP[1]);
					// Serial.print("IP2 = ");
					// Serial.println(config.IP[2]);
					// Serial.print("IP3 = ");
					// Serial.println(config.IP[3]);

				} else {
					Serial.println("failed to load Data file json config");
				}
			} else {									// file not found --> Write initial data
                Serial.println("set default");
				SetDefault();
			}
	//}
}	// end read config


