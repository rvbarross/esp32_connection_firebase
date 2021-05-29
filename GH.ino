#include <WiFi.h>
#include <FirebaseESP32.h>
 
 
#define FIREBASE_HOST "https://cps-gh-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "vlkS0EF5RGuf4CudV8hr68Ksc34LcL0HN5Qu87nj"
//Colocar noem da rede e senha.
#define WIFI_SSID "nome_da_rede"
#define WIFI_PASSWORD "senha_da_rede"

//Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json;

const int INTERRUPCAO_SENSOR = 27; //interrupt = 0 equivale ao pino digital 2
const int PINO_SENSOR = 27;

//definicao da variavel de contagem de voltas
unsigned long contador = 0;

//definicao do fator de calibracao para conversao do valor lido
const float FATOR_CALIBRACAO = 4.5;

//definicao das variaveis de fluxo e volume
float fluxo = 0;
float volume = 0;
float volume_total = 0;

//definicao da variavel de intervalo de tempo
unsigned long tempo_antes = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(PINO_SENSOR, INPUT);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  /*
  This option allows get and delete functions (PUT and DELETE HTTP requests) works for device connected behind the
  Firewall that allows only GET and POST requests.
  
  Firebase.enableClassicRequest(firebaseData, true);
  */

  //String path = "/vazao";
  

  Serial.println("------------------------------------");
  Serial.println("Connected...");
}

void loop() {
  if((millis() - tempo_antes) > 1000){

    //desabilita a interrupcao para realizar a conversao do valor de pulsos
    detachInterrupt(INTERRUPCAO_SENSOR);

    //conversao do valor de pulsos para L/min
    fluxo = ((1000.0 / (millis() - tempo_antes)) * contador) / FATOR_CALIBRACAO;

    //exibicao do valor de fluxo
    Serial.print("Fluxo de: ");
    Serial.print(fluxo);
    Serial.println(" L/min");

    //calculo do volume em L passado pelo sensor
    volume = fluxo / 60;

    //armazenamento do volume
    volume_total += volume;
    json.set("/vazao_value", volume_total);
    Firebase.updateNode(firebaseData,"/Sensor",json);

    //exibicao do valor de volume
    Serial.print("Volume: ");
    Serial.print(volume_total);
    Serial.println(" L");
    Serial.println();
   
    //reinicializacao do contador de pulsos
    contador = 0;

    //atualizacao da variavel tempo_antes
    tempo_antes = millis();

    //contagem de pulsos do sensor
    attachInterrupt(INTERRUPCAO_SENSOR, contador_pulso, FALLING);
    
  }
  // put your main code here, to run repeatedly:
  
  
}

void contador_pulso() {
  
  contador++;
  
}
