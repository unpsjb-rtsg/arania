//--------------------------------------------------------------
// Stompy the Robot - Mk 1 - APC Magazine, July 2013 issue
//
// Modified by Darren Yates 3 May 2013
// From code originally developed by Russell Cameron, Dagu.
// Editado y comentado por Konrad PEschka, Bs As, 2015
//--------------------------------------------------------------

#include <Servo.h>
#include <NewPing.h>

#define TRIG_PIN A0
#define ECHO_PIN A1
#define MAX_DISTANCE 400
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

//-------------------------------------------------------------------
//Con este timer se setea la velocidad de movimiento de todo el robot, mas chico, mas rapido
//-------------------------------------------------------------------

int Time=120;

//-------------------------------------------------------------------
//  Definicion de las constantes posicion inicial de cada servo
//-------------------------------------------------------------------

//subir = giro antihor en cuerpo y patas

#define FIC_centro      1100     //Frente_Izquierda_cuerpo_centro
#define FDC_centro      1700     //Frente_derecha_cuerpo_centro
#define AIC_centro      1800     //Atras_Izquierda_cuerpo_centro
#define ADC_centro      1250     //Atras_derecha_cuerpo_centro

#define FIP_centro      800      //Frente_Izquierda_pata_centro
#define FDP_centro      800      //Frente_derecha_pata_centro
#define AIP_centro      700      //Atras_Izquierda_pata_centro
#define ADP_centro      600      //Atras_derecha_pata_centro

//-------------------------------------------------------------------
//      Definicion Alias de cada servo y los Pines del Arduino
//-------------------------------------------------------------------

#define FIC_pin 4      // Pin Frente Izquierda Cuerpo
#define FDC_pin 7      // Pin Frente Derecha Cuerpo
#define AIC_pin 5      // Pin Atras Izquierda Cuerpo
#define ADC_pin 6      // Pin Atras Derecha Cuerpo

#define FIP_pin 8      // Pin Frente Izquierda Pata
#define FDP_pin 11     // Pin Frente Derecha Pata
#define AIP_pin 9      // Pin Atras Izquierda Pata
#define ADP_pin 10     // Pin Atras Derecha Pata

#define Cabeza_pin 3

#define pulsador_arranque 12  //defino donde va conectado el pulsador de arranque

//-------------------------------------------------------------------

int movimiento_izq;
int movimiento_der;
int levanta=-400;

int contador_sentarse;
int distancia_actual = 0;
int pos = 90;

int max_der = 0;
int max_izq = 0;

int variable_sienta=800;

const int tiempo_entre_servo=5;
const int tiempo_sienta=50;

//-------------------------------------------------------------------
//       Defino como le va a llamar cada servo
//-------------------------------------------------------------------

Servo FIC_servo;                                               // Front Left  Hip
Servo FDC_servo;                                               // Front Right Hip
Servo AIC_servo;                                               // Rear  Left  Hip
Servo ADC_servo;                                               // Rear  Right Hip
Servo FIP_servo;                                               // Front Left  Knee
Servo FDP_servo;                                               // Front Right Knee
Servo AIP_servo;                                               // Rear  left  Knee
Servo ADP_servo;                                               // Rear  Right Knee

Servo Cabeza_servo;

//-------------------------------------------------------------------
//                 Seteo de Entradas, salidas y funciones
//-------------------------------------------------------------------

void setup()
{
  
  //-------------------------------------------------------------------
  // Inicializo Los servos con los nombres y cual es su pin asociado
  //-------------------------------------------------------------------
 
  FIC_servo.attach(FIC_pin);           //define los servos con su pin
  FDC_servo.attach(FDC_pin);  
  AIC_servo.attach(AIC_pin);  
  ADC_servo.attach(ADC_pin);  
  FIP_servo.attach(FIP_pin);  
  FDP_servo.attach(FDP_pin);  
  AIP_servo.attach(AIP_pin);  
  ADP_servo.attach(ADP_pin); 
  Cabeza_servo.attach(Cabeza_pin); 


//-------------------------------------------------------------------
//         Arranco Todos los Servos en sus posiciones iniciales
//-------------------------------------------------------------------


  FIC_servo.writeMicroseconds(FIC_centro);       // envia servo a su posicion inicial
  delay(tiempo_entre_servo);
  FDC_servo.writeMicroseconds(FDC_centro); 
  delay(tiempo_entre_servo); 
  AIC_servo.writeMicroseconds(AIC_centro);  
  delay(tiempo_entre_servo);
  ADC_servo.writeMicroseconds(ADC_centro);  
  delay(tiempo_entre_servo);
  FIP_servo.writeMicroseconds(FIP_centro);  
  delay(tiempo_entre_servo);
  FDP_servo.writeMicroseconds(FDP_centro);  
  delay(tiempo_entre_servo);
  AIP_servo.writeMicroseconds(AIP_centro);  
  delay(tiempo_entre_servo);
  ADP_servo.writeMicroseconds(ADP_centro); 
  delay(tiempo_entre_servo);
  Cabeza_servo.write(90);

delay(500);  
}

