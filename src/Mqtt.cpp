#include "Mqtt.h"

Mqtt::Mqtt(string id, string publish_topic,vector<string> subscription_topic_list, string host, int port, string username, string password){

    mosqpp::lib_init();
    this->id = id;
    this->keepalive = 60;
    this->port = port;
    this->host = host;
    this->publish_topic = publish_topic;
    this->subscription_topic_list = subscription_topic_list;

    mosquittopp::username_pw_set(username.c_str(), password.c_str());


    /*
     * Connect to an MQTT broker. This is a non-blocking call. If you use mosquitto_connect_async your client must use
     * the threaded interface mosquitto_loop_start.
     */
    connect_async(this->host.c_str(), this->port, this->keepalive);
    loop_start();

}



Mqtt::Mqtt(string id, string publish_topic,vector<string> subscription_topic_list , string host, int port) : mosquittopp(id.c_str())
{
    mosqpp::lib_init();
    this->id = id;
    this->keepalive = 60;
    this->port = port;
    this->host = host;
    this->publish_topic = publish_topic;
    this->subscription_topic_list = subscription_topic_list;



    /*
     * Connect to an MQTT broker. This is a non-blocking call. If you use mosquitto_connect_async your client must use
     * the threaded interface mosquitto_loop_start.
     */
    connect_async(this->host.c_str(), this->port, this->keepalive);
    loop_start();
};

Mqtt::Mqtt(string id){

    mosqpp::lib_init();
    this->id = id;
    this->keepalive = 60;
    this->port = 1883;
    this->host = "127.0.0.1";
    this->publish_topic = "work";
	//std::vector<string> stla;
        //stla.push_back("senceTo/minD");
        //stla.push_back("senceTo/maxD");
        //stla.push_back("senceTo/minA");
        //stla.push_back("senceTo/maxA");
        //stla.push_back("infoToLed");
        //stla.push_back("command");

    //this->subscription_topic_list = subscription_topic_list;
	this->subscription_topic_list.push_back("senceTo/minD");
	this->subscription_topic_list.push_back("senceTo/maxD");
	this->subscription_topic_list.push_back("senceTo/minA");
	this->subscription_topic_list.push_back("senceTo/maxA");
	this->subscription_topic_list.push_back("infoToLed");
	this->subscription_topic_list.push_back("command");

    mosquittopp::username_pw_set("user", "passw0rd");

    //strcpy(str_from_mqtt, "no messages");
    //strcpy(str_old_mqtt, str_from_mqtt);


    /*
     * Connect to an MQTT broker. This is a non-blocking call. If you use mosquitto_connect_async your client must use
     * the threaded interface mosquitto_loop_start.
     */
    connect_async(this->host.c_str(), this->port, this->keepalive);
    loop_start();

}

void Mqtt::addSubTopic(string id)
{
        this->subscription_topic_list.push_back(id.c_str());
	syslog(LOG_NOTICE, ("Adding topic : " + id).c_str());
}

Mqtt::~Mqtt() {
    disconnect();
    loop_stop();
    mosqpp::lib_cleanup();
}

bool Mqtt::publish(string message)
{
    int answer = mosqpp::mosquittopp::publish(nullptr, publish_topic.c_str(), message.length(), message.c_str(), 1, false);
    return (answer == MOSQ_ERR_SUCCESS);
}

bool Mqtt::subscribe() {
    bool success = true;
    for(int i=0; i<((int)subscription_topic_list.size()); i++){
        int answer = mosquittopp::subscribe(nullptr, subscription_topic_list[i].c_str());
        if(answer != MOSQ_ERR_SUCCESS){
            success = false;
        }
    }
    return success;
}

void Mqtt::on_subscribe(int, int, const int *) {
    cout << TAG <<"Subscription succeeded." << endl;
}

void Mqtt::on_message(const struct mosquitto_message *message) {

    str_payload = string(static_cast<char *>(message->payload));
    str_topic = string(static_cast<char *>(message->topic));
    setMessageF(true);
    //memcpy(&msqm, message, sizeof(mosquitto_message));
    //displ->printFixedN(15, 275, (char *)message->payload, STYLE_NORMAL, 1);
/*
    cout<< TAG << "payload: " << payload << endl;
    cout<< TAG << "topic: " << topic << endl;
    if (topic == "senceTo/minD") mData.min_distance = stoi(payload);
    if (topic == "senceTo/maxD") mData.max_distance = stoi(payload);
    if (topic == "senceTo/minA") mData.min_angle = stoi(payload);
    if (topic == "senceTo/maxA") mData.max_angle = stoi(payload);
    if (topic == "senceTo/numC") mData.num_cycles_dist = stoi(payload);;
    write_all_settings();
*/
    //syslog(LOG_NOTICE, (char *)message->payload);
/*
    if (topic == "infoToLed")  {
		strcpy(str_old_mqtt, str_from_mqtt);
		strcpy(str_from_mqtt, (char *)message->payload);
    }; //infoToLed(payload);
    if (topic == "command") 
	if (payload == "halt") cmd = 0; system("shutdown -n now"); 
*/
}

void Mqtt::on_disconnect(int rc) {
    cout << TAG << "disconnection(" << rc << ")" << endl;
}

void Mqtt::on_connect(int rc)
{
    if ( rc == 0 ) {
        cout << TAG << "connected with server" << endl;
    } else {
        cout << TAG << "impossible to connect with server(" << rc << ")" << endl;
    }
}

void Mqtt::on_publish(int mid)
{
    //cout << TAG << "Message (" << mid << ") succeed to be published " << endl;
}
