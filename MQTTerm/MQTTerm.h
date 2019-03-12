/*
###############################################################################
# MQTTerm.h        : Simple MQTT thermometer
#
# Author            : Antonio Mancuso
#
# Date created      : 2019-03-11
#
# Version           : 1.1
#
# Purpose           : Main header file
#
# Revision History  
#
# Date        Author      Version    Changes
# 2019-03-11  anmancus      1.0      Initial version
# 2019-03-12  anmancus      1.1      Add version number
#
##########################################################################
*/

#ifndef TERMOMETROW_H
#define TERMOMETROW_H

#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define VERSION "1.1.1"

//----SERIAL CONFIG----
#define SERIAL_SPEED          115200

//----WIFI CONFIG----
#define WIFI_SSID             "YOUR_SSID"                     //your Wifi SSID
#define WIFI_PASSWD           "YOUR_WIFI_PASSWORD" //your wifi password
#define MAX_WIFI_INIT_RETRY   10
#define WIFI_RETRY_DELAY      1000

//----MQTT CONFIG----
#define MQTT_CLIENT_ID        "YOUR_CLIENT_ID"   //your unique client ID
#define MQTT_SERVER           "YOUR_MQTT_BROKER" //MQTT broker sever. I use https://www.cloudmqtt.com/
#define MQTT_UNAME            "MQTT_USER_NAME"   //MQTT broker user name - I use this broker https://www.cloudmqtt.com/
#define MQTT_PASSW            "MQTT_PASSWD"      //MQTT broker password
#define MQTT_BROKER_PORT      17780              //MQTT BROKER listening port
#define TERMOW_NAME           "YOUR_TERM_NAME"   //name of your thermometer
#define MQTT_SINGLE_TOPIC     "YOUR_MQTT_TOPIC"TERMOW_NAME"/single"   // MQTT topic for single temperature read
#define MQTT_CONTINUE_TOPIC   "YOUR_MQTT_TOPIC"TERMOW_NAME"/continue" // MQTT topic for continuous temperature read

//----MQTTerm CONFIG----
#define DS18B20_PIN           5   //data pin for DS18B20 sensor
#define BUTTON_CONTROL        12  //data pin for the control switch
#define DEBUG                 0   //1=enable DEBUG mode; 0=disble debug mode
#define DELAY_TEMP_READS      250 //delay between consecutive reads
#define NUM_TEMP_READS        3   //number of temperature read - mean value will be calculated
#define NUM_TEMP_DUMMY_READS  2   //number of initial temp read which will be discarded
#define MQTTERM_MODE_SINGLE    0
#define MQTTERM_MODE_CONTINUE  1
#define MQTTERM_SINGLE_DELAY   900 //number of seconds to deep sleep in SINGLE mode
#define MQTTERM_CONTINUE_DELAY 5  //number of seconds between temp read in CONTINUOS mode

#define log(mex) do {if (DEBUG) {Serial.println(mex);Serial.flush();}} while(0)

#endif



