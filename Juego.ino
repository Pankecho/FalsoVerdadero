const String preguntas[] = {"Pregunta 1", "Pregunta 2", "Pregunta 3", "Pregunta 4"};
const bool respuestas[] = {true, false, true, false};

const byte trigPin = 3;
const byte echoPin = 2;
const byte botonInicio = 8;
const byte botonSiguiente = 9;
unsigned long tiempo = 0;

int puntaje[] = {0, 0};

//False jugador 1 True jugador 2
bool turno = false;
bool pregunta = false;
bool r = false;
int aleatorio = 0;
byte rando = 0;
bool estado = false;
byte turnos = 2;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(botonInicio, INPUT);
  pinMode(botonSiguiente, INPUT);
  Serial.begin(9600); // Starts the serial communication
  aleatorio = random(10, 40);
}

void loop() {
  long t = millis();
  if (estado) {
    if (turnos > 0) {
      if (t - tiempo < 60000) {
        if (!pregunta) {
          int distanci = distancia();
          if (distanci == aleatorio)
            pregunta = true;
          else
            Serial.println(String("D " + String(distanci) + " " + String(aleatorio)));
        } else {
          if (!r) {
            rando = random(sizeof(preguntas) / sizeof(String));
            Serial.println(String("P " + preguntas[rando]));
            r = true;
          } else {
            bool bo = digitalRead(botonSiguiente);
            if (bo) {
              delay(100);
              rando = random(sizeof(preguntas) / sizeof(String));
              Serial.println(String("P " + preguntas[rando]));
            }
            int v = analogRead(A0);
            int f = analogRead(A1);
            if (v <= 20 && f >= 40) {
              respuestaa(true, (!turno) ? 1 : 2);
            } else if (f <= 20 && v >= 40) {
              respuestaa(false, (!turno) ? 1 : 2);
            }
          }
        }
      } else {
        turno = !turno;
        turnos--;
        if (turnos > 0) {
          Serial.println("T JUGADOR " +  String((!turno) ? 1 : 2));
          delay(10000);
          tiempo = millis();
          pregunta = false;
          r = false;
        }
      }
    } else {
      estado = false;
      if (puntaje[0] == puntaje[1])
        Serial.println("FIN EMPATE");
      else
        Serial.println("FIN GANADOR JUGADOR " + String((puntaje[0] > puntaje[1]) ? "1" : "2"));
    }
  } else {
    bool bo = digitalRead(botonInicio);
    if (bo) {
      delay(100);
      estado = true;
      turno = false;
      pregunta = false;
      r = false;
      turnos = 2;
      puntaje[0] = 0;
      puntaje[1] = 0;
      Serial.println("EMPIEZA EL JUEGO");
      tiempo = t;
    }
  }
}

int distancia() {
  int distance = 0;
  long duration;
  digitalWrite(trigPin, LOW);
  delay(10);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  return distance;
}

void respuestaa(bool respuesta, byte jugador) {
  if (respuesta == respuestas[rando]) {
    puntaje[jugador - 1] += 1;
    Serial.println("CORRECTO " + String(puntaje[jugador - 1]));
  } else {
    Serial.println("INCORRECTO");
  }
  pregunta = false;
  r = false;
  aleatorio = random(10, 40);
  delay(1000);
}

