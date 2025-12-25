
void sensorsTick() {
// #if (LIGHT_SENS_AMOUNT == 0)
  britnessVal1 = 150;
  britnessVal2 = britnessVal1;
// #elif (LIGHT_SENS_AMOUNT == 1)
  britnessVal1 = constrain(map(analogRead(LIGHT_SENS_1), 0, 400, 0, 255), 60, 255);
  britnessVal2 = britnessVal1;
// #else
  britnessVal1 = constrain(map(analogRead(LIGHT_SENS_1), 0, 400, 0, 255), 60, 255);
  britnessVal2 = constrain(map(analogRead(LIGHT_SENS_2), 0, 450, 0, 255), 60, 255);
// #endif

  switch (currentState) {
// #if (MOVE_SENS_AMOUNT == 0)
    updAnimation(FROM_1);
// #else
    case IDLE:
      if (digitalRead(MOVE_SENS_1)) updAnimation(FROM_1);
// #if (MOVE_SENS_AMOUNT > 1)
      else if (digitalRead(MOVE_SENS_2)) updAnimation(FROM_2);
// #if (MOVE_SENS_AMOUNT > 2)
      else if (digitalRead(MOVE_SENS_3)) updAnimation(FROM_3);
// #endif
// #endif
    break;

    case FROM_1:
// #if (MOVE_SENS_AMOUNT > 1)
      if (digitalRead(MOVE_SENS_2)) if (millis() - walkTimer > WALK_TIMEOUT * 2) updAnimation(TO_2); else updAnimation(FROM_2);
// #if (MOVE_SENS_AMOUNT > 2)
      else if (digitalRead(MOVE_SENS_3)) if (millis() - walkTimer > WALK_TIMEOUT) updAnimation(TO_3); else updAnimation(FROM_3);
// #endif
// #endif
      fadeOutCheck();
    break;
      
    case FROM_2:
      if (digitalRead(MOVE_SENS_1)) if (millis() - walkTimer > WALK_TIMEOUT * 2) updAnimation(TO_1); else updAnimation(FROM_1);
// #if (MOVE_SENS_AMOUNT > 2)
      else if (digitalRead(MOVE_SENS_3)) if (millis() - walkTimer > WALK_TIMEOUT) updAnimation(TO_3); else updAnimation(FROM_3);
// #endif
      fadeOutCheck();
    break;

    case FROM_3:
      if (digitalRead(MOVE_SENS_1)) if (millis() - walkTimer > WALK_TIMEOUT) updAnimation(TO_1); else updAnimation(FROM_1);
      else if (digitalRead(MOVE_SENS_2)) if (millis() - walkTimer > WALK_TIMEOUT) updAnimation(TO_2); else updAnimation(FROM_2);
      fadeOutCheck();
    break;
// #endif
  }
#if (DEBUG_MODE == 1)
  Serial.print(currentState);
  //Serial.print(F(","));
  //Serial.print(effect);
  Serial.print(F(","));
  Serial.print(britnessVal1);
  Serial.print(F(","));
  Serial.print(britnessVal2);
  Serial.print(F(","));
  Serial.print(analogRead(LIGHT_SENS_1));
  Serial.print(F(","));
// #if (LIGHT_SENS_AMOUNT < 2)
  Serial.println(analogRead(LIGHT_SENS_1));
// #else
  Serial.println(analogRead(LIGHT_SENS_2));
// #endif

#endif
}

void fadeOutCheck() {
  if (millis() - fadeOutTimer > FADE_OUT_TIMEOUT) {
    updAnimation(FADE_OUT);
  }
}

void updAnimation(State state) {
  currentState = state;
#if (USE_STEPS == 1)
  for (int i = 0; i < MAX_ANIMS; i++) {
    if (activeAnims[i].state == state) {
      fadeOutTimer = millis();
      sensorsTimer = millis();
      walkTimer = millis();
      break;
    }

    if (activeAnims[i].state == IDLE) {
      //if (state >= TO_1) currentState = IDLE;
      activeAnims[i].state = state;
      activeAnims[i].step = 0;
      fadeOutTimer = millis();
      sensorsTimer = millis();
      walkTimer = millis();
      break;
    }
  }
#endif
#if (USE_RAILING == 1)
  for (int i = 0; i < MAX_ANIMS; i++) {
#if (USE_STEPS == 0)
    if (railingAnims[i].state == state) {
      fadeOutTimer = millis();
      sensorsTimer = millis();
      walkTimer = millis();
      break;
    }
#endif
    if (railingAnims[i].state == IDLE) {
      railingAnims[i].state = state;
      railingAnims[i].step = 0;
#if (USE_STEPS == 0)
      fadeOutTimer = millis();
      sensorsTimer = millis();
      walkTimer = millis();
#endif
      break;
    }
  }
#endif
}