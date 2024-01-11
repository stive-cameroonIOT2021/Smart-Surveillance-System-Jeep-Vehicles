/* Defining motor pins */

struct motor{
int IN1;
int IN2;
int ENA;
  };

int ledVal = 20;  // setting bright of flash LED 0-255

const int ledPin = 9;  // set digital pin LED

const int Speed_mov = 190;
const int Speed_turn = 250;
//extern int servoPin = 2;  // set digital pin GPIO2 as servo pin (use SG90)

motor bck, fwrd;

void setup() {
  Serial.begin(115200); // Make sure the baud rate matches the one used in ESP32-CAM code
bck.IN1 = 5;
bck.IN2 = 4;
bck.ENA = 3;

fwrd.IN1 = 8;
fwrd.IN2 = 7;
fwrd.ENA = 6;

  // Set all the motor control pin to Output
  pinMode(bck.IN1, OUTPUT);
  pinMode(bck.IN2, OUTPUT);
  pinMode(bck.ENA, OUTPUT);

  pinMode(fwrd.IN1, OUTPUT);
  pinMode(fwrd.IN2, OUTPUT);
  pinMode(fwrd.ENA, OUTPUT);
  
  pinMode(ledPin, OUTPUT); // set the LED pin as an Output    
  digitalWrite(ledPin, LOW);

}

void loop() {
  while (Serial.available() > 0) {
    char cmd[20];  // Assuming a reasonable maximum length for your commands
    Serial.readBytesUntil('\n', cmd, sizeof(cmd));

    // Ignore non-printable characters
    for (int i = 0; i < strlen(cmd); i++) {
      if (!isPrintable(cmd[i])) {
        cmd[i] = '\0';  // Replace non-printable characters with null terminator
      }
    }

    if (strlen(cmd) > 0) {
    // Process the incoming byte as needed
    // For example, you can print it to the Serial Monitor
    Serial.println(cmd);

    if(!strcmp(cmd, "Forward")) {
      digitalWrite(bck.IN1, HIGH);
      digitalWrite(bck.IN2, LOW);
      
      analogWrite(bck.ENA,Speed_mov);
    }

     else if(!strcmp(cmd, "Stop")) {
      digitalWrite(bck.IN1, LOW);
      digitalWrite(bck.IN2, LOW);

      digitalWrite(fwrd.IN1, LOW);
      digitalWrite(fwrd.IN2, LOW);
      
      analogWrite(fwrd.ENA,0);
      analogWrite(bck.ENA,0);
    }

    else if(!strcmp(cmd, "Backward")) {
      digitalWrite(bck.IN1, LOW);
      digitalWrite(bck.IN2, HIGH);
      
      analogWrite(fwrd.ENA,0);
      analogWrite(bck.ENA,Speed_mov);
    }

 else if(!strcmp(cmd, "Turn Right")) {
      digitalWrite(fwrd.IN1, HIGH);
      digitalWrite(fwrd.IN2, LOW);
      
      analogWrite(fwrd.ENA,Speed_turn);
      analogWrite(bck.ENA,0);
    }

    else if(!strcmp(cmd, "Turn Left")) {
      digitalWrite(fwrd.IN1, LOW);
      digitalWrite(fwrd.IN2, HIGH);
      
      analogWrite(fwrd.ENA,Speed_turn);
      analogWrite(bck.ENA,0);
    }

     else if(!strcmp(cmd, "Forward Left")) {
      digitalWrite(fwrd.IN1, LOW);
      digitalWrite(fwrd.IN2, HIGH);

      digitalWrite(bck.IN1, HIGH);
      digitalWrite(bck.IN2, LOW);
      
      analogWrite(bck.ENA,Speed_mov);
      analogWrite(fwrd.ENA,Speed_turn);
      
    }


     else if(!strcmp(cmd, "Backward Left")) {
      digitalWrite(fwrd.IN1, LOW);
      digitalWrite(fwrd.IN2, HIGH);

       digitalWrite(bck.IN1, LOW);
      digitalWrite(bck.IN2, HIGH);
      
      analogWrite(bck.ENA,Speed_mov);      
      analogWrite(fwrd.ENA,Speed_turn);
    }


    else if(!strcmp(cmd, "Forward Right")) {
      digitalWrite(fwrd.IN1, HIGH);
      digitalWrite(fwrd.IN2, LOW);

      digitalWrite(bck.IN1, HIGH);
      digitalWrite(bck.IN2, LOW);
      
      analogWrite(bck.ENA,Speed_mov);
      analogWrite(fwrd.ENA,Speed_turn);
    }

    else if(!strcmp(cmd, "Backward Right")) {
      digitalWrite(fwrd.IN1, HIGH);
      digitalWrite(fwrd.IN2, LOW);

       digitalWrite(bck.IN1, LOW);
      digitalWrite(bck.IN2, HIGH);
      
      analogWrite(bck.ENA,Speed_mov);      
      analogWrite(fwrd.ENA,Speed_turn);
    }

    else if(!strcmp(cmd, "LED On")) {
      //digitalWrite(ledPin, HIGH);
      digitalWrite(ledPin, HIGH);
    }
    
    else if(!strcmp(cmd, "LED Off")) {
      //digitalWrite(ledPin, LOW);
      digitalWrite(ledPin, LOW);
    }

  }

  }
  // Add any other processing logic here
}
