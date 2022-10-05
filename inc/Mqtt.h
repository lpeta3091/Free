#ifndef SIMPLE_MQTT_CLIENT_MQTT_H
#define SIMPLE_MQTT_CLIENT_MQTT_H

#include <mosquittopp.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <syslog.h>
#include "lcd_ili9341.h"
#include "fonts.h"
#include "def.h"

#define TAG "Mqtt.cpp: "

using namespace std;
/*
struct _mData {
  uint8_t num_cycles = 10;
  uint8_t num_cycles_dist = 1;
  int min_distance = 330;
  int max_distance = 480;
  int min_angle = 10;
  int max_angle = 85;
};
*/
void write_all_settings();

//extern _mData mData;
extern DisplayILI9341_240x320x16_SPI display;

/**
 * @class Mqtt
 * This class is responsible for the mqtt connection using mosquittopp.
 */
class Mqtt : public mosqpp::mosquittopp {
private:
    string id;
    string host;
    string publish_topic;
    vector<string> subscription_topic_list;
    //char str_from_mqtt[64]; // = string("no messages");
    //char str_old_mqtt[64];    // = str_from_mqtt;
    //string str_payload;
    //string str_topic;
    int port;
    int cmd = -1;
    DisplayILI9341_240x320x16_SPI *displ;
    bool MESSAGEf = false;

    /**
     * the number of seconds after which the broker should send a PING message to the client if no other messages have
     * been exchanged in that time
     */
    int keepalive;
    void on_connect(int rc);
    void on_disconnect(int rc);
    void on_publish(int mid);
    void on_subscribe(int /*mid*/, int /*qos_count*/, const int */*granted_qos*/);

    /**
     * @brief callback function
     *
     * callback function that is called when a new message is received
     *
     * @param message structure that contains the message as payload, among other things
     */
    void on_message(const struct mosquitto_message *message);

public:
    string str_payload;
    string str_topic;
    mosquitto_message msqm;

//    char str_from_mqtt[64]; // = string("no messages");
    Mqtt(string id, string publish_topic,vector<string> subscription_topic_list, string host, int port, string username, string password);
    Mqtt(string id, string publish_topic,vector<string> subscription_topic_list, string host, int port);
    Mqtt(string id = string("default"));
    void addSubTopic(string id);
    string getPayload() { return str_payload; }
    string getTopic() { return str_topic; }
    void setCmd(int _cmd) { cmd = _cmd; }
    int getCmd() { return cmd; };
    void setDispl(DisplayILI9341_240x320x16_SPI *dspl) { displ = dspl; }
    bool getMessageF() { bool fm = MESSAGEf; MESSAGEf = false; return fm; }
    void setMessageF(bool fm) { MESSAGEf = fm; }

    ~Mqtt();

    bool publish(string message);
    bool subscribe();
};

int infoToLed(std::string);
int doEnd();

#endif //SIMPLE_MQTT_CLIENT_MQTT_H
