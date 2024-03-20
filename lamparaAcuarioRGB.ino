//#include "FS.h"
//#include "SPI.h"
//# include <SD.h>
#include "BluetoothSerial.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>


BluetoothSerial SerialBT;
TaskHandle_t task1;
TaskHandle_t task2;
TaskHandle_t task3;
String dispositivo= "IntelligentLigthPlus";
//-----------TIEMPO--------------------//
unsigned long previousMillis=0;
unsigned long previousMillisParametros=0;
unsigned long previousMillisIluminacion=0;
long interval = 1000;

// -------- ASIGNACION PINES -----------//
#define sondaTemp 13
//int ledUnicolor=26;
int ledRed=25;
int ledGreen=26;
int ledBlue=14;
int rele=27;
//------------ASIGNACION VARIABLES------------//
float numeroIntervalo;
int intervalo;
int valorRojo;
String valorRojoStr;
int valorGreen;
String valorGreenStr;
int valorBlue;
String valorBluesStr;
int programaDeIluminacion=1;
int luzRoja[]={255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};
int luzGreen[]={138,152,165,180,190,201,209,215,221,228,233,239,243,239,233,228,221,215,209,201,190,180,165,152,138};
int luzBlue[]={18,54,79,107,126,148,163,177,190,206,217,230,239,230,217,206,190,177,163,148,126,107,79,54,18};
int intervaloCambioColor=24;
int estadoRele=1;
int frecuenciaRed= 10000;
int resolucionRed=8;
int frecuenciaBlue= 10000;
int resolucionBlue=8;
int frecuenciaGreen= 10000;
int resolucionGreen=8;
int canalRed=0;
int canalGreen=1;
int canalBlue=2;
int canalUnicolor=3;
boolean multicolor=true;
char dataRx;
String dataRxst;
float temperatura = 0.0;
String temperaturaStr;
const char* temperaturaChar;
OneWire oneWire(sondaTemp);
DallasTemperature sensorTemperatura(&oneWire);
int temperaturaMaxima=30; 
const char* temperaturaMaximaChar;
String temperaturaMaximaStr;
int temperaturaMinima=24; 
const char* temperaturaMinimaChar;
String temperaturaMinimaStr;
int horaActual=21600;
//const char* horaActualChar;
String horaActualStr;
//int horaActualCopia=21600;
//const char* horaActualCopiaChar;
//String horaActualCopiaStr;
int horaActualON=21600;
const char* horaActualONChar;
String horaActualONStr;
int horaActualOFF=64800;
const char* horaActualOFFChar;
String horaActualOFFStr;
int intensidadInicial=50;
int intensidadMaxima=255;

void setup()
{
    Serial.begin(115200);
     //if(!SD.begin()){
       // Serial.println("Card Mount Failed");
        //return;
    //}
    pinMode(rele,OUTPUT);
    pinMode(4, OUTPUT);
    ledcSetup(canalRed,frecuenciaRed,resolucionRed);
    ledcSetup(canalGreen,frecuenciaGreen,resolucionGreen);
    ledcSetup(canalBlue,frecuenciaBlue,resolucionBlue);
    //ledcSetup(canalUnicolor,frecuencia,resolucion);
    //ledcAttachPin(ledUnicolor,canalUnicolor);
    ledcAttachPin(ledRed,canalRed);
    ledcAttachPin(ledGreen,canalGreen);
    ledcAttachPin(ledBlue,canalBlue);
    multicolor=true;
    SerialBT.begin(dispositivo);
    sensorTemperatura.begin();
    temperaturaMaximaStr=String(temperaturaMaxima);
    temperaturaMaximaChar=temperaturaMaximaStr.c_str();
    //escribirInt(SD, "/temperaturaMaxima.txt", temperaturaMaximaChar);	
    temperaturaMinimaStr=String(temperaturaMinima);
    temperaturaMinimaChar=temperaturaMinimaStr.c_str();
    //escribirInt(SD, "/temperaturaMinima.txt", temperaturaMinimaChar);
    horaActualStr=String(horaActual);
    //horaActualChar=horaActualStr.c_str();
    //escribirInt(SD, "/horaActual.txt",horaActualChar);
    //horaActualCopiaStr=String(horaActualCopia);
    //horaActualCopiaChar=horaActualCopiaStr.c_str();
    //escribirInt(SD, "/horaActualCopia.txt",horaActualCopiaChar);
    horaActualONStr=String(horaActualON);
    horaActualONChar=horaActualONStr.c_str();
    //escribirInt(SD,"/horaActualON.txt",horaActualONChar);
    horaActualOFFStr=String(horaActualOFF);
    horaActualOFFChar=horaActualOFFStr.c_str();
    //escribirInt(SD, "/horaActualOFF.txt",horaActualOFFChar);
    xTaskCreatePinnedToCore(recibirParametros, "recibirParametros", 10000, NULL, 1,  &task1, 0);
    xTaskCreatePinnedToCore(tiempoyTemperatura, "TiempoyTemperatura", 10000, NULL, 1,  &task2, 1);
    xTaskCreatePinnedToCore(controlIluminacion, "ControlIluminacion", 10000, NULL, 1,  &task3, 1);	
}




