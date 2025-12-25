/* sketch_LED_LADDER_1.0 
**
** 2025, may - jul.
**
*/

#define USE_RAILING 0
#define USE_STEPS   1
#define MAX_ANIMS   7
#define LIGHT_SENS_AMOUNT 1
#define MOVE_SENS_AMOUNT  3


#define DEBUG_MODE  0

// --- Настройки ленты ---
#define LED_TYPE         WS2812B
#define COLOR_ORDER      GRB
#define NUM_LEDS         570

#define DELAY_RAILING    40
#define DELAY_STEP       290
#define WALK_TIMEOUT     1500
#define FADE_OUT_TIMEOUT 10000

// --- ПИНЫ ---
#define MOVE_SENS_1     2
#define MOVE_SENS_2     3
#define MOVE_SENS_3     4
#define LED_PIN         5
#define LED_RAILING_PIN 6
#define LIGHT_SENS_1    A0
#define LIGHT_SENS_2    A1

#include "effectsSettings.h"
#include "stepsSettings.h"
#include <FastLED.h>

#if (USE_STEPS == 1)
CRGB leds[NUM_LEDS];
#endif

#if (USE_RAILING == 1)
CRGB ledsRailing[NUM_LEDS_RAILING];
#endif

enum State {
  IDLE,
  FROM_1,
  FROM_2,
  FROM_3,
  TO_1,
  TO_2,
  TO_3,
  FADE_OUT
};

State currentState = IDLE;

// --- Структура для хранения одной анимации ---
struct Animation {
  State state = IDLE;
  int step = 0;
  uint32_t timer;
};

#if (USE_STEPS == 1)
Animation activeAnims[MAX_ANIMS];
#endif
#if (USE_RAILING == 1)
Animation railingAnims[MAX_ANIMS];
#endif

uint16_t britnessVal1;
uint16_t britnessVal2;

uint8_t effect;
uint16_t stepStartPos[STEP_NUM + 2];

uint8_t rainbowHue;

uint32_t walkTimer;
uint32_t fadeOutTimer;
uint32_t sensorsTimer;
uint32_t effectsTimer;

void setup() {
#if (DEBUG_MODE == 1)
  Serial.begin(9600);
#endif
  calculateStepPos();

#if (USE_RAILING == 1)
  FastLED.addLeds<LED_TYPE, LED_RAILING_PIN, COLOR_ORDER>(ledsRailing, NUM_LEDS_RAILING);
#endif

#if (USE_STEPS == 1)
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
#endif

#if (USE_RAILING == 1 || USE_STEPS == 1)
  FastLED.setBrightness(0);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 8000);
  FastLED.clear();
  FastLED.show();
#endif
}

void loop() {
  randomazeEffects();
  sensorsTick();
  showEffects(effect);
  showRailingEffects(effect);
#if (USE_RAILING == 1 || USE_STEPS == 1)
  FastLED.show();
#endif
}