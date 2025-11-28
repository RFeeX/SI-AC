#include <Servo.h>

// ======================
//        SERVO PINCE
// ======================
Servo pince;
int PINCE_PIN = 30;
int OUVERT = 110;   // angle pince ouverte
int FERME = 60;     // angle pince fermée

// ======================
//       MOTEURS
// ======================

// Avant gauche
int ENA = 2;
int IN1 = 3;
int IN2 = 4;

// Avant droit
int ENB = 5;
int IN3 = 6;
int IN4 = 7;

// Arrière gauche
int ENC = 8;
int IN5 = 9;
int IN6 = 10;

// Arrière droit
int END = 11;
int IN7 = 12;
int IN8 = 13;

// Vitesse standard
int speedMotor = 150;

// ======================
//     CAPTEURS IR
// ======================
int IR_G = A0;
int IR_C = A1;
int IR_D = A2;

// ======================
//      ULTRASON
// ======================
int trig = 22;
int echo = 23;

// ======================
//        SETUP
// ======================
void setup() {

  // Moteurs
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENC, OUTPUT); pinMode(IN5, OUTPUT); pinMode(IN6, OUTPUT);
  pinMode(END, OUTPUT); pinMode(IN7, OUTPUT); pinMode(IN8, OUTPUT);

  // IR
  pinMode(IR_G, INPUT);
  pinMode(IR_C, INPUT);
  pinMode(IR_D, INPUT);

  // Ultrason
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // Servo pince
  pince.attach(PINCE_PIN);
  pince.write(OUVERT);

  Serial.begin(9600);
}

// ======================
//       ULTRASON
// ======================
long distance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH);
  long dist = duration * 0.034 / 2;
  return dist;
}

// ======================
//     FONCTIONS PINCE
// ======================
void fermerPince() {
  pince.write(FERME);
  delay(800);
}

void ouvrirPince() {
  pince.write(OUVERT);
  delay(800);
}

// ======================
//   FONCTIONS MOTEURS
// ======================
void moteur(int ena, int in1, int in2, int speedVal) {
  digitalWrite(in1, speedVal > 0);
  digitalWrite(in2, speedVal == 0);
  analogWrite(ena, speedVal);
}

void avancer() {
  moteur(ENA, IN1, IN2, speedMotor);
  moteur(ENB, IN3, IN4, speedMotor);
  moteur(ENC, IN5, IN6, speedMotor);
  moteur(END, IN7, IN8, speedMotor);
}

void tournerGauche() {
  moteur(ENA, IN1, IN2, 0);
  moteur(ENC, IN5, IN6, 0);
  moteur(ENB, IN3, IN4, speedMotor);
  moteur(END, IN7, IN8, speedMotor);
}

void tournerDroite() {
  moteur(ENB, IN3, IN4, 0);
  moteur(END, IN7, IN8, 0);
  moteur(ENA, IN1, IN2, speedMotor);
  moteur(ENC, IN5, IN6, speedMotor);
}

void stopRobot() {
  moteur(ENA, IN1, IN2, 0);
  moteur(ENB, IN3, IN4, 0);
  moteur(ENC, IN5, IN6, 0);
  moteur(END, IN7, IN8, 0);
}

void moteurAvanceLent() {
  moteur(ENA, IN1, IN2, 90);
  moteur(ENB, IN3, IN4, 90);
  moteur(ENC, IN5, IN6, 90);
  moteur(END, IN7, IN8, 90);
}

void moteurRecule(int s) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  digitalWrite(IN5, LOW); digitalWrite(IN6, HIGH);
  digitalWrite(IN7, LOW); digitalWrite(IN8, HIGH);

  analogWrite(ENA, s);
  analogWrite(ENB, s);
  analogWrite(ENC, s);
  analogWrite(END, s);
}

// ======================
//         LOOP
// ======================
void loop() {

  int g = digitalRead(IR_G);
  int c = digitalRead(IR_C);
  int d = digitalRead(IR_D);

  long dist = distance();

  // =============================
  //    DETECTION CANETTE
  // =============================
  if (dist > 6 && dist < 25) {
    stopRobot();
    delay(300);

    // Avancer doucement vers la canette
    while (distance() > 8) {
      moteurAvanceLent();
    }

    stopRobot();
    delay(300);

    // Fermer la pince
    fermerPince();
    delay(300);

    // Reculer
    moteurRecule(120);
    delay(500);
    stopRobot();
  }

  // =============================
  //        SUIVI DE LIGNE
  // =============================
  if (c == 0) {
    avancer();
  }
  else if (g == 0) {
    tournerGauche();
  }
  else if (d == 0) {
    tournerDroite();
  }
  else {
    stopRobot();
  }
}
