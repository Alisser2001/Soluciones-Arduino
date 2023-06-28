// C++ code
#include <LiquidCrystal.h> 

//Pines del LCD
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

const int HUMO_SENSOR_PIN = A0; 
const int SOLAR_SENSOR_PIN = A1;
const int TEMP_SENSOR_PIN = A2;
const int ALARMA_PIN = A3;
const int FLEXION_SENSOR_PIN = A4;
const int FUERZA_SENSOR_PIN = A5;
const int MOTOR_PIN2 = 8;
const int MOTOR_PIN1 = 9;
const int PIR_SENSOR_PIN = 10;
const int CLOSE_LED_PIN = 11;
const int OPEN_LED_PIN = 12;

const unsigned long SECURITY_CODE = 123456;
unsigned long enteredCode = 0;

float temperature = 25.0;
bool cloudDetected = false;
bool smokeDetected = false;
bool movementDetected = false;
bool flexDetected = false;
bool forceDetected = false;
bool heatingState = false;
bool ventilationState = false;
bool alarmState = false;
bool enterSecurityCode = false;
bool isOpen = false;
bool isClose = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("Conexion serial exitosa");
  
  pinMode(HUMO_SENSOR_PIN, INPUT);
  pinMode(SOLAR_SENSOR_PIN, INPUT);
  pinMode(TEMP_SENSOR_PIN, INPUT);
  pinMode(ALARMA_PIN, OUTPUT);
  pinMode(FLEXION_SENSOR_PIN, INPUT);
  pinMode(FUERZA_SENSOR_PIN, INPUT);
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  pinMode(PIR_SENSOR_PIN, INPUT);
  pinMode(CLOSE_LED_PIN, OUTPUT);
  pinMode(OPEN_LED_PIN, OUTPUT);
  
  digitalWrite(OPEN_LED_PIN, LOW);
  digitalWrite(CLOSE_LED_PIN, LOW);
  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ventanas: OPEN");
  openWindows();
  delay(1000);
}

void loop() {
  temperature = readTemperature();
  cloudDetected = isCloudDetected();
  smokeDetected = isSmokeDetected();
  movementDetected = isMovementDetected();
  flexDetected = isFlexDetected();
  forceDetected = isForceDetected();
  
  if(movementDetected || flexDetected || forceDetected)
  {
  	closeWindows();
    activateAlarm();
  }
  if(smokeDetected){
   	openWindows();
    activateAlarm();
    openVentilation();
  }
  if(temperature < 15 || cloudDetected){
    openHeating();
    if(ventilationState){
      closeVentilation();
    }
  	closeWindows();
  } 
  if(temperature > 40) {
    openVentilation();
    if(heatingState){
      closeHeating();
    }
    openWindows();
  }
  if(temperature > 15 && !cloudDetected && heatingState){
    closeHeating();
    openWindows();
  }
  if(temperature < 40 && ventilationState){
    closeVentilation();
  }
  if(enteredCode > 0){
    Serial.println(enteredCode);
  }
  if(alarmState){
    if(enteredCode == 0){
      Serial.println("La alarma del hogar esta activada");
      Serial.println("Ingrese el codigo de seguridad para desactivarla.");
    }
    Serial.println("Ingrese el proximo digito de la clave: ");
    char receivedChar = Serial.read();
    if (receivedChar >= '0' && receivedChar <= '9') {
      enteredCode = enteredCode * 10 + (receivedChar - '0');
      if (enteredCode > 99999 && enteredCode != SECURITY_CODE) {
        Serial.println("Codigo incorrecto");
        enteredCode = 0; 
        enterSecurityCode = false;
      }
  	}
    if (enteredCode == SECURITY_CODE) {
      Serial.println("Codigo correcto. Apagando alarmas");
      desactivateAlarm();
      delay(1000);
      openWindows();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Ventanas: OPEN");
      enteredCode = 0;
      enterSecurityCode = false;
    }
  }
}

bool isMovementDetected() {
  int pir_data = digitalRead(PIR_SENSOR_PIN);
  return pir_data==HIGH; 
}

bool isCloudDetected() {
  int lightLevel = analogRead(SOLAR_SENSOR_PIN);
  return lightLevel>200; 
}

float readTemperature() {
  return map(analogRead(TEMP_SENSOR_PIN),0,1023,0,170); 
}

bool isSmokeDetected() {
  int smokeLevel = analogRead(HUMO_SENSOR_PIN);
  return smokeLevel>500; 
}

bool isFlexDetected() {
  int flexLevel = analogRead(FLEXION_SENSOR_PIN);
  return flexLevel>200; 
}

bool isForceDetected() {
  int forceLevel = analogRead(FUERZA_SENSOR_PIN);
  return forceLevel>5; 
}

void activateAlarm() {
  analogWrite(ALARMA_PIN, 255);
  alarmState = true;
}

void desactivateAlarm() {
  analogWrite(ALARMA_PIN, 0); 
  alarmState = false;
}

void openVentilation() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ventanas: OPEN");
  lcd.setCursor(0,1);
  lcd.print("Ventilacion: ON");
  digitalWrite(MOTOR_PIN1, HIGH);
  delay(1000);
  ventilationState = true;
}

void closeVentilation() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ventanas: OPEN");
  lcd.setCursor(0,1);
  lcd.print("Ventilacion: OFF");
  digitalWrite(MOTOR_PIN1, LOW); 
  delay(1000);
  ventilationState = false;
}

void openHeating(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ventanas: CLOSE");
  lcd.setCursor(0,1);
  lcd.print("Calefaccion: ON");
  digitalWrite(MOTOR_PIN2, HIGH);
  delay(1000);
  heatingState = true;
}

void closeHeating(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ventanas: OPEN");
  lcd.setCursor(0,1);
  lcd.print("Calefaccion: OFF");
  digitalWrite(MOTOR_PIN2, LOW);
  delay(1000);
  heatingState = false;
}

void openWindows() {
  digitalWrite(CLOSE_LED_PIN, LOW);
  digitalWrite(OPEN_LED_PIN, HIGH);
  if(smokeDetected){
    lcd.clear();
  	lcd.setCursor(0,0);
  	lcd.print("Ventanas: OPEN");
  	lcd.setCursor(0,1);
  	lcd.print("Fuga o incendio");
    delay(1000);
  }
  isOpen = true;
  isClose = false;
}

void closeWindows() {
  digitalWrite(OPEN_LED_PIN, LOW);
  digitalWrite(CLOSE_LED_PIN, HIGH);
  if(movementDetected){
    lcd.clear();
  	lcd.setCursor(0,0);
  	lcd.print("Ventanas: CLOSE");
    lcd.setCursor(0,1);
  	lcd.print("Intruso detectado");
    delay(1000);
  }
  if(flexDetected || forceDetected){
    lcd.clear();
  	lcd.setCursor(0,0);
  	lcd.print("Ventanas: CLOSE");
    lcd.setCursor(0,1);
  	lcd.print("Entrada forzada");
    delay(1000);
  }
  if(temperature < 20 || cloudDetected){
    lcd.clear();
  	lcd.setCursor(0,0);
  	lcd.print("Ventanas: CLOSE");
   	lcd.setCursor(0,1);
  	lcd.print("Mal clima");
    delay(1000);
  }
  isOpen = false;
  isClose = true;
}