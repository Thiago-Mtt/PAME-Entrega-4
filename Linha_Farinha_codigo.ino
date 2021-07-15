#include "HX711.h"

#define calibration_factor -7050.0 //Valor obtido após calibrar a balança

#define MOTORES 2
#define T1 A0
#define T2 A1
#define T3 A2
#define T4 A3
#define T5 A4
#define BMB1 9
#define BMB2 10
#define BMB3 11
#define BMB4 3
#define BMB5 5
#define DOUT 4
#define SCK 6

HX711 scale;

void setup() {
  
 Serial.begin(9600);

 scale.begin(DOUT, SCK);
 scale.set_scale(calibration_factor);
 scale.tare(); //No momento deste comando, a balança não pode ter peso sobre ela
 
 pinMode(MOTORES, OUTPUT);
 pinMode(BMB1, OUTPUT);
 pinMode(BMB2, OUTPUT);
 pinMode(BMB3, OUTPUT);
 pinMode(BMB4, OUTPUT);
 pinMode(BMB5, OUTPUT);

 digitalWrite(MOTORES, HIGH);
 // Motores começam ligados
  
}

void loop() {
  int vazao_bomba;
  float temp1, temp2, temp3, temp4, temp5;

  //Medida das temperaturas
  temp1 = map(analogRead(T1), 0, 204, 0, 100);
  temp2 = map(analogRead(T2), 0, 204, 0, 100);
  temp3 = map(analogRead(T3), 0, 204, 0, 100);
  temp4 = map(analogRead(T4), 0, 204, 0, 100);
  temp5 = map(analogRead(T5), 0, 204, 0, 100);

  //Verificação de temperatura máxima (85Cº)
  if (temp1 > 85) digitalWrite(MOTORES, LOW);
  if (temp2 > 85) digitalWrite(MOTORES, LOW);
  if (temp3 > 85) digitalWrite(MOTORES, LOW);
  if (temp4 > 85) digitalWrite(MOTORES, LOW);
  if (temp5 > 85) digitalWrite(MOTORES, LOW);
  //Caso os motores desliguem, seria necessária reiniciar o programa usando o botão de reset


  //Determinação da potência das bombas hidráulicas
  vazao_bomba = calc_vazao(temp1);
  analogWrite(BMB1, vazao_bomba);

  vazao_bomba = calc_vazao(temp2);
  analogWrite(BMB2, vazao_bomba);

  vazao_bomba = calc_vazao(temp3);
  analogWrite(BMB3, vazao_bomba);

  vazao_bomba = calc_vazao(temp4);
  analogWrite(BMB4, vazao_bomba);

  vazao_bomba = calc_vazao(temp5);
  analogWrite(BMB5, vazao_bomba);
  
  //Transmissão de dados. Os dados de temperatura e peso podem ser transmitidos pelos pinos 0 e 1 do arduino ou pinos PD0 e PD1 do atmega328 (RX e TX)
  Serial.print("Temp Motor 1: ");
  Serial.print(temp1, 1);
  Serial.print("Cº ");

  Serial.print("Temp Motor 2: ");
  Serial.print(temp2, 1);
  Serial.print("Cº ");

  Serial.print("Temp Motor 3: ");
  Serial.print(temp3, 1);
  Serial.print("Cº ");

  Serial.print("Temp Motor 4: ");
  Serial.print(temp4, 1);
  Serial.print("Cº ");

  Serial.print("Temp Motor 5: ");
  Serial.print(temp5, 1);
  Serial.print("Cº ");

  Serial.print("Peso Sac Farinha: ");
  Serial.print(scale.get_units(), 1);
  Serial.println("Kg");
  
}

int calc_vazao(float temp){
  //A bomba d'água liga e aumenta a potência a partir de 50Cº e chega ao máximo em 80Cº
  int vazao;
  if (temp <= 50) return 0;
  if (temp > 50 && temp < 80){
    vazao = map(temp, 50, 80, 0, 255);
    return vazao;
  }
  if (temp >= 80) return 255;
}
