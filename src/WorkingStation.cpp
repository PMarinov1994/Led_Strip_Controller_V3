#include "WorkingStation.h"
#include "ConfigurationFile.h"
#include "globals.h"

struct CRGB;

/*
 *	\brief Initialize the component
 *
 */
bool CWorkingStation::Init()
{
    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        EffectsManager* pEffectsManager = new EffectsManager(i);

        pEffectsManager->init(this);
        pEffectsManager->onWiFiStatusChanged(false);
        pEffectsManager->onMqttStatusChanged(false);
        pEffectsManager->loop();

        m_vecEffects.push_back(pEffectsManager);
    }

    CConfigurationFile configFile;
    configFile.ParseConfiguration();

    int ssidLen = strlen(configFile.m_ssid) + 1;
    int pskLen = strlen(configFile.m_psk) + 1;

    if (m_ssid)
        delete[] m_ssid;

    m_ssid = new char[ssidLen];

    if (m_psk)
        delete[] m_psk;

    m_psk = new char[pskLen];

    memcpy(m_ssid, configFile.m_ssid, ssidLen);
    memcpy(m_psk, configFile.m_psk, pskLen);

    ConnectToWifi();

    m_vecEffects.at(0)->onWiFiStatusChanged(true);
    m_vecEffects.at(0)->onMqttStatusChanged(false);
    m_vecEffects.at(0)->loop();

    IPAddress mqttServerIPAddr;
    mqttServerIPAddr.fromString(configFile.m_mqttServerIP);

    m_client.setServer(mqttServerIPAddr, configFile.m_mqttServerPort);

    MQTT_CALLBACK_SIGNATURE = std::bind(
        &CWorkingStation::MQTT_Callback,
        this,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3);

    m_client.setCallback(callback);

    // Connect to MQTT Broker.
    ReconnectMQTT();

    m_vecEffects.at(0)->onMqttStatusChanged(true);
    m_vecEffects.at(0)->loop();

    return true;
}

uint8_t uiLedState = LOW;

/*
 *	\brief This will loop in main
 */
void CWorkingStation::Work()
{
    // Check WiFi state
    // and if down - try to reconnect
    if (WL_CONNECTED != WiFi.status())
    {
        m_vecEffects.at(0)->onWiFiStatusChanged(false);
        m_vecEffects.at(0)->loop(); // Loop the effects to redraw the error since we will be blocking untill reconnection

        Print("WIFI DOWN! Reconnecting");
        WIFI_WAIT_FOR_CONNECTION

        m_vecEffects.at(0)->onWiFiStatusChanged(true);
    }
    else if (!m_client.connected())
    {
        m_vecEffects.at(0)->onMqttStatusChanged(false);
        m_vecEffects.at(0)->loop(); // Loop the effects to redraw the error since we will be blocking untill reconnection

        Println("MQTT Client is disconnected. Will try to reconnect in a moment.");
        this->ReconnectMQTT();

        m_vecEffects.at(0)->onMqttStatusChanged(true);
    }
    else
    {
        m_client.loop();

        for (int i = 0; i < NUM_CHANNELS; i++)
            m_vecEffects.at(i)->loop();
    }

    //
    // delay(10);
}

/*
********************** MQTT ERRORS **********************
*
* TODO: This needs to be made into map and returned somewhere.
    -4 : MQTT_CONNECTION_TIMEOUT - the server didn't respond within the keepalive time
    -3 : MQTT_CONNECTION_LOST - the network connection was broken
    -2 : MQTT_CONNECT_FAILED - the network connection failed
    -1 : MQTT_DISCONNECTED - the client is disconnected cleanly
    0 : MQTT_CONNECTED - the client is connected
    1 : MQTT_CONNECT_BAD_PROTOCOL - the server doesn't support the requested version of MQTT
    2 : MQTT_CONNECT_BAD_CLIENT_ID - the server rejected the client identifier
    3 : MQTT_CONNECT_UNAVAILABLE - the server was unable to accept the connection
    4 : MQTT_CONNECT_BAD_CREDENTIALS - the username/password were rejected
    5 : MQTT_CONNECT_UNAUTHORIZED - the client was not authorized to connect
*/

/*
 *	\brief This will loop in main
 */
