/*==========================================
  =  Este codigo tiene bloqueo de puerta   =
  ==========================================*/

/*======================================================
  =  Este codigo no tiene habilitado el monitor serie  =
  ======================================================*/


const int velocidad1 = 6; // velocidades para el motor
const int velocidad2 = 5; //           ""
const int velocidad3 = 4; //           ""

const int motor_adelante = 3; // direccion del motor
const int motor_reversa = 2; //          ""

//=========================ENTRADAS===========================//

const int nivel_agua1 = 7; //sensor de nivel de agua
const int nivel_agua2 = 8; //         ""
const int nivel_agua3 = 9; //         ""

const int start = 10; //boton para iniciar ciclo

const int select_ciclo = 11; //boton del selecion de ciclo

//============================================================//

const int door_lock = 13;
const int agua_caliente = A0; // valvula de agua fria
const int agua_fria = A1; // valvula de agua caliente
const int vaciado = A2; // drenaje


const int led_indicador1 = A3; //led indicador hard
const int led_indicador2 = A4; //led indicador normal
const int led_indicador3 = A5; //led indicador suave
const int led_indicador4 = 12; //led indicador centrifugado
const int led_start = 0; //led indicador start

int num_ciclo = 0; //esta varible sirve para reconocer que ciclo es el que se esta pulsando//

//PINES SOBRANTES:

void setup() {

  pinMode(velocidad1, OUTPUT);
  pinMode(velocidad2, OUTPUT);
  pinMode(velocidad3, OUTPUT);

  pinMode(led_indicador1, OUTPUT);
  pinMode(led_indicador2, OUTPUT);
  pinMode(led_indicador3, OUTPUT);
  pinMode(led_indicador4, OUTPUT);
  pinMode(led_start, OUTPUT);

  pinMode(motor_adelante, OUTPUT);
  pinMode(motor_reversa, OUTPUT);

  pinMode(vaciado, OUTPUT);
  pinMode(door_lock, OUTPUT);

  pinMode(agua_caliente, OUTPUT);
  pinMode(agua_fria, OUTPUT);

  pinMode(nivel_agua1, INPUT_PULLUP);
  pinMode(nivel_agua2, INPUT_PULLUP);
  pinMode(nivel_agua3, INPUT_PULLUP);

  pinMode(select_ciclo, INPUT_PULLUP);
  pinMode(start, INPUT_PULLUP);


}

void loop() {

  digitalWrite(door_lock, LOW);
  digitalWrite(led_start, LOW);

  //-----para seleccionar ciclo de lavado-------//

  if (digitalRead(select_ciclo) == LOW) {

    delay(100);
    while (digitalRead(select_ciclo) == LOW);
    delay(100);
    num_ciclo ++;

    if (num_ciclo > 4) {
      num_ciclo = 0;
    }
  }


  indicador_ciclo(num_ciclo);


  //Cada una de estas funciones se ejecutaran dependiendo de lo antes seleccionado

  ciclo_delicado();
  ciclo_normal();
  ciclo_hard();
  centrifugado();

}

//--------ciclo de centrifugado--------//

void centrifugado() {

  if (num_ciclo == 4 and digitalRead(start) == LOW) {

    delay(100);
    while (digitalRead(start) == LOW);
    delay(100);

    //EL CENTRIFUGADO DURA 1MIN 10SEG

    digitalWrite(door_lock, HIGH);
    delay(500);

    digitalWrite(led_start, HIGH);
    digitalWrite(vaciado, LOW);


    digitalWrite(velocidad1, LOW);
    digitalWrite(velocidad2, LOW);
    digitalWrite(velocidad3, HIGH);

    digitalWrite(motor_reversa, HIGH);
    delay(10000);

    digitalWrite(velocidad1, HIGH);
    digitalWrite(velocidad2, HIGH);
    digitalWrite(velocidad3, LOW);

    digitalWrite(motor_reversa, HIGH);
    delay(90000);
    digitalWrite(motor_reversa, LOW);

    // digitalWrite(vaciado, LOW);

    delay(500);

    digitalWrite(velocidad1, LOW);
    digitalWrite(velocidad2, LOW);
    digitalWrite(velocidad3, LOW);

    digitalWrite(door_lock, LOW);

    delay(10000);

    digitalWrite(led_start, LOW);
    num_ciclo = 0;

    digitalWrite(door_lock, LOW);

    return;
  }
}

/*=====================================================
  =====================================================*/

/*==========================================
  ========       CICLO NORMAL       ========
  ==========================================*/

