#include "Particle.h"

STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY));
// PRODUCT_VERSION(1);

#include <cstring>

//
// DEFINE VERSION AND DEBUG
//
#define VERSION "2018_10_21_0001"
// #define DEBUG_SERIAL
#define DEBUG_WEB



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
#define SEND_AFTER_MS      10000 // run after N milli seconds connected to the cloud
#define SLEEP_DELAY_MS     30000 // after sending the message, wait N milli seconds before going to sleep
#define TRY_SENSOR_FOR_MS  10000 // try to connect to sensor for N milli seconds before giving up
#define SLEEP_FOR_S           60 // sleep for N SECONDS
#define UPDATE_CONF_EVERY_S 3600; // 3600 = 1h 86400 1 day

#define CHANNEL_DATA        0
#define CHANNEL_SENSOR      1
#define CHANNEL_TINKER      2
#define CHANNEL_ERROR       3
#define CHANNEL_WARN        4
#define CHANNEL_LOG         5
#define CHANNEL_INFO        6

#define CHANNEL_ROOT        "HomeTemp"
#define CHANNEL_DATA_NAME   "HomeTemp"
#define CHANNEL_SENSOR_NAME CHANNEL_ROOT"/sensor"
#define CHANNEL_TINKER_NAME CHANNEL_ROOT"/tinker"
#define CHANNEL_ERROR_NAME  CHANNEL_ROOT"/log/error"
#define CHANNEL_WARN_NAME   CHANNEL_ROOT"/log/warn"
#define CHANNEL_LOG_NAME    CHANNEL_ROOT"/log/log"
#define CHANNEL_INFO_NAME   CHANNEL_ROOT"/log/info"

#define CHANNEL_VERBOSITY   CHANNEL_ERROR

#define PIN_LIGHT                    D7

#define REPONSE_BUFFER_SIZE        2048
#define MAX_DEVICE_CONF_LIST         20
#define JSON_BUFFER_SIZE           1024
#define PUBLISH_STRING_BUFFER_SIZE  254

#ifdef DEBUG_SERIAL
    #define DEBUG_SERIAL_SPEED 115200
#endif




// https://community.particle.io/t/retained-memory-tips/38137
retained int           send_after_ms        = SEND_AFTER_MS;     // 10000 // run after N milli seconds connected to the cloud
retained int           sleep_delay_ms       = SLEEP_DELAY_MS;    // 30000 // after sending the message, wait N milli seconds before going to sleep
retained int           try_sensor_for_ms    = TRY_SENSOR_FOR_MS; // 10000 // try to connect to sensor for N milli seconds before giving up
retained int           verbosity            = CHANNEL_VERBOSITY; // verbosity
retained int           sleep_for_s          = SLEEP_FOR_S;       //    60 // sleep for N SECONDS
retained int           deviceConfListSize   = 0;
retained system_tick_t deviceConfLastUpdate = 0;
retained system_tick_t update_conf_every_s  = UPDATE_CONF_EVERY_S; // 3600 = 1h 86400 1 day



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
//
// INITIALIZE VARIABLES FOR LOADED MODULES
// DECLARE FUNCTIONS TO READ DATA
//
// standard dictated that they should be placed after all libraries are loaded
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
    #define SENSOR_HTU21D_NAME       "HTU21D"
    void checkSensorHTU21D();
#endif

#ifdef SENSOR_SI7021_ADAFRUIT
    #include <Adafruit_Si7021.h>
    #define SENSOR_SI7021_WAIT_BEGIN    5000
    #define SENSOR_SI7021_ADAFRUIT_NAME "SI7021_adafruit"
    void checkSensorSI7021();
#endif

#ifdef SENSOR_BMP085_ADAFRUIT
    #include <Adafruit_BMP085.h>
    #define SENSOR_BMP085_WAIT_BEGIN    5000
    #define SENSOR_BMP085_ADAFRUIT_NAME "BMP085_adafruit"
    void checkSensorBMP085();
#endif

#ifdef SENSOR_BMP183_ADAFRUIT
    #include <SPI.h>
    #include <Adafruit_Sensor.h>
    #include <Adafruit_BMP183_U.h>
    // You'll also need a chip-select pin, use any pin!
    #define BMP183_CS                   A2
    #define SENSOR_BMP183_WAIT_BEGIN    5000
    #define SENSOR_BMP183_ADAFRUIT_NAME "BMP183_adafruit"
    // For hardware SPI:
    // Connect SCK to SPI Clock, SDO to SPI MISO, and SDI to SPI MOSI
    // See  http://arduino.cc/en/Reference/SPI for your Arduino's SPI pins!
    // On UNO, Clock is #13, MISO is #12 and MOSI is #11
    void checkSensorBMP183();
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
    
    #define SEALEVELPRESSURE_HPA        (1013.25)
    #define SENSOR_BME280_WAIT_BEGIN    5000
    #define SENSOR_BME280_ADAFRUIT_NAME "BME280_adafruit"
    void checkSensorBME280();
#endif

#ifdef SENSOR_TMP36
    #define AREF_VOLTAGE                  3320
    #define SENSOR_TMP36_PIN                A7
    #define SENSOR_TMP36_SAMPLES             8 // samples to take
    #define SENSOR_TMP36_WAIT                2 // interval between samples
    #define SENSOR_TMP36_MAX_VOLTAGE      1550 // max voltage output at 125C for 3.3V
    #define SENSOR_TMP36_SLOPE          0.1128 // slope for pairs (mV,Tc) = (0,-50), (595.5311,18), (1550,125). 
                                               // second value is emprirical. third value leads to R-squared of zero
    #define SENSOR_TMP36_NAME           "TMP36"
    void   checkSensorTMP36();
    double readMilliVolts(int pin);
    double TMP36GetCentigrade(int pin);
#endif

#define SENSOR_EMPTY_NAME "EMPTY"


#ifdef TINKER
    /* Function prototypes -------------------------------------------------------*/
    int tinkerDigitalRead        (String pin);
    int tinkerDigitalWrite       (String command);
    int tinkerAnalogRead         (String pin);
    int tinkerAnalogWrite        (String command);

    int tinkerSendAfterMsSet     (String ms);
	int tinkerSendAfterMsGet     (String command);

	int tinkerSleepDelayMsSet    (String ms);
	int tinkerSleepDelayMsGet    (String command);

	int tinkerTrySensorForMsSet  (String ms);
	int tinkerTrySensorForMsGet  (String command);

	int tinkerSleepForMsSet      (String ms);
	int tinkerSleepForMsGet      (String command);

	int tinkerVerbositySet       (String level);
	int tinkerVerbosityGet       (String command);

	int tinkerUpdateConfEverySet (String seconds);
	int tinkerUpdateConfEveryGet (String command);
#endif







void log_serial(const String& msg, const bool& endl=true) {
    #ifdef DEBUG_SERIAL
        if ( endl ) {
            Serial.println(msg);
        } else {
            Serial.print(msg);
        }
    #endif
}

