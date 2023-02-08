
#include <ThingSpeak.h>

 #include <DHT.h>
#include <DHT_U.h>

#include <ESP8266WiFi.h>

const char* server = "api.thingspeak.com";

const char* ssid =  "STAMPEDE-NET";     // Enter your WiFi Name

const char* pass =  "Vidy0daya"; // Enter your WiFi Password

#define DHTPIN D3          // GPIO Pin where the dht11 is connected

DHT dht(DHTPIN, DHT11);

WiFiClient client;


const int moisturePin = A0;             // moisteure sensor pin

const int motorPin = D0;
const int lightPin = D1;
const int ledR = D4;
const int ledW = D8;


unsigned long interval = 10000;

unsigned long previousMillis = 0;

unsigned long interval1 = 1500;

unsigned long previousMillis1 = 0;

float waterstat;              //moisture reading

float h;                  // humidity reading

float t;                  //temperature reading

//height variables

const int trig = D5;
const int echo = D6;

int Height;
int total_Height =20;

//read variables


//channel initialisation
//ch1 temp
unsigned long ChNo1 = 1937136; //your public channel number
const char * WKey1 = "52YOXX2GUJFDK3LY";

//ch2 humidity

unsigned long ChNo2 = 2024863; //your public channel number
const char * WKey2 = "UM2LIQ4CXULYHE2V";

//ch3 height
unsigned long ChNo3 = 2024881; //your public channel number
const char * WKey3 = "DLBYTPLUVFWRWQ39";


//ch4 motor, light, water status
unsigned long ChNo4 = 1937136; //your public channel number
const char * WKey4 = "7E0V2W4PIHI3D3CR";

//unsigned int FieldToMonitor = 3; //the field you want to monitor
const char * myReadAPIKey = "IYE4KXWPFFAWROZL"; //read API



int motorState = 0;
int lightState = 0;


void setup()

{

  Serial.begin(115200);

  delay(10);

  pinMode(motorPin, OUTPUT);

  digitalWrite(motorPin, HIGH); // keep motor off initally

  dht.begin();

  Serial.println("Connecting to ");

  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)

  {

    delay(500);
    Serial.print(".");              // print ... till not connected

  }

  Serial.println("");

  Serial.println("WiFi connected");
 
  ThingSpeak.begin(client);  // Initialize ThingSpeak  
}


void loop()
{
  unsigned long currentMillis = millis(); // grab current time


    
    //waterstat = ( 100.00 - ( (analogRead(moisturePin) / 1023.00) * 100.00 ) );    //% of moisture not needed
   
    

 

  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
 
    if ((unsigned long)(currentMillis - previousMillis1) >= 4000) {
     
     digitalWrite(ledW,HIGH);
					digitalWrite(ledR,HIGH);
     delay(800);
     digitalWrite(ledW,LOW);
					digitalWrite(ledR,HIGH);
     delay(800);
     digitalWrite(ledW,HIGH);
					digitalWrite(ledR,LOW);
					delay(800);
					digitalWrite(ledW,LOW);
					digitalWrite(ledR,LOW);
					
     h = dht.readHumidity();     // read humiduty

     t = dht.readTemperature();     // read temperature 
     if(digitalRead(moisturePin) == HIGH){
       waterstat = HIGH;
     }
    else{
      waterstat = LOW;

    }
 
    
    ReadHeight();
     

      if ( waterstat == HIGH) {
 
      digitalWrite(motorPin, LOW);         // tun on motor
      delay(10000);
      digitalWrite(motorPin, HIGH);
      }
     
     Serial.print("waterstat :");
     Serial.print(waterstat);
  
  previousMillis1 = millis();
  }
 
  if (waterstat == LOW) {
 
    digitalWrite(motorPin, HIGH);          // turn off mottor
 
  }

  if ((unsigned long)(currentMillis - previousMillis1) >= 19000) {
    sendThingspeak();                                                       //send data to thing speak
    delay(1000);
    readThingspeak();
    previousMillis = millis();
  }
 
  if ((unsigned long)(currentMillis - previousMillis) >= interval) {
			  
			
  previousMillis = millis();
  client.stop();
  }
					digitalWrite(ledW,HIGH);
					digitalWrite(ledR,HIGH);
					delay(500);
					digitalWrite(ledW,LOW);
					digitalWrite(ledR,LOW);
 
}

void sendThingspeak() {


  digitalWrite(ledW,LOW);
  delay(100);
  digitalWrite(ledW,HIGH);
  delay(800);
  digitalWrite(ledW,LOW);
  delay(800);
  digitalWrite(ledW,HIGH);  
  digitalWrite(ledW,LOW);
  delay(700);
  digitalWrite(ledW,HIGH);

  if (client.connect(server, 80))
  {
    //GET https://api.thingspeak.com/update?api_key=52YOXX2GUJFDK3LY$field1=0
   
    Serial.print("Moisture Percentage: ");
    Serial.print(waterstat);
    Serial.print("%. Temperature: ");
    Serial.print(t);
    Serial.print(" C, Humidity: ");
    Serial.print(h);
    Serial.println("%. Sent to Thingspeak.");

    // Write to ThingSpeak. There are up to 8 fields in a channel, alLOWing you to store up to 8 different
    // pieces of information in a channel.  Here, we write to field 1.

   //1937136
  int x = ThingSpeak.writeField(ChNo1, 1, t, WKey1); //temp update
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(5000);


  //2024881 , DLBYTPLUVFWRWQ39
  int x2 = ThingSpeak.writeField( ChNo2, 1, h, WKey2); //humidity update
  if(x2 == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(5000);

 
  int x3 = ThingSpeak.writeField(ChNo3, 3, Height , WKey3);
  if(x3 == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(5000);

 
  int x4 = ThingSpeak.writeField(ChNo4, 1,motorState ,WKey4);
  if(x4 == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(5000);
 
   //2024863 , UM2LIQ4CXULYHE2V
  int x1 = ThingSpeak.writeField(ChNo4, 1, waterstat, WKey4); //moisture percentage
  if(x1 == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
   
  }

  digitalWrite(ledW,LOW);  
}


void readThingspeak() { 
  digitalWrite(ledR,LOW);
  delay(100);
  digitalWrite(ledR,HIGH);
  delay(800);
  digitalWrite(ledR,LOW);
  delay(800);
  digitalWrite(ledR,HIGH);
   
  
  
  motorState = ThingSpeak.readLongField(ChNo1, 2, myReadAPIKey);
  // Check the status of the read operation to see if it was successful
  int statusCode = 0;
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    Serial.println("Read: " + String(motorState));
  }
  else
  {
    Serial.println(F("Problem reading channel"));
  }
  delay(15000); // No need to read the counter too often.

  
//   lightState = ThingSpeak.readLongField(ChNo1, 3, myReadAPIKey);
//   // Check the status of the read operation to see if it was successful
//   int statusCode2 = 0;
//   statusCode2 = ThingSpeak.getLastReadStatus();
//   if (statusCode2 == 200)
//   {
//     Serial.println("Read: " + String(lightState));
//   }
//   else
//   {
//     Serial.println(F("Problem reading channel"));
//   }
  
 
 
  digitalWrite(ledR,LOW);
}


void ReadHeight() {
   
  float duration;
  int distance;
 
  digitalWrite(trig, LOW);        
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);          
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration/2) / 29.1;
  Height =  total_Height - distance;
  Serial.println("Distance: ");
  Serial.print(distance);
  Serial.println("Height: ");
  Serial.print(Height);
  delay(4000);
}