void tiempoyTemperatura(void *paremeter){
    for(;;){
        unsigned long currentMillis= millis();  

        if((currentMillis-previousMillis)>=interval){
            previousMillis=currentMillis;
            //horaActualStr=leerInt(SD,"/horaActual.txt");
        //horaActual=horaActualStr.toInt();
        horaActual=horaActual+1;
        
           // horaActualCopia=horaActual;
            //horaActualStr=String(horaActual);
            //horaActualChar=horaActualStr.c_str();
            //escribirInt(SD,"/horaActual.txt",horaActualChar);    
        
            
        
        Serial.print("horaActual: ");
        Serial.println(horaActual);
        if(horaActual>86400){
            horaActual=1;
            //horaActualStr=String(horaActual);
            //horaActualChar=horaActualStr.c_str();
            //escribirInt(SD,"/horaActual.txt",horaActualChar);  
        }
        
        
        }
        
    }
}

void recibirParametros(void *parameter){
    for(;;){
        unsigned long currentMillis= millis();
        if (SerialBT.available()){
            dataRx=SerialBT.read(); 
            Serial.print("dato Recibido: ");
            Serial.println(dataRx);
            
            vTaskDelay(100/portTICK_RATE_MS); 
            responderPreguntas();
            insertarValoresEnPlaca();
        }
        vTaskDelay(10/portTICK_RATE_MS);
    }
}
void controlIluminacion(void *parameter){
    for(;;){
         unsigned long currentMillis= millis();  

        if((currentMillis-previousMillisIluminacion)>=(interval*2)){
            previousMillisIluminacion=currentMillis;
            medirTemperatura();	
            intervalo=calcularIntervalo();
            cicloLampara();  
        }
    }
}

void loop()
{
	
}