void log_web(const String& msg, const int& channel) {
    #ifdef DEBUG_WEB
        if ( channel <= verbosity ) {
            if ( Particle.connected() ) {
                String channel_name;
                bool   hasChannel = false;
                
                switch (channel) {
                    case CHANNEL_DATA  : channel_name = CHANNEL_DATA_NAME  ; hasChannel = true; break;
                    case CHANNEL_SENSOR: channel_name = CHANNEL_SENSOR_NAME; hasChannel = true; break;
                    case CHANNEL_TINKER: channel_name = CHANNEL_TINKER_NAME; hasChannel = true; break;
                    case CHANNEL_ERROR : channel_name = CHANNEL_ERROR_NAME ; hasChannel = true; break;
                    case CHANNEL_WARN  : channel_name = CHANNEL_WARN_NAME  ; hasChannel = true; break;
                    case CHANNEL_LOG   : channel_name = CHANNEL_LOG_NAME   ; hasChannel = true; break;
                    case CHANNEL_INFO  : channel_name = CHANNEL_INFO_NAME  ; hasChannel = true; break;
                    default: break;
                }
                
                if ( hasChannel ) {
                    Particle.publish(channel_name, msg, PRIVATE);
                    delay(500);
                }
            } 
        }
    #endif
}



void logger(const String& msg, const int& channel) {
    log_serial(msg);
    log_web(msg, channel);
}

void updateNumbers();
void runFuncForDevice();
void printVariables();




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
void emptyPlaceholderFunc(void) {};

typedef struct {
    char           deviceID[30];
    char           deviceNick[30];
    char           sensorName[30];
} deviceConf;


typedef struct {
    String         sensorName;
    returnVoidFunc func;
} sensorConf;



const sensorConf sensorConfList[] = {
    #ifdef SENSOR_HTU21D
        { SENSOR_HTU21D_NAME         , checkSensorHTU21D    },
    #endif
    
    #ifdef SENSOR_BMP085_ADAFRUIT
        { SENSOR_BMP085_ADAFRUIT_NAME, checkSensorBMP085    },
    #endif
    
    #ifdef SENSOR_SI7021_ADAFRUIT
        { SENSOR_SI7021_ADAFRUIT_NAME, checkSensorSI7021    },
    #endif
    
    #ifdef SENSOR_BMP183_ADAFRUIT
        { SENSOR_BMP183_ADAFRUIT_NAME, checkSensorBMP183    },
    #endif

    #ifdef SENSOR_BME280_ADAFRUIT
        { SENSOR_BME280_ADAFRUIT_NAME, checkSensorBME280    },
    #endif

    #ifdef SENSOR_TMP36
        { SENSOR_TMP36_NAME          , checkSensorTMP36     },
    #endif
    
        { SENSOR_EMPTY_NAME          , emptyPlaceholderFunc }
};


















#define USE_HTTP
#ifndef USE_HTTP
    bool getConfig() {
        return true;
    }
    
    const deviceConf deviceConfList[] = {
        #ifdef SENSOR_HTU21D
            { "53ff6d066667574834441267", "neo"       , SENSOR_HTU21D_NAME },
        #endif
        
        #ifdef SENSOR_BMP085_ADAFRUIT
            // { "2e002c000a47343337373738", "morph"  , SENSOR_BMP085_ADAFRUIT_NAME },
        #endif
        
        #ifdef SENSOR_SI7021_ADAFRUIT
            // { "3d003c000d47343233323032", "trinity", SENSOR_SI7021_ADAFRUIT_NAME    },
            // { "420022001147343438323536", "switch" , SENSOR_SI7021_ADAFRUIT_NAME    },
        #endif
        
        #ifdef SENSOR_BMP183_ADAFRUIT
            // { "200021000c47343438323536", "apoc"   , SENSOR_BMP183_ADAFRUIT_NAME    },
        #endif
    
        #ifdef SENSOR_BME280_ADAFRUIT
            { "2e002c000a47343337373738", "morph"     , SENSOR_BME280_ADAFRUIT_NAME    },
            { "3d003c000d47343233323032", "trinity"   , SENSOR_BME280_ADAFRUIT_NAME    },
            // { "420022001147343438323536", "switchDEAD", SENSOR_BME280_ADAFRUIT_NAME    },
            { "290045000447363339343638", "switch"    , SENSOR_BME280_ADAFRUIT_NAME    },
            { "200021000c47343438323536", "apoc"      , SENSOR_BME280_ADAFRUIT_NAME    },
        #endif
    
        #ifdef SENSOR_TMP36
            // { "-"                       , "-"         , SENSOR_TMP36_NAME              },
        #endif
        
        // { "420022001147343438323536", "switch" , SENSOR_EMPTY_NAME } // offline
    };

