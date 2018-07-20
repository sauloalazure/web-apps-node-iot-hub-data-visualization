// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_BME280.h>

//
// DEFINE VERSION AND DEBUG
//
#define VERSION "2018_07_20_2100"
// #define DEBUG_SERIAL
// #define DEBUG_WEB



// NEO IS PARTICLE CORE AND THEREFORE DOES NOT HAVE
// ENOUGH MEMORY FOR ALL LIBRARIES
// SET THIS TO TRUE WHEN COMPILING TO NEO

// #define IS_NEO

#ifdef IS_NEO
    #define IS_CORE
#endif




//
// CONSTANTS
//
#define SEND_AFTER_MS     10000 // run after N milli seconds connected to the cloud
#define SLEEP_DELAY_MS    10000 // after sending the message, wait N milli seconds before going to sleep
#define TRY_SENSOR_FOR_MS 10000 // try to connect to sensor for N milli seconds before giving up
#define SLEEP_FOR_S          60 // sleep for N SECONDS

#define CHANNEL_LOG       "LOG"
#define CHANNEL_SENSOR    "SENSOR"
#define CHANNEL_DATA      "HomeTemp"
#define PIN_LIGHT         D7

#ifdef DEBUG_SERIAL
    #define DEBUG_SERIAL_SPEED 115200
#endif





//
// DEFINE WHICH MODULES TO LOAD
//
#define TINKER

#ifdef IS_NEO
    #define SENSOR_HTU21D_ADAFRUIT
#endif

#ifndef IS_CORE
    #define SENSOR_HTU21D_ADAFRUIT
    // #define SENSOR_HTU21D_GENERIC
    #define SENSOR_SI7021_ADAFRUIT
    #define SENSOR_BMP085_ADAFRUIT
    #define SENSOR_BMP183_ADAFRUIT
    #define SENSOR_BME280_ADAFRUIT
    #define SENSOR_TMP36
#endif




//
// LOAD LIBRARIES FOR MODULES TO LOAD
//
#ifdef SENSOR_HTU21D_ADAFRUIT
    #define SENSOR_HTU21D
    #include <Wire.h>
    #include <adafruit-htu21df.h>
#endif

#ifdef SENSOR_HTU21D_GENERIC
    #define SENSOR_HTU21D
    #include <HTU21DF.h>
#endif

#ifdef SENSOR_HTU21D
    #define SENSOR_HTU21D_WAIT_BEGIN 5000
#endif

#ifdef SENSOR_SI7021_ADAFRUIT
    #include <Adafruit_Si7021.h>
    #define SENSOR_SI7021_WAIT_BEGIN 5000
#endif

#ifdef SENSOR_BMP085_ADAFRUIT
    #include <Adafruit_BMP085.h>
    #define SENSOR_BMP085_WAIT_BEGIN 5000
#endif

#ifdef SENSOR_BMP183_ADAFRUIT
    #include <SPI.h>
    #include <Adafruit_Sensor.h>
    #include <Adafruit_BMP183_U.h>
    // You'll also need a chip-select pin, use any pin!
    #define BMP183_CS                  A2
    #define SENSOR_BMP183_WAIT_BEGIN 5000
#endif

#ifdef SENSOR_BME280_ADAFRUIT
    // https://github.com/adafruit/Adafruit_BME280_Library
    #define BME280_ADDRESS                (0x76)
    #include <Adafruit_Sensor.h>
    #include <Adafruit_BME280.h>
    
    // #define BME_SCK D4
    // #define BME_MISO D3
    // #define BME_MOSI D2
    // #define BME_CS D5
    
    #define SEALEVELPRESSURE_HPA (1013.25)
    #define SENSOR_BME280_WAIT_BEGIN 5000
#endif

#ifdef SENSOR_TMP36
    #define AREF_VOLTAGE               3320
    #define SENSOR_TMP36_PIN             A7
    #define SENSOR_TMP36_SAMPLES          8 // samples to take
    #define SENSOR_TMP36_WAIT             2 // interval between samples
    #define SENSOR_TMP36_MAX_VOLTAGE   1550 // max voltage output at 125C for 3.3V
    #define SENSOR_TMP36_SLOPE       0.1128 // slope for pairs (mV,Tc) = (0,-50), (595.5311,18), (1550,125). 
                                            // second value is emprirical. third value leads to R-squared of zero
#endif





//
// INITIALIZE VARIABLES FOR LOADED MODULES
// DECLARE FUNCTIONS TO READ DATA
//
// standard dictated that they should be placed after all libraries are loaded
//


