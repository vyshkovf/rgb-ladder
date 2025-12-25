
#if (USE_STEPS == 1)
void drawStepEffect(uint8_t effect, int step) {
  uint8_t brightness = (step <= STEPS_JOINT_POS ? britnessVal1 : britnessVal2);
  switch (effect) {
    case 0: whiteMain(step, brightness); break;
    case 1: staticRainbow(step, brightness); break;
    case 2: rainbowEffect(step, brightness); break;
    case 3: lavaNoice(step, brightness); break;
    case 4: oceanNoice(step, brightness); break;
    case 5: plasmaNoice(step, brightness); break;
  }
}
#endif

// просто попробовать изменение сделать

#if (USE_RAILING == 1)
void drawRailingEffect(uint8_t effect, int led) {
  uint8_t brightness = (britnessVal1 + britnessVal2) / 2;
  switch (effect) {
    case 0: whiteMainR(led, brightness); break;
    case 1: staticRainbowR(led, brightness); break;
    case 2: rainbowEffectR(led, brightness); break;
    case 3: lavaNoiceR(led, brightness); break;
    case 4: oceanNoiceR(led, brightness); break;
    case 5: plasmaNoiceR(led, brightness); break;
  }
}
#endif

void showEffects(uint8_t effect) {
#if (USE_STEPS == 1)
  if (effect == 2 && millis() - effectsTimer > 50) {
    effectsTimer = millis();
    if (rainbowHue < 250) rainbowHue += RAINBOW_SPEED;
    else rainbowHue = 0;
  }
  for (uint8_t i = 0; i < MAX_ANIMS; i++) {
    if (activeAnims[i].state != IDLE) {
      FastLED.setBrightness(255);

      switch (activeAnims[i].state) {
        case FROM_1:  // От начала до конца
          for (int step = 0; step < activeAnims[i].step; step++) {
            drawStepEffect(effect, step);
          }
          if (activeAnims[i].step < STEP_NUM && millis() - activeAnims[i].timer > DELAY_STEP) {
            activeAnims[i].timer = millis();
            fadeOutTimer = millis();
            activeAnims[i].step++;
          }
        break;

        case FROM_2:  // От конца до начала
          for (int step = STEP_NUM; step >= STEP_NUM - activeAnims[i].step; step--) {
            drawStepEffect(effect, step);
          }
          if (STEP_NUM - activeAnims[i].step > 0 && millis() - activeAnims[i].timer > DELAY_STEP) {
            activeAnims[i].timer = millis();
            fadeOutTimer = millis();
            activeAnims[i].step++;
          }
        break;

        case FROM_3:  // От середины в обе стороны
          for (int step = 0; step < activeAnims[i].step; step++) {
            drawStepEffect(effect, STEPS_JOINT_POS + step);
            drawStepEffect(effect, STEPS_JOINT_POS - step);
          }
          if (activeAnims[i].step < STEP_NUM_HALF && millis() - activeAnims[i].timer > DELAY_STEP) {
            activeAnims[i].timer = millis();
            fadeOutTimer = millis();
            activeAnims[i].step++;
          }
        break;

        case TO_1:  // От начала до конца
          for (int step = STEP_NUM; step >= STEP_NUM - activeAnims[i].step; step--) {
            turnBlack(step);
          }
          if (activeAnims[i].step < STEP_NUM && millis() - activeAnims[i].timer > DELAY_STEP) {
            activeAnims[i].timer = millis();
            fadeOutTimer = millis();
            activeAnims[i].step++;
          } else if (activeAnims[i].step >= STEP_NUM) {
            currentState = IDLE;
            for (uint8_t j = 0; j < MAX_ANIMS; j++) {
              activeAnims[j].state = IDLE;
            }
          }
        break;

        case TO_2:  // От конца до начала
          for (int step = 0; step < activeAnims[i].step; step++) {
            turnBlack(step);
          }
          if (activeAnims[i].step < STEP_NUM && millis() - activeAnims[i].timer > DELAY_STEP) {
            activeAnims[i].timer = millis();
            fadeOutTimer = millis();
            activeAnims[i].step++;
          } else if (activeAnims[i].step >= STEP_NUM) {
            currentState = IDLE;
            for (uint8_t j = 0; j < MAX_ANIMS; j++) {
              activeAnims[j].state = IDLE;
            }
          }
        break;

        case TO_3:  //
          for (int step = 0; step < activeAnims[i].step; step++) {
            turnBlack(step);
            turnBlack(STEP_NUM - step);
          }
          if (activeAnims[i].step < STEP_NUM_HALF && millis() - activeAnims[i].timer > DELAY_STEP) {
            activeAnims[i].timer = millis();
            fadeOutTimer = millis();
            activeAnims[i].step++;
          } else if (activeAnims[i].step >= STEP_NUM_HALF) {
            currentState = IDLE;
            for (uint8_t j = 0; j < MAX_ANIMS; j++) {
              activeAnims[j].state = IDLE;
            }
          }
        break;

        case FADE_OUT:  // Затухание
          for (int b = 255; b > 8; b -= 8) {
            FastLED.setBrightness(b);
            delay(8);
            FastLED.show();
          }
          FastLED.clear();
          currentState = IDLE;
          for (uint8_t j = 0; j < MAX_ANIMS; j++) {
            activeAnims[j].state = IDLE;
#if (USE_RAILING == 1)
            railingAnims[j].state = IDLE;
#endif
          }
        break;
      }
    }
  }
#endif
}