#else
    //https://github.com/hirotakaster/TlsTcpClient
    #include "application.h"
    #include "TlsTcpClient.h"
    #include <ArduinoJson.h>

    //
    //github.com 
    //EXPIRES 2020 05 13
    //CN      www.github.com
    //O       GitHub, Inc.
    //SERIAL  1B:1F:5E:AA:BF:50:54:45:A7:1C:2E:FC:7C:64:AF:5B
    //SHA-256 9E:9A:D3:18:0F:2F:D4:90:19:1A:95:99:25:91:30:EF:C3:39:8D:7F:EF:19:CC:33:2F:C6:B6:E9:92:59:70:01
    //SHA-1   F5:EE:9D:84:91:CD:55:2C:BB:EE:A8:73:9C:55:A1:DE:D4:51:DC:BF
    //PKCS #1 SHA-256 With RSA Encryption
    //Not Critical
    //DNS Name: www.github.com
    //DNS Name: *.github.com
    //DNS Name: github.com
    //DNS Name: *.github.io
    //DNS Name: github.io
    //DNS Name: *.githubusercontent.com
    //DNS Name: githubusercontent.com
     
    // wwwgithubcom.crt
    //     #define CA_PEM                                                      \
    // "-----BEGIN CERTIFICATE----- \r\n"                                      \
    // "MIIHqDCCBpCgAwIBAgIQCDqEWS938ueVG/iHzt7JZjANBgkqhkiG9w0BAQsFADBw\r\n"  \
    // "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n"  \
    // "d3cuZGlnaWNlcnQuY29tMS8wLQYDVQQDEyZEaWdpQ2VydCBTSEEyIEhpZ2ggQXNz\r\n"  \
    // "dXJhbmNlIFNlcnZlciBDQTAeFw0xNzAzMjMwMDAwMDBaFw0yMDA1MTMxMjAwMDBa\r\n"  \
    // "MGoxCzAJBgNVBAYTAlVTMRMwEQYDVQQIEwpDYWxpZm9ybmlhMRYwFAYDVQQHEw1T\r\n"  \
    // "YW4gRnJhbmNpc2NvMRUwEwYDVQQKEwxHaXRIdWIsIEluYy4xFzAVBgNVBAMTDnd3\r\n"  \
    // "dy5naXRodWIuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAxtPx\r\n"  \
    // "ijvPpEXyy3Bn10WfoWmKTW753Uv2PusDNmalx/7mqFqi5BqK4xWQHQgSpyhedgtW\r\n"  \
    // "IXWCJGHtgFVck+DBAbHiHsE67ewpV1a2l2GpqNCFTU77UsoNVD/xPyx3k+cPX9y8\r\n"  \
    // "rqjMiZB3xs1zKDYBkcoBVrA+iO323YkJmCLEXCO2O7b1twLFWkNwMd7e7nteu2uC\r\n"  \
    // "MvxNp5Qg22MIn33t2egMPfIDU/TcKDfyaty5+s6F3gzh7eIgnqNQN0T/5fpaYkqd\r\n"  \
    // "x8j21QDsIyF/CfSpA5qKLuhluu8xrUbnc0MigX7VThS9PbfxMSQ1cQQfbGdxoQNJ\r\n"  \
    // "TNHxXv+ZTXAxKCju5wIDAQABo4IEQjCCBD4wHwYDVR0jBBgwFoAUUWj/kK8CB3U8\r\n"  \
    // "zNllZGKiErhZcjswHQYDVR0OBBYEFDCCKdhtTODUosYQSAWAh6i8qukSMHsGA1Ud\r\n"  \
    // "EQR0MHKCDnd3dy5naXRodWIuY29tggwqLmdpdGh1Yi5jb22CCmdpdGh1Yi5jb22C\r\n"  \
    // "CyouZ2l0aHViLmlvgglnaXRodWIuaW+CFyouZ2l0aHVidXNlcmNvbnRlbnQuY29t\r\n"  \
    // "ghVnaXRodWJ1c2VyY29udGVudC5jb20wDgYDVR0PAQH/BAQDAgWgMB0GA1UdJQQW\r\n"  \
    // "MBQGCCsGAQUFBwMBBggrBgEFBQcDAjB1BgNVHR8EbjBsMDSgMqAwhi5odHRwOi8v\r\n"  \
    // "Y3JsMy5kaWdpY2VydC5jb20vc2hhMi1oYS1zZXJ2ZXItZzUuY3JsMDSgMqAwhi5o\r\n"  \
    // "dHRwOi8vY3JsNC5kaWdpY2VydC5jb20vc2hhMi1oYS1zZXJ2ZXItZzUuY3JsMEwG\r\n"  \
    // "A1UdIARFMEMwNwYJYIZIAYb9bAEBMCowKAYIKwYBBQUHAgEWHGh0dHBzOi8vd3d3\r\n"  \
    // "LmRpZ2ljZXJ0LmNvbS9DUFMwCAYGZ4EMAQICMIGDBggrBgEFBQcBAQR3MHUwJAYI\r\n"  \
    // "KwYBBQUHMAGGGGh0dHA6Ly9vY3NwLmRpZ2ljZXJ0LmNvbTBNBggrBgEFBQcwAoZB\r\n"  \
    // "aHR0cDovL2NhY2VydHMuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0U0hBMkhpZ2hBc3N1\r\n"  \
    // "cmFuY2VTZXJ2ZXJDQS5jcnQwDAYDVR0TAQH/BAIwADCCAfUGCisGAQQB1nkCBAIE\r\n"  \
    // "ggHlBIIB4QHfAHYApLkJkLQYWBSHuxOizGdwCjw1mAT5G9+443fNDsgN3BAAAAFa\r\n"  \
    // "/UBqBAAABAMARzBFAiBFXsWaC1bup8Q0JgrY9EgIxjqi1v2fA6Zg44iRXSQyywIh\r\n"  \
    // "AIzhzU1zlseJh5+yXc5U1I+pgqRmXb1XcPIsGL8oOdwjAHUAVhQGmi/XwuzT9eG9\r\n"  \
    // "RLI+x0Z2ubyZEVzA75SYVdaJ0N0AAAFa/UBqZQAABAMARjBEAiBKQMsySmj69oKZ\r\n"  \
    // "MeC+MDokLrrVN2tK+OMlzf1T5qgHtgIgRJLNGvfWDmMpCK/iWPSmMsYK2yYyTl9K\r\n"  \
    // "btHBtP5WpkcAdgDuS723dc5guuFCaR+r4Z5mow9+X7By2IMAxHuJeqj9ywAAAVr9\r\n"  \
    // "QGofAAAEAwBHMEUCIA2n0TbeAa5KbuOpnXpJbnObwckpOsHsaN+2rA7ZA16YAiEA\r\n"  \
    // "l7JTnVPdmFcauzwLjgNESMRFtn4Brzm9XJTPJbaWPacAdgC72d+8H4pxtZOUI5eq\r\n"  \
    // "kntHOFeVCqtS6BqQlmQ2jh7RhQAAAVr9QGoRAAAEAwBHMEUCIQCqrtuq71J6TM7w\r\n"  \
    // "KMWeSAROdTa8f35GoLMImJXONSNHfQIgONvSu/VH5jlZ1+PD+b6ThFF1+pV7wp7w\r\n"  \
    // "q+/8xiHUMlswDQYJKoZIhvcNAQELBQADggEBAJl+1i/OG6YV9RWz7/EwwR9UEJKk\r\n"  \
    // "jEPAvL2lDQBT4kLBhW/lp6lBmUtGEVrd/egnaZe2PKYOKjDbM1O+g7CqCIkEfmY1\r\n"  \
    // "5VyzLCh/p7HlJ3ltgSaJ6qBVUXAQy+tDWWuqUrRG/dL/iRaKRdoOv4cNU++DJMUX\r\n"  \
    // "rRJjQHSATb2kyd102d8cYQIKcbCTJC8tqSB6Q4ZEEViKRZvXXOJm66bG8Xyn3N2v\r\n"  \
    // "J4k598Gamch/NHrZOXODy3N1vBawTqFJLQkSjU4+Y//wiHHfUEYrpTg92zgIlylk\r\n"  \
    // "3svH64hwWd1i3BZ2LTBq46MvQKU2D8wFdtXgbgRAPWohX79Oo6hs0Jghub0=\r\n"  \
    // "-----END CERTIFICATE----- "
    
    
    
    // DigiCertSHA2HighAssuranceServerCA.crt
    // #define CA_PEM                                                          \
    // "-----BEGIN CERTIFICATE----- \r\n"                                      \
    // "MIIEsTCCA5mgAwIBAgIQBOHnpNxc8vNtwCtCuF0VnzANBgkqhkiG9w0BAQsFADBs\r\n"  \
    // "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n"  \
    // "d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\r\n"  \
    // "ZSBFViBSb290IENBMB4XDTEzMTAyMjEyMDAwMFoXDTI4MTAyMjEyMDAwMFowcDEL\r\n"  \
    // "MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\r\n"  \
    // "LmRpZ2ljZXJ0LmNvbTEvMC0GA1UEAxMmRGlnaUNlcnQgU0hBMiBIaWdoIEFzc3Vy\r\n"  \
    // "YW5jZSBTZXJ2ZXIgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC2\r\n"  \
    // "4C/CJAbIbQRf1+8KZAayfSImZRauQkCbztyfn3YHPsMwVYcZuU+UDlqUH1VWtMIC\r\n"  \
    // "Kq/QmO4LQNfE0DtyyBSe75CxEamu0si4QzrZCwvV1ZX1QK/IHe1NnF9Xt4ZQaJn1\r\n"  \
    // "itrSxwUfqJfJ3KSxgoQtxq2lnMcZgqaFD15EWCo3j/018QsIJzJa9buLnqS9UdAn\r\n"  \
    // "4t07QjOjBSjEuyjMmqwrIw14xnvmXnG3Sj4I+4G3FhahnSMSTeXXkgisdaScus0X\r\n"  \
    // "sh5ENWV/UyU50RwKmmMbGZJ0aAo3wsJSSMs5WqK24V3B3aAguCGikyZvFEohQcft\r\n"  \
    // "bZvySC/zA/WiaJJTL17jAgMBAAGjggFJMIIBRTASBgNVHRMBAf8ECDAGAQH/AgEA\r\n"  \
    // "MA4GA1UdDwEB/wQEAwIBhjAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIw\r\n"  \
    // "NAYIKwYBBQUHAQEEKDAmMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdpY2Vy\r\n"  \
    // "dC5jb20wSwYDVR0fBEQwQjBAoD6gPIY6aHR0cDovL2NybDQuZGlnaWNlcnQuY29t\r\n"  \
    // "L0RpZ2lDZXJ0SGlnaEFzc3VyYW5jZUVWUm9vdENBLmNybDA9BgNVHSAENjA0MDIG\r\n"  \
    // "BFUdIAAwKjAoBggrBgEFBQcCARYcaHR0cHM6Ly93d3cuZGlnaWNlcnQuY29tL0NQ\r\n"  \
    // "UzAdBgNVHQ4EFgQUUWj/kK8CB3U8zNllZGKiErhZcjswHwYDVR0jBBgwFoAUsT7D\r\n"  \
    // "aQP4v0cB1JgmGggC72NkK8MwDQYJKoZIhvcNAQELBQADggEBABiKlYkD5m3fXPwd\r\n"  \
    // "aOpKj4PWUS+Na0QWnqxj9dJubISZi6qBcYRb7TROsLd5kinMLYBq8I4g4Xmk/gNH\r\n"  \
    // "E+r1hspZcX30BJZr01lYPf7TMSVcGDiEo+afgv2MW5gxTs14nhr9hctJqvIni5ly\r\n"  \
    // "/D6q1UEL2tU2ob8cbkdJf17ZSHwD2f2LSaCYJkJA69aSEaRkCldUxPUd1gJea6zu\r\n"  \
    // "xICaEnL6VpPX/78whQYwvwt/Tv9XBZ0k7YXDK/umdaisLRbvfXknsuvCnQsH6qqF\r\n"  \
    // "0wGjIChBWUMo0oHjqvbsezt3tkBigAVBRQHvFwY+3sAzm2fTYS5yh+Rp/BIAV0Ae\r\n"  \
    // "cPUeybQ=\r\n"  \
    // "-----END CERTIFICATE----- "
    
    
    // DigiCertHighAssuranceEVRootCA.crt
        #define CA_PEM                                                      \
    "-----BEGIN CERTIFICATE----- \r\n"                                      \
    "MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs\r\n"  \
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n"  \
    "d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\r\n"  \
    "ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL\r\n"  \
    "MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\r\n"  \
    "LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug\r\n"  \
    "RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm\r\n"  \
    "+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW\r\n"  \
    "PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM\r\n"  \
    "xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB\r\n"  \
    "Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3\r\n"  \
    "hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg\r\n"  \
    "EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF\r\n"  \
    "MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA\r\n"  \
    "FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec\r\n"  \
    "nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z\r\n"  \
    "eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF\r\n"  \
    "hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2\r\n"  \
    "Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe\r\n"  \
    "vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep\r\n"  \
    "+OkuE6N36B9K\r\n"  \
    "-----END CERTIFICATE----- "


    const    char       CaPem[] = CA_PEM;
    unsigned char       response[REPONSE_BUFFER_SIZE];

    retained char       ETag[48] = {0};
    retained deviceConf deviceConfList[MAX_DEVICE_CONF_LIST];



    // #include <httpsclient-particle.h>

    // deviceConf deviceConfList[MAX_DEVICE_CONF_LIST];

    bool getNewConfig();

    bool getConfig() {
        // retained system_tick_t deviceConfLastUpdate = 0;
        // retained int deviceConfListSize   = 0;
        // system_tick_t (uint32_t),

        //https://docs.particle.io/reference/firmware/photon/#time
        bool   mustUpdate = false;
        String reason;
        
        if      (deviceConfListSize   == 0) { 
            mustUpdate = true;
            reason     = "no config list";
            memset(ETag, 0, sizeof(ETag));
            deviceConfListSize   = 0;
            deviceConfLastUpdate = 0;
        }
        else if (deviceConfLastUpdate == 0) {
            mustUpdate = true;
            reason     = "no last update";
            memset(ETag, 0, sizeof(ETag));
            deviceConfListSize   = 0;
            deviceConfLastUpdate = 0;
        }
        else if (Time.now() - deviceConfLastUpdate > update_conf_every_s) {
            mustUpdate = true;
            reason     = "time elapsed";
        }
		
		if ( mustUpdate ) {
		    logger("Updating config: " + reason, CHANNEL_INFO);
		    
			if ( getNewConfig() ) {
			    deviceConfLastUpdate = Time.now();
			    return true;
			
			} else {
                memset(ETag, 0, sizeof(ETag));
                deviceConfListSize   = 0;
                deviceConfLastUpdate = 0;
			    return false;
			}
		}
		
		return true;
    }


    bool getNewConfig() {
        TlsTcpClient client;
    
        // logger("setting PEM");
        
        // setup Root CA pem.
        client.init(CaPem, sizeof(CaPem));
    
        const char* host = "raw.githubusercontent.com";
        const char* url  = "sauloalazure/web-apps-node-iot-hub-data-visualization/master/public/cfg_sensors.json.min.json";
        const char* fmt  = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: Particle\r\nAccept: application/json\r\nAccept-Encoding: identity\r\nCache-Control: no-cache\r\nContent-Length: 0\r\n\r\n";
        const int   port = 443;
        
        //https://raw.githubusercontent.com/sauloalazure/web-apps-node-iot-hub-data-visualization/master/public/cfg_sensors.json
        // connect HTTPS server.
        client.connect((char*)host, port);
    
        if (!client.verify()) {
          logger("Server Certificates is INVALID.", CHANNEL_ERROR);
          return false;
        }

        // logger("Server Certificates is VALID.");
    
    
        // Send request to HTTPS web server.
        const int len = sprintf((char *)response, fmt, url, host);
        
        //If-None-Match: "950a78b5cce194fe1204786219db71270170a51c"
        
        // Particle.connect();
        
        // delay(500);
        
        // logger("Request length:" + String(len));
        
        // logger("Request " + String((char *)response));

        client.write( response, len );

        // logger("Sent Request " + String((char *)response));

        // GET HTTPS response.
        
        const char* endOfHeaders = "\r\n\r\n";
        // bool foundContent = false;
        // bool messageTooLong = false;
        
        memset(response, 0, sizeof(response));
        // int loopCount = 0;

        while(1) {
            // read renponse.
            memset(response, 0, sizeof(response));
            const int ret = client.read(response, sizeof(response) - 1);
            
            if (ret == MBEDTLS_ERR_SSL_WANT_READ) {
                delay(100);
                
                // ++loopCount;
                // if ( loopCount > 1 ) {
                //     messageTooLong = true;
                //     // continue;
                //     break;
                // }
            } else if (ret <= 0) {
                // no more read.
                break;
            } else if (ret > 0){
                // logger((char *)response);
                //TODO: CHECK TOTAL SIZE
                //TODO: USE BUFFER
                /*
                strcat((char *)response, (char *)buf);
                if ( !foundContent ) {
                    char* begin = strstr((char *)response, endOfHeaders);
                    if (begin) {
                        strcpy((char *)response, (char *)begin);
                        foundContent = true;
                    } else {
                        strcpy((char *)response, (char *)buf + sizeof(buf) - 2);
                    }
                }
                */
                // strcpy((char *)response, (char *)response);
                break;
            }
        }
        
        
        // logger("Loops: " + String(loopCount));
        
        // if ( messageTooLong ) {
        //     logger("Message too long: " + String((char *)response));
        //     return false;
        // }
        
        // logger("RESPONSE: " + String(String((char *)response).length()) + " - " + String((char *)response), CHANNEL_ERROR);
        
        char * line;
        String etag = "";
        line = strtok ((char *)response,"\n");
        while (line != NULL)
        {
            // printf ("%s\n",pch);
            
            etag = String(line);
            etag.trim();
            
            if ( etag.startsWith("ETag: \"") ) {
                etag = etag.substring(7, etag.length()-1);
                break;
            }
            
            line = strtok (NULL, "\n");
        }
        
        if (etag.length() == 0) {
            logger("NO ETAG", CHANNEL_ERROR);
            return false;
        }
        
        if (etag.length() < 40) {
            logger("SHORT ETAG: " + etag, CHANNEL_ERROR);
            return false;
        }
        
        String oldEtag = ETag;
        if ( oldEtag.length() != etag.length() ) {
            logger("DIFFERENT ETAG LENGTH: NEW: " + etag + " (" + String(etag.length()) + ") OLD: " + oldEtag + " (" + String(oldEtag.length()) + ")", CHANNEL_LOG);
        } else {
            if ( oldEtag.equals(etag) ) {
                logger("SAME ETAG: " + etag + " vs " + String(ETag), CHANNEL_LOG);
                return true;
            } else {
                logger("DIFFERENT ETAG: NEW: " + etag + " OLD: " + String(ETag), CHANNEL_LOG);
            }
        }


        // logger("GETTING CONTENT", CHANNEL_ERROR);

        line += 7 + etag.length() + 3;

        // logger("LINE: " + String(line), CHANNEL_ERROR);

        char* begin = strstr((char *)line, endOfHeaders);
        
        if (!begin) {
            logger("Invalid response!", CHANNEL_ERROR);
            // logger("Invalid response: " + String(begin), CHANNEL_ERROR);
            return false;
        } else {
            logger("Valid response: " + String(begin), CHANNEL_SENSOR);
        }
        
        // logger("GOT CONTENT", CHANNEL_ERROR);
        
        
        
        // retained int  deviceConfListSize = 0;
        // retained int  deviceConfLastUpdate = 0;
        // retained char ETag[48];

        
        // const int eepromOffset = 0;
        // const int sizeInt      = 8;
        // const int sizeEtag     = etag.length();
        // const int sizeData     = sizeof(deviceConf);
        
        // const int addrSizeEtag = eepromOffset;
        // const int addrSizeData = addrSizeEtag + sizeInt;
        // const int addrEtag     = addrSizeData + sizeInt;
        // const int addrData     = addrEtag     + sizeEtag;
        
        // // EEPROM.put(addrSizeEtag, sizeEtag);
        // // EEPROM.put(addrSizeData, sizeData;
        // // etag.c_str());
        
        
        
        // int headerLength = (int) ((int)begin - (int)response);
        // int jsonLength   = strlen((char*)begin);
        
        // char header[1024];
        // strncpy( (char*)response, header         , headerLength );
        // strncpy( begin          , (char*)response, jsonLength   );
        
        // header[headerLength] = '\0';   // null character manually added
        // response[jsonLength] = '\0';   // null character manually added
        
        // logger("headerLength       : " + String(headerLength));
        // logger("jsonLength         : " + String(jsonLength));
        // logger("Allocate JsonBuffer: " + String((char*)begin));
        
        //   const size_t BUFFER_SIZE = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
        const size_t BUFFER_SIZE = JSON_BUFFER_SIZE;
        DynamicJsonBuffer jsonBuffer(BUFFER_SIZE);
        
        // logger("Parse JSON object");
        // JsonObject& root = jsonBuffer.parseObject(response);
        
        JsonArray& arr = jsonBuffer.parseArray(begin);

        if (!arr.success()) {
            logger("Parsing JSON failed!", CHANNEL_ERROR);
            return false;
        } else {
            logger("Parsing JSON SUCCESS!", CHANNEL_LOG);
        }
        
        deviceConfListSize = 0;
        String deviceID;
        String deviceName;
        String deviceType;
        String sensorName;
        for (JsonObject& elem : arr) {
            // JsonObject& forecast = elem["item"]["forecast"];

            deviceID   = elem["id"  ].as<char*>();
            deviceName = elem["name"].as<char*>();
            deviceType = elem["type"].as<char*>();
            
            // const deviceConf deviceConfList[MAX_DEVICE_CONF_LIST] = {
            // { "53ff6d066667574834441267", "neo"       , SENSOR_HTU21D_NAME },
            
            //{"id":"2e002c000a47343337373738","name":"Morph","type":"iot","desc":"1st Floor"},
            
            if ( deviceType == "iot" ) {
                sensorName = elem["sensor"].as<char*>();
                
                strncpy(deviceConfList[deviceConfListSize].deviceID  , deviceID.c_str()  , deviceID.length());
                strncpy(deviceConfList[deviceConfListSize].deviceNick, deviceName.c_str(), deviceID.length());
                strncpy(deviceConfList[deviceConfListSize].sensorName, sensorName.c_str(), deviceID.length());

                // logger("Added sensor: " + deviceID + " " + deviceName + " " + deviceType + " " + sensorName);
            
                ++deviceConfListSize;
            }
        }
        
        
        
        if ( deviceConfListSize == 0 ) {
            logger("No devices found", CHANNEL_ERROR);
            return false;
        } else {
            logger("devices found: " + String(deviceConfListSize), CHANNEL_LOG);
        }
        
        updateNumbers();
        
        deviceConfLastUpdate = Time.now();
        
        strcpy(ETag, etag.c_str());
        
        logger("New ETAG: " + String(ETag), CHANNEL_LOG);
        
        printVariables();
        
        return true;


        //https://raw.githubusercontent.com/sauloalazure/web-apps-node-iot-hub-data-visualization/master/public/cfg_sensors.json
        //https://github.com/glowfishAPI/httpsclient-particle/blob/master/firmware/httpsclient-particle.cpp
        //https://github.com/hirotakaster/TlsTcpClient/issues/3
        //https://build.particle.io/libs/TlsTcpClient/0.2.13/tab/example/a2-example.ino
        //
        //ETag: "950a78b5cce194fe1204786219db71270170a51c"
        //Content-Length: 322
        //Content-Type: text/plain; charset=utf-8

        // const  bool g_https_trace = true;  // This controls debug info print to Serial
        // const  char host []       = "raw.githubusercontent.com";
        // const  char endpoint []   = "/sauloalazure/web-apps-node-iot-hub-data-visualization/master/public/cfg_sensors.json";
        // const  int g_port         = 443; // DoNOT change this unless you know what's up
        // static unsigned int freemem;
        // bool   g_https_complete;
        // uint32 g_bytes_received;
        
        // TCPClient client;
        
        // //g_path, msg_length, message
        // unsigned char httpRequestContent[] = "GET %s HTTP/1.0\r\n"
        //   "Host: " String(host) "\r\n"
        //   "User-Agent: MatrixSSL/" MATRIXSSL_VERSION "\r\n"
        //   "Accept: applcation/json\r\n"
        //   "Accept-Encoding: identity\r\n"
        //   "Content-Type: applcation/json\r\n"
        //   "Content-Length: %d\r\n\r\n%s";
        //   //If-None-Match: "950a78b5cce194fe1204786219db71270170a51c"
        
        // httpsclientSetup(host, endpoint);
        
        // int g_connected;
        // g_connected = client.connect(host, g_port);
        // if (!g_connected) {
        //     client.stop();
        //     // If TCP Client can't connect to host, exit here.
        //     return;
        // }
        
        // g_https_complete = false;
        // g_bytes_received = 0;
        // if (g_https_trace) {
        //     freemem = System.freeMemory();
        //     logger("free memory: " + String(freemem));
        // }
          
        // int32 rc;
        // if ((rc = httpsClientConnection(httpRequestContent, 0, NULL) < 0)) {
        //     // TODO: When massive FAIL
        //     httpsclientCleanUp();
        //     delay(2000);
        // } else {
        //     digitalWrite(heartbeatLed, HIGH);
        //     delay(250);
        //     digitalWrite(heartbeatLed, LOW);
        // }
        
        // client.stop();




        // //   echo("Extract values");
        // //   echo(root["sensor"].as<char*>());
        // //   echo(root["time"].as<char*>());
        // //   echo(root["data"][0].as<char*>());
        // //   echo(root["data"][1].as<char*>());
        
        // for (JsonObject& elem : root) {
            
        // }

        // logger("Disconnect");
        // client.stop();
    


    //     // StaticJsonBuffer<200> jsonBuffer;
    //     int capacity = 200;
    //     DynamicJsonBuffer jb(capacity);

    //     char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
      
    //     JsonObject& root = jsonBuffer.parseObject(json);
        
    //     if (!root.success()) {
    //         logger("parseObject() failed");
    //         return false;
    //     }
        
    //     for (JsonObject& elem : root) {
    //         String& deviceID   = elem["deviceID"];
    //         String& deviceNick = elem["deviceNick"];
    //         String& sensorName = elem["sensorName"];
            
    //     }
    //     //   const char* sensor = root["sensor"];
    //     //   long time = root["time"];
    //     //   double latitude = root["data"][0];
    //     //   double longitude = root["data"][1];
        
    //     //   Serial.println(sensor);
    //     //   Serial.println(time);
    //     //   Serial.println(latitude, 6);
    //     //   Serial.println(longitude, 6);
    }

