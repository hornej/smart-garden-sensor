#include <Adafruit_TSL2561_U.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>


/* How to use the DHT-22 sensor with Arduino uno
   Temperature and humidity sensor
*/


//Constants
#define DHTPIN 3     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino


//Variables
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
int val = 0; //value for storing moisture value
int soilPin = A0;//Declare a variable for the soil moisture sensor
int soilPower = 7;//Variable for Soil moisture Power

void setup()
{
  Serial.begin(9600);
  dht.begin();
  pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
  if (!tsl.begin())
  {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
}

void loop()
{
  delay(2000);
  /* Get a new light sensor event */
  sensors_event_t event;
  tsl.getEvent(&event);
  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  //Print temp and humidity values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print(" %, Temp: ");
  Serial.print(temp);
  Serial.print(" Celsius, ");
  Serial.print("Soil Moisture: ");
  Serial.print(readSoil());
  if (event.light)
  {
    Serial.print(", Light: ");
    Serial.print(event.light); 
    Serial.println(" lux");
  }
  else
  {
    /* If event.light = 0 lux the sensor is probably saturated
       and no reliable data could be generated! */
    Serial.println("Sensor overload");
  }
  delay(1000); //Delay 2 sec.
}

int readSoil()
{
  digitalWrite(soilPower, HIGH);//turn D7 "On"
  delay(10);//wait 10 milliseconds
  val = analogRead(soilPin);//Read the SIG value form sensor
  digitalWrite(soilPower, LOW);//turn D7 "Off"
  return val;//send current moisture value
}

