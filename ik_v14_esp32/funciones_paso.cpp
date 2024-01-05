#include "funciones_paso.h"


//Hay que decalrar las variables en el ".h" como extern y aqui inicializarlas para evitar error de compilacion
int PIERNA = IZQUIERDA;
int PASO = 20;
double Servo_Nuevos_Valores[NUM_SERVOS] = {rodillaR_ini, caderaR_ini, tobilloR_ini, cadera2R_ini,
                                           rodillaL_ini, caderaL_ini, tobilloL_ini, cadera2L_ini
                                          };
ServoLineal Servos;

void Swinging() //balanceo antes de dar un paso, para cambiar el peso de pierna
{
  if (PIERNA == DERECHA)
  {
    //PASO = 30;
    Servo_Nuevos_Valores[TOBILLO_L] -= 0.5 * SwingingChange; //0.5
    Servo_Nuevos_Valores[TOBILLO_R] -= 1.4*SwingingChange; //1.2
    Servos.WriteLineal(Servo_Nuevos_Valores, 500000.0);
  }
  else if (PIERNA == IZQUIERDA)
  {
    //PASO = 30;
    Servo_Nuevos_Valores[TOBILLO_R] += (0.5 * SwingingChange);//0.7
    Servo_Nuevos_Valores[TOBILLO_L] += 1.1*SwingingChange;
    Servos.WriteLineal(Servo_Nuevos_Valores, 300000.0);
  }
  else
  {
    Error_Func();
  }
  Serial.println("FASE SWINGING COMPLETADA...");
}

/*
  x : posición X del centro del pien en dos dimensiones
  y : posición X del centro del pien en dos dimensiones (cte)
  l1 : longitud entre la cadera y la rodilla
  l2 : longitud entre la rodilla y el pie
*/
void Inverse_Kinematics(double x, double y, double l1, double l2)
{
  double val1;
  double val2;


  // *** FASES DE CÁLCULO:
  // aplico ik para calcular los ángulos:
  val1 = (pow(x, 2.0) + pow(y, 2.0) - pow(l1, 2.0) - pow(l2, 2.0)) / (2 * l1 * l2);
  if (val1 >= 1) val1 = 1; //por errores de redondeo a veces me da > 1 y falla la función acos

  val2 = (y * (l1 + l2 * cos(0)) - x * l2 * sin(0)) / (x * (l1 + l2 * cos(0)) + y * l2 * sin(0));

  //PARA DEPURAR
  Serial.print("val1 ");
  Serial.println(val1);
  Serial.print("val2 ");
  Serial.println(val2);

  val1 = acos(val1); // valores que se pasen a acos deben estar entre -1 y 1
  val2 = atan(val2);

  //PARA DEPURAR
  Serial.print("val1 ");
  Serial.println(val1);
  Serial.print("val2 ");
  Serial.println(val2);

  //paso ambos valores de radianes a grados:
  val1 = (180 / 3.1416) * val1;
  val2 = (180 / 3.1416) * val2; // el servo se inicializa a 90 grados luego su posición lo calculo respecto a este

  //PARA DEPURAR
  Serial.print("val1 ");
  Serial.println(val1);
  Serial.print("val2 ");
  Serial.println(val2);

  // *** FASES DE MOVIMIENTO:
  if (val1 > 90 || val2 > 90) //COMPRUEBO QUE TODO HA IDO BIEN
  {
    Error_Func(); // en caso de mal calculo me salgo y no fuerzo los motores
  }
  else // SI TODO VA BIEN EJECUTO MOVIMIENTO
  {
    if (PIERNA == DERECHA)
    {
      //PRIMERA FASE DEL MOVIMIENTO:
      Servo_Nuevos_Valores[TOBILLO_L] += (-0.5 * SwingingChange);
      Servo_Nuevos_Valores[CADERA_R] += val2;
      Servos.WriteLineal(Servo_Nuevos_Valores, 400000.0);

      //SEGUNDA FASE DEL MOVIMIENTO:
      Servo_Nuevos_Valores[CADERA_L] = caderaL_ini;
      Servo_Nuevos_Valores[RODILLA_R] -= val1;
      Servos.WriteLineal(Servo_Nuevos_Valores, 100000.0);

      //TERCERA FASE DEL MOVIMIENTO:
      Servo_Nuevos_Valores[RODILLA_L] = rodillaL_ini;
      Servos.WriteLineal(Servo_Nuevos_Valores, 100000.0);
    }
    else if (PIERNA == IZQUIERDA)
    {
      //PRIMERA FASE DEL MOVIMIENTO:
      Servo_Nuevos_Valores[TOBILLO_R] += (0.5 * SwingingChange);
      Servo_Nuevos_Valores[CADERA_L] -= val2;
      Servos.WriteLineal(Servo_Nuevos_Valores, 400000.0);

      //SEGUNDA FASE DEL MOVIMIENTO:
      Servo_Nuevos_Valores[CADERA_R] = caderaR_ini;
      Servo_Nuevos_Valores[RODILLA_L] += val1;
      Servos.WriteLineal(Servo_Nuevos_Valores, 100000.0);

      //TERCERA FASE DEL MOVIMIENTO:
      Servo_Nuevos_Valores[RODILLA_R] = rodillaR_ini;
      Servos.WriteLineal(Servo_Nuevos_Valores, 100000.0);
    }
    else
    {
      Error_Func();
    }
    Serial.println("IK COMPLETADA...");
  }
}

