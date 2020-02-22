#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS A1
OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);
DeviceAddress Thermo1, Thermo2, Thermo3;

#include <LiquidCrystal.h>
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

#include <UIPEthernet.h>
EthernetClient client;
  signed long next;
  String Datar="";
  long count;
  String dev_id="0";
  
void setcon(){
  uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x08};
  Ethernet.begin(mac);
  lcd.setCursor(0, 0);
  lcd.print("IP:");
  lcd.setCursor(3, 0);
  lcd.print(Ethernet.localIP()); 
  Serial.print("subnetMask: ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("gatewayIP: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("dnsServerIP: ");
  Serial.println(Ethernet.dnsServerIP());
  next = 0;

}
   

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("                  ");
  setcon();
  Serial.println("Dallas Temperature IC Control Library Demo");

  sensors.begin();
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  if (!sensors.getAddress(Thermo1, 0)) Serial.println("Unable to find address for Device 0"); 
  if (!sensors.getAddress(Thermo2, 1)) Serial.println("Unable to find address for Device 1"); 
  if (!sensors.getAddress(Thermo3, 2)) Serial.println("Unable to find address for Device 2"); 

  Serial.print("Device 0 Address: ");
  printAddress(Thermo1);
  Serial.println();
  Serial.print("Device 1 Address: ");
 
  printAddress(Thermo2);
  Serial.println();
  Serial.print("Device 2 Address: ");
  printAddress(Thermo3);
  Serial.println();

}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)

{
  float tempC = sensors.getTempC(deviceAddress);
  Datar=tempC;
  lcd.setCursor(8, 1);
  lcd.print(Datar);
 }

void printResolution(DeviceAddress deviceAddress)
{
  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();    
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  Serial.print("Device Address: ");
  printAddress(deviceAddress);

  
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}

void loop(void)
{ 
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");
  
  // print the device information
  printData(Thermo1);
  dev_id=3;
  lcd.setCursor(1, 1);
  lcd.print("TEMP 1:");
  Kirim(Datar);
  delay(2000);
  
  printData(Thermo2);
  dev_id=4;
  lcd.setCursor(1, 1);
  lcd.print("TEMP 2:");
  Kirim(Datar);
  delay(2000);
  
  printData(Thermo3);
  dev_id=5;
  lcd.setCursor(1, 1);
  lcd.print("TEMP 3:");
  Kirim(Datar);
  delay(2000);
}
void Kirim(String Data){
      if (client.connect(IPAddress(10,0,40,203),1314))
        {
          //Serial.println("Client connected");
          client.print(dev_id+"|"+Data);
          Serial.println(dev_id+"|"+Data);
          lcd.setCursor(0, 1);
          lcd.print("<");
          count=0;
          digitalWrite(3, LOW);
        }
      else{
        Serial.println("Client connect failed");
        count=count+1;
        if (count==1){
          lcd.setCursor(0, 1);
          lcd.print(count);
          digitalWrite(3, LOW);
        }else if (count==2){
          lcd.setCursor(0, 1);
          lcd.print(count);
          digitalWrite(3, HIGH);
        }else if (count==3){
          lcd.setCursor(0, 1);
          lcd.print(count);
          digitalWrite(3, LOW);
        }else if (count==4){
          lcd.setCursor(0, 1);
          lcd.print(count);
          digitalWrite(3, HIGH);
        }else if (count==5){
          lcd.setCursor(0, 1);
          lcd.print(count);
          digitalWrite(3, LOW);        
        }
        Serial.println(count);
        if (count==5){
          setcon();  
          count=0;
        }
      }
}
