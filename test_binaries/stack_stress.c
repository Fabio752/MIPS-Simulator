int f(int a);
asm("li $sp, 0x23FFFFFC");
int entry() { 
  int a = 100000;
  int b = f(a);
  return b; 
} 

int f(int a) {
  return a == 0 ? 7 : f(a - 1);
}
