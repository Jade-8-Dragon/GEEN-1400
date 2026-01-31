#include <FastLED.h>
#include <math.h>

#define WIDTH 29
#define HEIGHT 9
#define NUM_LEDS (WIDTH * HEIGHT)
#define DATA_PIN 6

#define POT_LOW A0
#define POT_HIGH A1
#define MIC_PIN A2

CRGB leds[NUM_LEDS];

float phase = 0;

int XY(int x, int y) {
  return y * WIDTH + x;
}

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
}

void loop() {

  int lowRaw = analogRead(POT_LOW);
  int highRaw = analogRead(POT_HIGH);
  int micRaw = analogRead(MIC_PIN);

  float lowFreq  = map(lowRaw, 0, 1023, 1, 10);
  float highFreq = map(highRaw, 0, 1023, 10, 50);

  if (highFreq <= lowFreq) highFreq = lowFreq + 1;

  float amplitude = abs(micRaw - 512) / 512.0;
  amplitude *= HEIGHT / 2.0;

  FastLED.clear();

  for (int x = 0; x < WIDTH; x++) {

    float freq = map(x, 0, WIDTH - 1, lowFreq, highFreq);

    float yFloat = (sin(phase + x * 0.3 * freq) * amplitude)
                   + (HEIGHT / 2);

    int y = (int)yFloat;

    if (y >= 0 && y < HEIGHT) {
      leds[XY(x, y)] = CRGB::Blue;
    }
  }

  FastLED.show();

  phase += 0.15;   // wave movement speed
  delay(20);
}