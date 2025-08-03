#include <check.h>
#include <stdint.h>
#include <stdlib.h>
#include "../cpu.c"

/*
  Test the cpu library
  Here's a good sample of how to do these tests:
  https://github.com/vndmtrx/check-cmake-example/blob/master/tests/test_sample.c
 */

uint8_t bus[0xFFFF];
CpuState* cpu;

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

    CpuAddressingModeResult res = immediate_addressing_mode(cpu);
    uint8_t expected = 0x16;
    ck_assert_msg(res.operand == expected, "result %#02x != expected %#02x", res.operand, expected);

}
END_TEST

START_TEST(test_absolute_addressing_mode){
    bus[0x00FE] = 0x15;
    bus[0x00FF] = 0x16;
    bus[0x0100] = 0xFE;
    cpu->pc = 0x00FE;

    CpuAddressingModeResult res = absolute_addressing_mode(cpu);
    uint16_t expected = 0xFE16;
    ck_assert_msg(res.operand == expected, "result %#04x != expected %#04x", res.operand, expected);

}
END_TEST

START_TEST(test_absolute_x_addressing_mode){
    bus[0x00FE] = 0x15;
    bus[0x00FF] = 0x00;
    bus[0x0100] = 0xFE;
    cpu->x=0x01;
    cpu->pc = 0x00FE;
    uint16_t expected = 0xFE01;

    CpuAddressingModeResult res = absolute_x_addressing_mode(cpu);
    ck_assert_msg(res.operand == expected, "result %#04x != expected %#04x", res.operand, expected);
}
END_TEST

START_TEST(test_absolute_y_addressing_mode){
    bus[0x00FE] = 0x15;
    bus[0x00FF] = 0x00;
    bus[0x0100] = 0xFE;
    cpu->y=0x01;
    cpu->pc = 0x00FE;
    uint16_t expected = 0xFE01;

    CpuAddressingModeResult res = absolute_y_addressing_mode(cpu);
    ck_assert_msg(res.operand == expected, "result %#04x != expected %#04x", res.operand, expected);

}
END_TEST

START_TEST(test_zp_addressing_mode){
    bus[0x00FE] = 0x15;
    bus[0x00FF] = 0x16;
    cpu->pc = 0x00FE;

    CpuAddressingModeResult res = zero_page_addressing_mode(cpu);
    uint16_t expected = 0x0016;
    ck_assert_msg(res.operand == expected, "result %#04x != expected %#04x", res.operand, expected);
}
END_TEST

START_TEST(test_zpx_addressing_mode){
    bus[0x00FD] = 0x15;
    bus[0x00FE] = 0xa0;
    cpu->pc = 0x00FD;
    cpu->x = 0x01;

    CpuAddressingModeResult res = zero_page_x_offset_addressing_mode(cpu);
    uint16_t expected = 0x00a1;
    ck_assert_msg(res.operand == expected, "result %#04x != expected %#04x", res.operand, expected);
}
END_TEST

START_TEST(test_zpy_addressing_mode){
    bus[0x00FD] = 0x15;
    bus[0x00FE] = 0xa0;
    cpu->pc = 0x00FD;
    cpu->y = 0x01;

    CpuAddressingModeResult res = zero_page_y_offset_addressing_mode(cpu);
    uint16_t expected = 0x00a1;
    ck_assert_msg(res.operand == expected, "result %#04x != expected %#04x", res.operand, expected);

}
END_TEST

START_TEST(lookup_cpu_instruction_from_opcode){

    CpuInstruction* result = cpu_cycle(cpu);
    ck_assert(result->name == ASL);
    ck_assert(result->cycles_left == 6);
    ck_assert(result->addressing_mode == ZP);

}
END_TEST

START_TEST(test_absolute_adc_command) {
    // Operand location
    bus[0x00FE] = 0xDD;
    bus[0x00FF] = 0xDD;
    // Operand value
    bus[0xDDDD] = 0x01;
    cpu->pc = 0x00FD;

    const CpuState s = ADC_(*cpu, addr_mode_lookup[ABSOLUTE]);
    ck_assert_msg(s.a == 0x01, "result %#04x != expected %#04x", s.a, 0x01);
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
    tcase_add_test(tc, test_absolute_adc_command);

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
