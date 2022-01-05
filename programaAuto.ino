//Include the graphics library.
#include "U8glib.h" //Initialize display.
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0); // Pantalla a Utilizar

#define BTN 7
#define LED 13
//#define ledPrincipal 13
unsigned long signal_len,t1,t2;   //Variables para el tiempo que se presiona un botón

void setup(){
  pinMode(LED,OUTPUT); // lED
  pinMode(BTN,INPUT); // boton
}

boolean bandera = true;
String code = "";
String pw = "";
byte caracter=1;

void loop(){
  if(bandera){
    digitalWrite(LED, HIGH);
    delay(4000);
    digitalWrite(LED, LOW);
    bandera = false;
  }
NextDotDash:
      while (digitalRead(BTN) == LOW) {}
      t1 = millis();                            //Tiempo al presionar el botón
      //digitalWrite(LED, HIGH);               //LED encendido mientras se presiona el botón
      while (digitalRead(BTN) == HIGH) {}
      t2 = millis();                            //Tiempo en que no está presionado el botón
      //digitalWrite(LED, LOW);                //LED apagado cuando no está presionado el botón
      signal_len = t2 - t1;                     //Tiempo total en que es presionado el botón
      if (signal_len > 50)                      //Toma en cuenta el anti rebote del botón
      {
        code += readio();                       //Función para leer punto o guión
      }
      while ((millis() - t2) < 500)           //Si el tiempo entre punto o guión ingresado es mayor a 0.5 segundos, termina el bucle y recibe el siguiente alfabeto
      {     
        if (digitalRead(BTN) == HIGH)
        {
          goto NextDotDash;
        }
      }
      
      
      convertor();                          //Función para descifrar el código en el alfabeto
      
      if(pw == "R"){
        caracter=2;
        digitalWrite(LED, HIGH);
        delay(500);
        digitalWrite(LED, LOW);
      }
      if(pw == "Re"){
        caracter=3;
        digitalWrite(LED, HIGH);
        delay(500);
        digitalWrite(LED, LOW);
      }
      if(pw == "ReI"){
        caracter=4;
        digitalWrite(LED, HIGH);
        delay(500);
        digitalWrite(LED, LOW);
      }
      if(pw == "ReIS"){
        digitalWrite(LED, HIGH);
      }
}

//Función para la lectura de puntos o guiones
char readio()
{
  if (signal_len < 600 && signal_len > 50)
  {
    return '.';                        //Si el botón es presionado menos de 0.6 segundos, será un punto
  }
  else if (signal_len > 600)
  {
    return '-';                        //Si el botón es presionado más de 0.6 segundos, será un guión
  }
}

//Función para convertir puntos y guiones en alfabeto
void convertor()
{
  static String letters[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
                             ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "E"
                            }; //Combinaciones para las letras
                            
  static String alfabeto[] = {"A", "B", "C", "D", "e", "F", "G", "H", "I", "J", "K", "L", "M", "N", "Ñ", "O", "P",
                             "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "E"
                            }; //letras
  int i = 0;
  if (code == ".-.-.-") //Si la secuencia es la anterior, imprimirá en pantalla un punto
  {
    Serial.print(".");
  }
  else if (code == "-.-.-.") //Si la secuencia es la anterior, imprimirá en pantalla un espacio
  {
    Serial.print(" ");
  }
  else
  {
    while (letters[i] != "E")  //Ciclo para comparar las letras del arreglo
    {
      if (letters[i] == code) //Revisa la posición en variable "letters" y la compara con el alfabeto llamado "code", mientras avanza la variable "i"
      {
        Serial.print(char('A' + i)); //Recorre el alfabeto que todos conocemos desde la "A" hasta la "Z" con la varible "i"
        u8g.firstPage(); // INICIALIZA LA PANTALLA
        do {
          u8g.setFont(u8g_font_6x10);
        pw += alfabeto[i];
        pw.remove(caracter,pw.length()-1);
          u8g.drawStr(0, 20, alfabeto[i].c_str());
          u8g.drawStr(0, 60, pw.c_str());
        } while (u8g.nextPage());
        break; //En cuanto encuentra la letra se sale del ciclo
      }
      i++;
    }
    if (letters[i] == "E") //Si el alfabeto ingresado no es ninguna de las combinaciones anteriores, entonces será "E"
    {
      Serial.println(" ");  //En este caso la "E" será ENTER o salto de línea
    }
  }
  code = "";                            //Reseteamos el alfabeto esperando que se ingrese un nuevo caracter
}
