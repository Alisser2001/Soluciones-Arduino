//Se desea diseñar un sistema que permita el control 
//automático de la iluminación de un parqueadero. 
//Este cuenta con una puerta que permite el ingreso o salida 
//del parqueadero. 
//La administración ha adquirido 4 sensores (A,B,C,D), 
//que permiten detectar la cercanía de un objeto 
//(1=cerca, 0=lejos).

int b1 = 2;
int b2 = 3;
int b3 = 4;
int b4 = 5;
int s1 =13;

int a = 0;
int b = 0;
int c = 0;
int d = 0;
int Y = 0;

void setup() 
{
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);
  pinMode(b4, INPUT);
  
  pinMode(s1, OUTPUT);
}
void loop()
{
  a=digitalRead(b1);
  b=digitalRead(b2);
  c=digitalRead(b3);
  d=digitalRead(b4);
  
  Y=(a&b)|(c&d);
  digitalWrite(s1,Y);
  delay(2000);
}
