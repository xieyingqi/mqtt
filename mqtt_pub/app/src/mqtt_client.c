#include "includes.h"
#include "MQTTClient.h"

#define ADDRESS "tcp://test.mosquitto.org:1883"
#define CLIENTID "puber"
#define TOPIC "updt"
#define TOPIC2 "will"
#define QOS 1
#define TIMEOUT 10000L

MQTTClient mqttClient;

void MQTT_Client_Init(void)
{
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	MQTTClient_willOptions will_opts = MQTTClient_willOptions_initializer;
	int rc;

	rc = MQTTClient_create(&mqttClient, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	if (rc != MQTTCLIENT_SUCCESS)
	{
		logPrintf("Failed to create, return code %d", rc);
		exit(-1);
	}
	
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

	will_opts.topicName = TOPIC2;
	will_opts.message = "bye";
	conn_opts.will = &will_opts;

	rc = MQTTClient_connect(mqttClient, &conn_opts);
	if (rc != MQTTCLIENT_SUCCESS)
	{
		logPrintf("Failed to connect, return code %d", rc);
		exit(-1);
	}

	logPrintf("Subscribing to topic %s\nfor client %s using QoS%d", TOPIC, CLIENTID, QOS);
}

void MQTT_Client_Destroy(void)
{
	//MQTTClient_disconnect(mqttClient, 10000);
	MQTTClient_destroy(&mqttClient);
}

void MQTT_Publish(char *buf, int len)
{
	MQTTClient_message msg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;
	int rc;

	msg.payload = buf;
	msg.payloadlen = len;
	msg.qos = QOS;
	msg.retained = 0;

	rc = MQTTClient_publishMessage(mqttClient, TOPIC, &msg, &token);
	if (rc != MQTTCLIENT_SUCCESS)
	{
		logPrintf("Fail to publish message, return code %d", rc);
	}
}