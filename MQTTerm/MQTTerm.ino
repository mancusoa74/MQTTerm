/*
###############################################################################
# MQTTerm.ino       : Simple MQTT thermometer
#
# Author            : Antonio Mancuso
#
# Date created      : 2019-03-11
#
# Version           : 1.1
#
# Purpose           : Main Sketch file
#
# Revision History  
#
# Date        Author      Version    Changes
# 2019-03-11  anmancus      1.0      Initial version
# 2019-03-12  anmancus      1.2      Add power voltage reporting
#
##########################################################################
*/

#include "MQTTerm.h"
#include "timeapi.h"

OneWire           oneWire(DS18B20_PIN);
DallasTemperature tsensor(&oneWire);
WiFiClient        wifi_client;
PubSubClient      mqtt_client(wifi_client, MQTT_SERVER, MQTT_BROKER_PORT); 
bool              mqtt_status;
byte              termow_mode = MQTTERM_MODE_SINGLE;
String            temp_topic;
ADC_MODE(ADC_VCC); 

//Wifi Initialization function
int WiFi_init()
{
    int retries = 0;
    log("Connecting to WiFi AP..........");
    
    WiFi.mode(WIFI_STA); //set wifi station mode
    delay(100);
    WiFi.begin(WIFI_SSID, WIFI_PASSWD); //start connecting to WiFi AP
    delay(100);
    
    //check the status of WiFi connection to be WL_CONNECTED
    while ((WiFi.status() != WL_CONNECTED) && (retries < MAX_WIFI_INIT_RETRY)) {
        retries++;
        delay(WIFI_RETRY_DELAY);
        log("#");
    }
    log(String(WiFi.localIP()[0]) + "." + String(WiFi.localIP()[1]) + "." + String(WiFi.localIP()[2]) + "." + String(WiFi.localIP()[3]));
    return WiFi.status(); //return the WiFi connection status
}

int MQTT_init()
{
    log("Initializing MQTT communication.........");
    mqtt_client.set_max_retries(255);
    
    //here we connect to MQTT broker and we increase the keepalive for more reliability
    if (mqtt_client.connect(MQTT::Connect(MQTT_CLIENT_ID).set_keepalive(90).set_auth(String(MQTT_UNAME), String(MQTT_PASSW))))
        log("Connection to MQTT broker SUCCESS..........");
    else
        log("Connection to MQTT broker ERROR..........");
        
    return mqtt_client.connected();
}

float avg_temp_read() {
    float temps[NUM_TEMP_READS];
    float averaget = 0;

    log("execute dummy reads...");
    for(int i = 0; i < NUM_TEMP_DUMMY_READS; i++) {
      tsensor.requestTemperatures();
      log(tsensor.getTempCByIndex(0));
      delay(DELAY_TEMP_READS);
    }
    
    log("Requesting temperatures reads...");
    for(int i = 0; i < NUM_TEMP_READS; i++){
        tsensor.requestTemperatures();
        temps[i] = tsensor.getTempCByIndex(0);
        log(temps[i]);
        delay(DELAY_TEMP_READS);
        averaget += temps[i];
    }
    return(averaget/NUM_TEMP_READS);
}

void publish_temp(float temp) {
    String web_time = get_web_time();
    int power_voltage = ESP.getVcc();
    log(web_time);
    log(power_voltage);
    
    String json = "{\"datetime\":\"" + web_time + 
                    "\", \"voltage\":\"" + String(power_voltage) +
                    "\", \"thermometer\":\"" + TERMOW_NAME +
                    "\", \"temperature\":\"" + 
                    String(temp) + "\"}";
    mqtt_client.publish(temp_topic, json);
    log("MQTT TEMP message sent....");
}

void setup() { 
    int timeout = 0;
    int iterations = 0;
    float temperature;
    
    Serial.begin(SERIAL_SPEED);
    delay(100);    

    pinMode(BUTTON_CONTROL, INPUT_PULLUP);
    if (digitalRead(BUTTON_CONTROL) == LOW) {
        termow_mode = MQTTERM_MODE_CONTINUE;
        timeout = MQTTERM_CONTINUE_DELAY;
        iterations = INT_MAX;
        temp_topic = MQTT_CONTINUE_TOPIC;
        log("termow mode = MQTTERM_MODE_CONTINUE");
    } else {
        termow_mode = MQTTERM_MODE_SINGLE;
        timeout = MQTTERM_SINGLE_DELAY;
        iterations = 1;
        temp_topic = MQTT_SINGLE_TOPIC;
        log("termow mode = MQTTERM_MODE_SINGLE");
    }
    log(timeout);
    log(iterations);
    log(temp_topic);

    log("MQTT_basic starting....");
    if (WiFi_init() != WL_CONNECTED)
        log("WiFi connection ERROR....");
    else {
        log("WiFi connection OK....");
        mqtt_status = MQTT_init();
        if (!mqtt_status)
            log("MQTT connection ERROR....");        
        else
            log("MQTT connection OK....");      
    }

    //set DS18B20 parameters
    tsensor.begin();
    tsensor.setResolution(10);
    tsensor.setWaitForConversion(true);

    for(int i = 0; i < iterations; i++) {
        temperature = avg_temp_read();
        publish_temp(temperature);
        if (iterations > 1)
          delay(timeout * 1000);
    }
    log("entering deep sleep");
    ESP.deepSleep(timeout * 1000000);
}

void loop() {
}

