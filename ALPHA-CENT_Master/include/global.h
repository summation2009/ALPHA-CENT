#ifndef GLOBAL_H
#define GLOBAL_H


struct strConfig {
	String line_token;
	String ssid;
	String password;
	String uid;

	byte IP[4];
	byte Netmask[4];
	byte Gateway[4];
	byte DNS[4];
	boolean dhcp;

	String server_address;
	String uuid;
	String server_get_address;


	float out1on;
	float out2on;
	float out3on;
	float out4on;
	float out1off;
	float out2off;
	float out3off;
	float out4off;
	

	unsigned int SEND_DATA_EVERY;

	// unsigned int HOUR;
	// unsigned int MINUTES;
	// unsigned int SECONDS;
	// unsigned int ON_DELAY_SEC;
	

	//String line_message;		// start index 1
	//String messageOFF[16+1];		// start index 1
	//String messageTemp[6+1];		// start index 1

}   config;





/*struct VariableValue {

	unsigned int PM25_A;
	unsigned int PM25_B;
	
	unsigned int HUMIDITY_A;
	unsigned int HUMIDITY_B;

	unsigned int TEMPERATURE_A;
	unsigned int TEMPERATURE_B;

	unsigned int NOISE_A;
	unsigned int MOTORSPEED;

	unsigned int MOTOR_RUN_STATUS;
	unsigned int MOTOR_TIME_COUNT;
	
}   mysensor;



void writePumpPercent(unsigned int percent_val) {
	uint16_t volt_out = map(percent_val, 0, 100, 0, 4095);
	dac.setVoltage(volt_out, false);
	Serial.print("DAC = ");
	Serial.println(volt_out);
}

void setPump(bool state) {

  //mb.Coil(REG_COIL_MOTOR_RUN_STATUS, state);

  if (state) {
    PUMPSTATUSTEXT  = "ON";
    //writePumpPercent(config.MOTOR_SPEED);
    //writePumpPWM(SPEEDPWM[3]);              // fix SpeedHigPWM value, adjust % SPEEDPWM[3] (high)
    //Serial.print("setPump : ");
    //Serial.println(PUMPSTATUSTEXT);
  }
  else { 
    PUMPSTATUSTEXT = "OFF";
    writePumpPercent(0);
    //Serial.print("setPump : ");
    //Serial.println(PUMPSTATUSTEXT);
  }
}
*/


#endif