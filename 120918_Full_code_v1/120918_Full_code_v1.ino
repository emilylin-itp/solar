//SUN control
const int transistorPin = 7;    // connected to the base of the transistor
const int leftBar = A4; // connected to the left switch for SUN
const int rightBar = A5; // connected to the right switch for SUN

//button pins
const int button1 = 13;
const int button2 = 12;
const int button3 = 11;
const int button4 = 10;

//house & solar cell pins
const int houseLED1 = 3;
const int houseLED2 = 5;
const int scPin1 = A0;
const int scPin2 = A1;

const int numReadings = 10;
int solareadings[numReadings];      // the readings from the analog input from solar cells
int readIndex = 0;              // the index of the current reading
int solarTotal = 0;                  // the running total
int solarSmooth = 0;                // the average smoothed by 10 measurements
//


void setup() {
  //configure the digital input:
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);
  pinMode(transistorPin, OUTPUT);
  pinMode(houseLED1,OUTPUT);
  pinMode(houseLED2,OUTPUT);
  
  Serial.begin(9600);

  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    solareadings[thisReading] = 0;
  }
}

void loop() {

// Reading the buttons in order to change the page on iPad    
      // read b1 
      int button1Value = digitalRead(button1);
      Serial.print(button1Value);
      Serial.print(",");
      
      // read b2
      int button2Value = digitalRead(button2);
      Serial.print(button2Value);
      Serial.print(",");
    
      // read b3
      int button3Value = digitalRead(button3);
      Serial.print(button3Value);
      Serial.print(",");
      
      // read b4
      int button4Value = digitalRead(button4);
      Serial.print(button4Value);
      Serial.print(",");


 //SUN control with the left and right switches
        int leftSW = analogRead(leftBar);
        int rightSW = analogRead(rightBar);
          
        
        if (leftSW > 600){
          digitalWrite(transistorPin, LOW);
          //switch to starting page
        } else if (rightSW > 600){
          digitalWrite(transistorPin, LOW);
          //switch to battery page
        } else{
          digitalWrite(transistorPin, HIGH);
        }
          
        //  read the right switch location
        Serial.print(rightSW);
        Serial.print(",");
        
        //  read the left switch location
        Serial.print(leftSW);
        Serial.print(",");

///house LED and solar panel control
       int solar1 = analogRead(scPin1);
       int solar2 = analogRead(scPin2);
       int solarAvg = (solar1 + solar2)/2; // We need to find a way to average the data!!!

//////////solar measurement averaging over 10 readings///////////////
        solarTotal = solarTotal - solareadings[readIndex];
        solareadings[readIndex] = solarAvg;
        solarTotal = solarTotal + solareadings[readIndex];
        readIndex = readIndex + 1;
        if (readIndex >= numReadings) {
          // ...wrap around to the beginning:
          readIndex = 0;
        }
        // calculate the average:
        solarSmooth = solarTotal / numReadings;

       int mapSolar = map (solarSmooth, 650, 1023, 1, 255);   

       Serial.print(mapSolar); // binary sent to the computer
       Serial.println(",");
       delay(1);
//////////////////////////////////////////////////////////////////////

//       int mapSolar = map (solarAvg, 650, 1023, 1, 255);  
//       Serial.print(mapSolar); // ASCII sent to the computer
//       Serial.println(",");


    // Here you can add more lines for each condition in order to light up more LEDs in the house.
        if (solarAvg < 650) {
        analogWrite(houseLED1, 0);
         analogWrite(houseLED2, 0);}
        
        if (solarSmooth >= 600 && solarSmooth < 780) {
        analogWrite(houseLED1, 50);
        analogWrite(houseLED2, 50);}
    
        if (solarSmooth >= 780 && solarSmooth < 900) {
        analogWrite(houseLED1, 80);
        analogWrite(houseLED2, 80);}
    
        if (solarSmooth >= 920 && solarSmooth < 980) {
        analogWrite(houseLED1, 100);
        analogWrite(houseLED2, 100);}
    
        if (solarSmooth >= 980) {
        analogWrite(houseLED1, 254);
        analogWrite(houseLED2, 254);}
}
