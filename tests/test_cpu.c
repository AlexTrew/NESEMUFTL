#include <check.h>
#include <stdint.h>
#include <stdlib.h>
#include "../cpu.c"

/*
  Test the cpu library
  Here's a good sample of how to do these tests:
  https://github.com/vndmtrx/check-cmake-example/blob/master/tests/test_sample.c
 */

uint16_t bus[0xFFFE];
Cpu* cpu;

void setup(void){
    for(uint16_t i=0;i<sizeof(bus)/sizeof(bus[0]);i++){
	bus[i] = 0x16;
    }
    cpu = init_cpu(bus);
}

void teardown(void){
    delete_cpu(cpu);
}

START_TEST(test_immediate_addressing_mode){
    bus[0x00FE] = 0x15;
    bus[0x00FF] = 0x16;
    cpu->pc = 0x00FE;

    uint16_t res = get_operand_with_immediate_addressing(cpu);
    ck_assert(res == bus[0x00FF]);
}
END_TEST

START_TEST(test_absolute_addressing_mode){
    bus[0x00FE] = 0x15;
    bus[0x00FF] = 0x16;
    bus[0x0100] = 0xFE;
    cpu->pc = 0x00FE;

    uint16_t res = get_operand_with_absolute_addressing(cpu);
    ck_assert(res == 0xFE16);
}
END_TEST

START_TEST(test_absolute_x_addressing_mode){
    bus[0x00FE] = 0x15;
    bus[0x00FF] = 0x16;
    bus[0x0100] = 0xFE;
    cpu->x=0x04;
    cpu->pc = 0x00FE;

    uint16_t res = get_operand_with_absolute_x_addressing(cpu);
    ck_assert(res == 0xFE1A);
}
END_TEST

START_TEST(test_absolute_y_addressing_mode){
    bus[0x00FE] = 0x15;
    bus[0x00FF] = 0x16;
    bus[0x0100] = 0xFE;
    cpu->y=0x04;
    cpu->pc = 0x00FE;

    uint16_t res = get_operand_with_absolute_y_addressing(cpu);
    ck_assert(res == 0xFE1A);
}
END_TEST

START_TEST(test_zp_addressing_mode){
    bus[0x00FE] = 0x15;
    bus[0x00FF] = 0x16;
    cpu->pc = 0x00FE;

    uint16_t res = get_operand_with_zero_page_addressing(cpu);
    ck_assert(res == 0x0016);
}
END_TEST

START_TEST(test_zpx_addressing_mode){
    bus[0x00FE] = 0x15;
    bus[0x00FF] = 0x16;
    cpu->pc = 0x00FE;
    cpu->x = 0x01;

    uint16_t res = get_operand_with_zero_page_x_offset_addressing(cpu);
    ck_assert(res == 0x0017);
}
END_TEST

START_TEST(test_zpy_addressing_mode){
    bus[0x00FE] = 0x15;
    bus[0x00FF] = 0x16;
    cpu->pc = 0x00FE;
    cpu->y = 0x01;

    uint16_t res = get_operand_with_zero_page_y_offset_addressing(cpu);
    ck_assert(res == 0x0017);
}
END_TEST


START_TEST(lookup_cpu_instruction_from_opcode){

    CpuInstruction* result = cpu_cycle(cpu);
    ck_assert(result->name == ASL);
    ck_assert(result->cycles_left == 6);
    ck_assert(result->addressing_mode == ZP);

}
END_TEST

Suite* make_cpu_tests(void){
  Suite *s = suite_create("cpu instruction lookup test suite");
  TCase* tc = tcase_create("cpu instruction lookup test");

  tcase_add_checked_fixture(tc, setup, teardown);
  tcase_add_test(tc, lookup_cpu_instruction_from_opcode);
  tcase_add_test(tc, test_immediate_addressing_mode);
  tcase_add_test(tc, test_absolute_addressing_mode);
  tcase_add_test(tc, test_absolute_x_addressing_mode);
  tcase_add_test(tc, test_absolute_y_addressing_mode);
  tcase_add_test(tc, test_zp_addressing_mode);
  tcase_add_test(tc, test_zpx_addressing_mode);
  tcase_add_test(tc, test_zpy_addressing_mode);

  suite_add_tcase(s, tc);

  return s;
}

int main(){
  int failed = 0;
  SRunner* sr;
  sr = srunner_create(make_cpu_tests());
  srunner_set_fork_status(sr, CK_NOFORK); 
  srunner_set_log(sr, "test.log");
  srunner_set_xml(sr, "test.xml");
  srunner_run_all(sr, CK_VERBOSE);

  failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