void ciclo_normal () {


  if (num_ciclo == 2 and digitalRead(start) == LOW) {

    delay(100);
    while (digitalRead(start) == LOW);
    delay(100);

    digitalWrite(door_lock, HIGH);
    digitalWrite(led_start, HIGH);
    digitalWrite(vaciado, HIGH);

    //EL CICLO NORMAL SE LLENA SOLO CON AGUA FRIA HASTA QUE LLEGUE AL NIVEL 3 DE AGUA.

    for (int x = 0; x < 5; x++) {

      digitalWrite(agua_fria, HIGH);
      x = 0;

      if ((digitalRead(nivel_agua1) == LOW) and (digitalRead(nivel_agua2) == LOW) and (digitalRead(nivel_agua3) == LOW)) {
        digitalWrite(agua_fria, LOW);
        break;
      }
    }

    delay(3000);

    //COMIENZA EL CICLO DE LAVADO NORMAL(DURA 20MIN)

    digitalWrite(velocidad1, LOW);
    digitalWrite(velocidad2, HIGH);
    digitalWrite(velocidad3, LOW);

    delay(1000);

    //    for (int i = 0; i < 40; i++)

    for (int i = 0; i < 40; i++) {


      digitalWrite(motor_adelante, HIGH);
      delay(12000);
      digitalWrite(motor_adelante, LOW);
      delay(3000);
      digitalWrite(motor_reversa, HIGH);
      delay(12000);
      digitalWrite(motor_reversa, LOW);
      delay(3000);

    }

    delay(3000);

    //Vaciando para enjuagar

    for (int v = 0; v < 5; v++ ) {
      v = 0;
      digitalWrite(vaciado, LOW);

      if ((digitalRead(nivel_agua1) == HIGH) and (digitalRead(nivel_agua2) == HIGH) and (digitalRead(nivel_agua3) == HIGH)) {

        break;
      }
    }

    delay(15000);

    digitalWrite(vaciado, HIGH);

    //CICLO DE ENJUAGUE:

    enjuague();

    //Vaciando para centrifugar

    for (int v = 0; v < 5; v++ ) {
      v = 0;
      digitalWrite(vaciado, LOW);

      if ((digitalRead(nivel_agua1) == HIGH) and (digitalRead(nivel_agua2) == HIGH) and (digitalRead(nivel_agua3) == HIGH)) {
        break;
      }
    }


    delay(15000);

    /*==============CENTRIFUGADO==============*/

    delay(500);


    digitalWrite(velocidad1, LOW);
    digitalWrite(velocidad2, LOW);
    digitalWrite(velocidad3, HIGH);
    digitalWrite(motor_reversa, HIGH);
    delay(10000);
    digitalWrite(velocidad1, HIGH);
    digitalWrite(velocidad2, HIGH);
    digitalWrite(velocidad3, LOW);
    digitalWrite(motor_reversa, HIGH);
    delay(90000);
    digitalWrite(motor_reversa, LOW);

    delay(500);

    digitalWrite(velocidad1, LOW);
    digitalWrite(velocidad2, LOW);
    digitalWrite(velocidad3, LOW);


    /*=========TERMINA EL CENTRIFUGADO=========*/

    delay(10000);

    digitalWrite(vaciado, LOW);
    digitalWrite(led_start, LOW);
    num_ciclo = 0;
    digitalWrite(door_lock, LOW);
  }
  return;
}


/*=====================================================
  =====================================================*/




/*==========================================
  ========     CICLO DELICADO       ========
  ==========================================*/

