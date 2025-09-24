#include "../instructions.c"
#include <check.h>
#include <stdint.h>
#include "../cpu.c"

uint8_t bus[0xFFFF];
CpuState* cpu;

void setup(void) {
    for(uint16_t i=0;i<sizeof(bus)/sizeof(bus[0]);i++){
        bus[i] = 0x00;
    }
    cpu = init_cpu(bus);
}

void teardown(void) {
    delete_cpu(cpu);
}

START_TEST(test_convert_16_bit_uint_to_8_bit_signed_int) {
    uint16_t a = 244;
    int8_t res = convert_16_bit_uint_to_8_bit_signed_int(a);

    ck_assert(res == -12);
}
END_TEST


START_TEST(test_mem_addresses_on_same_page_check) {
    // Arrange
    uint16_t a = 0x00FD;
    uint16_t b = 0x00ED;

    // Act
    bool res = mem_addresses_on_same_page(a, b);

    // Assert
    ck_assert(res == false);

    // Arrange
    a = 0x00FD;
    b = 0x00FD;

    // Act
    res = mem_addresses_on_same_page(a, b);

    // Assert
    ck_assert(res == true);
}
END_TEST



START_TEST(test_absolute_adc_instruction) {
    // Arrange
    // Operand location
    bus[0x00FE] = 0xDD;
    bus[0x00FF] = 0xDD;
    // Operand value
    bus[0xDDDD] = 0x01;
    cpu->pc = 0x00FD;

    // Act
    ADC_(cpu, ABS);

    // Assert
    ck_assert_msg(cpu->a == 0x01, "result %#04x != expected %#04x", cpu->a, 0x01);
}
END_TEST

START_TEST(test_absolute_adc_instruction_with_overflow) {
    // Arrange
    // Operand location
    bus[0x00FE] = 0xDD;
    bus[0x00FF] = 0xDD;

    // Operand value
    bus[0xDDDD] = 0xFF;
    cpu->pc = 0x00FD;
    cpu->a = 0x02;

    // Act
    ADC_(cpu, ABS);

    // Assert
    ck_assert_msg(cpu->a == 0x01, "result %#04x != expected %#04x", cpu->a, 0x01);
    ck_assert_msg(cpu->p == 0x01, "result %#04x != expected %#04x", cpu->a, 0x01);

}
END_TEST

START_TEST(test_immediate_and_instruction) {
    // Arrange
    // Operand location
    bus[0x00FE] = 0x01;

    // Accumulator
    cpu->a = 0x11;
    
    cpu->pc = 0x00FD;

    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[ABS](cpu);

    // Act
    AND_(cpu, addr_mode_data.operand);

    // Assert
    uint8_t expected = 0x01;
    ck_assert_msg(cpu->a == expected, "result %#04x != expected %#04x", cpu->a, expected);
}
END_TEST

START_TEST(test_bcc_instruction) {
    // Arrange
    // Operand location
    bus[0x00FE] = 0x01;

    // Accumulator
    cpu->a = 0x11;
    
    cpu->pc = 0x00FD;

    CpuAddressingModeResult addr_mode_data = addr_mode_lookup[ABS](cpu);

    // Act
    AND_(cpu, addr_mode_data.operand);

    // Assert
    uint8_t expected = 0x01;
    ck_assert_msg(cpu->a == expected, "result %#04x != expected %#04x", cpu->a, expected);
}
END_TEST

Suite* create_instruction_case(Suite* s){
    TCase* tc = tcase_create("Instruction tests");

    tcase_add_checked_fixture(tc, setup, teardown);
    tcase_add_test(tc, test_mem_addresses_on_same_page_check);
    tcase_add_test(tc, test_convert_16_bit_uint_to_8_bit_signed_int);
    tcase_add_test(tc, test_bcc_instruction);
    tcase_add_test(tc, test_absolute_adc_instruction);
    tcase_add_test(tc, test_absolute_adc_instruction_with_overflow);
    tcase_add_test(tc,test_immediate_and_instruction);

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