#endif //if ndef use_http

const String myDeviceID = System.deviceID();



//
// LOOP VARIABLES
//
unsigned long firstAvailable     = 0;
bool          wifiReady          = false;
bool          cloudReady         = false;
// int           deviceConfListSize = 0;
int           sensorConfListSize = 0;
char          publishString[PUBLISH_STRING_BUFFER_SIZE];


void updateNumbers() {
#ifndef USE_HTTP
    //otherwise set the number while reading
    deviceConfListSize = sizeof(deviceConfList)/sizeof(deviceConfList[0]);
#endif
    sensorConfListSize = sizeof(sensorConfList)/sizeof(sensorConfList[0]);
}



/* This function is called once at start up ----------------------------------*/
void setup()
{
	//Setup the Tinker application here
    #ifdef DEBUG_SERIAL
        Serial.begin(DEBUG_SERIAL_SPEED);
    #endif

	log_serial("SERIAL DEBUG");
    
    #ifdef TINKER
    	logger("registering tinker functions", CHANNEL_INFO);
    	
    	//Register all the Tinker functions
    	Particle.function("digitalread"       , tinkerDigitalRead        );
    	Particle.function("digitalwrite"      , tinkerDigitalWrite       );
    	Particle.function("analogread"        , tinkerAnalogRead         );
    	Particle.function("analogwrite"       , tinkerAnalogWrite        );

    	Particle.function("sendaftermsset"    , tinkerSendAfterMsSet     );
    	Particle.function("sendaftermsget"    , tinkerSendAfterMsGet     );

    	Particle.function("sleepdelaymsset"   , tinkerSleepDelayMsSet    );
    	Particle.function("sleepdelaymsget"   , tinkerSleepDelayMsGet    );

    	Particle.function("trysensorformsset" , tinkerTrySensorForMsSet  );
    	Particle.function("trysensorformsget" , tinkerTrySensorForMsGet  );

    	Particle.function("sleepformsset"     , tinkerSleepForMsSet      );
    	Particle.function("sleepformsget"     , tinkerSleepForMsGet      );

    	Particle.function("verbosityset"      , tinkerVerbositySet       );
    	Particle.function("verbosityget"      , tinkerVerbosityGet       );
    	
        // Particle.function("updateconfeveryset", tinkerUpdateConfEverySet ); 
        // Particle.function("updateconfeveryget", tinkerUpdateConfEveryGet );
    #endif

    logger("setting blue LED low", CHANNEL_INFO);

    pinMode(PIN_LIGHT, OUTPUT);
	digitalWrite(PIN_LIGHT, LOW);

    firstAvailable = 0;
    wifiReady      = false;
    cloudReady     = false;
    updateNumbers();
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
		if (millis() - firstAvailable > send_after_ms) {
			// After we've been up for 30 seconds, go to sleep. The delay is so the serial output gets written out before
			// sleeping.
// 			logger("calling System.sleep(SLEEP_MODE_DEEP, 30)");

            Particle.syncTime();

	        logger("connected. sending signal", CHANNEL_INFO);

            // logger("deviceConfListSize: " + String(deviceConfListSize) + " sensorConfListSize: " + String(sensorConfListSize), CHANNEL_LOG);

            // printVariables();

            if ( getConfig() ) {
    			delay(500);
    
                digitalWrite(PIN_LIGHT, HIGH);
                
                runFuncForDevice();
                
    	        logger("signal sent. sleeping", CHANNEL_INFO);
    
    			digitalWrite(PIN_LIGHT, LOW);
    
    			delay(sleep_delay_ms);
    
    			System.sleep(SLEEP_MODE_DEEP, sleep_for_s);
    
    			// The rest of the code here is not reached. SLEEP_MODE_DEEP causes the code execution to stop,
    			// and when wake up occurs, it's like a reset where you start again with setup(), all variables are
    			// cleared, etc.
    // 			logger("returned from sleep, should not occur");

            } else {
                logger("failed to read config. sleeping", CHANNEL_ERROR);
                
                delay(100);
                
                delay(sleep_delay_ms);
            }
		}
	}
	else {
		firstAvailable = 0;
	}
}