//-------------------------------------------------------------------
//         Sector donde comienza la funcion Principal,
//        Donde se va a quedar el resto de la ejecucion
//-------------------------------------------------------------------
  
void loop()
{
  //checkPath();
  
  while(digitalRead(pulsador_arranque)==0)
  ;
  
  while(1)   //se queda en este loop luego de haber pulsado el pulsador de arranque
  {
    distancia_actual = readPing();
    
    if (distancia_actual == 0 or distancia_actual > 30) 
    {
      movimiento_izq=-300;
      movimiento_der=-300;
      Camina_frente();
    } 
    
    else
    {
      if (distancia_actual <= 30 && distancia_actual >= 10)
      {
        mira_costados();
      } 
      else 
      {
        if (distancia_actual < 10 && distancia_actual !=0)
        {
          movimiento_izq=300;
          movimiento_der=300;
          Camina_frente();
          Camina_frente();
        }
      }
    }
    
    contador_sentarse++;
    if (contador_sentarse == 7) 
    sienta(); 


        if (contador_sentarse == 15) 
    { 
    saluda(); 
    contador_sentarse = 0; 
    }

     
  }
  
}

//-------------------------------------------------------------------
//            Sector definicion delas Funciones Utilizadas
//-------------------------------------------------------------------


//-------------------------------------------------------------------
//             Funcion Avanzar hacia el frente
//-------------------------------------------------------------------
void Camina_frente()
{

  if (movimiento_izq<0 && movimiento_der<0) 
   { 
    Cabeza_servo.write(110); 
    decide_costados(); 
   }
   
  FDP_servo.writeMicroseconds(FDP_centro-levanta);                // raise front right leg 
  delay(tiempo_entre_servo);
  AIP_servo.writeMicroseconds(AIP_centro-levanta);                // raise rear  left  leg
  delay(tiempo_entre_servo);
  FIC_servo.writeMicroseconds(FIC_centro-movimiento_izq);         // move  front left  leg backward
  delay(tiempo_entre_servo);
  ADC_servo.writeMicroseconds(ADC_centro+movimiento_der);         // move  rear  right leg backward
  delay(Time/2);
  FDC_servo.writeMicroseconds(FDC_centro-movimiento_der);         // move  front right leg forward
  delay(tiempo_entre_servo);
  AIC_servo.writeMicroseconds(AIC_centro+movimiento_izq);         // move  rear  left  leg forward
  delay(Time);
  FDP_servo.writeMicroseconds(FDP_centro);                        // lower front right leg 
  delay(tiempo_entre_servo);
  AIP_servo.writeMicroseconds(AIP_centro);                        // lower rear  left  leg
  delay(Time);
  
  if (movimiento_izq<0 && movimiento_der<0) 
   { 
    Cabeza_servo.write(70);
    decide_costados(); 
    }
    
  FIP_servo.writeMicroseconds(FIP_centro-levanta);                // raise front left  leg
  delay(tiempo_entre_servo);
  ADP_servo.writeMicroseconds(ADP_centro-levanta);                // raise rear  right leg
  delay(tiempo_entre_servo);
  FDC_servo.writeMicroseconds(FDC_centro+movimiento_der);         // move  front right leg backward
  delay(tiempo_entre_servo);
  AIC_servo.writeMicroseconds(AIC_centro-movimiento_izq);         // move  rear  left  leg backward
  delay(Time/2);
  FIC_servo.writeMicroseconds(FIC_centro+movimiento_izq);         // move  front left  leg forward
  delay(tiempo_entre_servo);
  ADC_servo.writeMicroseconds(ADC_centro-movimiento_der);         // move  rear  right leg forward
  delay(Time);
  FIP_servo.writeMicroseconds(FIP_centro);                        // lower front left  leg
  delay(tiempo_entre_servo);
  ADP_servo.writeMicroseconds(ADP_centro);                        // lower rear  right leg
  delay(Time);  

  }
  
