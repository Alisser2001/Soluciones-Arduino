//Ejemplo FSM y Teclado matricial
//Se desea diseñar un sistema de seguridad que permita 
//el acceso a una zona restringida. 
//El acceso solo se permite al personal que digite una 
//clave correcta de 3 dígitos (1-2-7) ingresados de manera 
//secuencial por un teclado de 10 botones (0 a 9).

//Biblioteca para el manejo del teclado
#include <Keypad.h>

//Define es un macro de programación que 
//permite hacer remplazos directos
#define ON  HIGH 
#define OFF LOW

//FSM
enum fsm_estados {
  s0, 
  s1, 
  s2 
};
enum fsm_estados estado;

//Parametros
const int led = 13;	// the number of the LED pin

//es posible usar los puertos análogos como digitales
const int d0=A0;	// Salida binaria
const int d1=A1;	// 
const int d2=A2;	// 
const int d3=A3;	// 

const byte numRows= 4;	//number of rows on the keypad
const byte numCols= 4;	//number of columns on the keypad

//Matriz para identificar el teclado
char keymap[numRows][numCols]= 
{
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

//Puertos de conexión para el teclado
byte rowPins[numRows] = {9,8,7,6}; //Filas 0 to 3
byte colPins[numCols]= {5,4,3,2}; //Columnas 0 to 3

//Función para inicializar el teclado
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup()
{
    pinMode(led, OUTPUT);
 
    pinMode(d0, OUTPUT);
  	pinMode(d1, OUTPUT);
  	pinMode(d2, OUTPUT);
  	pinMode(d3, OUTPUT);

    estado = s0;

}

//Funcion para generar un código binario
//Es necesario completar la función para todos los digitos
void int2binary(char digito)
{
  switch (digito)
  {
    case '1':  digitalWrite(d0, ON); break;
    case '2':  digitalWrite(d1, ON); break;
    case '7':  digitalWrite(d2, ON); 
  			   digitalWrite(d1, ON); 
  			   digitalWrite(d0, ON); break;
  }
  delay(500);
  digitalWrite(d3, OFF);
  digitalWrite(d2, OFF); 
  digitalWrite(d1, OFF); 
  digitalWrite(d0, OFF); 
}
//'keypressed' almanecena el digito presionado
//'NO_KEY' indica cuando una tecla es activada 
void loop()
{
  
   char keypressed = myKeypad.getKey();
   if (keypressed != NO_KEY) 
   {
      int2binary(keypressed);
      switch (estado){
      case s0: 
            if(keypressed=='1')
              	estado=s1;
            else
                estado=s0;
            break;
      case s1: 
            if(keypressed=='2')
                estado=s2;
            else
                estado=s0;
            break;
      case s2: 
            if(keypressed=='7')
            {
                digitalWrite(led, ON); 
                delay(1000);
                digitalWrite(led, OFF); 
                estado=s0;
            }
            else
                estado=s0;
            break;
    }
   }
  delay(500);
}
