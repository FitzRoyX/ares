auto APU::Triangle::clockLinearLength() -> void {
  if(reloadLinear) {
    linearLengthCounter = linearLength;
  } else if(linearLengthCounter) {
    linearLengthCounter--;
  }

  if(length.halt == 0) reloadLinear = false;
}

auto APU::Triangle::clock() -> n8 {
  n8 result = stepCounter & 0x0f;
  if((stepCounter & 0x10) == 0) result ^= 0x0f;
  if(length.counter == 0 || linearLengthCounter == 0) return result;

  if(--periodCounter == 0) {
    stepCounter++;
    periodCounter = period + 1;
  }

  return result;
}

auto APU::Triangle::power(bool reset) -> void {
  length.power(reset, true);

  periodCounter = 1;
  linearLength = 0;
  period = 0;
  stepCounter = 16;
  linearLengthCounter = 0;
  reloadLinear = 0;
}