void ciclo_delicado () {


  if (num_ciclo == 3 and digitalRead(start) == LOW) {

    delay(100);
    while (digitalRead(start) == LOW);
    delay(100);

    digitalWrite(door_lock, HIGH);
    digitalWrite(led_start, HIGH);
    digitalWrite(vaciado, HIGH);

    //SE ABREN LAS DOS VALVULAS DE AGUA AL MISMO TIEMPO

    for (int x = 0; x < 5; x++) {

      digitalWrite(agua_fria, HIGH);
      digitalWrite(agua_caliente, HIGH);
      x = 0;

      //CUANDO EL AGUA LLEGUE AL NIVEL 1 SE CIERRA EL AGUA CALIENTE ...

      if ((digitalRead(nivel_agua1) == LOW) and (digitalRead(nivel_agua2) == HIGH) and (digitalRead(nivel_agua3) == HIGH)) {
        digitalWrite(agua_caliente, LOW);
        break;
      }
    }

    //Y SE TERMINA DE RELLENAR CON AGUA FRIA HASTA EL NIVEL 2

    for (int x = 0; x < 5; x++) {

      digitalWrite(agua_fria, HIGH);
      x = 0;

      if ((digitalRead(nivel_agua1) == LOW) and (digitalRead(nivel_agua2) == LOW) and (digitalRead(nivel_agua3) == HIGH)) {
        digitalWrite(agua_fria, LOW);
        break;
      }
    }

    delay(3000);

    //COMIENZA EL CICLO DE LAVADO (DELICADO "DURACION 10MIN")


    digitalWrite(velocidad1, HIGH);
    digitalWrite(velocidad2, LOW);
    digitalWrite(velocidad3, LOW);

    delay(1000);

    //for (int i = 0; i < 20; i++)

    for (int i = 0; i < 20; i++) {


      digitalWrite(motor_adelante, HIGH);
      delay(12000);
      digitalWrite(motor_adelante, LOW);
      delay(3000);
      digitalWrite(motor_reversa, HIGH);
      delay(12000);
      digitalWrite(motor_reversa, LOW);
      delay(3000);

    }

    delay(5000);

    //Vaciando para enjuagar

    for (int v = 0; v < 5; v++ ) {
      v = 0;
      digitalWrite(vaciado, LOW);

      if ((digitalRead(nivel_agua1) == HIGH) and (digitalRead(nivel_agua2) == HIGH) and (digitalRead(nivel_agua3) == HIGH)) {

        break;
      }
    }

    delay(15000);

    digitalWrite(vaciado, HIGH);

    //CICLO DE ENJUAGUE:

    enjuague_delicado();

    //Vaciando para centrifugar

    for (int v = 0; v < 5; v++ ) {
      v = 0;
      digitalWrite(vaciado, LOW);

      if ((digitalRead(nivel_agua1) == HIGH) and (digitalRead(nivel_agua2) == HIGH) and (digitalRead(nivel_agua3) == HIGH)) {

        break;
      }
    }


    delay(15000);

    /*==============CENTRIFUGADO==============*/

    delay(500);


    digitalWrite(velocidad1, LOW);
    digitalWrite(velocidad2, LOW);
    digitalWrite(velocidad3, HIGH);
    digitalWrite(motor_reversa, HIGH);
    delay(10000);
    digitalWrite(velocidad1, HIGH);
    digitalWrite(velocidad2, HIGH);
    digitalWrite(velocidad3, LOW);
    digitalWrite(motor_reversa, HIGH);
    delay(90000);
    digitalWrite(motor_reversa, LOW);

    delay(500);

    digitalWrite(velocidad1, LOW);
    digitalWrite(velocidad2, LOW);
    digitalWrite(velocidad3, LOW);



    /*=========TERMINA EL CENTRIFUGADO=========*/
    delay(10000);

    digitalWrite(vaciado, LOW);
    digitalWrite(led_start, LOW);
    num_ciclo = 0;
    digitalWrite(door_lock, LOW);
  }
  return;
}

/*=====================================================
  =====================================================*/


/*==========================================
  ========       CICLO FUERTE       ========
  ==========================================*/

void ciclo_hard () {


  if (num_ciclo == 1 and digitalRead(start) == LOW) {

    delay(100);
    while (digitalRead(start) == LOW);
    delay(100);

    digitalWrite(door_lock, HIGH);
    digitalWrite(led_start, HIGH);
    digitalWrite(vaciado, HIGH);

    for (int x = 0; x < 5; x++) {

      digitalWrite(agua_fria, HIGH);
      digitalWrite(agua_caliente, HIGH);
      x = 0;

      if ((digitalRead(nivel_agua1) == LOW) and (digitalRead(nivel_agua2) == LOW) and (digitalRead(nivel_agua3) == LOW)) {
        digitalWrite(agua_fria, LOW);
        digitalWrite(agua_caliente, LOW);
        break;
      }
    }

    delay(3000);

    digitalWrite(velocidad1, LOW);
    digitalWrite(velocidad2, HIGH);
    digitalWrite(velocidad3, LOW);

    delay(1000);

    for (int i = 0; i < 45; i++) {


      digitalWrite(motor_adelante, HIGH);
      delay(12000);
      digitalWrite(motor_adelante, LOW);
      delay(3000);
      digitalWrite(motor_reversa, HIGH);
      delay(12000);
      digitalWrite(motor_reversa, LOW);
      delay(3000);

    }

    delay(3000);

    //Vaciando para enjuagar

    for (int v = 0; v < 5; v++ ) {
      v = 0;
      digitalWrite(vaciado, LOW);

      if ((digitalRead(nivel_agua1) == HIGH) and (digitalRead(nivel_agua2) == HIGH) and (digitalRead(nivel_agua3) == HIGH)) {

        break;
      }
    }

    delay(15000);

    digitalWrite(vaciado, HIGH);

    //CICLO DE ENJUAGUE:

    enjuague();

    //Vaciando para centrifugar

    for (int v = 0; v < 5; v++ ) {
      v = 0;
      digitalWrite(vaciado, LOW);

      if ((digitalRead(nivel_agua1) == HIGH) and (digitalRead(nivel_agua2) == HIGH) and (digitalRead(nivel_agua3) == HIGH)) {
        break;
      }
    }


    delay(15000);

    /*==============CENTRIFUGADO==============*/

    delay(500);

    digitalWrite(velocidad1, LOW);
    digitalWrite(velocidad2, LOW);
    digitalWrite(velocidad3, HIGH);
    digitalWrite(motor_reversa, HIGH);
    delay(10000);
    digitalWrite(velocidad1, HIGH);
    digitalWrite(velocidad2, HIGH);
    digitalWrite(velocidad3, LOW);
    digitalWrite(motor_reversa, HIGH);
    delay(90000);
    digitalWrite(motor_reversa, LOW);

    delay(500);

    digitalWrite(velocidad1, LOW);
    digitalWrite(velocidad2, LOW);
    digitalWrite(velocidad3, LOW);



    /*=========TERMINA EL CENTRIFUGADO=========*/

    delay(10000);

    digitalWrite(vaciado, LOW);
    digitalWrite(led_start, LOW);
    num_ciclo = 0;
    digitalWrite(door_lock, LOW);
  }
  return;
}

