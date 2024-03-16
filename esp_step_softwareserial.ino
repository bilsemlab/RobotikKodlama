String agAdi = "RobotikLab";                   //Ağimizin adini buraya yaziyoruz.
String agSifresi = "12345678";             //Ağimizin şifresini buraya yaziyoruz.
int motorPin1 = 3, motorPin2 = 4, motorPin3 = 5, motorPin4 = 6; //Step motor pinlerini tanimliyoruz.
#include <SoftwareSerial.h>

int rxPin = 10;                                               //ESP8266 RX pini
int txPin = 11;

SoftwareSerial esp(rxPin, txPin);

void setup() {

  pinMode(motorPin1, OUTPUT);                                   //Motor pinleri OUTPUT olarak ayarlanir.
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  //Step Motor Kontrolü
  for (int adim = 0; adim < 150; adim++)
    stepileri(5);
  delay(1000);

  Serial.begin(9600);  //Seri port ile haberleşmemizi başlatiyoruz.

  esp.begin(115200);                                         //Seri haberleşmeyi başlatiyoruz.
  esp.println("AT");                                         //AT komutu ile modül kontrolünü yapiyoruz.
  Serial.println("AT Yollandi");
  while (!esp.find("OK")) {                                  //Modül hazir olana kadar bekliyoruz.
    esp.println("AT");
    Serial.println("ESP8266 Araniyor.");
  }

  Serial.println("OK Komutu Alindi");
  esp.println("AT+CWMODE=1");                                 //ESP8266 modülünü client olarak ayarliyoruz.
  while (!esp.find("OK")) {                                   //Ayar yapilana kadar bekliyoruz.
    esp.println("AT+CWMODE=1");
    Serial.println("Ayar Yapiliyor....");
  }
  Serial.println("Client olarak ayarlandi");
  Serial.println("Aga Baglaniliyor...");
  esp.println("AT+CWJAP=\"" + agAdi + "\",\"" + agSifresi + "\""); //Ağimiza bağlaniyoruz.
  while (!esp.find("OK"));                                    //Ağa bağlanana kadar bekliyoruz.
  Serial.println("Aga Baglandi.");
  delay(1000);


  esp.print("AT+CiFSR\r\n");                                 //iP adresini ve MAC adresini okuyoruz.
  Serial.print(espOkuma(1000));                                 //iP ve MAC adresini ekrana yaziyoruz.
  espTemizle(2000);                                          //Seri haberleşmede kullanmadiğimiz verileri temizliyoruz.
  esp.print("AT+CiPMUX=1\r\n");                              //Çoklu bağlanti ayarini yapiyoruz.
  espTemizle(2000);
  esp.print("AT+CiPSERVER=1,80\r\n");                        //Server oluşturuyoruz ve 80. porttan dinlemeye başliyoruz.
  espTemizle(2000);



}
void loop() {
  if (esp.available()) {                                     //Seri haberleşme verisi geldiğinde içindeki komutlar çalişir.
    if (esp.find("+iPD,")) {                                 //Bağlanti isteği geldiğinde içindeki komutlar çalişir.
      delay(200);
      int connectionid = esp.read() - 48;                    //Bağlanti numarasini okuyoruz.
      String komut = espOkuma(1000);                            //Bağlanti komutlarini aliyoruz.
      if (komut.indexOf("step=ileri") != -1) {                  //İleri komutu geldiğinde içerisindeki komutlar çalişir.
        for (int adim = 0; adim < 150; adim++) {                  //Step motora 5 kere ileri hareketi yaptirilir.
          Serial.println("ileri komutu alindi");
          stepileri(5);
        }
      }
      else if (komut.indexOf("step=geri") != -1) {              //Geri komutu geldiğinde içerisindeki komutlar çalişir.
        for (int adim = 0; adim < 150; adim++) {                  //Step motora 5 kere geri hareketi yaptirilir.
          Serial.println("geri komutu alindi");
          stepGeri(5);
        }
      }
      String sayfa = "<h1>Step Motor Kontrol</h1><br>";
      sayfa += "<br><a href=\"?step=ileri\"><button><h1>ileri</h1></button></a>";
      sayfa += "<br><br><a href=\"?step=geri\"><button><h1>Geri</h1></button></a>";
      komut = "AT+CiPSEND=";                                    //Sayfa verisinin uzunluğu komut değişkenine eşitlenir.
      komut += connectionid;
      komut += ",";
      komut += sayfa.length();
      komut += "\r\n";
      esp.print(komut);                                      //Komut gönderilir.
      delay(1000);
      esp.print(sayfa);                                      //Sayfa verisi gönderilir.
      delay(1000);
      komut = "AT+CiPCLOSE=";                                   //Bağlanti sonlandirma komutu ayarlanir.
      komut += connectionid;
      komut += "\r\n";
      esp.print(komut);                                      //Bağlanti sonlandirma komutu gönderilir.
    }
  }
}
String espOkuma(long int zamanAsimi) {                          //ESP'den gelen dönütleri okur.
  long int baslangic = millis();
  String gelen;
  while (millis() - baslangic < zamanAsimi) {
    if (esp.available() > 0) {
      char c = esp.read();
      gelen += c;
    }
  }
  gelen.replace("AT+", "");
  return gelen;
}
void espTemizle(long int zamanAsimi) {                        //Seri haberleşmede kullanmadiğimiz bytelari temizler.
  long int baslangic = millis();
  while (millis() - baslangic < zamanAsimi) {
    if (esp.available() > 0) {
      esp.read();
    }
  }
}

void stepileri(int beklemeSuresi) {                              //Step motora ileri hareketi yaptirir.
  digitalWrite(motorPin1, HiGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(beklemeSuresi);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HiGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(beklemeSuresi);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HiGH);
  digitalWrite(motorPin4, LOW);
  delay(beklemeSuresi);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HiGH);
  delay(beklemeSuresi);
}

void stepGeri(int beklemeSuresi) {                               //Step motora ileri hareketi yaptirir.
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HiGH);
  delay(beklemeSuresi);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HiGH);
  digitalWrite(motorPin4, LOW);
  delay(beklemeSuresi);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HiGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(beklemeSuresi);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(beklemeSuresi);
}