#ifdef SENSOR_HTU21D
    void checkSensorHTU21D();
#endif    


#ifdef SENSOR_SI7021_ADAFRUIT
    void checkSensorSI7021();
#endif


#ifdef SENSOR_BMP085_ADAFRUIT
    void checkSensorBMP085();
#endif


#ifdef SENSOR_BMP183_ADAFRUIT
    // For hardware SPI:
    // Connect SCK to SPI Clock, SDO to SPI MISO, and SDI to SPI MOSI
    // See  http://arduino.cc/en/Reference/SPI for your Arduino's SPI pins!
    // On UNO, Clock is #13, MISO is #12 and MOSI is #11
    void checkSensorBMP183();
#endif


#ifdef SENSOR_BME280_ADAFRUIT
    void checkSensorBME280();
#endif


#ifdef SENSOR_TMP36
    void   checkSensorTMP36();
    double readMilliVolts(int pin);
    double TMP36GetCentigrade(int pin);
#endif


#ifdef TINKER
    /* Function prototypes -------------------------------------------------------*/
    int tinkerDigitalRead(String pin);
    int tinkerDigitalWrite(String command);
    int tinkerAnalogRead(String pin);
    int tinkerAnalogWrite(String command);
#endif





//
// DEVICE CONFIG
//

// STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));
// retained int retainedCounter = 0;
// int retainedCounter = 0;

// template<class T, size_t N>
// constexpr size_t size(T (&)[N]) { return N; }

// template<class T>
// int32_t sizeOfArray(T p) { return (sizeof(p)/sizeof(*p)); }

// System.deviceID()

// void checkSensorHTU21D();
// void checkSensorSI7021();
// void checkSensorBMP085();
// void checkSensorBMP183();
// void checkSensorTMP36();

// typedef double (*returnDoubleFunc)(void);
// typedef int32_t (*returnIntFunc)(void);
typedef void (*returnVoidFunc)(void);


typedef struct {
    String         deviceID;
    String         deviceNick;
    returnVoidFunc func;
} deviceConf;


void emptyPlaceholderFunc(void) {};

const deviceConf deviceConfList[] = {
    #ifdef SENSOR_HTU21D
        { "53ff6d066667574834441267", "neo"    , checkSensorHTU21D    },
    #endif
    
    #ifdef SENSOR_BMP085_ADAFRUIT
        { "2e002c000a47343337373738", "morph"  , checkSensorBMP085    },
    #endif
    
    #ifdef SENSOR_SI7021_ADAFRUIT
        { "3d003c000d47343233323032", "trinity", checkSensorSI7021    },
        { "420022001147343438323536", "switch" , checkSensorSI7021    },
    #endif
    
    #ifdef SENSOR_BMP183_ADAFRUIT
        // { "200021000c47343438323536", "apoc"   , checkSensorBMP183    },
    #endif

    #ifdef SENSOR_TMP36
        { "200021000c47343438323536", "apoc"   , checkSensorBME280    },
    #endif
    
    // { "420022001147343438323536", "switch" , emptyPlaceholderFunc } // offline
};

const String myDeviceID = System.deviceID();


//
// LOOP VARIABLES
//
unsigned long firstAvailable = 0;
bool          wifiReady      = false;
bool          cloudReady     = false;
// const int numDevices = 5;
int           numDevices     = 0;
char          publishString[254];





