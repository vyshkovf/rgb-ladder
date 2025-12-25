#if (USE_RAILING == 1)
void turnBlackR(int led) {
  ledsRailing[led] = CHSV(0, 0, 0);
}

void whiteMainR(int led, uint8_t britness) {
  ledsRailing[led] = CHSV(
    WHITE_HUE,
    WHITE_SAT,
    constrain(WHITE_BRIGHT - (255 - britness), 0, 255));
}

void staticRainbowR(int led, uint8_t britness) {
  ledsRailing[led] = CHSV(
    led * RAINBOW_SCALE,
    RAINBOW_SAT,
    constrain(RAINBOW_BRIGHT - (255 - britness), 0, 255));
}

void rainbowEffectR(int led, uint8_t britness) {
  ledsRailing[led] = CHSV(
    rainbowHue + led * RAINBOW_SCALE,
    RAINBOW_SAT,
    constrain(RAINBOW_BRIGHT - (255 - britness), 0, 255));
}

void lavaNoiceR(int led, uint8_t britness) {
  uint8_t noise = inoise8(led * NOISE_SCALE, millis() / 4);

  ledsRailing[led] = CHSV(
    map(noise, 0, 255, FIRE_MIN_HUE, FIRE_MAX_HUE),
    map(noise, 0, 255, FIRE_MIN_SAT, FIRE_MAX_SAT),
    constrain(map(noise - (255 - britness), 0, 255, FIRE_MIN_BRIGHT, FIRE_MAX_BRIGHT), 0, 255));
}

void oceanNoiceR(int led, uint8_t britness) {
  uint8_t noise = inoise8(led * NOISE_SCALE, millis() / 4);

  ledsRailing[led] = CHSV(
    map(noise, 0, 255, OCEAN_MIN_HUE, OCEAN_MAX_HUE),
    map(noise, 0, 255, OCEAN_MIN_SAT, OCEAN_MAX_SAT),
    constrain(map(noise - (255 - britness), 0, 255, OCEAN_MIN_BRIGHT, OCEAN_MAX_BRIGHT), 0, 255));
}

void plasmaNoiceR(int led, uint8_t britness) {
  uint8_t noise = inoise8(led * NOISE_SCALE, millis() / 2);

  ledsRailing[led] = CHSV(
    map(noise, 0, 255, PLASMA_MIN_HUE, PLASMA_MAX_HUE),
    map(noise, 0, 255, PLASMA_MIN_SAT, PLASMA_MAX_SAT),
    constrain(map(noise, 0, 255, PLASMA_MIN_BRIGHT, PLASMA_MAX_BRIGHT) - (255 - britness), 0, 255));
}
#endif