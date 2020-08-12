#include<LiquidCrystal.h>
#include<TinyGPS.h>
LiquidCrystal lcd(22,23,24,25,26,27);
TinyGPS gps;
int count=0,i=0,alen=0,csc=1,dsc=1,j=0,f=0;
char a[80], b[]="+CMT", c[20], d[]="$1", msg[20];
void CountDisp(int);
void GPSLoc(char );
void GPSLoc(char ack)
{
 float lat, lon;
 while(Serial1.available())
 {
  if(gps.encode(Serial1.read()))
  { 
   gps.f_get_position(&lat,&lon);
  }
 }
 String latitude = String(lat,6);
 String longitude = String(lon,6);
 Serial.println("AT+CMGF=1");
 delay(500);
 Serial.println("AT+CMGS=\"8129127297\"\r");
 delay(500);
 Serial.println(ack);
 delay(500);
 Serial.println(latitude+";"+longitude);
 delay(500);
 Serial.println((char)26);
 delay(500);
}
void CountDisp(int x)
{
 lcd.setCursor(0,0);
 lcd.print("Passengers : ");
 lcd.setCursor(13,0);
 lcd.print(x);
 delay(1000);
}

void setup()
{
 pinMode(12,INPUT); // IR 1
 pinMode(11,INPUT); // IR 2
 pinMode(10,INPUT); // Door Switch
 pinMode(9,OUTPUT); // Motor 1
 pinMode(8,OUTPUT); // Motor 2
 lcd.begin(16,2);
 Serial.begin(9600);
 Serial1.begin(9600);
 delay(12000);
 Serial.println("ATE0");
 Serial.println("AT+CMGF=1");
 Serial.println("AT+CNMI=2,2,0,0,0");
 delay(1000);
}

void loop()
{ 
 if(digitalRead(12)==0) // Checking entry of person
 {
  //delay(500); 
  while(digitalRead(12)==0);
  if(digitalRead(11)==0)
  {
    while(digitalRead(12)==0);

   count++; // Count incremented
  }
 }
 if(digitalRead(11)==0) // Checking exit of person
 {
  //delay(500);
  while(digitalRead(11)==0);
  if(digitalRead(12)==0)
  {
    while(digitalRead(12)==0);
   count--; // Count decremented
  }
 }
 if(count<10)
 {
  CountDisp(count);
  f=0;
 }
 else
 {
  f=1; // Flag variable set if count=10
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Bus Full");
  delay(1000);
 }

 if(digitalRead(10)==1) // Checks Door switch press
 {
  while(digitalRead(10)==1);
  if(f==0) // Checks Flag variable ie if count<10
  {
   digitalWrite(9,1);
   digitalWrite(8,0); // Door opens
   delay(2000);
   digitalWrite(9,0);
   digitalWrite(8,0); // Remains open
   delay(5000);       // for 5 seconds
   digitalWrite(9,0);
   digitalWrite(8,1); // Door closes
   delay(2000);
   digitalWrite(9,0);
   digitalWrite(8,0); // Remains closed
   delay(1000);
  }
 }
 
 while(Serial.available()>0) // Reading SMS
 {
  a[i]=Serial.read(); 
  i++;
 }
 alen=strlen(a);
 Serial.println(a);
 
 for(j=0,i=2;i<=5;i++,j++)
 {
  c[j]=a[i];
 }
 Serial.println(c);
 csc=strcmp(c,b);
 if(csc==0)
 {
  csc=1;
  for(i=51,j=0;i<53;i++,j++)
  {
   msg[j]=a[i];
  }
  Serial.println(msg);
 }
 dsc=strcmp(msg,d);//d=$1
 if(dsc==0)
 {
  dsc=1;
  if(count<10)
  {
   GPSLoc('#'); // Boarding point confirmation symbol
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("BOARDING POINT A");
   delay(2000);
  }
  else
  {
   GPSLoc('+'); // Bus full symbol
  }
 }
}
