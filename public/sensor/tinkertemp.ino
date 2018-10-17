#include <cstring>

//
// DEFINE VERSION AND DEBUG
//
#define VERSION "2018_10_18_0000"
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
#define SEND_AFTER_MS     10000 // run after N milli seconds connected to the cloud
#define SLEEP_DELAY_MS    30000 // after sending the message, wait N milli seconds before going to sleep
#define TRY_SENSOR_FOR_MS 10000 // try to connect to sensor for N milli seconds before giving up
#define SLEEP_FOR_S          60 // sleep for N SECONDS

#define CHANNEL_LOG       "LOG"
#define CHANNEL_SENSOR    "SENSOR"
#define CHANNEL_DATA      "HomeTemp"
#define PIN_LIGHT         D7

#define MAX_DEVICE_CONF_LIST   20
#define JSON_BUFFER_SIZE     4096

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
    #define SENSOR_HTU21D_NAME       "HTU21D"
#endif

#ifdef SENSOR_SI7021_ADAFRUIT
    #include <Adafruit_Si7021.h>
    #define SENSOR_SI7021_WAIT_BEGIN    5000
    #define SENSOR_SI7021_ADAFRUIT_NAME "SI7021_adafruit"
#endif

#ifdef SENSOR_BMP085_ADAFRUIT
    #include <Adafruit_BMP085.h>
    #define SENSOR_BMP085_WAIT_BEGIN    5000
    #define SENSOR_BMP085_ADAFRUIT_NAME "BMP085_adafruit"
#endif

#ifdef SENSOR_BMP183_ADAFRUIT
    #include <SPI.h>
    #include <Adafruit_Sensor.h>
    #include <Adafruit_BMP183_U.h>
    // You'll also need a chip-select pin, use any pin!
    #define BMP183_CS                   A2
    #define SENSOR_BMP183_WAIT_BEGIN    5000
    #define SENSOR_BMP183_ADAFRUIT_NAME "BMP183_adafruit"
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
#endif

#ifdef SENSOR_TMP36
    #define AREF_VOLTAGE               3320
    #define SENSOR_TMP36_PIN             A7
    #define SENSOR_TMP36_SAMPLES          8 // samples to take
    #define SENSOR_TMP36_WAIT             2 // interval between samples
    #define SENSOR_TMP36_MAX_VOLTAGE   1550 // max voltage output at 125C for 3.3V
    #define SENSOR_TMP36_SLOPE       0.1128 // slope for pairs (mV,Tc) = (0,-50), (595.5311,18), (1550,125). 
                                            // second value is emprirical. third value leads to R-squared of zero
    #define SENSOR_TMP36_NAME         "TMP36"
#endif

#define SENSOR_EMPTY_NAME "EMPTY"





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



void log_serial(const String& msg) {
    #ifdef DEBUG_SERIAL
        Serial.println(msg);
    #endif
}

void log_web(const String& msg, const String& channel=CHANNEL_LOG) {
    #ifdef DEBUG_WEB
        Particle.publish(channel, msg, PRIVATE);
        delay(500);
    #endif
}

void logger(const String& msg, const String& channel=CHANNEL_LOG) {
    log_serial(msg);
    log_web(msg, channel);
}



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
    String         deviceID;
    String         deviceNick;
    String         sensorName;
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



