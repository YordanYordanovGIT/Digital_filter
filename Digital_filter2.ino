#define inputPin A0

unsigned long previousReadTime = 0;
int readPeriod = 1000;
const int readValArrSize = 6;
int readValArr[readValArrSize];

void setup() {
  Serial.begin(9600);
  pinMode(inputPin, INPUT);

  for(int i = 0; i < readValArrSize;i++)
   readValArr[i] = -1;
}

double Thermistor(int RawADC)
  {
    double Temp;
    Temp = log(10000.0 / (1024.0 / RawADC - 1));
    Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
    Temp = Temp - 273.15;            // Convert Kelvin to Celcius
    Temp = Temp - 18.6; // correction value
    //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
    return Temp;
  }

void loop() {
  filter();
}

void filter()
{
  unsigned long readTime = millis();
  if (readTime < previousReadTime)
    previousReadTime = readTime;

  if ((unsigned long)(readTime - previousReadTime) >= readPeriod)
  {
    int realValsCount = 1, tempSumOfArr = 0;
    int inputPin = analogRead(inputPin);

    for (int i = readValArrSize - 2; i > -1; i--)
    {
      readValArr[i + 1] = readValArr[i];
      if (readValArr[i] != -1)
        realValsCount++;
    }
    readValArr[0] = inputPin;

    for (int i = 0; i < readValArrSize; i++)
      tempSumOfArr += readValArr[i];

    
    
    double tempVal = (double)tempSumOfArr / (double)realValsCount;

    Serial.println((String)realValsCount + " " + (String)tempSumOfArr + " " + (String)tempVal);

    double temp   =  Thermistor(tempVal);

    Serial.println(temp);

    previousReadTime = millis();
  }
}
