//Libreria para el manejo de la matriz
#include <Adafruit_NeoPixel.h>
#define size 20

//Parámetros de configuración
const byte oledPin_1 = 2; 
const byte oledPin_2 = 3; 
Adafruit_NeoPixel neopixel1 = Adafruit_NeoPixel(size, oledPin_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel neopixel2 = Adafruit_NeoPixel(size, oledPin_2, NEO_GRB + NEO_KHZ800);

//Configuración para cada dibujo
int vector1[size]={	0,0,1,0,
                  	0,1,1,0,
                  	0,0,1,0,
                  	0,0,1,0,
                  	0,1,1,1
                 };
int vector2[size]={	1,1,1,0,
                	0,0,0,1,
                	0,1,1,0,
                	1,0,0,0,
                	0,1,1,1
                 };

int vector3[size]={	1,1,1,0,
                	0,0,0,1,
                	0,1,1,0,
                	0,0,0,1,
                	1,1,1,0
                 };

//Función para imprimir en la matriz
void printColor(int *vector, Adafruit_NeoPixel *neopixel);

//FSM
enum fsm_estados {s0,s1,s2};
enum fsm_estados estado;

void setup() 
{
  neopixel1.begin();  
  neopixel2.begin();  
  estado=s0;
}

void loop() 
{
  if (estado==s0)
  {
    printColor(&vector1[0],&neopixel1);
  	printColor(&vector2[0],&neopixel2);
    estado=s1;
  }
  else if (estado==s1)
  {
    printColor(&vector2[0],&neopixel1);
  	printColor(&vector3[0],&neopixel2);
    estado=s2;
  }
  else
  {
    printColor(&vector3[0],&neopixel1);
  	printColor(&vector1[0],&neopixel2);
    estado=s0;
  	
  }
  delay(2000);    
  neopixel1.clear(); 
  neopixel2.clear();
  //delay(2000);   
}

//Función para imprimir en la matriz
void printColor(int *vector, Adafruit_NeoPixel *neopixel)
{
  for(int i=0;i<size;i++)
  { 
    if(vector[i])
    	neopixel->setPixelColor(i,190,0,40);//RGB
  }
  neopixel->show();
}