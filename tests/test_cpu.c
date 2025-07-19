#include <check.h>
#include "../cpu.h"

/*
  Test the cpu library
  Here's a good sample of how to do these tests:
  https://github.com/vndmtrx/check-cmake-example/blob/master/tests/test_sample.c
 */

static Cpu* cpu;

void setup(void){
  uint16_t bus[0xFFFE];
  for(uint16_t i=0;i<sizeof(bus)/sizeof(bus[0]);i++){
    bus[i] = 0x16;
  }

  cpu = init_cpu(bus);
}

void teardown(void){
  delete_cpu(cpu);
}

START_TEST(lookup_cpu_instruction_from_opcode){
}

Suite* make_cpu_tests(void){
  Suite *s = suite_create("cpu instruction lookup test suite");
  TCase* cases = tcase_create("cpu instruction lookup test cases");

  tcase_add_test(cases, lookup_cpu_instruction_from_opcode);

  suite_add_tcase(s, cases);

  return s;
}

int main(){
  return 0;
}
