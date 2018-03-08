#define VERSION "201803082134"
//#define DEBUG_SERIAL
//#define DEBUG_WEB



#define TINKER

// #define SENSOR_TMP36
// #define SENSOR_HTU21D_ADAFRUIT
// #define SENSOR_HTU21D_GENERIC
#define SENSOR_BMP085



#ifdef SENSOR_HTU21D_ADAFRUIT
#define SENSOR_HTU21D
#include <Wire.h>
#include <adafruit-htu21df.h>
#endif

#ifdef SENSOR_HTU21D_GENERIC
#define SENSOR_HTU21D
#include <HTU21DF.h>
#endif

#ifdef SENSOR_BMP085
#include <Adafruit_BMP085.h>
#endif






#ifdef SENSOR_HTU21D_ADAFRUIT
Adafruit_HTU21DF htu = Adafruit_HTU21DF();
#endif

#ifdef SENSOR_HTU21D_GENERIC
HTU21DF htu = HTU21DF();
#endif

#ifdef SENSOR_HTU21D
#define SENSOR_HTU21D_WAIT_BEGIN 5000
#endif


#ifdef SENSOR_BMP085
#define SENSOR_BMP085_WAIT_BEGIN 5000
Adafruit_BMP085 bmp;
#endif


#ifdef SENSOR_TMP36
const int     pinTemp36      = A7;
#define AREF_VOLTAGE             3320
#define SENSOR_TMP36_SAMPLES        8   // samples to take
#define SENSOR_TMP36_WAIT           2   // interval between samples
#define SENSOR_TMP36_MAX_VOLTAGE 1550   // max voltage output at 125C for 3.3V
#define SENSOR_TMP36_SLOPE       0.1128 // slope for pairs (mV,Tc) = (0,-50), (595.5311,18), (1550,125). 
                                        // second value is emprirical. third value leads to R-squared of zero
#endif



#ifdef TINKER
/* Function prototypes -------------------------------------------------------*/
int tinkerDigitalRead(String pin);
int tinkerDigitalWrite(String command);
int tinkerAnalogRead(String pin);
int tinkerAnalogWrite(String command);
#endif

// STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));
// retained int retainedCounter = 0;
// int retainedCounter = 0;

// int counter;



// CONSTANTS
const char*   channelName    = "HomeTemp";
const int     sendAfterS     = 10; // run after N seconds connected to the cloud
const int     sleepDelayS    = 10; // after sending the message, wait N seconds before going to sleep
const int     sleepForS      = 60; // sleep for N seconds

const int     pinLight       = D7;



// LOOP VARIABLES
unsigned long firstAvailable = 0;
bool          wifiReady      = false;
bool          cloudReady     = false;
char          publishString[254];