int calcularIntervalo(){
    int tiempoON=horaActualOFF-horaActualON;
    float intervalos=tiempoON/24;
    if (horaActual>horaActualON && horaActual<horaActualOFF){
        numeroIntervalo=ceil((horaActual-horaActualON)/intervalos);
    }
    else if(horaActual==horaActualON){
        numeroIntervalo=1;
    }else if(horaActual==horaActualOFF){
        numeroIntervalo=24;
    }else{
        numeroIntervalo=0;
    }
    return numeroIntervalo;
}
void intervaloLuz(int numeroIntervalo){
    if ((horaActual-horaActualON)<300){
        ledcWrite(canalRed,(luzRoja[numeroIntervalo])/4);
        ledcWrite(canalGreen,(luzGreen[numeroIntervalo])/4);
        ledcWrite(canalBlue,(luzBlue[numeroIntervalo])/4);
        Serial.print("Valor Led Rojo: ");
        Serial.println((luzRoja[numeroIntervalo])/4);
    }
    if ((horaActual-horaActualON)>=300 && (horaActual-horaActualON)<600 ){
        ledcWrite(canalRed,luzRoja[numeroIntervalo]/3);
        ledcWrite(canalGreen,luzGreen[numeroIntervalo]/3);
        ledcWrite(canalBlue,luzBlue[numeroIntervalo]/3);
    }
    
    if ((horaActual-horaActualON)>=600 && (horaActual-horaActualON)<900 ){
        ledcWrite(canalRed,luzRoja[numeroIntervalo]/2);
        ledcWrite(canalGreen,luzGreen[numeroIntervalo]/2);
        ledcWrite(canalBlue,luzBlue[numeroIntervalo]/2);
    }
    if ((horaActual-horaActualON)>=900 && (horaActual-horaActualON)<1200 ){
        ledcWrite(canalRed,luzRoja[numeroIntervalo]/1.5);
        ledcWrite(canalGreen,luzGreen[numeroIntervalo]/1.5);
        ledcWrite(canalBlue,luzBlue[numeroIntervalo]/1.5);
    }
    if ((horaActual-horaActualON)>=1200){
        ledcWrite(canalRed,luzRoja[numeroIntervalo]);
        ledcWrite(canalGreen,luzGreen[numeroIntervalo]);
        ledcWrite(canalBlue,luzBlue[numeroIntervalo]);
    }
    if ((horaActualOFF-horaActual)>=900 && (horaActualOFF-horaActual)<1200 ){
        ledcWrite(canalRed,luzRoja[numeroIntervalo]/1.5);
        ledcWrite(canalGreen,luzGreen[numeroIntervalo]/1.5);
        ledcWrite(canalBlue,luzBlue[numeroIntervalo]/1.5);
    }
    if ((horaActualOFF-horaActual)>=600 && (horaActualOFF-horaActual)<900 ){
        ledcWrite(canalRed,luzRoja[numeroIntervalo]/2);
        ledcWrite(canalGreen,luzGreen[numeroIntervalo]/2);
        ledcWrite(canalBlue,luzBlue[numeroIntervalo]/2);
    }
    if ((horaActualOFF-horaActual)>=300 && (horaActualOFF-horaActual)<600 ){
        ledcWrite(canalRed,luzRoja[numeroIntervalo]/3);
        ledcWrite(canalGreen,luzGreen[numeroIntervalo]/3);
        ledcWrite(canalBlue,luzBlue[numeroIntervalo]/3);
    }
    if ((horaActualOFF-horaActual)<300){
        ledcWrite(canalRed,luzRoja[numeroIntervalo]/4);
        ledcWrite(canalGreen,luzGreen[numeroIntervalo]/4);
        ledcWrite(canalBlue,luzBlue[numeroIntervalo]/4);
    }

        
}
void intervaloPersonalizado(){
    ledcWrite(canalRed,valorRojo);
    ledcWrite(canalGreen,valorGreen);
    ledcWrite(canalBlue,valorBlue);

}
void intervaloFotosintesis(){
    Serial.println("PROGRAMA FOTOSINTESIS");
    ledcWrite(canalRed,255);
    ledcWrite(canalBlue,255);
    ledcWrite(canalGreen,0);

}
void cicloLampara(){
    int tiempoON=horaActualOFF-horaActualON;
    int intervalos=tiempoON/24;
    Serial.println("Ciclo Lmapara: ");
    Serial.println(programaDeIluminacion);
    Serial.println("HORA ON");
    Serial.println(horaActualON);
    if (horaActual>horaActualON && horaActual<horaActualOFF){
        if(programaDeIluminacion==1){
            intervaloLuz(intervalo);   
        }
        if(programaDeIluminacion==2){
           intervaloFotosintesis();
        }
        if(programaDeIluminacion==3){
            intervaloPersonalizado();
        }
    }
    else{
        ledcWrite(canalRed,0);
        ledcWrite(canalGreen,0);
        ledcWrite(canalBlue,0);
    }

}
/*
void escribirInt(fs::FS &fs, const char * path, const char* message){
    Serial.printf("Writing file: %s\n", path);
    int estadoEscritura=0;
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        //Serial.println("Failed to open file for writing");
        estadoEscritura=0;
        return;
    } else{
        estadoEscritura=1;
    }

    if(estadoEscritura==1){
        file.seek(0);
        file.print(message);
        //Serial.println("File written");
    }
    else {
        //Serial.println("Write failed");
    }
    file.close();
}*/

