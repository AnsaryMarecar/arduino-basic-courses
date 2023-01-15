#include <SPI.h>
#include <Ethernet.h>

// L'adresse MAC du shield à voir au dos de votre matériel
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x12, 0x92 };  
// L'adresse IP que prendra le shield
IPAddress ip(192,168,1,30); //



IPAddress dns(192,168,1,254);
IPAddress gateway(192,168,1,254);
IPAddress subnet(255, 255, 255, 0);

//EthernetServer server(80);    //Initialise le serveur Ethernet, port 80 par défaut pour HTTP

float temperature_celcius = 0.0;

EthernetClient client;

EthernetServer server_port(80);
IPAddress server_ip(192,168,1,128);//{127, 100, 100, 100};//{192,168,1,128};
int test_connect = -10;

void setup(){
  // On démarre la voie série pour déboguer
  Serial.begin(9600);

  char erreur = 0;
  // On démarre le shield Ethernet SANS adresse IP (donc donnée via DHCP)
  erreur = Ethernet.begin(mac); //(mac,ip,dns,gateway,subnet);
  if (erreur == 0) {
    Serial.println("Parametrage avec ip fixe...");
    // si une erreur a eu lieu cela signifie que l'attribution DHCP
    // ne fonctionne pas. On initialise donc en forçant une IP
    Ethernet.begin(mac, ip);
  }
  Serial.println("Initialisation ...");
  // Donne une seconde au shield pour s'initialiser
  delay(1000);
  Serial.println("Etude du matériel réseau ");
  
  // print out Arduino's IP address, subnet mask, gateway's IP address, and DNS server's IP address
  Serial.print("- Arduino's IP address   : ");
  Serial.println(Ethernet.localIP());

  Serial.print("- Gateway's IP address   : ");
  Serial.println(Ethernet.gatewayIP()); 
  
  Serial.print("- Link Status   : ");
  Serial.println(Ethernet.linkStatus());

  Serial.print("- Network's subnet mask  : ");
  Serial.println(Ethernet.subnetMask());

  Serial.print("- DNS server's IP address: ");
  Serial.println(Ethernet.dnsServerIP());
  delay(1000);
  Serial.println("connecting...");
}

void loop(){

  // Read pin A0
  int value_brut = analogRead(A0);
  
  // Transform value
  temperature_celcius = value_brut * (5.0 / 1023.0 * 100.0);
  int test_connect = client.connect("http://mytemperatureroom.000webhostapp.com",80);
  if (test_connect) {
    Serial.println("connected");
    client.print("GET "); //client.print("/web-basic-courses/");
    client.print("/add_temperature.php?");
    client.print("temperature_value=");
    client.print(temperature_celcius);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println("http://mytemperatureroom.000webhostapp.com");
    client.println( "Content-Type: application/x-www-form-urlencoded" );
    Serial.println(client.read());
    client.println( "Connection: close" );
    client.println();
  }
  else{
    Serial.println("cannot connect test_connect:");
    Serial.println(test_connect);
    switch(test_connect) {
      case(-1):
        Serial.println("Time out");
        break;
      case(-2):
        Serial.println("Serveur invalide");
        break;
      case(-3):
        Serial.println("Tronque");
        break;
      case(-4):
        Serial.println("Reponse invalide");
        break;
    }
  }
  // Seep 250ms
  delay(25000);
  client.stop();
  Serial.println(temperature_celcius);
}