#include <LiquidCrystal.h> 

//Pines del LCD
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

//Asignación de pines
const int FOTO_RES_PIN = A0; 
const int TEMP_SENSOR_PIN = A1;
const int HUMO_SENSOR_PIN = A2;
const int ALARMA_PIN = A3;
const int HORNO_LED_PIN = 13;
const int MOTOR_PIN1 = 9;
const int MOTOR_PIN2 = 8;
const int CALEFACCION_PIN1 = 12;
const int CALEFACCION_PIN2 = 11;
const int CALEFACCION_PIN3 = 10;

// Código de seguridad
const unsigned long SECURITY_CODE = 123456;

// Variables para el estado del sistema
float temperature = 40.0;
bool crackDetected = false;
bool smokeDetected = false;
bool heatingState = false;
bool ventilationState = false;
bool enterSecurityCode = false;

// Variable para almacenar el código ingresado desde el teléfono móvil
unsigned long enteredCode = 0;

bool flag=true;
int segundos = 0; 

//Interrupción
ISR(TIMER1_COMPA_vect)
{  
  flag=false; 
  segundos=segundos+1;
}

void setup(){
  
  Serial.begin(9600);
  Serial.println("Conexion serial exitosa");
  
  pinMode(FOTO_RES_PIN, INPUT);
  pinMode(TEMP_SENSOR_PIN, INPUT);
  pinMode(HUMO_SENSOR_PIN, INPUT);
  pinMode(HORNO_LED_PIN, OUTPUT);
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  pinMode(CALEFACCION_PIN1, OUTPUT);
  pinMode(CALEFACCION_PIN2, OUTPUT);
  pinMode(CALEFACCION_PIN2, OUTPUT);
  pinMode(ALARMA_PIN, OUTPUT);
  
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
  
  startOven();
}

void loop(){
  
    /*
  
  inputCode = serial.read();
  
    if (enteredCode == SECURITY_CODE) {
    Serial.println("Codigo correcto. Reiniciando horno");
    restartOven();
    enteredCode = 0;
    enterSecurityCode = false;
  }

  
  */
  
  
  
  
  

  // Verificar si hay una fisura
  crackDetected = isCrackDetected();

  // Verificar si hay humo
  smokeDetected = isSmokeDetected();
  
  if(segundos%5==0){
  	temperature = readTemperature();
  }
  
  // Actualizar el estado del sistema
  if (temperature < 20 || temperature > 60 || crackDetected || smokeDetected) {
    // Apagar el horno y activar la alarma
    turnOffOven();
    activateAlarm();
  } else if(digitalRead(HORNO_LED_PIN)==LOW){
    restartOven();
    desactivateAlarm();
  }
  
  if (temperature>50){
   	 openVentilation();
  }
  
  if (temperature<30){
   	openHeating(); 
  }

  // Leer el código ingresado desde el teléfono móvil
  if(enteredCode > 0){
    Serial.println(enteredCode);
  }
  if(!enterSecurityCode){
    Serial.println("Ingrese 1 para imprimir el estado de los sensores");
    Serial.println("Ingrese 2 para imprimir el estado de la temperatura");
    Serial.println("Ingrese 3 para abrir o cerrar la calefaccion");
    Serial.println("Ingrese 4 para abrir o cerrar la ventilacion");
    Serial.println("Ingrese 5 para validar codigo de reinicio");
    delay(500);
  }else{
    Serial.println("Ingrese el proximo digito de la clave: ");
  }
  char receivedChar = Serial.read();
  if(receivedChar == '1' && !enterSecurityCode){
    lcd.clear();
  	lcd.setCursor(0,0);
  	lcd.print("S. Fisuras: OFF");
    lcd.setCursor(0,1);
    lcd.print("S. Humo: OFF");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
  	lcd.print("Horno: ON");
    receivedChar = '0';
  }
  if(receivedChar == '2' && !enterSecurityCode){
    lcd.clear();
  	lcd.setCursor(0,0);
  	lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.setCursor(0,1);
    if(temperature<30){
    	lcd.print("C: ON, ");
    }else{
      lcd.print("C: OFF, ");
    }
    if(temperature>50){
    	lcd.print("V: ON");
    }else{
      lcd.print("V: OFF");
    }
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
  	lcd.print("Horno: ON");
    receivedChar = '0';
  }
  if(receivedChar == '3' && !enterSecurityCode){
    if(temperature<30){
    	openHeating();
    }else{
      	closeHeating();
    }
    receivedChar = '0';
  }
  if(receivedChar == '4' && !enterSecurityCode){
    if(temperature>50){
    	openVentilation();
    }else{
      closeVentilation();
    }
    receivedChar = '0';
  }
  if(receivedChar == '5' && !enterSecurityCode){
    enterSecurityCode = true;
    receivedChar = '0';
  }else if(enterSecurityCode){
  	if (receivedChar >= '0' && receivedChar <= '9') {
      enteredCode = enteredCode * 10 + (receivedChar - '0');
      if (enteredCode > 99999 && enteredCode != SECURITY_CODE) {
        Serial.println("Codigo incorrecto");
        enteredCode = 0; 
        enterSecurityCode = false;
      }
  	}
  }
  // Reiniciar el horno si se ingresa el código de seguridad correcto
  if (enteredCode == SECURITY_CODE) {
    Serial.println("Codigo correcto. Reiniciando horno");
    restartOven();
    enteredCode = 0;
    enterSecurityCode = false;
  }

  
  delay(1000);
}

