#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "HDPS";
const char* password = "Salomao112290";
String serverAddress = "https://database1.salomaobraga.repl.co";  // Substitua pelo endereço do seu servidor
int httpsCode;
String url;
String payload;
String dado;
char tipo; //ml[m] , ml/min[n] ou rpm[r]

void setup() {
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    //Serial.print("Conectando ao WiFi...");

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        //Serial.print(".");
    }
    //Serial.println();

    //Serial.println("Conectado ao WiFi");
}

void loop() {
  // Faça a solicitação HTTP para obter a última informação
  std :: unique_ptr < BearSSL :: WiFiClientSecure > client ( new  BearSSL :: WiFiClientSecure );
  client -> setInsecure (); // HTTPS
  HTTPClient https;

  url = serverAddress + "/obter-ultima-informacao";
  https.begin(* client, url);

  httpsCode = https.GET();
  if (httpsCode > 0) {
      if (httpsCode == HTTP_CODE_OK) {
          payload = https.getString();
          tipo = payload[2];
          for(int i = 2; i<100; ++i){
            if(payload[i] == '"'){
              break;
            }
            dado += payload[i];
          }
          if(payload[2] != '0'){
            //Serial.println(tipo);
            Serial.println(dado);
            dado = "";
          }else{
            dado = "";
          }
          url = serverAddress + "/receber-dados?nome=0";
          https.begin(* client, url);
          httpsCode = https.GET();
      } else {
          //Serial.println("Falha na solicitação HTTP");
      }
  } else {
      //Serial.println("Falha na conexão ao servidor");
  }

  https.end();

  // Aguarde por algum tempo antes de fazer a próxima solicitação
  delay(500);  // Aguarde 500 milisegundos antes da próxima solicitação
}