/*=====================================================
  =====================================================*/

void indicador_ciclo(int aux) {

  switch (aux) {

    case 0:
      digitalWrite(led_indicador1, LOW);
      digitalWrite(led_indicador2, LOW);
      digitalWrite(led_indicador3, LOW);
      digitalWrite(led_indicador4, LOW);
      break;

    case 1:
      digitalWrite(led_indicador1, HIGH);
      digitalWrite(led_indicador2, LOW);
      digitalWrite(led_indicador3, LOW);
      digitalWrite(led_indicador4, LOW);
      break;

    case 2:
      digitalWrite(led_indicador1, LOW);
      digitalWrite(led_indicador2, HIGH);
      digitalWrite(led_indicador3, LOW);
      digitalWrite(led_indicador4, LOW);
      break;

    case 3:
      digitalWrite(led_indicador1, LOW);
      digitalWrite(led_indicador2, LOW);
      digitalWrite(led_indicador3, HIGH);
      digitalWrite(led_indicador4, LOW);
      break;

    case 4:
      digitalWrite(led_indicador1, LOW);
      digitalWrite(led_indicador2, LOW);
      digitalWrite(led_indicador3, LOW);
      digitalWrite(led_indicador4, HIGH);
      break;

  }
  return;
}


//------ciclo de enjuague-----//
//este ciclo dura 5min


void enjuague() {

  for (int x = 0; x < 5; x++) {

    digitalWrite(agua_fria, HIGH);
    x = 0;

    if ((digitalRead(nivel_agua1) == LOW) and (digitalRead(nivel_agua2) == LOW) and (digitalRead(nivel_agua3) == LOW)) {
      digitalWrite(agua_fria, LOW);
      break;
    }
  }

  digitalWrite(velocidad1, LOW);
  digitalWrite(velocidad2, HIGH);
  digitalWrite(velocidad3, LOW);

  //  for (int i = 0; i < 10 ; i++) {

  for (int i = 0; i < 10 ; i++) {

    digitalWrite(motor_adelante, HIGH);
    delay(12000);
    digitalWrite(motor_adelante, LOW);
    delay(3000);
    digitalWrite(motor_reversa, HIGH);
    delay(12000);
    digitalWrite(motor_reversa, LOW);
    delay(3000);
  }

  return;
}

//------ciclo de enjuague_delicado-----//
//este ciclo dura 5min

void enjuague_delicado() {


  for (int x = 0; x < 5; x++) {

    digitalWrite(agua_fria, HIGH);
    x = 0;

    if ((digitalRead(nivel_agua1) == LOW) and (digitalRead(nivel_agua2) == LOW) and (digitalRead(nivel_agua3) == HIGH)) {
      digitalWrite(agua_fria, LOW);
      break;
    }
  }

  digitalWrite(velocidad1, HIGH);
  digitalWrite(velocidad2, LOW);
  digitalWrite(velocidad3, LOW);

  //for (int i = 0; i < 10 ; i++) {

  for (int i = 0; i < 10 ; i++) {

    digitalWrite(motor_adelante, HIGH);
    delay(12000);
    digitalWrite(motor_adelante, LOW);
    delay(3000);
    digitalWrite(motor_reversa, HIGH);
    delay(12000);
    digitalWrite(motor_reversa, LOW);
    delay(3000);
  }
  return;
}