void runFuncForDevice() {
    bool deviceFound = false;
    bool sensorFound = false;
    
    String deviceID_key;
    String deviceNick_key;
    String sensorName_key;
    String status;
    status.reserve(100);
    
    for ( int i = 0; i < deviceConfListSize; ++i ) {
        auto  &deviceConf     = deviceConfList[i];
               deviceID_key   = deviceConf.deviceID;
               deviceNick_key = deviceConf.deviceNick;
               sensorName_key = deviceConf.sensorName;

        if ( deviceID_key == myDeviceID ) {
            deviceFound = true;
            sensorFound = false;
            
            status      = "Machine identified :: Id: " + deviceID_key + " Nick: " + deviceNick_key + " Sensor: " + sensorName_key;
            
            for ( int j = 0; j < sensorConfListSize; ++j ) {
                auto &sensor = sensorConfList[j];
                
                if ( sensorName_key == sensor.sensorName ) {
                    status += " :: SENSOR FOUND";
                    logger("STATUS: " + status, CHANNEL_LOG);
                    
                    sensor.func();
                    sensorFound = true;
                    
                    break;
                }
            }
            
            if ( !sensorFound ) {
                status += " :: SENSOR NOT FOUND";
                logger("STATUS: " + status, CHANNEL_ERROR);
            }

            break;
        }
    }
    
    
    if ( ! deviceFound ) {
        status = "Machine NOT identified :: Id: " + myDeviceID;
        
        logger("STATUS: " + status, CHANNEL_ERROR);
    }
}