/* This function is called once at start up ----------------------------------*/
void setup()
{
	//Setup the Tinker application here

    #ifdef DEBUG_SERIAL
        Serial.begin(115200);
    
    	Serial.println("SERIAL DEBUG");
    
    	Serial.println("registering functions");
    #endif


    #ifdef TINKER
    	//Register all the Tinker functions
    	Particle.function("digitalread" , tinkerDigitalRead );
    	Particle.function("digitalwrite", tinkerDigitalWrite);
    	Particle.function("analogread"  , tinkerAnalogRead  );
    	Particle.function("analogwrite" , tinkerAnalogWrite );
    #endif
    

    #ifdef DEBUG_SERIAL
    	Serial.println("setting blue led low");
    #endif

    pinMode(pinLight, OUTPUT);
	digitalWrite(pinLight, LOW);


    // Spark.variable("temperature", &temperature, DOUBLE);
    #ifdef SENSOR_TMP36
        #ifdef DEBUG_SERIAL
        	Serial.println("setting up TMP36");
        #endif
        
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

            #ifdef DEBUG_SERIAL
    	        Serial.println("connected. sending signal");
            #endif

            digitalWrite(pinLight, HIGH);






            #ifdef SENSOR_TMP36
                #ifdef DEBUG_WEB
                    Particle.publish("SENSOR","SENSOR_TMP36",PRIVATE);
                #endif
                sprintf(publishString,"{\"t\":%0.2f,\"h\":null,\"p\":null,\"s\":\"TMP36\",\"v\":\"%s\"}",TMP36GetCentigrade(pinTemp36),VERSION);
                #ifdef DEBUG_SERIAL
            	    Serial.print("sending TMP36 ");
            	    Serial.print(publishString);
            	    Serial.println("");
                #endif
                Particle.publish(channelName, publishString, PRIVATE);
            #endif







            #ifdef SENSOR_HTU21D
                #ifdef DEBUG_WEB
                    Particle.publish("SENSOR","SENSOR_HTU21D",PRIVATE);
                #endif

                #ifdef DEBUG_SERIAL
                	Serial.println("setting up HTU21D");
                #endif

                // htu.begin();
                // delay(SENSOR_HTU21D_WAIT_BEGIN);
                
                #ifdef DEBUG_WEB
                    Particle.publish("LOG","TRYING I2C",PRIVATE);
                #endif
            	while(! htu.begin()){
            	    #ifdef DEBUG_WEB
                        Particle.publish("LOG","WIRE IS NOT ENABLED",PRIVATE);
                    #endif

                    #ifdef DEBUG_SERIAL
                    	Serial.print("HTU21D not found. waiting ");
                    	Serial.print(SENSOR_HTU21D_WAIT_BEGIN);
                    	Serial.println("");
                    #endif
                
            	    delay(SENSOR_HTU21D_WAIT_BEGIN);
            	}
            	#ifdef DEBUG_WEB
            	    Particle.publish("LOG","I2C SUCCESSFUL",PRIVATE);
            	#endif
                
                #ifdef DEBUG_SERIAL
                    Serial.println("HTU21D found. continuing");
                #endif

                sprintf(publishString,"{\"t\":%0.2f,\"h\":%0.2f,\"p\":null,\"s\":\"HTU21D\",\"v\":\"%s\"}",htu.readTemperature(),htu.readHumidity(),VERSION);
    
                #ifdef DEBUG_SERIAL
            	    Serial.print("sending HTU21D ");
            	    Serial.print(publishString);
            	    Serial.println("");
                #endif
                
                Particle.publish(channelName, publishString, PRIVATE);
            #endif





            #ifdef SENSOR_BMP085
                #ifdef DEBUG_WEB
                    Particle.publish("SENSOR","SENSOR_BMP085",PRIVATE);
                #endif

                #ifdef DEBUG_SERIAL
                	Serial.println("setting up BMP085");
                #endif

                #ifdef DEBUG_WEB
                    Particle.publish("LOG","TRYING I2C",PRIVATE);
                #endif

            	while(! bmp.begin()){
            	    #ifdef DEBUG_WEB
                        Particle.publish("LOG","WIRE IS NOT ENABLED",PRIVATE);
                    #endif

                    #ifdef DEBUG_SERIAL
                    	Serial.print("BMP085 not found. waiting ");
                    	Serial.print(SENSOR_BMP085_WAIT_BEGIN);
                    	Serial.println("");
                    #endif
                
            	    delay(SENSOR_BMP085_WAIT_BEGIN);
            	}
            	#ifdef DEBUG_WEB
            	    Particle.publish("LOG","I2C SUCCESSFUL",PRIVATE);
            	#endif
                
                #ifdef DEBUG_SERIAL
                    Serial.println("BMP085 found. continuing");
                #endif

                sprintf(publishString,"{\"t\":%0.2f,\"h\":null,\"p\":%ld,\"s\":\"BMP085\",\"v\":\"%s\"}",bmp.readTemperature(),bmp.readPressure(),VERSION);
    
                #ifdef DEBUG_SERIAL
            	    Serial.print("sending BMP085 ");
            	    Serial.print(publishString);
            	    Serial.println("");
                #endif
                
                Particle.publish(channelName, publishString, PRIVATE);
            #endif






            #ifdef DEBUG_SERIAL
    	        Serial.println("signal sent. sleeping");
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
double readMilliVolts(int pin) {
    double val = 0.0;
    for ( int i = 0; i < SENSOR_TMP36_SAMPLES; ++i ) {
        val += analogRead(pin);
        delay(SENSOR_TMP36_WAIT);
    }
    
    val /= SENSOR_TMP36_SAMPLES;
    
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






#ifdef TINKER

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

#endif
