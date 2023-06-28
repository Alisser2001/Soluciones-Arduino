//Uso del módulo TIMER
// Ejemplo: Semáforo
#include <avr/sleep.h>

//Parametros
int led_verde = 13;
int led_amarillo = 12;
int led_rojo = 11;
int segundos=0;

int led_aux = 8; //usado para pruebas
bool flag=true;

//Interrupción
//Cambiar TIMER_1 a TIMER_0 o 	TIMER_2 
//para usar los otros timer
ISR(TIMER1_COMPA_vect)
{  
  flag=false; 
  segundos=segundos+1;
  if (segundos==6)
  {
    segundos=0;
  }
}

void setup() 
{
	//Configuración de puertos de salida OUTPUT.
	pinMode(led_verde, OUTPUT);
	pinMode(led_amarillo, OUTPUT);
	pinMode(led_rojo, OUTPUT);
  	pinMode(led_aux, OUTPUT);

	//**************************************  
	//Configuración de la interrupcion
  	//**************************************
	
	cli();//Detine todas las interrupciones
	
    TCCR1A = 0;//Registro de configuración A
	TCCR1B = 0;//Registro de configuración B
  
	TCNT1  = 0;//Registro que lleva la cuenta=0
	OCR1A = 15624;//Registro de comparación
   //15624=(16*10^6)/(1*1024) - 1
	
	TCCR1B |= (1 << WGM12); //COnfiguración del preescaler
	TCCR1B |= (1 << CS12) | (1 << CS10);  
	
 	TIMSK1 |= (1 << OCIE1A);//Habilita interrupciones por Timer
	sei();//Activa todas las interrupciones
  	//**************************************

}


void loop(){
  
  if (segundos<2)
  {
    digitalWrite(led_verde,LOW); 
    digitalWrite(led_rojo,HIGH); 
  }
  else if (segundos<3)
  {
    digitalWrite(led_rojo,LOW); 
	digitalWrite(led_amarillo,HIGH); 
  }
  else if (segundos<6)
  {
    digitalWrite(led_amarillo,LOW);
	digitalWrite(led_verde,HIGH); 
  }
  flag=true;
  //infiniteLoop(); 

  sleep_enable();
}

//Esta función demuestra el comportamiento de las
//interrupciones ¿Cuándo se sale de este loop?
void infiniteLoop()
{
  while(flag)
  {
    digitalWrite(led_aux,HIGH);
    delay(500);
    digitalWrite(led_aux,LOW);
    delay(500);
  }
}





