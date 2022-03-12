#include <Joystick.h>

// Constantes de programa
const int numBotonesDigitales = 8;
const int pinDigitalInicial = 2;
const int pinAnalogicoX = 21;
const int pinAnalogicoY = 22;
const int valoresJoystick = 1024;
const int zonaMuertaJoystick = 50;
const int delayLag = 50;
const bool debug = true;

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK, numBotonesDigitales, 0, true, true, false, false, false, false, false, false, false, false, false);

void setup() {

  // Inicializaciones
  pinMode(pinDigitalInicial, INPUT_PULLUP);
  pinMode(pinDigitalInicial + 1, INPUT_PULLUP);
  pinMode(pinDigitalInicial + 2, INPUT_PULLUP);
  pinMode(pinDigitalInicial + 3, INPUT_PULLUP);
  pinMode(pinDigitalInicial + 4, INPUT_PULLUP);
  pinMode(pinDigitalInicial + 5, INPUT_PULLUP);
  pinMode(pinDigitalInicial + 6, INPUT_PULLUP);
  pinMode(pinDigitalInicial + 7, INPUT_PULLUP);

  Joystick.begin();
  Joystick.setXAxisRange(0, valoresJoystick -1);
  Joystick.setYAxisRange(0, valoresJoystick -1);

  Serial.begin(9600);

}

// Función de redondeo a decenas
int redondear (int a)
{
  return a - (a % 10);
}

// Función de detección de zona muerta
bool zonamuerta (int a)
{
  return (a < ((valoresJoystick / 2) - zonaMuertaJoystick)) || (a > ((valoresJoystick / 2) + zonaMuertaJoystick));
}

// Estado inicial de botones digitales
int estadoBotonesDigital[numBotonesDigitales] = {0,0,0,0,0,0,0,0};

// Estado inicial de joystick
int estadoJoystickX = redondear((valoresJoystick / 2));
int estadoJoystickY = redondear((valoresJoystick / 2));

// Bucle de programa

void loop() {

  // Lectura de botones
  for (int index = 0; index < numBotonesDigitales; index++)
  {
    int estadoBotonDigital = !digitalRead(index + pinDigitalInicial);
    if (estadoBotonDigital != estadoBotonesDigital[index])
    {
      Joystick.setButton(index, estadoBotonDigital);
      estadoBotonesDigital[index] = estadoBotonDigital;
    }
  }

  // Lectura de joystick

  // Eje X
  int estadoActualJoystickX = redondear(analogRead(pinAnalogicoX));
  if ((estadoJoystickX != estadoActualJoystickX) && !zonamuerta(estadoActualJoystickX))
  { 
    estadoJoystickX = estadoActualJoystickX;
    Joystick.setXAxis(estadoJoystickX);
  }

  // Eje Y
  int estadoActualJoystickY = redondear(analogRead(pinAnalogicoY));
  if ((estadoJoystickY != estadoActualJoystickY) && !zonamuerta(estadoActualJoystickY))
  { 
    estadoJoystickY = estadoActualJoystickY;
    Joystick.setYAxis(estadoJoystickY);
  }

  // Pausa
   delay(delayLag);

  // Log
  if (debug)
  {
    Serial.println("Eje x: " + estadoJoystickX);
    Serial.println("Eje y: " + estadoJoystickY);
    Serial.println("-----------");
  }
}