void printVariables() {
    logger("config :: "          + String( ""                   ) + \
        " send_after_ms "        + String( send_after_ms        ) + \
        " sleep_delay_ms "       + String( sleep_delay_ms       ) + \
        " try_sensor_for_ms "    + String( try_sensor_for_ms    ) + \
        " verbosity "            + String( verbosity            ) + \
        " sleep_for_s "          + String( sleep_for_s          ) + \
        " deviceConfListSize "   + String( deviceConfListSize   ) + \
        " deviceConfLastUpdate " + String( deviceConfLastUpdate ) + \
        " update_conf_every_s "  + String( update_conf_every_s  ) + \
        " deviceConfListSize "   + String( deviceConfListSize   ) + \
        " sensorConfListSize "   + String( sensorConfListSize   ) + \
        " ETag "                 + String( ETag                 ) \
        , CHANNEL_SENSOR);

	delay(100);

    String deviceID_key;
    String deviceNick_key;
    String sensorName_key;
    String status;
    status.reserve(100);
    
    for ( int i = 0; i < deviceConfListSize; ++i ) {
        auto  &deviceConf     = deviceConfList[i];
               deviceID_key   = deviceConf.deviceID;
               deviceNick_key = deviceConf.deviceNick;
               sensorName_key = deviceConf.sensorName;
        
        logger("config :: "          + String( ""             ) + \
        " deviceID_key "             + String( deviceID_key   ) + \
        " deviceNick_key "           + String( deviceNick_key ) + \
        " sensorName_key "           + String( sensorName_key ) \
        , CHANNEL_SENSOR);
        
    	delay(100);
    }
}

