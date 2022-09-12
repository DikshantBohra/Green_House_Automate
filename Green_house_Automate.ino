#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <Wire.h>
#include <DS3231.h>

// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);
#define ONE_WIRE_BUS 2                    //TEMPERATURE
#define DHTPIN 7                          //DHT22
//#define anInput A3                        //MQ135
int LDRSensor = 3;                        //LDR SENSOR
int sensor_pin = A2;                      //MOISTURE


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#define co2Zero     55                        //calibrated CO2 0 level
//Constants
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino



// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//........................................................PIN DEFINE...........................................


int water = 8;                            //WATERING
int hot = 6;                              //HOW AIR
int cold = 10;                             //FAN
int FL = 11;                              //FLUORESCENT LIGHT
int water1=12;
int cold1= 5;
int fan=4;  
Time  t;


// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//........................................................VARIABLE DEFINE...........................................

//int carbonlvl;
int temp;
int temp2;
int humid;
int light;
int moist;
int timeh;
int timehs;



void setup() {

  sensors.begin();
  dht.begin();
  pinMode (LDRSensor, INPUT);
//  pinMode(anInput,INPUT);                     //MQ135 analog feed set for input

  pinMode (water, OUTPUT);
  pinMode (hot, OUTPUT);
  pinMode (cold, OUTPUT);
  pinMode (FL, OUTPUT);
  pinMode (water1, OUTPUT);
  pinMode (cold1, OUTPUT);
  digitalWrite(hot,HIGH);
  digitalWrite(cold,HIGH);
  digitalWrite(water,HIGH);
  
  
  Serial.begin(9600); 
  rtc.begin();
  //serial comms for debuging
// The following lines can be uncommented to set the date and time
  //rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
//  rtc.setTime(16,59,0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(1, 1, 2014);   // Set the date to January 1st, 2014


}


void loop() {
    timeh=times();
    timehs=timehd();
//     timeh=3;
    Serial.print(timeh);
//    carbonlvl=carbon();
    temp=wtemp();
    temp2=tempa();
    humid=humi();
    light=LDR(); 
    moist=moisture();
// --------------------------------------------------------------------------------------------------------------------------------------------
//......................................ALGO.................................
//.........................................LIGHT............................
   
   
   if(light==1 && timeh>4 && timeh<24 ){
    digitalWrite(FL,LOW);
   }
   else if(light==0 && timeh>4 && timeh<24 ){
    digitalWrite(FL,HIGH);
   }
   else if(timeh<=4){
    digitalWrite(FL,HIGH);
   }
 //.........................................Temperature............................
   //..........................DAY......................

   
   if(timeh>5 && timeh<20 )
   {
    if(temp<20){
     digitalWrite(hot,LOW);
    }
    else if(temp>30)
    {
      digitalWrite(cold,LOW); 
      Serial.print("cold");    
    }
     else if(temp>=22 && temp<=26)
     {
     digitalWrite(hot,HIGH);
     digitalWrite(cold,HIGH);
    }
   }

   
   //........................night.....................
   
  if(timeh<5 || timeh>20 ){
    if(temp<18)
    {
     digitalWrite(hot,LOW);
     }
    else if(temp>30)
    {
      digitalWrite(cold,LOW);      
    }
      else if(temp>=22 && temp<=27)
     {
     digitalWrite(hot,HIGH);
     digitalWrite(cold,HIGH);
    }
  }

  //...............................................Water.................................................


   if( timeh%2==0 && timehs==1 && timeh>3 && timeh<22 ){
    digitalWrite(water,LOW);
    Serial.print("h1");
    delay(30000);
    digitalWrite(water,HIGH);
    Serial.print("loq");
    delay(30000);
   }

//   if( moist<=15 && timeh>3 && timeh<22 ){
//    digitalWrite(water,LOW);
//   }
//   else  if(moist>=19 && timeh>3 && timeh<22 ){
//    digitalWrite(water,HIGH);
//   }

   //...............................................HUMIDITY.................................................

   if( humid<=40 && timeh>3 && timeh<22 ){
    digitalWrite(water1,LOW);
   }
   else  if(humid>=70 && timeh>3 && timeh<22 ){
    digitalWrite(cold1,LOW);
   }
   else if (humid>40 && humid<70  && timeh>3 && timeh<22){
    digitalWrite(water1,HIGH);
    digitalWrite(cold1,HIGH);
   }

   
    Serial.print("----------------------------------------------------\n\n");
    delay(3000);
//...............................................C02.................................................

   
}
//-------------------------------------------------------------------------------------------------------------------------------------

//....................................TIME SENSOR.............................................

int times(){
   t = rtc.getTime();
//  Serial.print(t.hour);
//  Serial.print(" hour(s), ");
//  Serial.print(t.min);
//  Serial.print(" minute(s) and ");
//  Serial.print(t.sec, DEC);
  
  int ti=t.hour;

   return ti;
}

int timehd(){
   t = rtc.getTime();
  Serial.print(t.hour);
  Serial.print(" hour(s), ");
  Serial.print(t.min);
  Serial.print(" minute(s) and ");
  Serial.print(t.sec, DEC);
  
  int tm=t.min;

   return tm;
}


//-------------------------------------------------------------------------------------------------------------------------------------

//....................................MOISTURE SENSOR.............................................

int moisture() {

   int mos= analogRead(sensor_pin);
   Serial.print(mos);

   mos = map(mos,1024,530,0,20);

   Serial.print("Mositure : ");

   Serial.print(mos);
   Serial.print(" \n");

   return mos;

   }


//-------------------------------------------------------------------------------------------------------------------------------------

//....................................LDR SENSOR.............................................


int LDR()
 {
  int Sensordata = digitalRead (LDRSensor);
  Serial.print("LIGHT :");
  Serial.println(Sensordata);
  Serial.print("  .\n");

  return Sensordata;
 }


//-------------------------------------------------------------------------------------------------------------------------------------

//....................................TEMPERATURE AIR SENSOR.............................................

int tempa()
{
    float temp; //Stores temperature value
    temp= dht.readTemperature();
    //Print temp values to serial monitor
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius\n");

    return temp;    
}

//-------------------------------------------------------------------------------------------------------------------------------------

//....................................HUMIDITY SENSOR.............................................

int humi()
{
    float hum;  //Stores humidity value
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity:");
    Serial.print(hum);
    Serial.print(" \n");

    return hum;
}

//-------------------------------------------------------------------------------------------------------------------------------------

//....................................WATERPROOF TEMPERATURE SENSOR.............................................


int wtemp(){
 
  // Send the command to get temperatures
  sensors.requestTemperatures(); 

  //print the temperature in Celsius
  Serial.print("Temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
//  Serial.print((char)176);//shows degrees character
  Serial.print("C  |  \n");
  
  return sensors.getTempCByIndex(0);
}
