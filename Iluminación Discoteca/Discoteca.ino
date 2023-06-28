// FSM
//Se desea diseñar un sistema control de iluminación 
//para una discoteca que siga el ritmo de la música. 
//El comportamiento debe ser tal que por cada nota:

//•Alta (DO), la iluminación debe ser azul.
//•Baja (RE), la iluminación debe ser verde.
//•Aguda (MI), la iluminación debe ser roja.
//(Suponga que siempre llegue una nota por el puerto serial)


//enum permite definir valores no numéricos
enum fsm_estados {
  alta, 
  baja, 
  aguda 
};

enum fsm_estados estado;

//Parametros
int led_verde = 13;
int led_azul = 12;
int led_rojo = 11;
char nota;

void setup() {
  // se indica que cada pin es de salida OUTPUT.
  pinMode(led_verde, OUTPUT);
  pinMode(led_azul, OUTPUT);
  pinMode(led_rojo, OUTPUT);

  Serial.begin(9600);
  estado = alta;
  
}


void loop(){
  digitalWrite(led_azul, LOW);
  digitalWrite(led_rojo, LOW);
  digitalWrite(led_verde, LOW);
  nota = Serial.read();
  switch (estado){
    case alta:
    	digitalWrite(led_azul, HIGH);
    	if(nota=='R')
        	estado=baja;
       	else if(nota=='M')
        	estado=aguda;
        else
          	estado=alta;
        break;
    case baja:
    	digitalWrite(led_verde, HIGH);
    	if(nota=='D')
        	estado=alta;
       	else if(nota=='M')
        	estado=aguda;
        else
          	estado=baja;
        break;
    case aguda:
      	digitalWrite(led_rojo, HIGH);
    	if(nota=='D')
        	estado=alta;
       	else if(nota=='R')
        	estado=baja;
        else
          	estado=aguda;
        break;
  }
  delay(1000);
}





