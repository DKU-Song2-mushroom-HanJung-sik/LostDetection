int led = 13;                       
int r = A0;  int l = A1;                       
                          

void setup() 
{
  pinMode(led, OUTPUT);                 
  Serial.begin(9600);         
}

void loop() 
{
  int rv = analogRead(r);
  int lv = analogRead(l);
       
  Serial.println("r = " + String(rv) + " l = " + String(lv));                       

  if (rv > 10 or lv >10){
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
  delay(1000);                                
}
