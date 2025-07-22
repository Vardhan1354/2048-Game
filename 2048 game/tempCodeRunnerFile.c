 uint8_t getDigitCount(uint32_t number) {
	 uint8_t count = 0;
	 do {
		 number /= 10;
		 count += 1;
	 } while (number);
	 return count;
 }