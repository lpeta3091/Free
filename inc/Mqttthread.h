#ifndef MQTTTHREAD_H
#define MQTTTHREAD_H

#pragma once

#include <mosquittopp.h>

class Mqttthread : public mosqpp::mosquittopp
{
	private:
		std::string publish_topic;
		std::string subscribe_topic;
        public:
                Mqttthread(const char *id, const char *host, int port);
                ~Mqttthread();

                void on_connect(int rc);
                void on_message(const struct mosquitto_message *message);
                void on_subscribe(int mid, int qos_count, const int *granted_qos);
		bool publishP(std::string message);
};

#endif