bool CWorkingStation::ReconnectMQTT()
{
    unsigned long ulRestartTime = ELAPSED_SECONDS + WAIT_BEFORE_RESTART_SEC;
    while (!m_client.connected())
    {
        if (ulRestartTime < ELAPSED_SECONDS)
        {
            Println("Failed to reconnect to MQTT Brocker for 5 mins. Calling ESP.restart()");
            SERIAL_END;
            ESP.restart();
        }

        if (m_client.connect(stationID))
        {
            Println("MQTT Client connected!");
            PublishCurrPlayEffect();
        }
        else
        {
            Print("failed, rc=");
            Print(m_client.state());
            Println(" try again in 2 seconds");
            // Wait 2 seconds before retrying
            delay(2000);
        }
    }

    ulRestartTime = ELAPSED_SECONDS + WAIT_BEFORE_RESTART_SEC;
    while (!m_client.subscribe(subscribeTopic, MQTTQOS0))
    {
        if (ulRestartTime < ELAPSED_SECONDS)
        {
            Println("Failed to subscribe to MQTT Topic for 5 mins. Calling ESP.restart()");
            SERIAL_END;
            ESP.restart();
        }

        Println("Subscribe failed. Trying again...");
        delay(500);
    };

    Println("MQTT ALL OK");
    return true;
}

void CWorkingStation::ConnectToWifi()
{
    WiFi.mode(WIFI_STA);
    delay(20);

    WiFi.begin(m_ssid, m_psk);

    Print("Connecting");
    WIFI_WAIT_FOR_CONNECTION

    Println("");
    Print("Connected, IP address: ");
    Println(WiFi.localIP());

    // The ESP8266 tries to reconnect automatically when the connection is lost
    WiFi.setAutoReconnect(true);
}

void CWorkingStation::PublishCurrPlayEffect()
{
    if (!m_client.connected())
        return;

    String strEffects = "";
    String strBrightness = "";
    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        EffectsManager* pEffectsManager = m_vecEffects.at(i);
        strEffects += pEffectsManager->getCurrEffectName() + "\n";
        strBrightness += String(pEffectsManager->getBrightnes()) + "\n";
    }

    bool pubStateResult = m_client.publish(reportCurrEffectTopic, strEffects.c_str(), false);
    Print("Publishing curr play effect. Result: ");
    Println(pubStateResult);

    pubStateResult &= m_client.publish(reportCurrBrightnessTopic, strBrightness.c_str(), false);
    Print("Publishing curr brightness. Result: ");
    Println(pubStateResult);

    if (!pubStateResult)
        ESP.restart();
}

void CWorkingStation::ReportError(String err)
{
    if (!m_client.connected())
        return;

    bool pubStateResult = m_client.publish(reportErrorTopic, err.c_str(), false);

    Print("Error MQTT post result: ");
    Println(pubStateResult);
}

void CWorkingStation::MQTT_Callback(char *topic, uint8_t *payload, unsigned int length)
{
    if (0 == length)
        return;

    Print("Got MQTT message -> ");
    Println(topic);

    if (0 == strcmp(topic, setEffectTopic))
    {
        char *buff;
        NullTerminateArray(payload, length, (void **)&buff);

        String value(buff);
        delete[] buff;

        Println("Message:");
        Println(value);

        // Notify each effects manager. The correct channel is inside the JSON payload.
        // Channels will check if the payload is for them and if not they will ignore it.
        for (int i = 0; i < NUM_CHANNELS; i++)
            m_vecEffects.at(i)->changeEffect(value);

        PublishCurrPlayEffect();
    }
    else if (0 == strcmp(topic, setBrightnessTopic))
    {
        char *buff = new char[length + 1];
        NullTerminateArray(payload, length, (void **)&buff);

        int value = strtol(buff, NULL, 10);

        int iChannelNum = value / 1000;
        int iChannelValue = value % 1000;

        if (iChannelValue >= 0 && iChannelValue <= 255)
        {
            if (iChannelNum == 0)
                for (int i = 0; i < NUM_CHANNELS; i++)
                    m_vecEffects.at(i)->setBrightnes(iChannelValue);
            else if (iChannelNum > 0 && iChannelNum <= NUM_CHANNELS)
                m_vecEffects.at(iChannelNum - 1)->setBrightnes(iChannelValue);
        }

        Println("Message:");
        Println(value);

        delete[] buff;
        PublishCurrPlayEffect();
    }
}

void CWorkingStation::NullTerminateArray(void *src, uint8_t len, void **dest)
{
    if (((char *)src)[len - 1] != '\0')
    {
        *dest = new char[len + 1];
        memset(*dest, 0, len + 1);
        memcpy(*dest, src, len);
    }
    else
    {
        *dest = new char[len];
        memcpy(*dest, src, len);
    }
}