void showRailingEffects(uint8_t effect) {
#if (USE_RAILING == 1)
#if (USE_STEPS == 0)
  if (effect == 2 && millis() - effectsTimer > 50) {
    effectsTimer = millis();
    if (rainbowHue < 250) rainbowHue += RAINBOW_SPEED;
    else rainbowHue = 0;
  }
#endif

  for (uint8_t i = 0; i < MAX_ANIMS; i++) {
    if (railingAnims[i].state != IDLE) {
#if (USE_STEPS == 0)
      FastLED.setBrightness(255);
#endif
      switch (railingAnims[i].state) {
        case FROM_1: // От начала до конца
          for (int led = 0; led < railingAnims[i].step; led++) {
            drawRailingEffect(effect, led);
          }
          if (railingAnims[i].step < NUM_LEDS_RAILING && millis() - railingAnims[i].timer > DELAY_RAILING) {
            railingAnims[i].timer = millis();
#if (USE_STEPS == 0)
            fadeOutTimer = millis();
#endif
            railingAnims[i].step += RAILING_ADD_NUM;
          }
        break;

        case FROM_2: // От конца до начала
          for (int led = 0; led < railingAnims[i].step; led++) {
            drawRailingEffect(effect, NUM_LEDS_RAILING - led);
          }
          if (railingAnims[i].step <= NUM_LEDS_RAILING && millis() - railingAnims[i].timer > DELAY_RAILING) {
            railingAnims[i].timer = millis();
#if (USE_STEPS == 0)
            fadeOutTimer = millis();
#endif
            railingAnims[i].step += RAILING_ADD_NUM;
          }
        break;

        case FROM_3: // От середины в обе стороны
          for (int led = RAILING_JOINT_POS; led < railingAnims[i].step + RAILING_JOINT_POS; led++) {
            drawRailingEffect(effect, led);
          }
          for (int led = RAILING_JOINT_POS; led > RAILING_JOINT_POS - railingAnims[i].step; led--) {
            drawRailingEffect(effect, led);
          }
          if (railingAnims[i].step < NUM_LEDS_RAILING_HALF && millis() - railingAnims[i].timer > DELAY_RAILING) {
            railingAnims[i].timer = millis();
#if (USE_STEPS == 0)
            fadeOutTimer = millis();
#endif
            railingAnims[i].step += RAILING_ADD_NUM;
          }
        break;

        case TO_1: // От начала до конца
          for (int led = NUM_LEDS_RAILING; led >= NUM_LEDS_RAILING - railingAnims[i].step; led--) {
            turnBlackR(led);
          }
          if (railingAnims[i].step < NUM_LEDS_RAILING && millis() - railingAnims[i].timer > DELAY_RAILING) {
            railingAnims[i].timer = millis();
#if (USE_STEPS == 0)
            fadeOutTimer = millis();
#endif
            railingAnims[i].step += RAILING_ADD_NUM;
          } else if (railingAnims[i].step >= NUM_LEDS_RAILING) {
#if (USE_STEPS == 0)
            currentState = IDLE;
#endif
            for (uint8_t j = 0; j < MAX_ANIMS; j++) {
              railingAnims[j].state = IDLE;
            }
          }
        break;

        case TO_2: // От конца до начала
          for (int led = 0; led < railingAnims[i].step; led++) {
            turnBlackR(led);
          }
          if (railingAnims[i].step < NUM_LEDS_RAILING && millis() - railingAnims[i].timer > DELAY_RAILING) {
            railingAnims[i].timer = millis();
#if (USE_STEPS == 0)
            fadeOutTimer = millis();
#endif
            railingAnims[i].step += RAILING_ADD_NUM;
          } else if (railingAnims[i].step >= NUM_LEDS_RAILING) {
#if (USE_STEPS == 0)
            currentState = IDLE;
#endif
            for (uint8_t j = 0; j < MAX_ANIMS; j++) {
              railingAnims[j].state = IDLE;
            }
          }
        break;

        case TO_3: //
          for (int led = 0; led < railingAnims[i].step; led++) {
            turnBlackR(led);
          }
          for (int led = NUM_LEDS_RAILING; led > NUM_LEDS_RAILING - railingAnims[i].step; led--) {
            turnBlackR(led);
          }
          if (railingAnims[i].step < NUM_LEDS_RAILING_HALF && millis() - railingAnims[i].timer > DELAY_RAILING) {
            railingAnims[i].timer = millis();
#if (USE_STEPS == 0)
            fadeOutTimer = millis();
#endif
            railingAnims[i].step += RAILING_ADD_NUM;
          } else if (railingAnims[i].step >= NUM_LEDS_RAILING_HALF) {
#if (USE_STEPS == 0)
            currentState = IDLE;
#endif
            for (uint8_t j = 0; j < MAX_ANIMS; j++) {
              railingAnims[j].state = IDLE;
            }
          }
        break;
#if (USE_STEPS == 0)
        case FADE_OUT:  // Затухание
          for (int b = 255; b > 8; b -= 8) {
            FastLED.setBrightness(b);
            delay(8);
            FastLED.show();
          }
          FastLED.clear();
          currentState = IDLE;
          for (uint8_t j = 0; j < MAX_ANIMS; j++) {
            railingAnims[j].state = IDLE;
          }
        break;
#endif
      }
    }
  }
#endif
}