void resetI2C() {
    // https://github.com/particle-iot/firmware/issues/598

    pinMode(SDA, INPUT);
    pinMode(SCL, INPUT);
    
    digitalWrite(SDA, LOW);
    digitalWrite(SCL, LOW);

    Wire.reset();
    
}

//
// FUNCTIONS TO READ SENSORS
//
#ifdef SENSOR_HTU21D
    void checkSensorHTU21D() {
        const String sensorName = SENSOR_HTU21D_NAME;
        
        logger(sensorName + ": setting up", CHANNEL_SENSOR);

        resetI2C();
        
        #ifdef SENSOR_HTU21D_ADAFRUIT
            Adafruit_HTU21DF htu = Adafruit_HTU21DF();
        #endif
        
        #ifdef SENSOR_HTU21D_GENERIC
            HTU21DF          htu = HTU21DF();
        #endif

        const unsigned long sensorStartTime = millis();
        bool                sensorSuccess   = true;

    	while(! htu.begin()) {
    	    logger(sensorName + ": NOT found. waiting " + String(SENSOR_SI7021_WAIT_BEGIN) + "s", CHANNEL_LOG);

    	    if (millis() - sensorStartTime > try_sensor_for_ms) {
    	        sensorSuccess   = false;
    	        break;   
    	    }
        
    	    delay(SENSOR_HTU21D_WAIT_BEGIN);
    	}

        if ( sensorSuccess ) {
            logger(sensorName + ": found. continuing", CHANNEL_INFO);
            
    	    delay(SENSOR_HTU21D_WAIT_BEGIN);

            sprintf(publishString,"{\"t\":%0.1f,\"h\":%0.1f,\"p\":null,\"s\":\"%s\",\"v\":\"%s\"}",htu.readTemperature(),htu.readHumidity(),sensorName.c_str(),VERSION);
        
    	    logger(sensorName + ": sending :: " + String(publishString), CHANNEL_INFO);

            logger(publishString, CHANNEL_DATA);
        } else {
            logger(sensorName + ": NOT found. QUITTING", CHANNEL_ERROR);
        }
    }
#endif



#ifdef SENSOR_SI7021_ADAFRUIT
    void checkSensorSI7021() {
        const String sensorName = SENSOR_SI7021_ADAFRUIT_NAME;
        
        logger(sensorName + ": setting up", CHANNEL_SENSOR);

        resetI2C();

        Adafruit_Si7021 si7021 = Adafruit_Si7021();

        const unsigned long sensorStartTime = millis();
        bool                sensorSuccess   = true;

    	while(! si7021.begin()){
    	    logger(sensorName + ": NOT found. waiting " + String(SENSOR_SI7021_WAIT_BEGIN) + "s", CHANNEL_LOG);
    	    
    	    if (millis() - sensorStartTime > try_sensor_for_ms) {
    	        sensorSuccess   = false;
    	        break;   
    	    }
        
    	    delay(SENSOR_SI7021_WAIT_BEGIN);
    	}

        if ( sensorSuccess ) {
        	logger(sensorName + ": found. continuing", CHANNEL_INFO);
    
    	    delay(SENSOR_SI7021_WAIT_BEGIN);

            sprintf(publishString,"{\"t\":%0.1f,\"h\":%0.1f,\"p\":null,\"s\":\"%s\",\"v\":\"%s\"}",si7021.readTemperature(),si7021.readHumidity(),sensorName.c_str(),VERSION);

            logger(sensorName + ": sending :: " + String(publishString), CHANNEL_INFO);
            
            logger(publishString, CHANNEL_DATA);
        } else {
            logger(sensorName + ": NOT found. QUITTING", CHANNEL_ERROR);
        }
    }
#endif



#ifdef SENSOR_BMP085_ADAFRUIT
    void checkSensorBMP085() {
        const String sensorName = SENSOR_BMP085_ADAFRUIT_NAME;
        
        logger(sensorName + ": setting up", CHANNEL_SENSOR);

        resetI2C();
        
        Adafruit_BMP085 bmp085 = Adafruit_BMP085();

        const unsigned long sensorStartTime = millis();
        bool                sensorSuccess   = true;

    	while(! bmp085.begin()){
            logger(sensorName + ": NOT found. waiting " + String(SENSOR_BMP085_WAIT_BEGIN) + "s", CHANNEL_LOG);

    	    if (millis() - sensorStartTime > try_sensor_for_ms) {
    	        sensorSuccess   = false;
    	        break;   
    	    }
        
    	    delay(SENSOR_BMP085_WAIT_BEGIN);
    	}


        if ( sensorSuccess ) {
        	logger(sensorName + ": found. continuing", CHANNEL_INFO);
    
    	    delay(SENSOR_BMP085_WAIT_BEGIN);

            sprintf(publishString,"{\"t\":%0.1f,\"h\":null,\"p\":%ld,\"s\":\"%s\",\"v\":\"%s\"}",bmp085.readTemperature(),bmp085.readPressure(),sensorName.c_str(),VERSION);
    
            logger(sensorName + ": sending :: " + String(publishString), CHANNEL_INFO);
            
            logger(publishString, CHANNEL_DATA);
        } else {
            logger(sensorName + ": NOT found. QUITTING", CHANNEL_ERROR);
        }
    }
#endif



#ifdef SENSOR_BMP183_ADAFRUIT
    void checkSensorBMP183() {
        const String sensorName = SENSOR_BMP183_ADAFRUIT_NAME;
        
        logger(sensorName + ": setting up", CHANNEL_SENSOR);

        resetI2C();

        Adafruit_BMP183_Unified bmp183 = Adafruit_BMP183_Unified(BMP183_CS);  // use hardware SPI

        const unsigned long sensorStartTime = millis();
        bool                sensorSuccess   = true;

    	while(! bmp183.begin()){
    	    logger(sensorName + ": NOT found. waiting " + String(SENSOR_BMP183_WAIT_BEGIN) + "s", CHANNEL_LOG);
        
    	    delay(SENSOR_BMP183_WAIT_BEGIN);

    	    if (millis() - sensorStartTime > try_sensor_for_ms) {
    	        sensorSuccess   = false;
    	        break;   
    	    }
    	}


        if ( sensorSuccess ) {
            logger(sensorName + ": found. continuing", CHANNEL_INFO);

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
                const int32_t pressure = (int32_t)(bmp183.getPressure());
                sprintf(publishString,"{\"t\":%0.1f,\"h\":null,\"p\":%ld,\"s\":\"%s\",\"v\":\"%s\"}",bmp183.getTemperature(),pressure,sensorName.c_str(),VERSION);
    
                logger(sensorName + ": sending :: " + String(publishString), CHANNEL_INFO);
                
                logger(publishString, CHANNEL_DATA);
            } else {
                logger(sensorName + ": sensor error. QUITTING", CHANNEL_ERROR);
            }
        } else {
            logger(sensorName + ": NOT found. QUITTING", CHANNEL_ERROR);
        }
    }