//-------------------------------------------------------------------
//            Funcion que lee el sensor frontal
//-------------------------------------------------------------------  
  
  int readPing()                            //funcion para leer US y pasarlo a cm
{
  unsigned int uS = sonar.ping();
  delay(50);
  int cm = uS/US_ROUNDTRIP_CM;
  return cm;
}


//-------------------------------------------------------------------
//                    Elije para donde avanzar
//-------------------------------------------------------------------
int decide_costados() 
{
delay(50);
distancia_actual = readPing();

if (distancia_actual < 30 && distancia_actual > 5)       //chequeo si obstaculo esta entrte 10 y 30 cm
{ 
 mira_costados();                             //encontrar la salida
} 

if (distancia_actual < 6 && distancia_actual != 0)       //chequea obstaculo menor a 10cm
{ 
camina_atras();                               //hace secuancia para atras
}

}


//-------------------------------------------------------------------
//  Mide para lso costados para despues decidir hacia donde avanzar
//-------------------------------------------------------------------

int mira_costados()                          //hace un barrido y decide para donde ir
  {

  delay(90);                           
  max_der = 0; 
  max_izq = 0;
  
  for(pos = 144; pos >= 36; pos-= 36)     // goes from 180 degrees to 0 degrees 
  {
   Cabeza_servo.write(pos);                 // tell servo to go to position in variable 'pos' 
    delay(90);                      
    distancia_actual = readPing();                 //lee ultrasonido
    if (pos > 90 && distancia_actual > max_izq)    // si es mayor al maximo izquierda lo actualiza
    { 
    max_izq = distancia_actual;                     
    } 
    if (pos < 90 && distancia_actual > max_der)   // si es mayor al maximo derecha lo actualiza
    { 
    max_der = distancia_actual; 
    }
  }
  
  
   Cabeza_servo.write(90);                //posiciona servo en el medio
    if (max_der > max_izq) 
     { 
     Gira_der();                       //gira derecha
     }
    if (max_izq > max_der) 
     { 
      Gira_izq();                      //gira izquierda
     }
}
    
//-------------------------------------------------------------------
//                 Gira Derecha
//-------------------------------------------------------------------


void Gira_der() 
{
   movimiento_izq=300;
   movimiento_der=-300;
   Camina_frente();
   Camina_frente();
}  

//-------------------------------------------------------------------
//                 Gira Izquierda
//-------------------------------------------------------------------

void Gira_izq() 
{
   movimiento_izq=-300;
   movimiento_der=300;
  Camina_frente();
  Camina_frente();
}  

//-------------------------------------------------------------------
//               Camina hacia atras
//-------------------------------------------------------------------

void camina_atras()
{
   movimiento_izq=300;
  movimiento_der=300;
   Camina_frente();
}

//-------------------------------------------------------------------
//                  Funcion Sentarse
//-------------------------------------------------------------------
void sienta()
{
  
  FDC_servo.writeMicroseconds(FDC_centro);
  delay(tiempo_entre_servo);
  FIC_servo.writeMicroseconds(FIC_centro);
  delay(tiempo_entre_servo);

  
  ADC_servo.writeMicroseconds(ADC_centro+variable_sienta/3);  //se tira de a poco
 delay(tiempo_sienta);
  AIC_servo.writeMicroseconds(AIC_centro-variable_sienta/3);
  delay(tiempo_sienta);
  ADP_servo.writeMicroseconds(ADP_centro+variable_sienta/3);
  delay(tiempo_sienta);
  AIP_servo.writeMicroseconds(AIP_centro+variable_sienta/3);
  delay(tiempo_sienta);

  ADC_servo.writeMicroseconds(ADC_centro+variable_sienta/2);  //se tira de a poco
  delay(tiempo_sienta);
  AIC_servo.writeMicroseconds(AIC_centro-variable_sienta/2);
  delay(tiempo_sienta);
  ADP_servo.writeMicroseconds(ADP_centro+variable_sienta/2);
  delay(tiempo_sienta);
  AIP_servo.writeMicroseconds(AIP_centro+variable_sienta/2);
  delay(tiempo_sienta);
  
  ADC_servo.writeMicroseconds(ADC_centro+variable_sienta);   //se termina de tirar
  delay(tiempo_sienta);
  AIC_servo.writeMicroseconds(AIC_centro-variable_sienta);
  delay(tiempo_sienta);
  ADP_servo.writeMicroseconds(ADP_centro+variable_sienta);
 delay(tiempo_sienta);
  AIP_servo.writeMicroseconds(AIP_centro+variable_sienta);
 delay(tiempo_sienta);
  
  delay(2500);                //espera 2,5 Segundos
  
  Cabeza_servo.write(45);
  delay(300);
  Cabeza_servo.write(135);
  delay(300);
  Cabeza_servo.write(90);
  delay(1900);
  
  
  FIP_servo.writeMicroseconds(FIP_centro); 
  delay(tiempo_entre_servo); 
  FDP_servo.writeMicroseconds(FDP_centro); 
  delay(tiempo_entre_servo);
  FIC_servo.writeMicroseconds(FIC_centro);  
  delay(tiempo_entre_servo);
  FDC_servo.writeMicroseconds(FDC_centro);  
  delay(tiempo_entre_servo);
  AIP_servo.writeMicroseconds(AIP_centro); 
  delay(tiempo_entre_servo); 
  ADP_servo.writeMicroseconds(ADP_centro); 
  delay(tiempo_entre_servo);
  AIC_servo.writeMicroseconds(AIC_centro);                      // center servos
  delay(tiempo_entre_servo);
  ADC_servo.writeMicroseconds(ADC_centro); 
  
  delay(500);
}  