void insertarValoresEnPlaca(){
    if(dataRx=='b'){
        temperaturaMaximaStr=recibirVariosCaracteresBlue(3,'#');
        temperaturaMaxima=temperaturaMaximaStr.toInt();
        temperaturaMaximaStr=String(temperaturaMaxima);
        temperaturaMaximaChar=temperaturaMaximaStr.c_str();
        //escribirInt(SD,"/temperaturaMaxima.txt",temperaturaMaximaChar);
    }
    else if(dataRx=='c'){
        temperaturaMinimaStr=recibirVariosCaracteresBlue(3,'#');
        temperaturaMinima=temperaturaMinimaStr.toInt();
        temperaturaMinimaStr=String(temperaturaMinima);
        temperaturaMinimaChar=temperaturaMinimaStr.c_str();
        //escribirInt(SD,"/temperaturaMinima.txt",temperaturaMinimaChar);
    }
    else if(dataRx=='g'){
        horaActualStr=recibirVariosCaracteresBlue(7,'#');
        Serial.print("HORA DE LA LISTA ACTUALIZADA: ");
        Serial.println(horaActual);
        horaActual=horaActualStr.toInt();
        //horaActualStr=String(horaActual);
        //horaActualChar=horaActualStr.c_str();
        //escribirInt(SD, "/horaActual.txt",horaActualChar);
    }
    else if(dataRx=='k'){
        horaActualONStr=recibirVariosCaracteresBlue(7,'#');
        horaActualON=horaActualONStr.toInt();
        horaActualONStr=String(horaActualON);
        horaActualONChar=horaActualONStr.c_str();
        //escribirInt(SD,"/horaActualON.txt",horaActualONChar);
    }
    else if(dataRx=='l'){
        horaActualOFFStr=recibirVariosCaracteresBlue(7,'#');
        horaActualOFF=horaActualOFFStr.toInt();
        horaActualOFFStr=String(horaActualOFF);
        horaActualOFFChar=horaActualOFFStr.c_str();
        //escribirInt(SD,"/horaActualOFF.txt",horaActualOFFChar);
    }
    else if (dataRx=='n'){
        valorRojoStr=recibirVariosCaracteresBlue(4,'#');
        valorRojo=valorRojoStr.toInt();
        programaDeIluminacion=3;

    }
    else if (dataRx=='o'){
        valorGreenStr=recibirVariosCaracteresBlue(4,'#');
        valorGreen=valorGreenStr.toInt();
        programaDeIluminacion=3;
    }
    else if (dataRx=='p'){
        valorBluesStr=recibirVariosCaracteresBlue(4,'#');
        valorBlue=valorBluesStr.toInt();
        programaDeIluminacion=3;
    }
    else if (dataRx=='q'){
        programaDeIluminacion=1;
    }
    else if (dataRx=='s'){
        programaDeIluminacion=2;
    }

}
/*
String leerInt(fs::FS &fs, const char * path)
{   
    String cadena;
    char parametro;
    int longitud;
    int estadoLectura=0;
    int posicion=0;
    File file = fs.open(path, FILE_READ);
     if(!file){
        //Serial.println("Failed to open file for writing");
        estadoLectura=0;
        return cadena;
    } else{
        estadoLectura=1;
    }
    if(estadoLectura==1){
        longitud=file.size();
        while(file.available()){
            while (posicion<longitud)
            {
                parametro=file.read();
                cadena=cadena+parametro; 
                posicion++;       
            }      
        //Serial.println("File read");
        }
    }
    else {
        //Serial.println("Read failed");
    }
    file.close();
    return cadena;
}
*/
void medirTemperatura(){
    
    sensorTemperatura.requestTemperatures();
    temperatura=sensorTemperatura.getTempCByIndex(0);
    if(temperatura<0 || temperatura>100){
       Serial.println("error en lectura");
    } else{
        temperaturaStr=String(temperatura);
    temperaturaChar=temperaturaStr.c_str();
    //escribirInt(SD,"/temperatura.txt",temperaturaChar);
    //temperaturaMinimaStr=leerInt(SD,"/temperaturaMinima.txt");
    //temperaturaMinima=temperaturaMinimaStr.toInt();
    //temperaturaMaximaStr=leerInt(SD,"/temperaturaMaxima.txt");
    //temperaturaMaxima=temperaturaMaximaStr.toInt();
    //Serial.print("Temperatura Maxima: ");
    //Serial.println(temperaturaMaxima);
    //Serial.print("Temperatura Minima: ");
    //Serial.println(temperaturaMinimaStr);
    if(temperatura< float(temperaturaMinima) && temperatura< float(temperaturaMaxima)){
        estadoRele=1;}
    else if(estadoRele==1 && temperatura<float(temperaturaMaxima)){
        estadoRele=1;
    }
    else if(temperatura>float(temperaturaMaxima)){
        estadoRele=0;
    }
    if(estadoRele==1){
        digitalWrite(rele, HIGH);
        Serial.println("RELE ENCENDIDO");
    } else if(estadoRele==0){
        digitalWrite(rele,LOW);
        Serial.println("RELE APAGADO");
    }
    }
    
    Serial.print("Temperatura: ");
    Serial.println(temperatura);
    //delay(1000);
}

