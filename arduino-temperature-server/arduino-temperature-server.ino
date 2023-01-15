#include <SPI.h>
#include <Ethernet.h>

// L'adresse MAC du shield
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x12, 0x92 };  //90-A2-DA-0F-12-92
// L'adresse IP que prendra le shield
IPAddress ip(192,168,1,30); //

EthernetClient client;

EthernetServer server(80);    //Initialise le serveur Ethernet, port 80 par défaut pour HTTP

float temperature_celcius = 0.0;
String data;
String stringVal;

void setup()
{
  // On démarre la voie série pour déboguer
  Serial.begin(9600);

  char erreur = 0;
  // On démarre le shield Ethernet SANS adresse IP (donc donnée via DHCP)
  erreur = Ethernet.begin(mac);
  server.begin();    
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
  
}

void loop(){
  // Mesure la tension sur la broche A0
  int valeur_brute = analogRead(A0);
  
  // Transforme la mesure (nombre entier) en température via un produit en croix
  temperature_celcius = valeur_brute * (5.0 / 1023.0 * 100.0);
  
  // Ecouter les clients entrants
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      Serial.println("new client");
      Serial.print("IP adress: ");
      Serial.println(Ethernet.localIP());
      if (client.available()) {
        client.println();
        client.println("<!DOCTYPE html>");
        client.println("<html>");
        client.println("<head>");
        client.println("<META CHARSET='utf-8'>");
        client.println("<META HTTP-EQUIV='Content-Type'     CONTENT='text/html'>");
        client.println("<META HTTP-EQUIV='Content-Language' CONTENT='fr-FX'>");
        //client.println("<link rel=\"stylesheet\" href=\"../192.168.1.128/style_station_meteo.css\">");
        client.println("<title>Access to temperature sensor</title>");
        client.println("</head>");
        client.print("<body>");
        client.println();
        client.println("<META HTTP-EQUIV='Refresh' CONTENT='60'>");
        client.println("<header>");
        client.println("<h1>Weather channel  </h1>");
		    client.println("<nav class='menu'>");
			  client.println("<ul class='link-menu'>");
				client.println("<li><a href='../192.168.1.128/index.php'>Home</a></li>");
				client.println("<li><a href='../192.168.1.128/list_temperature.php'>Database </a></li>");
				client.println("<li><a href='../192.168.1.128/add_temperature.php'>Add</a></li>");
				client.println("<li><a href='#'>Sensor server</a></li>");
			  client.println("</ul>");
		    client.println("</nav>");
	      client.println("</header>");
        client.println("<main style='background-color:green;color:white;padding:20px;'>"); 
        client.println("<p style=''>");
        client.print("Classroom temperature:  </br>");
        client.print(temperature_celcius);  
        client.print("&degC");
        client.println("</p>");
        client.println("</main>"); 
        client.println("</body>");  
        client.println("</html>");
        break;
      }
    }
  }
  // Pause 250ms
  delay(25000);
  client.stop();
  Serial.println(temperature_celcius);
  
}