#endif


#ifdef SENSOR_BME280_ADAFRUIT
    void checkSensorBME280() {
        // https://github.com/adafruit/Adafruit_BME280_Library

        const String sensorName = SENSOR_BME280_ADAFRUIT_NAME;
        
        logger(sensorName + ": setting up", CHANNEL_SENSOR);

        resetI2C();

        Adafruit_BME280 bme280 = Adafruit_BME280(); // I2C

        const unsigned long sensorStartTime = millis();
        bool                sensorSuccess   = true;

    	while(! bme280.begin(BME280_ADDRESS)){
    	    logger(sensorName + ": NOT found. waiting " + String(SENSOR_BME280_WAIT_BEGIN) + "s", CHANNEL_LOG);
        
    	    delay(SENSOR_BME280_WAIT_BEGIN);

    	    if (millis() - sensorStartTime > try_sensor_for_ms) {
    	        sensorSuccess   = false;
    	        break;   
    	    }
    	}


        if ( sensorSuccess ) {
        	logger(sensorName + ": found. continuing", CHANNEL_INFO);

    	    delay(SENSOR_BME280_WAIT_BEGIN);

            const float temperature = bme280.readTemperature();
            delay(10);
            const float pressure    = bme280.readPressure();
            delay(10);
            const float humidity    = bme280.readHumidity();
            // float altitude    = bme280.readAltitude(SEALEVELPRESSURE_HPA);

            log_serial("Temperature = " + String(temperature) + " *C");
            log_serial("Humidity    = " + String(humidity) + " %");
            log_serial("Pressure    = " + String((pressure / 100.0F)) + " hPa");
            // log_serial("Approx. Altitude = " + String(altitude) + " m");

            #ifdef DEBUG_SERIAL
                delay(500);
            #endif

            if (pressure)
            {
                // int32_t pressureI = (int32_t)(pressure);
                sprintf(publishString,"{\"t\":%0.1f,\"h\":%0.1f,\"p\":%0.0f,\"s\":\"%s\",\"v\":\"%s\"}",temperature,humidity,pressure,sensorName.c_str(),VERSION);
    
                logger(sensorName + ": sending :: " + String(publishString), CHANNEL_INFO);
                
                logger(publishString, CHANNEL_DATA);
            } else {
                logger(sensorName + ": sensor error. QUITTING", CHANNEL_ERROR);
            }
        } else {
            logger(sensorName + ": NOT found. QUITTING", CHANNEL_ERROR);
        }
    }
#endif



#ifdef SENSOR_TMP36
    void checkSensorTMP36() {
        const String sensorName = SENSOR_TMP36_NAME;
        
        logger(sensorName + ": setting up", CHANNEL_SENSOR);

        pinMode(SENSOR_TMP36_PIN, INPUT);
        analogRead(SENSOR_TMP36_PIN);
    
        const double temp = TMP36GetCentigrade(SENSOR_TMP36_PIN);
    
        sprintf(publishString,"{\"t\":%0.1f,\"h\":null,\"p\":null,\"s\":\"%s\",\"v\":\"%s\"}",temp,sensorName.c_str(),VERSION);
    
        logger(sensorName + ": sending :: " + String(publishString), CHANNEL_INFO);
    
        logger(publishString, CHANNEL_DATA);
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

    //https://www.systutorials.com/131/convert-string-to-int-and-reverse/
    //https://www.arduino.cc/reference/en/language/variables/data-types/string/functions/toint/
    int tinkerSendAfterMsSet    (String ms)
    {
        // int number = std::atoi (ms.c_str());
        
        // if (errno == ERANGE) {
        //     return -1;
        // } else if ( number == 0 ) {
        //     return -2;
        // }
        
        int number = ms.toInt();
        
        if ( number == 0 ) {
            return -1;
        }
        
        send_after_ms = number;
        
        logger("tinker: set send after MS " + ms, CHANNEL_TINKER);
        
        return 0;
    }
    
	int tinkerSendAfterMsGet    (String command)
	{
	    return send_after_ms;
	}

	int tinkerSleepDelayMsSet   (String ms)
	{
	    int number = ms.toInt();
        
        if ( number == 0 ) {
            return -1;
        }
        
        sleep_delay_ms = number;
        
        logger("tinker: set sleep delay MS " + ms, CHANNEL_TINKER);
        
	    return 0;
	}
	
	int tinkerSleepDelayMsGet   (String command)
	{
	    return sleep_delay_ms;
	}

	int tinkerTrySensorForMsSet (String ms)
	{
	    int number = ms.toInt();
        
        if ( number == 0 ) {
            return -1;
        }
        
        try_sensor_for_ms = number;
        
        logger("tinker: set try sendor for MS " + ms, CHANNEL_TINKER);
        
	    return 0;
	}
	
	int tinkerTrySensorForMsGet (String command)
	{
	    return try_sensor_for_ms;
	}

	int tinkerSleepForMsSet     (String ms)
	{
	    int number = ms.toInt();
        
        if ( number == 0 ) {
            return -1;
        }
        
        sleep_for_s = number;
        
        logger("tinker: set sleep for MS " + ms, CHANNEL_TINKER);
        
	    return 0;
	}
	
	int tinkerSleepForMsGet     (String command)
	{
	    return sleep_for_s;
	}

	int tinkerVerbositySet      (String level)
	{
        bool   hasLevel = false;
        
        if      ( level == CHANNEL_DATA_NAME   ) { verbosity = CHANNEL_DATA  ; hasLevel = true; }
        else if ( level == CHANNEL_SENSOR_NAME ) { verbosity = CHANNEL_SENSOR; hasLevel = true; }
        else if ( level == CHANNEL_TINKER_NAME ) { verbosity = CHANNEL_TINKER; hasLevel = true; }
        else if ( level == CHANNEL_ERROR_NAME  ) { verbosity = CHANNEL_ERROR ; hasLevel = true; }
        else if ( level == CHANNEL_WARN_NAME   ) { verbosity = CHANNEL_WARN  ; hasLevel = true; }
        else if ( level == CHANNEL_LOG_NAME    ) { verbosity = CHANNEL_LOG   ; hasLevel = true; }
        else if ( level == CHANNEL_INFO_NAME   ) { verbosity = CHANNEL_INFO  ; hasLevel = true; }

        if ( hasLevel ) {
            logger("tinker: set verbosity to level " + level + " (" + String(verbosity) + ")", CHANNEL_TINKER);
        } else {
            logger("tinker: FAILED TO set verbosity to level " + level + " (" + String(verbosity) + ")", CHANNEL_TINKER);
        }
	}
	
	int tinkerVerbosityGet      (String command)
	{
	    return verbosity;
	}


// 	int tinkerUpdateConfEverySet (String seconds) {
// 	    int number = seconds.toInt();
        
//         if ( number == 0 ) {
//             return -1;
//         }
        
//         // update_conf_every_s = number;
        
//         logger("tinker: set update conf every S " + seconds, CHANNEL_TINKER);
        
// 	    return 0;
// 	}
	
// 	int tinkerUpdateConfEveryGet (String command)
// 	{
//         return update_conf_every_s;
// 	}

#endif

