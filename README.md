# Bežične senzorske mreže - Lab 4

### FESB, smjer 110/111/112/114/120, akademska godina 2016/2017

U situacijama kada kompleksnost koda raste, potrebno je posegnuti za nekim programskim tehnikama koje su istovremeno jednostavne i efikasne, a koje kod mogu učiniti preglednijim te samim time pojednostavniti njegovo razumijevanje. U sklopu današnje vježbe studenti će koristiti **state mašinu**, nakon ćega će izraditi **bibiloteku** za čitanje senzora.

Bežični senzorski uređaji su u osnovi jednostavni čvorovi koji periodički obavljaju neke radnje (npr. buđenje, čitanje vrijednosti temperature i vlage, slanje preko radio kanala, primanje poruke preko radio kanala te odlazak u spavanje). Imajući uvida u jednostavnost radnji, ovakve operacije se mogu realizirati pomoću jednostavne state mašine (eng. *state machine*). Iako postoje implementacije state mašina koje koriste *function pointere*, u sklopu ove vježbi će realizirati state mašinu korištenjem jednostavnih **switch/case** naredbi.

## State Machine

Prvi korak u ralizaciji state mašine je stvoriti enumeraciju svakog stanja mašine. U donjem primjeru je naveden primjer enumeracije koja je bila definirana korištenjem **typedef** enumeracije. Ovaj primjer sadrži četiri stanja uz NUM_STATES koji praktički označava ukupan broj stanja u našoj state mašini.

```arduino
typedef enum {
  READ_SERIAL,
  GOTO_SLEEP,
  READ_SENSORS,
  NUM_STATES
} StateType;
```

Nakon toga je potrebno definirati state varijablu u koju ćemo pohraniti trenutno stanje naše state mašine.

```arduino
StateType state = READ_SERIAL;
```

Vaš zadatak je da napravite state mašinu korištenjem switch/case naredbi na način da prilagodite kod ``TempHumLight.ino`` (u direktoriju vjezba) state mašini koja je dana u nastavku: 


```arduino
void loop() {
  StateType state = READ_SERIAL;
  for(;;) {
    switch(state) {
      case READ_SERIAL:
	//Run state machine code
	//za sada mozete ostaviti prazno

	//Move to the next state
        state = GOTO_SLEEP;
        break;
      case GOTO_SLEEP:
	//Run state machine code
	  
	
	//Move to the next state
        state = READ_SENSORS;
        break;
      case READ_SENSORS:
	//Run state machine code


	//Move to the next state
        state = READ_SERIAL;
        break;
    }
  }
}
```

## Kako pisati biblioteku za Arduino?

Da bismo pojednostavnili **organizaciju** i **korištenje koda**, povećali njegovu **čitljivost**, a ujedno i **decentralizirali** cijelu logiku, možemo jednostavno izraditi biblioteke za neke funkcije kao što su čitanje temperature, vlage i osvjetljenja. Biblioteka je skup uputa koje su dizajnirane za izvršavanje određenog zadatka. Dobre bibilioteke opisuju task-ove koje one pokušavaju riješiti samim imenom biblioteke, kao i nazivima funkcija koje su asocirane uz tu biblioteku. U konkretnom primjeru Arduina biblioteke služe kao *wrapper* oko Arduino API jezika kako bi se poboljšala čitljivost koda i korisničko iskustvo. U sklopu današnje vježbe student će izraditi biblioteku za senzore temperature/vlage i osvjetljenja.

Prije nego budemo išli u izradu biblioteke za senzore, potrebno je identificirati varijable i funkcije koje su bitne za našu biblioteku. Nakon što smo to napravili, u sljedećem koraku idemo u kreiranje **C++ klase**. Generalno govoreći, klasu možemo podijeliti na dva dijela, **header** i **source** datoteku. Deklaracija se odnosi na *header* datotetku. U našem primjeru, ``Sensors.h`` označava da je datoteka deklarirala klasu **SENSORS**. Deklaracija je proces definiranja što bi klasa trebala raditi. S druge strane, imeplementacija se odnosi na *source* datoteku. U našem primjeru ``Sensors.cpp`` označava da datoteka implementira deklarirane funkcije i varijable iz ``Sensors.h``. Implementacija je proces pisanja koda i označava na koji način su imeplementirane deklarirane funkcije. U nastavku je dan header ``Sensors.h``:

```arduino
#ifndef Sensors_h
#define Sensors_h

#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>

#define DHTPIN 3
#define DHTPIN_VCC 4
#define DHTTYPE DHT22

class SENSORS {
public:
        SENSORS();
        ~SENSORS();
        void DHT_init();
        void BH1750_init();
        void readTempHum();
        void readLight();
};

#endif
```

Ovdje je potrebno naglasiti da preprocesorske komande ``#ifdef`` (*if defined*) i ``#ifndef`` (*if not defined*) provjeravaju je li neka varijabla već negdje drugdje bila definirana. 

Vaš zadatak je da kreirate ``Sensors.cpp`` dadoteku (i ``Sensors.h``) koja koristi gore navedene funkcije. U nastavku je dan primjer početka ``Sensors.cpp`` datoteke. Vaš zadatak je implementirati deklarirane funkcije ``void readTempHum()`` i ``void readLight()``. Primjetite da ``::`` označava tzv. *scope resolution operator*.


```arduino
#include "Sensors.h"

DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter(0x23);

SENSORS::SENSORS(){}
SENSORS::~SENSORS(){}

void SENSORS::DHT_init(){
  pinMode(DHTPIN_VCC,OUTPUT);
  digitalWrite(DHTPIN_VCC,LOW);
  Serial.println(F("DHTxx test!"));
  dht.begin();
}

void SENSORS::BH1750_init(){
  lightMeter.begin(BH1750_CONTINUOUS_HIGH_RES_MODE);
  Serial.println(F("BH1750 Test"));
}
```

Nakon što je napravljena klasa Sensors, trebate inicijalizirati instancu te klase u glavnoj datoteci ``TempHumLight.ino`` (npr. ``SENSORS sensor``). Primjetite da jednostavnom instancom klase možete pozivati funkcije i varijable deklarirane i implementirane u Sensors klasi. U navedenom primjeru u ``setup()`` funkciji pozivamo ``DHT_init()`` i ``BH1750_init()``.

```arduino
SENSORS sensor;

void setup() {
  Serial.begin(115200);
  sensor.DHT_init();
  sensor.BH1750_init();
  delay(100); // give some time to send data over Serial before going to sleep
}
```

Vaš zadatak je da u state mašini na odgovarajućim mjestima pozovete ``sensor.readTempHum()`` i ``sensor.readLight()`` te testirate rad koda.


## Korisni linkovi:

[1] http://www.edn.com/electronics-blogs/embedded-basics/4406821/Function-pointers---Part-3--State-machines  
[2] http://playground.arduino.cc/Code/Library  
[3] https://learn.adafruit.com/multi-tasking-the-arduino-part-1/overview  
[4] http://www.fredosaurus.com/notes-cpp/preprocessor/ifdef.html  