void Support() //inclino hacia adelante para apoyar la pierna que ha ejecutado el paso
{
  if (PIERNA == DERECHA)
  {
    //PRIMERA FASE DEL MOVIMIENTO:
    Servo_Nuevos_Valores[CADERA_L] += SupportChange;
    Servos.WriteLineal(Servo_Nuevos_Valores, 100000.0);

    //SEGUNDA FASE DEL MOVIMIENTO:
    Servo_Nuevos_Valores[TOBILLO_L] = tobilloL_ini;
    Servo_Nuevos_Valores[TOBILLO_R] = tobilloR_ini;
    Servos.WriteLineal(Servo_Nuevos_Valores, 400000.0);

    PIERNA = IZQUIERDA; // cambio estado
  }
  else if (PIERNA == IZQUIERDA)
  {
    //PRIMERA FASE DEL MOVIMIENTO:
    Servo_Nuevos_Valores[CADERA_R] -= SupportChange;
    Servos.WriteLineal(Servo_Nuevos_Valores, 100000.0);

    //SEGUNDA FASE DEL MOVIMIENTO:
    Servo_Nuevos_Valores[TOBILLO_L] = tobilloL_ini;
    Servo_Nuevos_Valores[TOBILLO_R] = tobilloR_ini;
    Servos.WriteLineal(Servo_Nuevos_Valores, 400000.0);

    PIERNA = DERECHA; // cambio estado

  }
  else
  {
    Error_Func();
  }

  if (PASO == 10) //TRAS EL PRIMER PASO AUMENTO LA ZANCADA
  {
    PASO = 20;
  }
  Serial.println("FASE SUPPORT COMPLETADA...");
}

void Calculate_Step(double dist) //basicamente la regla de los catetos y la hipotenusa
{
  double x;
  x = sqrt(pow(Long_Pierna, 2.0) - pow(dist, 2.0));
  Serial.print("x ");
  Serial.println(x);

  Inverse_Kinematics(x, dist , Long_Cadera, Long_Rodilla);
}

// *** FUNCIONES GIRAR:
void Rotation_Right() //giro a la izq apoyando sobre esta
{
  PIERNA = IZQUIERDA;

  //PRIMERA FASE GIRO: (BALANCEO)
  Servo_Nuevos_Valores[TOBILLO_R] += (0.5 * SwingingChange);//0.7
  Servo_Nuevos_Valores[TOBILLO_L] += 1.1*SwingingChange;
  Servos.WriteLineal(Servo_Nuevos_Valores, 600000.0);
  
  // SEGUNDA FASE GIRO: (GIRO INICIAL)
  Servo_Nuevos_Valores[CADERA2_R] -= RotationAngle;
  Servos.WriteLineal(Servo_Nuevos_Valores, 500000.0);
  
  // TERCERA FASE GIRO: (APOYO)
  Servo_Nuevos_Valores[TOBILLO_L] = tobilloL_ini;
  Servo_Nuevos_Valores[TOBILLO_R] = tobilloR_ini;
  Servos.WriteLineal(Servo_Nuevos_Valores, 800000.0);
  
  //CUARTA FASE GIRO: (BALANCEO 2)
  Servo_Nuevos_Valores[TOBILLO_L] -= SwingingChange;
  Servo_Nuevos_Valores[TOBILLO_R] -= 1.5*SwingingChange;
  Servos.WriteLineal(Servo_Nuevos_Valores, 800000.0);
  Servo_Nuevos_Valores[TOBILLO_L] -= 0.4*SwingingChange;
  Servo_Nuevos_Valores[RODILLA_L] += 7;
  Servos.WriteLineal(Servo_Nuevos_Valores, 300000.0);
  
  // QUINTA FASE GIRO: (GIRO 2)
  Servo_Nuevos_Valores[CADERA2_R] = cadera2R_ini;
  Servos.WriteLineal(Servo_Nuevos_Valores, 800000.0);
  
  // SEXTA FASE GIRO: (APOYO FINAL)
  Servo_Nuevos_Valores[RODILLA_L] = rodillaL_ini;
  Servo_Nuevos_Valores[TOBILLO_R] = tobilloR_ini;
  Servo_Nuevos_Valores[TOBILLO_L] = tobilloL_ini;
  Servos.WriteLineal(Servo_Nuevos_Valores, 700000.0);

  PIERNA = DERECHA;
}
// *** END - FUNCIONES GIRAR:

void Error_Func()
{
  Serial.println("\t\t ERROR, EXITING... \n");

  // setea la posición inicial y corta el loop:
  Servos.RobotInitialize();
  exit(0);
}
