int f(int a);
asm("li $sp, 0x23FFFFFC");
int entry() { 
  int a = 7;
  int b = f(a);
  return b; 
} 

int f(int a) {
  return a + 1;
}
