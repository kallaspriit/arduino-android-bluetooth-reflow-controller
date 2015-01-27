#define COMM Serial
#define BTN_PIN A0

void setup() {
  COMM.begin(9600);
  
  pinMode(BTN_PIN, INPUT);
}

void loop() {
  while (COMM.available()) {
    byte byteRead = COMM.read();
    
    COMM.write(byteRead);
  }
  
  int btnState1 = digitalRead(BTN_PIN);
  int btnState2 = analogRead(BTN_PIN);
  
  if (btnState2 == 1023) {
    COMM.print("PRESSED: ");
  } else {
    COMM.print("RELEASED: ");
  }
  
  COMM.print(btnState1);
  COMM.print("; ");
  COMM.println(btnState2);
  
  delay(500);
}
