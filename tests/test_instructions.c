#include "../commands.h"
#include <check.h>
#include "../cpu.c"

uint8_t bus[0xFFFF];
CpuState* cpu;

void setup(void) {
    for(uint16_t i=0;i<sizeof(bus)/sizeof(bus[0]);i++){
        bus[i] = 0x16;
    }
    cpu = init_cpu(bus);
}

void teardown(void) {
    delete_cpu(cpu);
}

START_TEST(test_absolute_adc_command) {
    // Arrange
    // Operand location
    bus[0x00FE] = 0xDD;
    bus[0x00FF] = 0xDD;
    // Operand value
    bus[0xDDDD] = 0x01;
    cpu->pc = 0x00FD;

    // Act
    const CpuState s = ADC_(*cpu, addr_mode_lookup[ABS]);

    // Assert
    ck_assert_msg(s.a == 0x01, "result %#04x != expected %#04x", s.a, 0x01);
}
END_TEST

START_TEST(test_immediate_adc_overflow) {
    // Arrange
    // Immediate operand
    bus[0x00FE] = 0xFF;
    cpu->a = 0xFF;
    cpu->pc = 0x00FD;

    // Act
    const CpuState s = ADC_(*cpu, addr_mode_lookup[IMM]);

    // Assert
    ck_assert_msg(s.a == 0xFF, "Acc value incorrect");
    ck_assert_msg(s.p == 0b00000001, "result %#04x != expected %#04x", s.p, 0b00000001);
}
END_TEST

Suite* create_instruction_case(Suite* s){
    TCase* tc = tcase_create("Instruction tests");

    tcase_add_checked_fixture(tc, setup, teardown);
    tcase_add_test(tc, test_absolute_adc_command);
    tcase_add_test(tc, test_immediate_adc_overflow);

    suite_add_tcase(s, tc);

    return s;
}

int main(){
    int failed = 0;

    Suite *s = suite_create("Instruction tests");

    create_instruction_case(s);

    SRunner* sr = srunner_create(s);
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_set_log(sr, "test_instructions.log");
    srunner_set_xml(sr, "test_instructions.xml");
    srunner_run_all(sr, CK_VERBOSE);

    failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}