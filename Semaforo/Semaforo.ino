//Introducción a Arduino
int led_rojo = 11;
int led_verde = 13;
int led_amarillo = 12;
int tiempo_rojo = 0;
int tiempo_verde = 0;
int tiempo_amarillo = 0;

void setup()
{
  pinMode(led_rojo, OUTPUT);
  pinMode(led_verde, OUTPUT);
  pinMode(led_amarillo, OUTPUT);
  tiempo_rojo=3000;
  tiempo_verde=5000;
  tiempo_amarillo=1000;
}

void loop()
{
  digitalWrite(led_rojo, HIGH);
  delay(tiempo_rojo);
  digitalWrite(led_rojo, LOW);
  digitalWrite(led_amarillo, HIGH);
  delay(tiempo_amarillo);
  digitalWrite(led_amarillo, LOW);
  digitalWrite(led_verde, HIGH);
  delay(tiempo_verde);
  digitalWrite(led_verde, LOW);
}

/*Tarea 1. 
El cruce de una calle, cuenta con un semáforo que opera bajo 
las siguientes condiciones:
a) La luz roja permanece activa durante 3 segundos.
b) La luz amarilla se activa solo por 1 segundo.
c) La luz verde permanece activa durante 5 segundos.
*/
