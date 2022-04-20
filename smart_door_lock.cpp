#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

int buzzer = 8 ;
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

// constants won't change
const int RELAY_PIN = A5;  // the Arduino pin, which connects to the IN pin of relay
int lock_state = 0;
int buzzer_mode;

void buzzer_fun()
{
    digitalWrite (buzzer, LOW) ; //send tone
    delay (500) ;
    //  remove below code for continuous tone
    digitalWrite (buzzer, HIGH) ; //no tone
    delay (500) ;  
}

void setup()
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  pinMode(RELAY_PIN, OUTPUT);
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode (buzzer, OUTPUT) ;//buzzer code
  digitalWrite(buzzer, HIGH);
  
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
}

void buzzer_fast()
{
    digitalWrite (buzzer, LOW) ; //send tone
    delay (200) ;
    //  remove below code for continuous tone
    digitalWrite (buzzer, HIGH) ; //no tone
    delay (200) ;
    digitalWrite (buzzer, LOW) ; //send tone
    delay (200) ;
    //  remove below code for continuous tone
    digitalWrite (buzzer, HIGH) ; //no tone
    delay (200) ;
}

void loop()
{
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if(distance<20)
  {
    if (buzzer_mode == 0)
      buzzer_fun();
    if (buzzer_mode == 1)
      buzzer_fast();
  }
  delay(500);
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "77 11 EE F4") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    buzzer_fast();
    buzzer_mode = 2;
    Serial.println();
    if (lock_state == 0)
    {
      digitalWrite(RELAY_PIN, LOW);
      lock_state = 1;
      return;
    }
    if (lock_state == 1)
    {
      digitalWrite(RELAY_PIN, HIGH);
      lock_state = 0;
      return;
    }
  }
  else   
  {
    Serial.println(" Access denied");
    buzzer_mode = 1;
    delay(2000);
  }
}