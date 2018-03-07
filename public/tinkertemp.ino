/* Function prototypes -------------------------------------------------------*/
int tinkerDigitalRead(String pin);
int tinkerDigitalWrite(String command);
int tinkerAnalogRead(String pin);
int tinkerAnalogWrite(String command);

// STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));
// retained int retainedCounter = 0;
// int retainedCounter = 0;

// int counter;

#define AREF_VOLTAGE 3320

#define SENSOR_TMP36

// CONSTANTS
const char*   channelName    = "HomeTemp";
const int     sendAfterS     = 10; // run after N seconds connected to the cloud
const int     sleepDelayS    = 10; // after sending the message, wait N seconds before going to sleep
const int     sleepForS      = 60; // sleep for N seconds

const int     pinLight       = D7;

#ifdef SENSOR_TMP36
#define TMP36_SAMPLES     8       // samples to take
#define TMP36_WAIT        2       // interval between samples
#define TMP36_MAX_VOLTAGE 1550    // max voltage output at 125C for 3.3V
#define SENSOR_TMP36_SLOPE 0.1128 // slope for pairs (mV,Tc) = (0,-50), (595.5311,18), (1550,125). 
                                  // second value is emprirical. third value leads to R-squared of zero

const int     pinTemp36      = A7;
#endif

// LOOP VARIABLES
unsigned long firstAvailable = 0;
bool          wifiReady      = false;
bool          cloudReady     = false;

/* This function is called once at start up ----------------------------------*/
void setup()
{
	//Setup the Tinker application here

	//Register all the Tinker functions
	Particle.function("digitalread" , tinkerDigitalRead );
	Particle.function("digitalwrite", tinkerDigitalWrite);
	Particle.function("analogread"  , tinkerAnalogRead  );
	Particle.function("analogwrite" , tinkerAnalogWrite );

    pinMode(pinLight, OUTPUT);
	digitalWrite(pinLight, LOW);

    // Spark.variable("temperature", &temperature, DOUBLE);
#ifdef SENSOR_TMP36
    pinMode(pinTemp36, INPUT);
    analogRead(pinTemp36);
#endif

    firstAvailable = 0;
    wifiReady      = false;
    cloudReady     = false;
}

/* This function loops forever --------------------------------------------*/
void loop()
{
	//This will run in a loop
	wifiReady  = WiFi.ready();
	cloudReady = Particle.connected();

    //https://community.particle.io/t/sleep-mode-explained-using-code-samples/21173
	if (wifiReady && cloudReady) {
		if (firstAvailable == 0) {
			firstAvailable = millis();
		}
		if (millis() - firstAvailable > sendAfterS * 1000) {
			// After we've been up for 30 seconds, go to sleep. The delay is so the serial output gets written out before
			// sleeping.
// 			Serial.println("calling System.sleep(SLEEP_MODE_DEEP, 30)");

            digitalWrite(pinLight, HIGH);

#ifdef SENSOR_TMP36
            // ALL CALCULATION
            // double analogValue    = analogRead(pinTemp36);
            // // converting that reading to voltage, which is based off the reference voltage
            // double voltage        = AREF_VOLTAGE * (((double)analogValue) / 4095.0);
            // // converting from 10 mv per degree wit 500 mV offset
            // // to degrees ((volatge - 500mV) times 100)
            // double temperature    = (voltage - 500.0) / 10.0;
            // Particle.publish(channelName, String(analogValue), PRIVATE);
            // Particle.publish(channelName, String(voltage    ), PRIVATE);
            // Particle.publish(channelName, String(temperature), PRIVATE);

            // SOME CALCULATION
            // double voltage        = readMilliVolts(pinTemp36);
            // double temperature    = (voltage - 500.0) / 10.0;
            // Particle.publish(channelName, String(voltage    ), PRIVATE);
            // Particle.publish(channelName, String(temperature), PRIVATE);

            // SHORT
            Particle.publish(channelName, String(TMP36GetCentigrade(pinTemp36)), PRIVATE);
#endif
            
			digitalWrite(pinLight, LOW);

			delay(sleepDelayS * 1000);

			System.sleep(SLEEP_MODE_DEEP, sleepForS);

			// The rest of the code here is not reached. SLEEP_MODE_DEEP causes the code execution to stop,
			// and when wake up occurs, it's like a reset where you start again with setup(), all variables are
			// cleared, etc.
// 			Serial.println("returned from sleep, should not occur");
		}
	}
	else {
		firstAvailable = 0;
	}
}


