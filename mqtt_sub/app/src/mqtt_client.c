#include "includes.h"
#include "MQTTClient.h"

#define ADDRESS "tcp://test.mosquitto.org:1883"
#define CLIENTID "suber"
#define TOPIC "updt"
#define TOPIC2 "will"
#define QOS 1
#define TIMEOUT 10000L

MQTTClient mqttClient;
volatile MQTTClient_deliveryToken deliveredtoken;

static void delivered(void *context, MQTTClient_deliveryToken dt)
{
	logPrintf("Message with token value %d delivery confirmed\n", dt);
	deliveredtoken = dt;
}

static int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	logPrintf("message form %s: %s", topicName, (char *)message->payload);

	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}

static void connlost(void *context, char *cause)
{
	logPrintf("Connection lost cause: %s", cause);
}

void MQTT_Client_Init(void)
{
	MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
	int rc;

	rc = MQTTClient_create(&mqttClient, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	if (rc != MQTTCLIENT_SUCCESS)
	{
		logPrintf("Failed to create, return code %d", rc);
		exit(-1);
	}

	rc = MQTTClient_setCallbacks(mqttClient, NULL, connlost, msgarrvd, delivered);
	if (rc != MQTTCLIENT_SUCCESS)
	{
		logPrintf("Failed to setCallbacks, return code %d", rc);
		exit(-1);
	}

	opts.keepAliveInterval = 20;
	opts.cleansession = 1;
	rc = MQTTClient_connect(mqttClient, &opts);
	if (rc != MQTTCLIENT_SUCCESS)
	{
		logPrintf("Failed to connect, return code %d", rc);
		exit(-1);
	}

	logPrintf("Subscribing to topic %s\nfor client %s using QoS%d", TOPIC, CLIENTID, QOS);
	MQTTClient_subscribe(mqttClient, TOPIC, QOS);
	MQTTClient_subscribe(mqttClient, TOPIC2, QOS);
}

void MQTT_Client_Destroy(void)
{
	MQTTClient_disconnect(mqttClient, 10000);
	MQTTClient_destroy(&mqttClient);
}