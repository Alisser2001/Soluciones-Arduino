//Una fabrica de quesos cuenta con tres sensores que 
//permiten medir el largo, ancho y peso de cada queso. 
//El producto se rechaza si al menos dos de las medidas 
//son erradas (0). El producto pasa a revisión si al menos 
//uno de los sensores reporta un valor errado (0). 
//El producto sale para despacho si todos los sensores 
//reportan un buen estado (1).


int ledRojo = 4;     //PUERTO DE SALIDA
int ledAmarillo = 3;  //PUERTO DE SALIDA
int ledVerde = 2;     //PUERTO DE SALIDA

int sensorA = 8; // ENTRADA DEL SENSOR A
int sensorB = 9; // ENTRADA DEL SENSOR B 
int sensorC = 10; //ENTRADA DEL SENSOR C

// (sensorA and sensor B) or sensorC

int x=0; //PARA ALMACENAR EL VALOR DEL SENSOR A
int y=0; //PARA ALMACENAR EL VALOR DEL SENSOR B
int z=0; //PARA ALMACENAR EL VALOR DEL SENSOR C

int verde;
int Rojo;
int amarillo;

void setup()
{
  pinMode(ledRojo, OUTPUT);     //SALIDA
  pinMode(ledAmarillo, OUTPUT);   //SALIDA
  pinMode(ledVerde, OUTPUT);    //SALIDA
  
   
  pinMode(sensorA, INPUT); //ENTRADA
  pinMode(sensorB, INPUT); //ENTRADA
  pinMode(sensorC, INPUT); //ENTRADA

}

void loop()
{
  x = digitalRead(sensorA);  //LEER EL SENSOR A
  y = digitalRead(sensorB);  //LEER EL SENSOR B
  z = digitalRead(sensorC);  //LEER EL SENSOR C
  
  verde = x&y&z;
  amarillo = (!x & y & z) | (x & !y & z) | (x & y & !z); 
  Rojo = (!x & !y) | (!x & !z) | (!y & !z);

  digitalWrite(ledVerde, verde);
  digitalWrite(ledRojo, Rojo);
  digitalWrite(ledAmarillo, amarillo);
   
}