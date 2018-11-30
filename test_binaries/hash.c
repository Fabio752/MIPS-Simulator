// Initialize stack pointer.
asm("li $sp, 0x23FFFFFC");

int entry() { 
  char str[3] = {'a', 'b', 'c'};
  int h = 5381;
  int c;

  for (int i = 0; i < 3; i++) {
    c = (int)str[i];
    h = ((h << 5) + h) + c;
  }

  return h;
}
