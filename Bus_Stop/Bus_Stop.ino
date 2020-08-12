#include<LiquidCrystal.h>
LiquidCrystal lcd(7,6,5,4,3,2);
int i=0,j=0,alen=0,csc=1;
char a[100], b[]="+CMT", c[20], d[20], msg[20];
void SendMsg();
void SendMsg()
{
 Serial.println("AT+CMGF=1");
 delay(500);
 Serial.println("AT+CMGS=\"8129127297\"\r");
 delay(500);
 Serial.println("$1");
 delay(500);
 Serial.println((char)26);
 delay(500);
}
void setup()
{
 pinMode(12,INPUT);
 Serial.begin(9600);
 lcd.begin(16,2);
 delay(12000);
 Serial.println("ATE0");
 Serial.println("AT+CMGF=1");
 Serial.println("AT+CNMI=2,2,0,0,0");
 delay(1000);
}

void loop()
{
 if(digitalRead(12)==1)
 {
  while(digitalRead(12)==1);
  SendMsg();
 }
 
 while(Serial.available()>0)
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
  for(i=51,j=0;i<alen-1;i++,j++)
  {
   msg[j]=a[i];
  }
  Serial.println(msg);
  if(msg[0]=='#')
  {
   lcd.setCursor(0,0);
   lcd.print("Board Confirmed");
   lcd.setCursor(0,1);
   for(i=1,j=0;i<alen-1-51-2;i++,j++)
   {
    d[j]=msg[i];
   }
   lcd.print(d);
   delay(2000);
  }
  if(msg[0]=='+')
  {
   lcd.setCursor(0,0);
   lcd.print("BUS FULL");
   delay(2000);
  } 
 }
}