#ifdef SENSOR_TMP36
double readMilliVolts(int pin)
{
    double val = 0.0;
    for ( int i = 0; i < TMP36_SAMPLES; ++i ) {
        val += analogRead(pin);
        delay(TMP36_WAIT);
    }
    
    val /= TMP36_SAMPLES;
    
	return map(val, 0, 4095, 0, AREF_VOLTAGE);
}

double TMP36GetCentigrade(int pin)
{
	// http://www.analog.com/media/en/technical-documentation/data-sheets/TMP35_36_37.pdf
// 	return (readMilliVolts(pin) - 500.0) / 10.0;
	
	// =C2*0.1133-50
	// max voltage is 1550, not 3300, as shown in the spec
	return (readMilliVolts(pin) * SENSOR_TMP36_SLOPE) - 50;
}
#endif




/*******************************************************************************
 * Function Name  : tinkerDigitalRead
 * Description    : Reads the digital value of a given pin
 * Input          : Pin
 * Output         : None.
 * Return         : Value of the pin (0 or 1) in INT type
                    Returns a negative number on failure
 *******************************************************************************/
int tinkerDigitalRead(String pin)
{
	//convert ascii to integer
	int pinNumber = pin.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(pin.startsWith("D"))
	{
		pinMode(pinNumber, INPUT_PULLDOWN);
		return digitalRead(pinNumber);
	}
	else if (pin.startsWith("A"))
	{
		pinMode(pinNumber+10, INPUT_PULLDOWN);
		return digitalRead(pinNumber+10);
	}
	return -2;
}

/*******************************************************************************
 * Function Name  : tinkerDigitalWrite
 * Description    : Sets the specified pin HIGH or LOW
 * Input          : Pin and value
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int tinkerDigitalWrite(String command)
{
	bool value = 0;
	//convert ascii to integer
	int pinNumber = command.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(command.substring(3,7) == "HIGH") value = 1;
	else if(command.substring(3,6) == "LOW") value = 0;
	else return -2;

	if(command.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		digitalWrite(pinNumber, value);
		return 1;
	}
	else if(command.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		digitalWrite(pinNumber+10, value);
		return 1;
	}
	else return -3;
}

/*******************************************************************************
 * Function Name  : tinkerAnalogRead
 * Description    : Reads the analog value of a pin
 * Input          : Pin
 * Output         : None.
 * Return         : Returns the analog value in INT type (0 to 4095)
                    Returns a negative number on failure
 *******************************************************************************/
int tinkerAnalogRead(String pin)
{
	//convert ascii to integer
	int pinNumber = pin.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(pin.startsWith("D"))
	{
		return -3;
	}
	else if (pin.startsWith("A"))
	{
		return analogRead(pinNumber+10);
	}
	return -2;
}

/*******************************************************************************
 * Function Name  : tinkerAnalogWrite
 * Description    : Writes an analog value (PWM) to the specified pin
 * Input          : Pin and Value (0 to 255)
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int tinkerAnalogWrite(String command)
{
	//convert ascii to integer
	int pinNumber = command.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	String value = command.substring(3);

	if(command.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		analogWrite(pinNumber, value.toInt());
		return 1;
	}
	else if(command.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		analogWrite(pinNumber+10, value.toInt());
		return 1;
	}
	else return -2;
}

