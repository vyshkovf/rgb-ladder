#if (USE_STEPS == 1)
void turnBlack(uint8_t step) {
  // fill_solid - быстрая функция FastLED для заливки участка одним цветом
  fill_solid(&leds[START_POS(step)], steps[step], CRGB::Black);
}

void whiteMain(uint8_t step, uint8_t britness) {
  // Вычисляем цвет один раз
  CRGB color = CHSV(
    WHITE_HUE,
    WHITE_SAT,
    constrain(WHITE_BRIGHT - (255 - britness), 0, 255)
  );
  fill_solid(&leds[START_POS(step)], steps[step], color);
}

void staticRainbow(uint8_t step, uint8_t britness) {
  CRGB color = CHSV(
    step * RAINBOW_SCALE,
    RAINBOW_SAT,
    constrain(RAINBOW_BRIGHT - (255 - britness), 0, 255)
  );
  fill_solid(&leds[START_POS(step)], steps[step], color);
}

void rainbowEffect(uint8_t step, uint8_t britness) {
  for (uint16_t p = START_POS(step); p < END_POS(step); p++) {
    leds[p] = CHSV(
      rainbowHue + step * RAINBOW_SCALE,
      RAINBOW_SAT,
      constrain(RAINBOW_BRIGHT - (255 - britness), 0, 255));
  }
}

void lavaNoice(uint8_t step, uint8_t britness) {
  uint8_t noise = inoise8(step * NOISE_SCALE, millis() / 4);

  for (uint16_t p = START_POS(step); p < END_POS(step); p++) {
    leds[p] = CHSV(map(
                     noise, 0, 255, FIRE_MIN_HUE, FIRE_MAX_HUE),
                   map(noise, 0, 255, FIRE_MIN_SAT, FIRE_MAX_SAT),
                   constrain(map(noise - (255 - britness), 0, 255, FIRE_MIN_BRIGHT, FIRE_MAX_BRIGHT), 0, 255));
  }
}

void oceanNoice(uint8_t step, uint8_t britness) {
  uint8_t noise = inoise8(step * NOISE_SCALE, millis() / 4);

  for (uint16_t p = START_POS(step); p < END_POS(step); p++) {
    leds[p] = CHSV(
      map(noise, 0, 255, OCEAN_MIN_HUE, OCEAN_MAX_HUE),
      map(noise, 0, 255, OCEAN_MIN_SAT, OCEAN_MAX_SAT),
      constrain(map(noise - (255 - britness), 0, 255, OCEAN_MIN_BRIGHT, OCEAN_MAX_BRIGHT), 0, 255));
  }
}

void plasmaNoice(uint8_t step, uint8_t britness) {
  uint8_t noise = inoise8(step * NOISE_SCALE, millis() / 2);

  CRGB color = CHSV(
    map(noise, 0, 255, PLASMA_MIN_HUE, PLASMA_MAX_HUE),
    map(noise, 0, 255, PLASMA_MIN_SAT, PLASMA_MAX_SAT),
    constrain(map(noise, 0, 255, PLASMA_MIN_BRIGHT, PLASMA_MAX_BRIGHT) - (255 - britness), 0, 255));
    
  fill_solid(&leds[START_POS(step)], steps[step], color);
}

uint16_t START_POS(uint8_t step) {
  return stepStartPos[step];
}

uint16_t END_POS(uint8_t step) {
  return stepStartPos[step + 1];
}
#endif

void randomazeEffects() {
  if (currentState == IDLE) effect = random8(0, 6);
}

void calculateStepPos() {
#if (USE_STEPS == 1)
  stepStartPos[0] = 0;
  for (int i = 0; i < STEP_NUM + 1; i++) {
    stepStartPos[i + 1] = stepStartPos[i] + steps[i];
  }
#endif
}

