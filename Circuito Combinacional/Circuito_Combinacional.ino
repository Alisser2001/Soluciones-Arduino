// C++ code
//

int a = 0;
int b = 0;
int c = 0;
int segundosDer=0;
int segundosIzq=0;

int izquierda;
int derecha;

int sensorA = 13; // ENTRADA DEL SENSOR A
int sensorB = 12; // ENTRADA DEL SENSOR B 
int sensorC = 11; //ENTRADA DEL SENSOR C

int salidaDer = 7; //Salida de el primer valor
int salidaIzq = 6; //Salida de el segundo valor

bool flag=true;

//Interrupción
ISR(TIMER1_COMPA_vect)
{  
  flag=false; 
  segundosDer=segundosDer+1;
  segundosIzq=segundosIzq+1;
  if (segundosIzq==13)
	{
    	segundosIzq=0;
  	}
  if(segundosDer==12)
  {
  	segundosDer=0;
  }
}

void setup()
{
  pinMode(sensorA, INPUT);
  pinMode(sensorB, INPUT);
  pinMode(sensorC, INPUT);
  
  pinMode(salidaIzq, OUTPUT);
  pinMode(salidaDer, OUTPUT);
  
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
  	//********
}

void loop()
{
  a = digitalRead(sensorA);
  b = digitalRead(sensorB);
  c = digitalRead(sensorC);
  
  izquierda = (!a & c) | (a & !b & !c);
  derecha = (b & !c) | (a & c);
  
  digitalWrite(salidaDer, derecha);
  digitalWrite(salidaIzq, izquierda);
  
  flag=true;
  while(flag)
  {
    if(segundosIzq<3 && derecha)
    {
    	digitalWrite(salidaDer, derecha);
    }
    else{
    	digitalWrite(salidaDer, 0);
    }
    if(segundosIzq<3 && izquierda)
    {
    	digitalWrite(salidaIzq, izquierda);
    }
    else{
    	digitalWrite(salidaIzq, 0);
    }
  }
}