/* This function is called once at start up ----------------------------------*/
void setup()
{
	//Setup the Tinker application here
    #ifdef DEBUG_SERIAL
        Serial.begin(DEBUG_SERIAL_SPEED);
    
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

    pinMode(PIN_LIGHT, OUTPUT);
	digitalWrite(PIN_LIGHT, LOW);

    firstAvailable = 0;
    wifiReady      = false;
    cloudReady     = false;
    numDevices     = sizeof(deviceConfList)/sizeof(deviceConfList[0]);
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
		if (millis() - firstAvailable > SEND_AFTER_MS) {
			// After we've been up for 30 seconds, go to sleep. The delay is so the serial output gets written out before
			// sleeping.
// 			Serial.println("calling System.sleep(SLEEP_MODE_DEEP, 30)");

            #ifdef DEBUG_SERIAL
    	        Serial.println("connected. sending signal");
            #endif

            digitalWrite(PIN_LIGHT, HIGH);
            
                        
            for ( int i = 0; i < numDevices; ++i ) {
                if ( deviceConfList[i].deviceID == myDeviceID ) {
                    #ifdef DEBUG_SERIAL
                        Serial.println("Machine identified :: Id: " + deviceConfList[i].deviceID + " Nick: " + deviceConfList[i].deviceNick);
                    #endif
                    
                    #ifdef DEBUG_WEB
                        Particle.publish(CHANNEL_LOG,"Machine identified :: Id: " + deviceConfList[i].deviceID + " Nick: " + deviceConfList[i].deviceNick,PRIVATE);
                    #endif
                    
                    deviceConfList[i].func();
                }
            }

            #ifdef DEBUG_SERIAL
    	        Serial.println("signal sent. sleeping");
            #endif

			digitalWrite(PIN_LIGHT, LOW);

			delay(SLEEP_DELAY_MS);

			System.sleep(SLEEP_MODE_DEEP, SLEEP_FOR_S);

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





//
// FUNCTIONS TO READ SENSORS
//
#ifdef SENSOR_HTU21D
    void checkSensorHTU21D() {
        #ifdef DEBUG_WEB
            Particle.publish(CHANNEL_SENSOR,"SENSOR_HTU21D",PRIVATE);
        #endif
    
        #ifdef DEBUG_SERIAL
        	Serial.println("setting up HTU21D");
        #endif
    
        // htu.begin();
        // delay(SENSOR_HTU21D_WAIT_BEGIN);
        
        #ifdef DEBUG_WEB
            Particle.publish(CHANNEL_LOG,"TRYING I2C",PRIVATE);
        #endif


        #ifdef SENSOR_HTU21D_ADAFRUIT
            Adafruit_HTU21DF htu = Adafruit_HTU21DF();
        #endif
        
        #ifdef SENSOR_HTU21D_GENERIC
            HTU21DF          htu = HTU21DF();
        #endif


        unsigned long sensorStartTime = millis();
        bool          sensorSuccess   = true;

    	while(! htu.begin()){
    	    #ifdef DEBUG_WEB
                Particle.publish(CHANNEL_LOG,"WIRE IS NOT ENABLED",PRIVATE);
            #endif
    
            #ifdef DEBUG_SERIAL
            	Serial.print("HTU21D not found. waiting ");
            	Serial.print(SENSOR_HTU21D_WAIT_BEGIN);
            	Serial.println("");
            #endif
    	    
    	    if (millis() - sensorStartTime > TRY_SENSOR_FOR_MS) {
    	        sensorSuccess   = false;
    	        break;   
    	    }
        
    	    delay(SENSOR_HTU21D_WAIT_BEGIN);
    	}

        if ( sensorSuccess ) {
        	#ifdef DEBUG_WEB
        	    Particle.publish(CHANNEL_LOG,"I2C SUCCESSFUL",PRIVATE);
        	#endif
            
            #ifdef DEBUG_SERIAL
                Serial.println("HTU21D found. continuing");
            #endif
        
    	    delay(SENSOR_HTU21D_WAIT_BEGIN);

            sprintf(publishString,"{\"t\":%0.1f,\"h\":%0.1f,\"p\":null,\"s\":\"HTU21D\",\"v\":\"%s\"}",htu.readTemperature(),htu.readHumidity(),VERSION);
        
            #ifdef DEBUG_SERIAL
        	    Serial.print("sending HTU21D ");
        	    Serial.print(publishString);
        	    Serial.println("");
            #endif
            
            Particle.publish(CHANNEL_DATA, publishString, PRIVATE);
        } else {
            #ifdef DEBUG_WEB
        	    Particle.publish(CHANNEL_LOG,"I2C UNSUCCESSFUL",PRIVATE);
        	#endif
            
            #ifdef DEBUG_SERIAL
                Serial.println("HTU21D not found. skipping");
            #endif
        }
    }
#endif



#ifdef SENSOR_SI7021_ADAFRUIT
    void checkSensorSI7021() {
        #ifdef DEBUG_WEB
            Particle.publish(CHANNEL_SENSOR,"SENSOR_SI7021_ADAFRUIT",PRIVATE);
        #endif

        #ifdef DEBUG_SERIAL
        	Serial.println("setting up SI7021");
        #endif

        #ifdef DEBUG_WEB
            Particle.publish(CHANNEL_LOG,"TRYING I2C",PRIVATE);
        #endif

        Adafruit_Si7021 si7021 = Adafruit_Si7021();

        unsigned long sensorStartTime = millis();
        bool          sensorSuccess   = true;

    	while(! si7021.begin()){
    	    #ifdef DEBUG_WEB
                Particle.publish(CHANNEL_LOG,"WIRE IS NOT ENABLED",PRIVATE);
            #endif

            #ifdef DEBUG_SERIAL
            	Serial.print("SI7021 not found. waiting ");
            	Serial.print(SENSOR_SI7021_WAIT_BEGIN);
            	Serial.println("");
            #endif

    	    if (millis() - sensorStartTime > TRY_SENSOR_FOR_MS) {
    	        sensorSuccess   = false;
    	        break;   
    	    }
        
    	    delay(SENSOR_SI7021_WAIT_BEGIN);
    	}

        if ( sensorSuccess ) {
        	#ifdef DEBUG_WEB
        	    Particle.publish(CHANNEL_LOG,"I2C SUCCESSFUL",PRIVATE);
        	#endif

            #ifdef DEBUG_SERIAL
                Serial.println("SI7021 found. continuing");
            #endif
    
    	    delay(SENSOR_SI7021_WAIT_BEGIN);

            sprintf(publishString,"{\"t\":%0.1f,\"h\":%0.1f,\"p\":null,\"s\":\"SI7021\",\"v\":\"%s\"}",si7021.readTemperature(),si7021.readHumidity(),VERSION);

            #ifdef DEBUG_SERIAL
        	    Serial.print("sending SI7021 ");
        	    Serial.print(publishString);
        	    Serial.println("");
            #endif
            
            Particle.publish(CHANNEL_DATA, publishString, PRIVATE);
        } else {
            #ifdef DEBUG_WEB
        	    Particle.publish(CHANNEL_LOG,"I2C UNSUCCESSFUL",PRIVATE);
        	#endif
            
            #ifdef DEBUG_SERIAL
                Serial.println("SI7021 not found. skipping");
            #endif
        }
    }
#endif



#ifdef SENSOR_BMP085_ADAFRUIT
    void checkSensorBMP085() {
        #ifdef DEBUG_WEB
            Particle.publish(CHANNEL_SENSOR,"SENSOR_BMP085",PRIVATE);
        #endif

        #ifdef DEBUG_SERIAL
        	Serial.println("setting up BMP085");
        #endif

        #ifdef DEBUG_WEB
            Particle.publish(CHANNEL_LOG,"TRYING I2C",PRIVATE);
        #endif

        Adafruit_BMP085 bmp085 = Adafruit_BMP085();

        unsigned long sensorStartTime = millis();
        bool          sensorSuccess   = true;

    	while(! bmp085.begin()){
    	    #ifdef DEBUG_WEB
                Particle.publish(CHANNEL_LOG,"WIRE IS NOT ENABLED",PRIVATE);
            #endif

            #ifdef DEBUG_SERIAL
            	Serial.print("BMP085 not found. waiting ");
            	Serial.print(SENSOR_BMP085_WAIT_BEGIN);
            	Serial.println("");
            #endif

    	    if (millis() - sensorStartTime > TRY_SENSOR_FOR_MS) {
    	        sensorSuccess   = false;
    	        break;   
    	    }
        
    	    delay(SENSOR_BMP085_WAIT_BEGIN);
    	}


        if ( sensorSuccess ) {
        	#ifdef DEBUG_WEB
        	    Particle.publish(CHANNEL_LOG,"I2C SUCCESSFUL",PRIVATE);
        	#endif
            
            #ifdef DEBUG_SERIAL
                Serial.println("BMP085 found. continuing");
            #endif
    
    	    delay(SENSOR_BMP085_WAIT_BEGIN);

            sprintf(publishString,"{\"t\":%0.1f,\"h\":null,\"p\":%ld,\"s\":\"BMP085\",\"v\":\"%s\"}",bmp085.readTemperature(),bmp085.readPressure(),VERSION);
    
            #ifdef DEBUG_SERIAL
        	    Serial.print("sending BMP085 ");
        	    Serial.print(publishString);
        	    Serial.println("");
            #endif
            
            Particle.publish(CHANNEL_DATA, publishString, PRIVATE);
        } else {
            #ifdef DEBUG_WEB
        	    Particle.publish(CHANNEL_LOG,"I2C UNSUCCESSFUL",PRIVATE);
        	#endif
            
            #ifdef DEBUG_SERIAL
                Serial.println("BMP085 not found. skipping");
            #endif
        }
    }
#endif



#ifdef SENSOR_BMP183_ADAFRUIT
    void checkSensorBMP183() {
        #ifdef DEBUG_WEB
            Particle.publish(CHANNEL_SENSOR,"SENSOR_BMP183",PRIVATE);
        #endif

        #ifdef DEBUG_SERIAL
        	Serial.println("setting up BMP183");
        #endif

        #ifdef DEBUG_WEB
            Particle.publish(CHANNEL_LOG,"TRYING SPI",PRIVATE);
        #endif

        Adafruit_BMP183_Unified bmp183 = Adafruit_BMP183_Unified(BMP183_CS);  // use hardware SPI

        unsigned long sensorStartTime = millis();
        bool          sensorSuccess   = true;

    	while(! bmp183.begin()){
    	    #ifdef DEBUG_WEB
                Particle.publish(CHANNEL_LOG,"SPI IS NOT ENABLED",PRIVATE);
            #endif

            #ifdef DEBUG_SERIAL
            	Serial.print("BMP183 not found. waiting ");
            	Serial.print(SENSOR_BMP183_WAIT_BEGIN);
            	Serial.println("");
            #endif
        
    	    delay(SENSOR_BMP183_WAIT_BEGIN);

    	    if (millis() - sensorStartTime > TRY_SENSOR_FOR_MS) {
    	        sensorSuccess   = false;
    	        break;   
    	    }
    	}


        if ( sensorSuccess ) {
        	#ifdef DEBUG_WEB
        	    Particle.publish(CHANNEL_LOG,"SPI SUCCESSFUL",PRIVATE);
        	#endif
            
            #ifdef DEBUG_SERIAL
                Serial.println("BMP183 found. continuing");
        	#endif

    	    delay(SENSOR_BMP183_WAIT_BEGIN);

            #ifdef DEBUG_SERIAL
                sensor_t sensor;
                bmp183.getSensor(&sensor);
                Serial.println("------------------------------------");
                Serial.print  ("Sensor:       "); Serial.println(sensor.name      );
                Serial.print  ("Driver Ver:   "); Serial.println(sensor.version   );
                Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id );
                Serial.print  ("Max Value:    "); Serial.print(  sensor.max_value ); Serial.println(" hPa");
                Serial.print  ("Min Value:    "); Serial.print(  sensor.min_value ); Serial.println(" hPa");
                Serial.print  ("Resolution:   "); Serial.print(  sensor.resolution); Serial.println(" hPa");  
                Serial.println("------------------------------------");
                Serial.println("");
                delay(500);
            #endif

            sensors_event_t event;
            bmp183.getEvent(&event);
            
            /* Display the results (barometric pressure is measure in hPa) */
            if (event.pressure)
            {
                int32_t pressure = (int32_t)(bmp183.getPressure());
                sprintf(publishString,"{\"t\":%0.1f,\"h\":null,\"p\":%ld,\"s\":\"BMP183\",\"v\":\"%s\"}",bmp183.getTemperature(),pressure,VERSION);
    
                #ifdef DEBUG_SERIAL
            	    Serial.print("sending BMP183 ");
            	    Serial.print(publishString);
            	    Serial.println("");
                #endif
                
                Particle.publish(CHANNEL_DATA, publishString, PRIVATE);
            } else {
                #ifdef DEBUG_WEB
        	        Particle.publish(CHANNEL_LOG,"SENSOR ERROR",PRIVATE);
        	    #endif

                #ifdef DEBUG_SERIAL
                    Serial.println("BMP183 error");
                #endif
            }
        } else {
            #ifdef DEBUG_WEB
        	    Particle.publish(CHANNEL_LOG,"I2C UNSUCCESSFUL",PRIVATE);
        	#endif
            
            #ifdef DEBUG_SERIAL
                Serial.println("BMP183 not found. skipping");
            #endif
        }
    }
#endif


#ifdef SENSOR_BME280_ADAFRUIT
    void checkSensorBME280() {

        // https://github.com/adafruit/Adafruit_BME280_Library

        #ifdef DEBUG_WEB
            Particle.publish(CHANNEL_SENSOR,"SENSOR_BME280",PRIVATE);
        #endif

        #ifdef DEBUG_SERIAL
        	Serial.println("setting up BME280");
        #endif

        #ifdef DEBUG_WEB
            Particle.publish(CHANNEL_LOG,"TRYING I2C",PRIVATE);
        #endif

        Adafruit_BME280 bme280 = Adafruit_BME280(); // I2C

        unsigned long sensorStartTime = millis();
        bool          sensorSuccess   = true;

    	while(! bme280.begin(BME280_ADDRESS)){
    	    #ifdef DEBUG_WEB
                Particle.publish(CHANNEL_LOG,"I2C IS NOT ENABLED",PRIVATE);
            #endif

            #ifdef DEBUG_SERIAL
            	Serial.print("BME280 not found. waiting ");
            	Serial.print(SENSOR_BME280_WAIT_BEGIN);
            	Serial.println("");
            #endif
        
    	    delay(SENSOR_BME280_WAIT_BEGIN);

    	    if (millis() - sensorStartTime > TRY_SENSOR_FOR_MS) {
    	        sensorSuccess   = false;
    	        break;   
    	    }
    	}


        if ( sensorSuccess ) {
        	#ifdef DEBUG_WEB
        	    Particle.publish(CHANNEL_LOG,"I2C SUCCESSFUL",PRIVATE);
        	#endif
            
            #ifdef DEBUG_SERIAL
                Serial.println("BME280 found. continuing");
        	#endif

    	    delay(SENSOR_BME280_WAIT_BEGIN);

            float temperature = bme280.readTemperature();
            delay(10);
            float pressure    = bme280.readPressure();
            delay(10);
            float humidity    = bme280.readHumidity();
            // float altitude    = bme280.readAltitude(SEALEVELPRESSURE_HPA);

            #ifdef DEBUG_SERIAL
                Serial.print("Temperature = ");
                Serial.print(temperature);
                Serial.println(" *C");
                
                Serial.print("Pressure = ");
                
                Serial.print(pressure / 100.0F);
                Serial.println(" hPa");
                
                // Serial.print("Approx. Altitude = ");
                // Serial.print(altitude);
                // Serial.println(" m");
                
                Serial.print("Humidity = ");
                Serial.print(humidity);
                Serial.println(" %");

                delay(500);
            #endif

            if (pressure)
            {
                int32_t pressureI = (int32_t)(pressure);
                sprintf(publishString,"{\"t\":%0.1f,\"h\":%0.1f,\"p\":%ld,\"s\":\"BME280\",\"v\":\"%s\"}",temperature,humidity,pressureI,VERSION);
    
                #ifdef DEBUG_SERIAL
            	    Serial.print("sending BME280 ");
            	    Serial.print(publishString);
            	    Serial.println("");
                #endif
                
                Particle.publish(CHANNEL_DATA, publishString, PRIVATE);
            } else {
                #ifdef DEBUG_WEB
        	        Particle.publish(CHANNEL_LOG,"SENSOR ERROR",PRIVATE);
        	    #endif

                #ifdef DEBUG_SERIAL
                    Serial.println("BME280 error");
                #endif
            }
        } else {
            #ifdef DEBUG_WEB
        	    Particle.publish(CHANNEL_LOG,"I2C UNSUCCESSFUL",PRIVATE);
        	#endif
            
            #ifdef DEBUG_SERIAL
                Serial.println("BME280 not found. skipping");
            #endif
        }
    }
#endif



#ifdef SENSOR_TMP36
    void checkSensorTMP36() {
        #ifdef DEBUG_WEB
            Particle.publish(CHANNEL_SENSOR,"SENSOR_TMP36",PRIVATE);
        #endif
    
        // Spark.variable("temperature", &temperature, DOUBLE);
        #ifdef DEBUG_SERIAL
        	Serial.println("setting up TMP36");
        #endif
        
        pinMode(SENSOR_TMP36_PIN, INPUT);
        analogRead(SENSOR_TMP36_PIN);
    
        sprintf(publishString,"{\"t\":%0.1f,\"h\":null,\"p\":null,\"s\":\"TMP36\",\"v\":\"%s\"}",TMP36GetCentigrade(SENSOR_TMP36_PIN),VERSION);
    
        #ifdef DEBUG_SERIAL
    	    Serial.print("sending TMP36 ");
    	    Serial.print(publishString);
    	    Serial.println("");
        #endif
    
        Particle.publish(CHANNEL_DATA, publishString, PRIVATE);
    }
    
    double readMilliVolts(int pin) {
        double val = 0.0;
        
        for ( int i = 0; i < SENSOR_TMP36_SAMPLES; ++i ) {
            val += analogRead(pin);
            delay(SENSOR_TMP36_WAIT);
        }
        
        val /= SENSOR_TMP36_SAMPLES;
        
    	return map(val, double(0), double(4095), double(0), double(AREF_VOLTAGE));
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

