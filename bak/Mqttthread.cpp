#include <cstdio>
#include <cstring>
#include <string>
#include "inc/Mqttthread.h"
#include <mosquittopp.h>

Mqttthread::Mqttthread(const char *id, const char *host, int port) : mosquittopp(id)
{
	int keepalive = 60;
	publish_topic = std::string(id);

	/* Connect immediately. This could also be done by calling
	 * mqtt_tempconv->connect(). */
	//connect(host, port, keepalive);
	connect_async(host, port, keepalive);
	loop_start();
};

Mqttthread::~Mqttthread()
{
	loop_stop();
}

void Mqttthread::on_connect(int rc)
{
	printf("Connected with code %d.\n", rc);
	if(rc == 0){
		/* Only attempt to subscribe on a successful connect. */
		subscribe(NULL, subscribe_topic.c_str());
	}
}

void Mqttthread::on_message(const struct mosquitto_message *message)
{
	double temp_celsius, temp_fahrenheit;
	char buf[51];

	if(!strcmp(message->topic, subscribe_topic.c_str())){
		memset(buf, 0, 51*sizeof(char));
		/* Copy N-1 bytes to ensure always 0 terminated. */
		memcpy(buf, message->payload, 50*sizeof(char));
		temp_celsius = atof(buf);
		temp_fahrenheit = temp_celsius*9.0/5.0 + 32.0;
		snprintf(buf, 50, "%f", temp_fahrenheit);
		publish(NULL, "tmp/f", strlen(buf), buf);
	}
}

void Mqttthread::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
	printf("Subscription succeeded.\n");
}

bool Mqttthread::publishP(std::string message)
{
	//char buf[51]  = "ASDF123";
	publish(NULL, publish_topic.c_str(), strlen(message.c_str()), message.c_str());
    	return 0; //(answer == MOSQ_ERR_SUCCESS);
}