//-------------------------------------------------------------------
//                  Funcion Saluda
//-------------------------------------------------------------------

void saluda(void)
{
  
  FDC_servo.writeMicroseconds(FDC_centro);
  delay(tiempo_entre_servo);
  FIC_servo.writeMicroseconds(FIC_centro);
  delay(tiempo_entre_servo);

  ADC_servo.writeMicroseconds(ADC_centro+variable_sienta);  //se tira al piso de costado
  delay(tiempo_entre_servo);
  AIC_servo.writeMicroseconds(AIC_centro-variable_sienta);
  delay(tiempo_entre_servo);

  FDP_servo.writeMicroseconds(2550);
  delay(tiempo_sienta);
  ADP_servo.writeMicroseconds(ADP_centro+variable_sienta/3);
  delay(tiempo_sienta);
  AIP_servo.writeMicroseconds(AIP_centro+variable_sienta/3);
  delay(tiempo_sienta);
  AIP_servo.writeMicroseconds(AIP_centro+variable_sienta/2);
  delay(tiempo_sienta);
  AIP_servo.writeMicroseconds(AIP_centro+variable_sienta);
  delay(tiempo_sienta);

  mueve_pata_frente();
  mueve_pata_frente();
  delay(1000);

  
  Cabeza_servo.write(45);
  delay(300);
  Cabeza_servo.write(135);
  delay(300);
  Cabeza_servo.write(90);
  delay(2000);
  
  
  FIP_servo.writeMicroseconds(FIP_centro); 
  delay(tiempo_entre_servo); 
  FDP_servo.writeMicroseconds(FDP_centro); 
  delay(tiempo_entre_servo);
  FIC_servo.writeMicroseconds(FIC_centro);  
  delay(tiempo_entre_servo);
  FDC_servo.writeMicroseconds(FDC_centro);  
  delay(tiempo_entre_servo);
  AIP_servo.writeMicroseconds(AIP_centro); 
  delay(tiempo_entre_servo); 
  ADP_servo.writeMicroseconds(ADP_centro); 
  delay(tiempo_entre_servo);
  AIC_servo.writeMicroseconds(AIC_centro);                      // center servos
  delay(tiempo_entre_servo);
  ADC_servo.writeMicroseconds(ADC_centro); 
  
  delay(500);
}  

//-------------------------------------------------------------------
//                  Funcion Mueve Pata en Frente
//-------------------------------------------------------------------

void mueve_pata_frente(void)
{
  int maximo=500;
  int tiempo_saluda=80;
    for(int j=0;j<maximo;j=j+100)
  {
  FDC_servo.writeMicroseconds(FDC_centro+j);  //se tira al piso de costado
  delay(tiempo_saluda);
  }

  for(int j=maximo;j>0;j=j-100)
  {
  FDC_servo.writeMicroseconds(FDC_centro+j);  //se tira al piso de costado
  delay(tiempo_saluda);
  }

  for(int j=0;j<maximo;j=j+100)
  {
  FDC_servo.writeMicroseconds(FDC_centro-j);  //se tira al piso de costado
  delay(tiempo_saluda);
  }

  for(int j=maximo;j>0;j=j-100)
  {
  FDC_servo.writeMicroseconds(FDC_centro-j);  //se tira al piso de costado
  delay(tiempo_saluda);
  }
}
//-------------------------------------------------------------------
//               FIN PROGRAMA
//-------------------------------------------------------------------