bool isCrackDetected() {
  int lightLevel = analogRead(FOTO_RES_PIN);
  return lightLevel>100; 
}

float readTemperature() {
  return map(analogRead(TEMP_SENSOR_PIN),0,1023,-50,450); 
}

bool isSmokeDetected() {
  int smokeLevel = analogRead(HUMO_SENSOR_PIN);
  return smokeLevel>500; 
}

void openVentilation() {
  lcd.setCursor(0,1);
  lcd.print("Ventilacion: ON");
  digitalWrite(MOTOR_PIN1, HIGH);
  delay(2000);
  digitalWrite(MOTOR_PIN1, LOW);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Horno: ON");
}

void closeVentilation() {
  lcd.setCursor(0,1);
  lcd.print("Ventilacion: OFF");
  digitalWrite(MOTOR_PIN2, HIGH); 
  delay(2000);
  digitalWrite(MOTOR_PIN2, LOW);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Horno: ON");
}

void activateAlarm() {
  analogWrite(ALARMA_PIN, 255);
}

void desactivateAlarm() {
 analogWrite(ALARMA_PIN, 0); 
}

void openHeating(){
  lcd.setCursor(0,1);
  lcd.print("Calefaccion: ON");
  digitalWrite(CALEFACCION_PIN1, HIGH);
  delay(1000);
  digitalWrite(CALEFACCION_PIN2, HIGH);
  delay(1000);
  digitalWrite(CALEFACCION_PIN3, HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Horno: ON");
}

void closeHeating(){
  lcd.setCursor(0,1);
  lcd.print("Calefaccion: OFF");
  digitalWrite(CALEFACCION_PIN1, LOW);
  delay(1000);
  digitalWrite(CALEFACCION_PIN2, LOW);
  delay(1000);
  digitalWrite(CALEFACCION_PIN3, LOW);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Horno: ON");
}

void startOven() {
  digitalWrite(HORNO_LED_PIN, HIGH);
  lcd.begin(16, 2);
  delay(1000); 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Horno: ON");
}

void restartOven() {
  // Reiniciar el horno
  digitalWrite(HORNO_LED_PIN, LOW);

  // Mostrar el mensaje en el LCD
  lcd.clear();
  lcd.print("Horno: RESTART");
  delay(1000);
    
  //Encender el horno
  startOven();
}

void turnOffOven() {
  // Apagar el horno
  digitalWrite(HORNO_LED_PIN, LOW);
  digitalWrite(MOTOR_PIN1, LOW);
  digitalWrite(MOTOR_PIN2, LOW);
  digitalWrite(CALEFACCION_PIN1, LOW);
  digitalWrite(CALEFACCION_PIN2, LOW);
  digitalWrite(CALEFACCION_PIN3, LOW);

  //Encencer la alarma
  activateAlarm();
  
  // Mostrar el mensaje en el LCD
  lcd.clear();
  lcd.setCursor(0,0);
  if(temperature < 20 || temperature > 60){
    lcd.print("Temperatura fuera de rango");
  }else{
    if(crackDetected){
    	lcd.print("Fisura detectada");
    }
    if(smokeDetected){
      	lcd.print("Fuga de gas");
    }
  }
  lcd.setCursor(0,1);
  lcd.print("Horno: OFF");
}