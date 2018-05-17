

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306_STM32.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);



// resistor value of voltage divider in ohm
float resistor = 2155;
float resistor1 = 10000;
#define BCOEFFICIENT 3950
#define THERMISTORNOMINAL 10000
#define TEMPERATURENOMINAL 25
uint8_t thermistorPin1 = 5;
uint8_t thermistorPin2 = 6;
uint8_t disp = 0;
uint8_t counter = 0;


void setup() {
    Serial1.begin(115200);
    pinMode(thermistorPin1, INPUT_ANALOG);
    pinMode(thermistorPin2, INPUT_ANALOG);

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

    

}

void loop() {

float sum1 = 0, sum2 = 0;
uint8_t samples = 100;


for(int i = 0; i < samples; i++){
    sum1 += kty(thermistorPin1);
    sum2 += thermistor(thermistorPin2);
    delay(20);
}

// Clear the buffer.
  display.clearDisplay();


  // text display tests
  
  display.setTextColor(WHITE);


  if(disp == 0 && counter++ < 5){
     display.setCursor(5,0);
  display.setTextSize(1);
  display.print("Motor");
  
  display.setCursor(5,11);
  display.setTextSize(3);
    display.print(sum1/samples,1);
  }
  if(disp == 1 && counter++ < 5){
     display.setCursor(5,0);
  display.setTextSize(1);
  display.print("Controller");
  
  display.setCursor(5,11);
  display.setTextSize(3);
    display.print(sum2/samples,1);
  }

  if(counter >= 5){
    counter = 0;
    disp++;
  }

  if(disp>1)
    disp = 0;

  display.setTextSize(2);
  display.print((char)247);
    display.setTextSize(3);
  display.println("C");
  display.display();

  
    Serial1.print("Temperature1: ");
    Serial1.println(sum1/samples,1);
    Serial1.print("Temperature2: ");
    Serial1.println(sum2/samples,1);
    Serial1.println("\n");

}


float kty(unsigned int port){

    float tempmedie = 0;
    float sumatemp;
    float temp = analogRead(port);
    float ukty = 5 * temp / 4095.0 ;
    float a = 0.00001874*1000;
    float b = 0.007884*1000;
    float c = 1000 - resistor * ukty / (5 - ukty);
    float delta = b * b - 4 * a * c;
    float delta1 = sqrt (delta);
    float x2 =(-b + delta1)/(2 * a);
    float temp1 = x2 + 25 ;
    return temp1;
}

float thermistor(unsigned int port){


    float reading;

    reading = analogRead(port);

    // convert the value to resistance
    reading = (4095 / reading)  - 1;     // (1023/ADC - 1)
    reading = resistor1 / reading;  // 10K / (1023/ADC - 1)
    float steinhart;
    steinhart = reading / THERMISTORNOMINAL;     // (R/Ro)
    steinhart = log(steinhart);                  // ln(R/Ro)
    steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                 // Invert
    steinhart -= 273.15;
    return steinhart;

}

