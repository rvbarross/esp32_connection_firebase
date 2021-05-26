#include <WiFi.h>
#include <FirebaseESP32.h>
 
 
#define FIREBASE_HOST "https://esp32andandroid-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "GJuW80c7q0n5Os7XGagfRx4ldxBkwyKL9l6MORvX"
//Colocar noem da rede e senha.
#define WIFI_SSID "nome_da_rede"
#define WIFI_PASSWORD "senha_da_rede"

//Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json;

const int vazao = 27;

void setup() {
  // put your setup code here, to run once:
  pinMode(vazao, INPUT);
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
  // put your main code here, to run repeatedly:
  int vazaoValue = digitalRead(vazao);
  Serial.println(vazaoValue);
  json.set("/vazao", vazaoValue);
  Firebase.updateNode(firebaseData,"/Sensor",json);
}