#ifndef USE_HTTP
    const deviceConf deviceConfList[MAX_DEVICE_CONF_LIST] = {
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
#define CA_PEM                                                          \
"-----BEGIN CERTIFICATE----- \r\n"                                      \
"MIIEsTCCA5mgAwIBAgIQBOHnpNxc8vNtwCtCuF0VnzANBgkqhkiG9w0BAQsFADBs\r\n"  \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n"  \
"d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\r\n"  \
"ZSBFViBSb290IENBMB4XDTEzMTAyMjEyMDAwMFoXDTI4MTAyMjEyMDAwMFowcDEL\r\n"  \
"MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\r\n"  \
"LmRpZ2ljZXJ0LmNvbTEvMC0GA1UEAxMmRGlnaUNlcnQgU0hBMiBIaWdoIEFzc3Vy\r\n"  \
"YW5jZSBTZXJ2ZXIgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC2\r\n"  \
"4C/CJAbIbQRf1+8KZAayfSImZRauQkCbztyfn3YHPsMwVYcZuU+UDlqUH1VWtMIC\r\n"  \
"Kq/QmO4LQNfE0DtyyBSe75CxEamu0si4QzrZCwvV1ZX1QK/IHe1NnF9Xt4ZQaJn1\r\n"  \
"itrSxwUfqJfJ3KSxgoQtxq2lnMcZgqaFD15EWCo3j/018QsIJzJa9buLnqS9UdAn\r\n"  \
"4t07QjOjBSjEuyjMmqwrIw14xnvmXnG3Sj4I+4G3FhahnSMSTeXXkgisdaScus0X\r\n"  \
"sh5ENWV/UyU50RwKmmMbGZJ0aAo3wsJSSMs5WqK24V3B3aAguCGikyZvFEohQcft\r\n"  \
"bZvySC/zA/WiaJJTL17jAgMBAAGjggFJMIIBRTASBgNVHRMBAf8ECDAGAQH/AgEA\r\n"  \
"MA4GA1UdDwEB/wQEAwIBhjAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIw\r\n"  \
"NAYIKwYBBQUHAQEEKDAmMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdpY2Vy\r\n"  \
"dC5jb20wSwYDVR0fBEQwQjBAoD6gPIY6aHR0cDovL2NybDQuZGlnaWNlcnQuY29t\r\n"  \
"L0RpZ2lDZXJ0SGlnaEFzc3VyYW5jZUVWUm9vdENBLmNybDA9BgNVHSAENjA0MDIG\r\n"  \
"BFUdIAAwKjAoBggrBgEFBQcCARYcaHR0cHM6Ly93d3cuZGlnaWNlcnQuY29tL0NQ\r\n"  \
"UzAdBgNVHQ4EFgQUUWj/kK8CB3U8zNllZGKiErhZcjswHwYDVR0jBBgwFoAUsT7D\r\n"  \
"aQP4v0cB1JgmGggC72NkK8MwDQYJKoZIhvcNAQELBQADggEBABiKlYkD5m3fXPwd\r\n"  \
"aOpKj4PWUS+Na0QWnqxj9dJubISZi6qBcYRb7TROsLd5kinMLYBq8I4g4Xmk/gNH\r\n"  \
"E+r1hspZcX30BJZr01lYPf7TMSVcGDiEo+afgv2MW5gxTs14nhr9hctJqvIni5ly\r\n"  \
"/D6q1UEL2tU2ob8cbkdJf17ZSHwD2f2LSaCYJkJA69aSEaRkCldUxPUd1gJea6zu\r\n"  \
"xICaEnL6VpPX/78whQYwvwt/Tv9XBZ0k7YXDK/umdaisLRbvfXknsuvCnQsH6qqF\r\n"  \
"0wGjIChBWUMo0oHjqvbsezt3tkBigAVBRQHvFwY+3sAzm2fTYS5yh+Rp/BIAV0Ae\r\n"  \
"cPUeybQ=\r\n"  \
"-----END CERTIFICATE----- "


// DigiCertHighAssuranceEVRootCA.crt
//     #define CA_PEM                                                      \
// "-----BEGIN CERTIFICATE----- \r\n"                                      \
// "MIIDxTCCAq2gAwIBAgIQAqxcJmoLQJuPC3nyrkYldzANBgkqhkiG9w0BAQUFADBs\r\n"  \
// "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n"  \
// "d3cuZGlnaWNlcnQuY29tMSswKQYDVQQDEyJEaWdpQ2VydCBIaWdoIEFzc3VyYW5j\r\n"  \
// "ZSBFViBSb290IENBMB4XDTA2MTExMDAwMDAwMFoXDTMxMTExMDAwMDAwMFowbDEL\r\n"  \
// "MAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQd3d3\r\n"  \
// "LmRpZ2ljZXJ0LmNvbTErMCkGA1UEAxMiRGlnaUNlcnQgSGlnaCBBc3N1cmFuY2Ug\r\n"  \
// "RVYgUm9vdCBDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMbM5XPm\r\n"  \
// "+9S75S0tMqbf5YE/yc0lSbZxKsPVlDRnogocsF9ppkCxxLeyj9CYpKlBWTrT3JTW\r\n"  \
// "PNt0OKRKzE0lgvdKpVMSOO7zSW1xkX5jtqumX8OkhPhPYlG++MXs2ziS4wblCJEM\r\n"  \
// "xChBVfvLWokVfnHoNb9Ncgk9vjo4UFt3MRuNs8ckRZqnrG0AFFoEt7oT61EKmEFB\r\n"  \
// "Ik5lYYeBQVCmeVyJ3hlKV9Uu5l0cUyx+mM0aBhakaHPQNAQTXKFx01p8VdteZOE3\r\n"  \
// "hzBWBOURtCmAEvF5OYiiAhF8J2a3iLd48soKqDirCmTCv2ZdlYTBoSUeh10aUAsg\r\n"  \
// "EsxBu24LUTi4S8sCAwEAAaNjMGEwDgYDVR0PAQH/BAQDAgGGMA8GA1UdEwEB/wQF\r\n"  \
// "MAMBAf8wHQYDVR0OBBYEFLE+w2kD+L9HAdSYJhoIAu9jZCvDMB8GA1UdIwQYMBaA\r\n"  \
// "FLE+w2kD+L9HAdSYJhoIAu9jZCvDMA0GCSqGSIb3DQEBBQUAA4IBAQAcGgaX3Nec\r\n"  \
// "nzyIZgYIVyHbIUf4KmeqvxgydkAQV8GK83rZEWWONfqe/EW1ntlMMUu4kehDLI6z\r\n"  \
// "eM7b41N5cdblIZQB2lWHmiRk9opmzN6cN82oNLFpmyPInngiK3BD41VHMWEZ71jF\r\n"  \
// "hS9OMPagMRYjyOfiZRYzy78aG6A9+MpeizGLYAiJLQwGXFK3xPkKmNEVX58Svnw2\r\n"  \
// "Yzi9RKR/5CYrCsSXaQ3pjOLAEFe4yHYSkVXySGnYvCoCWw9E1CAx2/S6cCZdkGCe\r\n"  \
// "vEsXCS+0yx5DaMkHJ8HSXPfqIbloEpw8nL+e/IBcm2PN7EeqJSdnoDfzAIJ9VNep\r\n"  \
// "+OkuE6N36B9K\r\n"  \
// "-----END CERTIFICATE----- "

    const char CaPem[] = CA_PEM;


    // #include <httpsclient-particle.h>

    // deviceConf deviceConfList[MAX_DEVICE_CONF_LIST];

    bool getConfig() {
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
        //     log_serial("free memory: " + String(freemem));
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


        unsigned char response[2048];
    
        TlsTcpClient client;
    
        // logger("setting PEM");
        
        // setup Root CA pem.
        client.init(CaPem, sizeof(CaPem));
    
        const char* host = "raw.githubusercontent.com";
        const char* url  = "sauloalazure/web-apps-node-iot-hub-data-visualization/master/public/cfg_sensors.json";
        int         port = 443;
        
        //https://raw.githubusercontent.com/sauloalazure/web-apps-node-iot-hub-data-visualization/master/public/cfg_sensors.json
        // connect HTTPS server.
        client.connect((char*)host, port);
    
        if (!client.verify()) {
          logger("Server Certificates is INVALID.");
          return false;
        }

        // logger("Server Certificates is VALID.");
    
    
        // Send request to HTTPS web server.
        int len = sprintf((char *)response, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: Particle\r\nAccept: application/json\r\nAccept-Encoding: identity\r\nContent-Length: 0\r\n\r\n", url, host);
        
            //If-None-Match: "950a78b5cce194fe1204786219db71270170a51c"
        
        // Particle.connect();
        
        // delay(500);
        
        // logger("Request length:" + String(len));
        
        // logger("Request " + String((char *)response));

        client.write( response, len );

        // logger("Sent Request " + String((char *)response));

        // GET HTTPS response.
        
        const char* endOfHeaders = "\r\n\r\n";
        bool foundContent = false;
        
        memset(response, 0, sizeof(response));
        unsigned char buf[256];
        memset(buf, 0, sizeof(buf));
        
        while(1) {
            // read renponse.
            memset(buf, 0, sizeof(buf));
            int ret = client.read(buf, sizeof(buf) - 1);
            if (ret == MBEDTLS_ERR_SSL_WANT_READ) {
                delay(100);
                // continue;
            } else if (ret <= 0) {
                // no more read.
                break;
            } else if (ret > 0){
                // logger((char *)response);
                //TODO: CHECK TOTAL SIZE
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
            }
        }
        
        // logger("Response " + String((char *)response));
        
        return false;
        
        
        char* begin = strstr((char *)response, endOfHeaders);
        
        if (!begin) {
            logger("Invalid response");
            return false;
        }
        
        logger("Valid response: " + String((char *)response));
        
        /*
        
        int headerLength = (int) ((int)begin - (int)response);
        int jsonLength   = strlen((char*)response) - headerLength;
        
        char header[1024];
        strncpy( (char*)response, header         , headerLength );
        strncpy( begin          , (char*)response, jsonLength   );
        
        header[headerLength] = '\0';   // null character manually added
        response[jsonLength] = '\0';   // null character manually added
        
        logger("Allocate JsonBuffer");
        //   const size_t BUFFER_SIZE = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
        const size_t BUFFER_SIZE = JSON_BUFFER_SIZE;
        DynamicJsonBuffer jsonBuffer(BUFFER_SIZE);
        
        logger("Parse JSON object");
        // JsonObject& root = jsonBuffer.parseObject(response);
        
        JsonArray& arr = jsonBuffer.parseArray(response);

        if (!arr.success()) {
            logger("Parsing failed!");
            return false;
        }
        
        int elCount = 0;
        for (JsonObject& elem : arr) {
            // JsonObject& forecast = elem["item"]["forecast"];

            String deviceID   = elem["deviceID"];
            String deviceNick = elem["deviceNick"];
            String sensorName = elem["sensorName"];
            
            deviceConfList[elCount] = { deviceID, deviceNick, sensorName };
            
            ++elCount;
        }
        
        */

        // //   echo("Extract values");
        // //   echo(root["sensor"].as<char*>());
        // //   echo(root["time"].as<char*>());
        // //   echo(root["data"][0].as<char*>());
        // //   echo(root["data"][1].as<char*>());
        
        // for (JsonObject& elem : root) {
            
        // }

        // log_serial("Disconnect");
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
unsigned long firstAvailable = 0;
bool          wifiReady      = false;
bool          cloudReady     = false;
// const int numDevices = 5;
int           numDevices     = 0;
int           numSensors     = 0;
char          publishString[254];




void runFuncForDevice() {
    bool deviceFound = false;
    
    for ( int i = 0; i < numDevices; ++i ) {
        auto  &deviceConf     = deviceConfList[i];
        String deviceID_key   = deviceConf.deviceID;
        String deviceNick_key = deviceConf.deviceNick;
        String sensorName_key = deviceConf.sensorName;

        if ( deviceID_key == myDeviceID ) {
            deviceFound      = true;
            bool sensorFound = false;
            
            String status = "Machine identified :: Id: " + deviceID_key + " Nick: " + deviceNick_key + " Sensor: " + sensorName_key;

            for ( int j = 0; j < numSensors; ++j ) {
                auto &sensor = sensorConfList[j];
                if ( sensorName_key == sensor.sensorName ) {
                    status += " :: SENSOR FOUND";
                    logger(status);
                    
                    sensor.func();
                    sensorFound = true;
                    break;
                }
            }
            
            if ( !sensorFound ) {
                status += " :: SENSOR NOT FOUND";
                logger(status);
            }

            break;
        }
    }
    
    
    if ( ! deviceFound ) {
        String status = "Machine NOT identified :: Id: " + myDeviceID;
        
        logger(status);
    }
}





/* This function is called once at start up ----------------------------------*/
void setup()
{
	//Setup the Tinker application here
    #ifdef DEBUG_SERIAL
        Serial.begin(DEBUG_SERIAL_SPEED);
    #endif

	log_serial("SERIAL DEBUG");

	log_serial("registering functions");
    
    #ifdef TINKER
    	//Register all the Tinker functions
    	Particle.function("digitalread" , tinkerDigitalRead );
    	Particle.function("digitalwrite", tinkerDigitalWrite);
    	Particle.function("analogread"  , tinkerAnalogRead  );
    	Particle.function("analogwrite" , tinkerAnalogWrite );
    #endif

    log_serial("setting blue led low");

    pinMode(PIN_LIGHT, OUTPUT);
	digitalWrite(PIN_LIGHT, LOW);

    firstAvailable = 0;
    wifiReady      = false;
    cloudReady     = false;
    numDevices     = sizeof(deviceConfList)/sizeof(deviceConfList[0]);
    numSensors     = sizeof(sensorConfList)/sizeof(sensorConfList[0]);
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

            Particle.syncTime();

	        log_serial("connected. sending signal");

            digitalWrite(PIN_LIGHT, HIGH);
            
            runFuncForDevice();
            
	        log_serial("signal sent. sleeping");

			digitalWrite(PIN_LIGHT, LOW);
			
			delay(500);

            // getConfig();

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



void resetI2C() {
    // https://github.com/particle-iot/firmware/issues/598
    Wire.reset();
    // pinMode(SDA, INPUT);
    // pinMode(SCL, INPUT);
    // digitalWrite(SDA, LOW);
    // digitalWrite(SCL, LOW);
}

//
// FUNCTIONS TO READ SENSORS
//
#ifdef SENSOR_HTU21D
    void checkSensorHTU21D() {
        String sensorName = "HTU21D";
        
        logger(sensorName + ": setting up", CHANNEL_SENSOR);

        resetI2C();
        
        #ifdef SENSOR_HTU21D_ADAFRUIT
            Adafruit_HTU21DF htu = Adafruit_HTU21DF();
        #endif
        
        #ifdef SENSOR_HTU21D_GENERIC
            HTU21DF          htu = HTU21DF();
        #endif

        unsigned long sensorStartTime = millis();
        bool          sensorSuccess   = true;

    	while(! htu.begin()) {
    	    logger(sensorName + ": NOT found. waiting " + String(SENSOR_SI7021_WAIT_BEGIN) + "s");

    	    if (millis() - sensorStartTime > TRY_SENSOR_FOR_MS) {
    	        sensorSuccess   = false;
    	        break;   
    	    }
        
    	    delay(SENSOR_HTU21D_WAIT_BEGIN);
    	}

        if ( sensorSuccess ) {
            logger(sensorName + ": found. continuing");
            
    	    delay(SENSOR_HTU21D_WAIT_BEGIN);

            sprintf(publishString,"{\"t\":%0.1f,\"h\":%0.1f,\"p\":null,\"s\":\"HTU21D\",\"v\":\"%s\"}",htu.readTemperature(),htu.readHumidity(),VERSION);
        
    	    logger(sensorName + ": sending :: " + String(publishString));

            Particle.publish(CHANNEL_DATA, publishString, PRIVATE);
        } else {
            logger(sensorName + ": NOT found. QUITTING");
        }
    }
#endif



#ifdef SENSOR_SI7021_ADAFRUIT
    void checkSensorSI7021() {
        String sensorName = "SI7021";
        
        logger(sensorName + ": setting up", CHANNEL_SENSOR);

        resetI2C();

        Adafruit_Si7021 si7021 = Adafruit_Si7021();

        unsigned long sensorStartTime = millis();
        bool          sensorSuccess   = true;

    	while(! si7021.begin()){
    	    logger(sensorName + ": NOT found. waiting " + String(SENSOR_SI7021_WAIT_BEGIN) + "s");
    	    
    	    if (millis() - sensorStartTime > TRY_SENSOR_FOR_MS) {
    	        sensorSuccess   = false;
    	        break;   
    	    }
        
    	    delay(SENSOR_SI7021_WAIT_BEGIN);
    	}

        if ( sensorSuccess ) {
        	logger(sensorName + ": found. continuing");
    
    	    delay(SENSOR_SI7021_WAIT_BEGIN);

            sprintf(publishString,"{\"t\":%0.1f,\"h\":%0.1f,\"p\":null,\"s\":\"SI7021\",\"v\":\"%s\"}",si7021.readTemperature(),si7021.readHumidity(),VERSION);

            logger(sensorName + ": sending :: " + String(publishString));
            
            Particle.publish(CHANNEL_DATA, publishString, PRIVATE);
        } else {
            logger(sensorName + ": NOT found. QUITTING");
        }
    }
#endif



#ifdef SENSOR_BMP085_ADAFRUIT
    void checkSensorBMP085() {
        String sensorName = "BMP085";
        
        logger(sensorName + ": setting up", CHANNEL_SENSOR);

        resetI2C();
        
        Adafruit_BMP085 bmp085 = Adafruit_BMP085();

        unsigned long sensorStartTime = millis();
        bool          sensorSuccess   = true;

    	while(! bmp085.begin()){
            logger(sensorName + ": NOT found. waiting " + String(SENSOR_BMP085_WAIT_BEGIN) + "s");

    	    if (millis() - sensorStartTime > TRY_SENSOR_FOR_MS) {
    	        sensorSuccess   = false;
    	        break;   
    	    }
        
    	    delay(SENSOR_BMP085_WAIT_BEGIN);
    	}


        if ( sensorSuccess ) {
        	logger(sensorName + ": found. continuing");
    
    	    delay(SENSOR_BMP085_WAIT_BEGIN);

            sprintf(publishString,"{\"t\":%0.1f,\"h\":null,\"p\":%ld,\"s\":\"BMP085\",\"v\":\"%s\"}",bmp085.readTemperature(),bmp085.readPressure(),VERSION);
    
            logger(sensorName + ": sending :: " + String(publishString));
            
            Particle.publish(CHANNEL_DATA, publishString, PRIVATE);
        } else {
            logger(sensorName + ": NOT found. QUITTING");
        }
    }
#endif



#ifdef SENSOR_BMP183_ADAFRUIT
    void checkSensorBMP183() {
        String sensorName = "BMP183";
        
        logger(sensorName + ": setting up", CHANNEL_SENSOR);

        resetI2C();

        Adafruit_BMP183_Unified bmp183 = Adafruit_BMP183_Unified(BMP183_CS);  // use hardware SPI

        unsigned long sensorStartTime = millis();
        bool          sensorSuccess   = true;

    	while(! bmp183.begin()){
    	    logger(sensorName + ": NOT found. waiting " + String(SENSOR_BMP183_WAIT_BEGIN) + "s");
        
    	    delay(SENSOR_BMP183_WAIT_BEGIN);

    	    if (millis() - sensorStartTime > TRY_SENSOR_FOR_MS) {
    	        sensorSuccess   = false;
    	        break;   
    	    }
    	}


        if ( sensorSuccess ) {
            logger(sensorName + ": found. continuing");

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
    
                logger(sensorName + ": sending :: " + String(publishString));
                
                Particle.publish(CHANNEL_DATA, publishString, PRIVATE);
            } else {
                logger(sensorName + ": sensor error. QUITTING");
            }
        } else {
            logger(sensorName + ": NOT found. QUITTING");
        }
    }
#endif


#ifdef SENSOR_BME280_ADAFRUIT
    void checkSensorBME280() {
        // https://github.com/adafruit/Adafruit_BME280_Library

        String sensorName = "BME280";
        
        logger(sensorName + ": setting up", CHANNEL_SENSOR);

        resetI2C();

        Adafruit_BME280 bme280 = Adafruit_BME280(); // I2C

        unsigned long sensorStartTime = millis();
        bool          sensorSuccess   = true;

    	while(! bme280.begin(BME280_ADDRESS)){
    	    logger(sensorName + ": NOT found. waiting " + String(SENSOR_BME280_WAIT_BEGIN) + "s");
        
    	    delay(SENSOR_BME280_WAIT_BEGIN);

    	    if (millis() - sensorStartTime > TRY_SENSOR_FOR_MS) {
    	        sensorSuccess   = false;
    	        break;   
    	    }
    	}


        if ( sensorSuccess ) {
        	logger(sensorName + ": found. continuing");

    	    delay(SENSOR_BME280_WAIT_BEGIN);

            float temperature = bme280.readTemperature();
            delay(10);
            float pressure    = bme280.readPressure();
            delay(10);
            float humidity    = bme280.readHumidity();
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
                sprintf(publishString,"{\"t\":%0.1f,\"h\":%0.1f,\"p\":%0.0f,\"s\":\"BME280\",\"v\":\"%s\"}",temperature,humidity,pressure,VERSION);
    
                logger(sensorName + ": sending :: " + String(publishString));
                
                Particle.publish(CHANNEL_DATA, publishString, PRIVATE);
            } else {
                logger(sensorName + ": sensor error. QUITTING");
            }
        } else {
            logger(sensorName + ": NOT found. QUITTING");
        }
    }
#endif



#ifdef SENSOR_TMP36
    void checkSensorTMP36() {
        String sensorName = "TMP36";
        
        logger(sensorName + ": setting up", CHANNEL_SENSOR);

        pinMode(SENSOR_TMP36_PIN, INPUT);
        analogRead(SENSOR_TMP36_PIN);
    
        sprintf(publishString,"{\"t\":%0.1f,\"h\":null,\"p\":null,\"s\":\"TMP36\",\"v\":\"%s\"}",TMP36GetCentigrade(SENSOR_TMP36_PIN),VERSION);
    
        logger(sensorName + ": sending :: " + String(publishString));
    
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


