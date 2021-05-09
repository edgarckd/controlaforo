#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>			// libreria para bus I2C
#include <Adafruit_GFX.h>		// libreria para pantallas graficas
#include <Adafruit_SSD1306.h>		// libreria para controlador SSD1306
#include <Servo.h>


#define ANCHO 128			// reemplaza ocurrencia de ANCHO por 128
#define ALTO 64				// reemplaza ocurrencia de ALTO por 64

#define SS_PIN 53
#define RST_PIN 5
 
#define OLED_RESET 	4		// necesario por la libreria pero no usado
Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET);	// crea objeto
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];
byte data_base[4][5] = {{0x83,0x96,0xde,0x18,0x0},{0x19,0x33,0x92,0x98,0x0},{0xca,0x3,0xa4,0x24,0x3},{0xd3,0x24,0x3d,0x16,0x0}};

int aforo = 0;
int aforo_max = 2;

Servo puerta; 

void setup(){
  puerta.attach(13);
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  
  Wire.begin();					// inicializa bus I2C
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);	// inicializa pantalla con direccion 0x3C
  oled.clearDisplay();  
}
void loop(){
  imprimir_texto("Esperando...");
  if ( ! rfid.PICC_IsNewCardPresent())
    return; 
  if ( ! rfid.PICC_ReadCardSerial())
    return;
      
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);        
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }     
  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  

}



void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void imprimir_texto(String mensaje){
  oled.clearDisplay();  
  oled.setTextColor(WHITE);		// establece color al unico disponible (pantalla monocromo)
  oled.setTextSize(1);	
  oled.setCursor(0,0);
  oled.print(mensaje);
  oled.display();
}

void imprimir_ID(byte nuidPICC[4]){
  for(int i=0; i < 4;i++){  
    oled.setTextColor(WHITE);
    int m = (i*20)+20;
    oled.setTextSize(1);	
    oled.setCursor(m,20);
    String mensaje = String(nuidPICC[i], HEX);
    oled.print(mensaje);
    oled.display();     
  }
  delay(3000);
}

void Ingresando(int ubi){
  data_base[ubi][4] == 0x1;
  imprimir_texto("ingresando...");
  delay(3000);
  imprimir_texto("abriendo puerta...");  
  for(int i = 128; i > 0; i-- ){
    puerta.write(i);
    delay(25);

  }
  imprimir_texto("cerrando, cuidado...");  
  for(int i = 0; i < 128; i++ ){
    puerta.write(i);
    delay(25);
    
  }  
  
}

void Saliendo(int ubi){
  data_base[ubi][4] = 0x0;
  imprimir_texto("saliendo...");
  delay(3000);  
  imprimir_texto("abriendo puerta...");     
  for(int i = 128; i < 255; i++ ){
    puerta.write(i);
    delay(25);
    
  }
  imprimir_texto("cerrando, cuidado...");  
  for(int i = 255; i > 128; i-- ){
    puerta.write(i);
    delay(25);
    
  }  
  
}





























