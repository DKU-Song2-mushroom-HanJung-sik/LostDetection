#define TRIG_r1 9 //TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO_r1 8 //ECHO 핀 설정 (초음파 받는 핀)
#define TRIG_r2 7
#define ECHO_r2 6

void setup() {
  Serial.begin(9600); 
  pinMode(TRIG_r1, OUTPUT);
  pinMode(ECHO_r1, INPUT);
  pinMode(TRIG_r2, OUTPUT);
  pinMode(ECHO_r2, INPUT);
}

void loop()
{
  long duration_r1, distance_r1;
  long duration_r2, distance_r2;

  
  digitalWrite(TRIG_r1, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_r1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_r1, LOW);
  
  duration_r1 = pulseIn (ECHO_r1, HIGH); 
  distance_r1 = duration_r1 * 17 / 1000;
  
  digitalWrite(TRIG_r2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_r2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_r2, LOW);
  
  duration_r2 = pulseIn (ECHO_r2, HIGH); 
  distance_r2 = duration_r2 * 17 / 1000;
  
  Serial.println(duration_r2 ); //초음파가 반사되어 돌아오는 시간을 보여줍니다.
  Serial.print("\nDIstance_r1 : ");
  Serial.print(distance_r1); //측정된 물체로부터 거리값(cm값)을 보여줍니다.
  Serial.println(" Cm");

  Serial.print("\nDIstance_r2 : ");
  Serial.print(distance_r2); //측정된 물체로부터 거리값(cm값)을 보여줍니다.
  Serial.println(" Cm");

  delay(1000); //1초마다 측정값을 보여줍니다.
}
