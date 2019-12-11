#include <WiFi.h>
#include <PubSubClient.h>

int pinoI = 32;
int pinoC1 = 33;
int pinoM = 25;
int pinoC2 = 26;

int status_I;
int status_C1;
int status_M;
int status_C2;

int porta_rele1 = 17; //Pino ligado ao Relé (ativado lamapada)
int acionamento; //Variavel para guardar valor do sensor

#define TOPICO1        "letras/i"
#define TOPICO2        "letras/c1"
#define TOPICO3        "letras/m"
#define TOPICO4		     "letras/c2"
#define TOPICO5 	     "comodo1/sensor1"

#define ID_MQTT  "esp32_mqtt"

const char* SSID = "TP-Link_1B7A"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = "71289168"; // Senha da rede WI-FI que deseja se conectar
 
const char* BROKER_MQTT = "192.168.0.100"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; // Porta do Broker MQTT
int status_luz1;

 
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient


void initWiFi(void);
void initMQTT(void);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void reconnectMQTT(void);
void reconnectWiFi(void);
void VerificaConexoesWiFIEMQTT(void);

void initWiFi(void) 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
      
    reconnectWiFi();
}

void initMQTT(void) 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

void altera_lampada(String msg, char* topic){
  if(!strcmp(topic, TOPICO1)){

    if(msg.equals("0")) status_I=0;
    if(msg.equals("1")) status_I=1;
    if(msg.equals("2")) status_I=2;

    if(msg.equals("0")) Serial.println("I: desligado.");
    if(msg.equals("1")) Serial.println("I: ligado.");
    if(msg.equals("2")) Serial.println("I: no sensor.");
  }
  if(!strcmp(topic, TOPICO2)){
    status_C1 = (int)msg[0];

    if(msg.equals("0")) status_C1=0;
    if(msg.equals("1")) status_C1=1;
    if(msg.equals("2")) status_C1=2;
    
    if(msg.equals("0")) Serial.println("C1: desligado.");
    if(msg.equals("1")) Serial.println("C1: ligado.");
    if(msg.equals("2")) Serial.println("C1: no sensor.");
  }
  if(!strcmp(topic, TOPICO3)){
    status_M = (int)msg[0];
  
    if(msg.equals("0")) status_M=0;
    if(msg.equals("1")) status_M=1;
    if(msg.equals("2")) status_M=2;
    
    if(msg.equals("0")) Serial.println("M: desligado.");
    if(msg.equals("1")) Serial.println("M: ligado.");
    if(msg.equals("2")) Serial.println("M: no sensor.");
  }
  if(!strcmp(topic, TOPICO4)){
    status_C2 = (int)msg[0];

    if(msg.equals("0")) status_C2=0;
    if(msg.equals("1")) status_C2=1;
    if(msg.equals("2")) status_C2=2;
    
    if(msg.equals("0")) Serial.println("C2: desligado.");
    if(msg.equals("1")) Serial.println("C2: ligado.");
    if(msg.equals("2")) Serial.println("C2: no sensor.");
  }
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    String msg;
  
    /* obtem a string do payload recebido */
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }

    Serial.println(topic);
    Serial.print("Chegou a seguinte string via MQTT: ");
    Serial.println(msg);
    
    altera_lampada(msg, topic);
}

void reconnectMQTT(void) 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            MQTT.subscribe(TOPICO1); 
            MQTT.subscribe(TOPICO2); 
            MQTT.subscribe(TOPICO3); 
            MQTT.subscribe(TOPICO4); 

            Serial.println("Conectado com sucesso ao broker MQTT!"); 
        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}

void VerificaConexoesWiFIEMQTT(void)
{
    if (!MQTT.connected()) 
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
     reconnectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}

void reconnectWiFi(void) 
{
    //se já está conectado a rede WI-FI, nada é feito. 
    //Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
          
    WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI
      
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}

void seta_I(int stat){
	status_I = stat; 
}

void seta_C1(int stat){
	status_C1 = stat; 
}
void seta_M(int stat){
	status_M = stat; 
}
void seta_C2(int stat){
	status_C2 = stat; 
}

void att_pinos(){
	if(status_I == 0) digitalWrite(pinoI, LOW);
	if(status_C1 == 0) digitalWrite(pinoC1, LOW);
	if(status_M == 0) digitalWrite(pinoM, LOW);
	if(status_C2 == 0) digitalWrite(pinoC2, LOW);

	if(status_I == 1) digitalWrite(pinoI, HIGH);
	if(status_C1 == 1) digitalWrite(pinoC1, HIGH);
	if(status_M == 1) digitalWrite(pinoM, HIGH);
	if(status_C2 == 1) digitalWrite(pinoC2, HIGH);

	if(acionamento == LOW){
		if(status_I == 2) digitalWrite(pinoI, LOW);
		if(status_C1 == 2) digitalWrite(pinoC1, LOW);
		if(status_M == 2) digitalWrite(pinoM, LOW);
		if(status_C2 == 2) digitalWrite(pinoC2, LOW);
	}

	if(acionamento == HIGH){
		if(status_I == 2) digitalWrite(pinoI, HIGH);
		if(status_C1 == 2) digitalWrite(pinoC1, HIGH);
		if(status_M == 2) digitalWrite(pinoM, HIGH);
		if(status_C2 == 2) digitalWrite(pinoC2, HIGH);
	}
}


void lePIR(){
   acionamento = digitalRead(porta_rele1); //Le o valor do sensor PIR
    if (acionamento == LOW) //Sem movimento, mantem rele desligado
    {
      Serial.println("Parado.");
        MQTT.publish(TOPICO5, "0");
    }
    else //Caso seja detectado um movimento, aciona o rele
    {
      Serial.println("Movimento!");
        MQTT.publish(TOPICO5, "1");
    }
}

void setup()
{
	Serial.begin(115200);  

  	seta_I(1);
  	seta_C1(1);
  	seta_M(1);
  	seta_C2(1);


	/* Inicializa a conexao wi-fi */
    initWiFi();
 
    /* Inicializa a conexao ao broker MQTT */
    initMQTT();

	  pinMode(pinoI, OUTPUT); 
  	pinMode(pinoC1, OUTPUT); 
  	pinMode(pinoM, OUTPUT); 
  	pinMode(pinoC2, OUTPUT); 

  	pinMode(porta_rele1, INPUT); //Define pino sensor como entrada

  	att_pinos();
}
 

void loop()
{
    VerificaConexoesWiFIEMQTT();
   
    MQTT.loop();

    lePIR();
    att_pinos();
    delay(1000); 
 }