int recibirVariosCaracteresBlue(int longitudLista, char caracterDetenerStr){
  /*   String cadena="";
     int cadenaNumerica;
    dataRx=SerialBT.read();
    cadena+=String(dataRx);
    while (dataRx!= caracterDetenerStr){
        dataRx=SerialBT.read();
        if (dataRx!= caracterDetenerStr){
            cadena+=String(dataRx);
        }
        
    }
    
    cadenaNumerica=cadena.toInt();
    Serial.print("Cadena Numerica: ");
    Serial.println(cadenaNumerica);

    return cadenaNumerica;
*/
    
  int contador=0;
  dataRx=SerialBT.read();
    Serial.print("Dato para la lista: ");
    Serial.println(dataRx);
  int longitud=longitudLista+1;  
  dataRxst=String(dataRx); 
  char lista[longitud]; 
  char caracterDetener[2];
  lista[contador]=dataRx; 
  contador++;
  vTaskDelay(100/portTICK_RATE_MS);
  //caracterDetenerStr.toCharArray(caracterDetener,2);
  while (dataRx!= caracterDetenerStr){
    dataRx=SerialBT.read();
    Serial.print("Dato para la lista: ");
    Serial.println(dataRx);
    //dataRxst=String(dataRx); 
    if (dataRx!= caracterDetenerStr){
        
            lista[contador]=dataRx;  
            Serial.print ("Lista: ");
            Serial.println (lista);
        
      } 
    contador++; 
    vTaskDelay(100/portTICK_RATE_MS);          
  } 
  String respuestaUnida=lista;
  Serial.print("Respuesta UNIDA: ");
  Serial.println(respuestaUnida);
  int i;
  int posicionCadena=0;
  String cadena;
  String cadenaDefinitiva;
  String cadenaRespuesta;
  int cadenaNumerica;
  for (i=0;i<=longitud;i++){
      cadena=cadena+respuestaUnida[posicionCadena];
      if (cadena.toInt()){
          posicionCadena++;
          cadenaDefinitiva=cadena;
          cadenaNumerica=cadenaDefinitiva.toInt();
      }
  }
  Serial.print("Lista Definitiva: ");
  Serial.println(cadenaNumerica);
  return   cadenaNumerica;  
} 

void responderPreguntas(){ //en esta función se pregunta por los valores y se le responde a la app el valor actual de cada pregunta
    Serial.println("INICIA PRUEBA FUNCION RESPONDER PREGUNTAS"); 
    if(dataRx=='a'){
       //temperaturaStr=String(temperatura);//leerInt(SD, "/temperatura.txt"); 
       SerialBT.print(temperaturaStr);
       Serial.print("Temperatura enviada: ");
       Serial.println(temperaturaStr);

    }
    else if(dataRx=='e'){
        temperaturaMaximaStr=String(temperaturaMaxima);//leerInt(SD,"/temperaturaMaxima.txt");
        SerialBT.print(temperaturaMaximaStr);
        Serial.print("temperaturaMAXIMA enviada: ");
       Serial.println(temperaturaMaximaStr);
    }
    else if(dataRx=='f'){
        temperaturaMinimaStr=String(temperaturaMinima);//leerInt(SD,"/temperaturaMinima.txt");
        SerialBT.print(temperaturaMinimaStr);
        Serial.print("temperaturaMINIMA enviada: ");
       Serial.println(temperaturaMinimaStr);
    }
    else if(dataRx=='H'){
        horaActualStr=String(horaActual);// leerInt(SD,"/horaActual.txt");
        SerialBT.print(horaActualStr);
        Serial.print("HORA ACTUAL enviada: ");
       Serial.println(horaActualStr);
    }
    else if(dataRx=='i'){
        horaActualONStr=String(horaActualON);//leerInt(SD,"/horaActualON.txt");
        SerialBT.print(horaActualONStr);
    }
    else if(dataRx=='J'){
        horaActualOFFStr=String(horaActualOFF);//leerInt(SD,"/horaActualOFF.txt");
        SerialBT.print(horaActualOFFStr);
    }
    else if(dataRx=='m'){
        if (multicolor==true){
            SerialBT.print("1");
        }
        else{
            SerialBT.print("0");
        }
    }
    }