/*void lavaNoice(uint8_t step, uint8_t britness) {
  uint8_t noise = inoise8(step * NOISE_SCALE, millis() / 4);

  CRGB color = CHSV(
    map(noise, 0, 255, FIRE_MIN_HUE, FIRE_MAX_HUE),
    map(noise, 0, 255, FIRE_MIN_SAT, FIRE_MAX_SAT),
    constrain(map(noise - (255 - britness), 0, 255, FIRE_MIN_BRIGHT, FIRE_MAX_BRIGHT), 0, 255));

  fill_solid(&leds[START_POS(step)], steps[step], color);
}

void oceanNoice(uint8_t step, uint8_t britness) {
  uint8_t noise = inoise8(step * NOISE_SCALE, millis() / 4);

  CRGB color = CHSV(
    map(noise, 0, 255, OCEAN_MIN_HUE, OCEAN_MAX_HUE),
    map(noise, 0, 255, OCEAN_MIN_SAT, OCEAN_MAX_SAT),
    constrain(map(noise - (255 - britness), 0, 255, OCEAN_MIN_BRIGHT, OCEAN_MAX_BRIGHT), 0, 255));

  fill_solid(&leds[START_POS(step)], steps[step], color);
}

void plasmaNoice(uint8_t step, uint8_t britness) {
  uint8_t noise = inoise8(step * NOISE_SCALE, millis() / 2);

  CRGB color = CHSV(
    map(noise, 0, 255, PLASMA_MIN_HUE, PLASMA_MAX_HUE),
    map(noise, 0, 255, PLASMA_MIN_SAT, PLASMA_MAX_SAT),
    constrain(map(noise, 0, 255, PLASMA_MIN_BRIGHT, PLASMA_MAX_BRIGHT) - (255 - britness), 0, 255));

  fill_solid(&leds[START_POS(step)], steps[step], color);
}*/

/*void rainbowNoice(uint8_t step, uint8_t britness) {
  for (uint16_t p = START_POS(step); p < END_POS(step); p++) {
    uint8_t noise = inoise8(p * NOISE_SCALE, millis() / 2);
    fire_heat[p] = map(noise, 0, 255, 0, 255);
  }

  for (int p = START_POS(step); p < END_POS(step); p++) {
    leds[p] = CHSV(
      map(fire_heat[p], 0, 255, CL_LAVA_MIN_HUE, CL_LAVA_MAX_HUE),
      map(fire_heat[p], 0, 255, CL_LAVA_MIN_SAT, CL_LAVA_MAX_SAT),
      constrain(map(fire_heat[p] - ((255 - britness) / 1.5), 0, 255, CL_LAVA_MIN_BRIGHT, CL_LAVA_MAX_BRIGHT), 0, 255)
    );
  }
}*/

/*void fireEffect(uint8_t step, uint8_t britness) {
  if (random8() < FIRE_SPARKING) fire_heat[START_POS(step)] = qadd8(fire_heat[START_POS(step)], random8(160, 255));

  for (uint16_t p = END_POS(step) - 1; p >= START_POS(step) + 1; p--) {
    fire_heat[p] = fire_heat[p - 1];
  }

  for (uint16_t p = START_POS(step); p < END_POS(step); p++) {
    fire_heat[p] = qsub8(fire_heat[p], random8(0, (FIRE_COOLING * 10) / (END_POS(step) - START_POS(step))));
  }

  for (uint16_t p = START_POS(step); p < END_POS(step); p++) {
    leds[p] = CHSV(
      map(fire_heat[p], 0, 255, FIRE_MIN_HUE, FIRE_MAX_HUE),
      map(fire_heat[p], 0, 255, FIRE_MIN_SAT, FIRE_MAX_SAT),
      constrain(map(fire_heat[p] - (255 - britness), 0, 255, FIRE_MIN_BRIGHT, FIRE_MAX_BRIGHT), 0, 255));
  }
}*/

/*void sparklesEffect(uint8_t step) {
  uint8_t noiseValue;
  uint16_t pos = 0;
  for (uint16_t i = 0; i < step; i++) pos += steps[i];
  for (uint16_t i = pos; i < pos + steps[step + 1]; i++) {
    noiseValue = inoise8((uint32_t)((x_noise_coord + i * 2) / SMOOTH_K), (uint32_t)((y_noise_coord + i) / SMOOTH_K));
    if (noiseValue > 160 && flashesColor[i - 1] == 0) {
      noiseValue = inoise8((uint32_t)((x_noise_coord + i * 2) / SMOOTH_K), (uint32_t)((y_noise_coord + i * 5) / SMOOTH_K));
      if (flashesColor[i] == 0) flashesColor[i] = noiseValue;
      if (noiseValue - (255 -  britness) > 0) {
        leds[i] = CHSV(flashesColor[i],
          map(noiseValue, 0, 255, MIN_SAT, MAX_SAT), 
          map(noiseValue - (255 - britness), 0, 255, MIN_BRIGHT, MAX_BRIGHT)
        );
      } else {
        leds[i] = CHSV(flashesColor[i],
          map(noiseValue, 0, 255, MIN_SAT, MAX_SAT), 
          10
        );
      }
    } else flashesColor[i] = 0;
  }
}*/