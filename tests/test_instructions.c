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
    // Operand_Address location
    bus[0x00FE] = 0xDD;
    bus[0x00FF] = 0xDD;
    // Operand_Address value
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
    // Operand_Address location
    bus[0x00FE] = 0xDD;
    bus[0x00FF] = 0xDD;

    // Operand_Address value
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
    // Operand_Address location
    bus[0x00FE] = 0x01;

    // Accumulator
    cpu->a = 0x11;
    
    cpu->pc = 0x00FD;


    // Act
    AND_(cpu, IMM);

    // Assert
    uint8_t expected = 0x01;
    ck_assert_msg(cpu->a == expected, "result %#04x != expected %#04x", cpu->a, expected);
}
END_TEST

START_TEST(test_bcc_instruction_same_page) {
    // Arrange

    set_status_flag(cpu, C, false);
    int8_t offset = -12;
    bus[0x00FE] = (uint16_t)offset;
    cpu->pc = 0x00FD;

    // Act
    CpuInstructionResult res = BCC_(cpu, RELATIVE);

    // Assert
    uint16_t expected_pc_addr = 0x00F1;
    uint8_t expected_additional_cpu_cycles = 1;

    ck_assert_msg(cpu->pc == expected_pc_addr, "result %#04x != expected %#04x", cpu->pc, expected_pc_addr);
    ck_assert_msg(res.additional_cpu_cycles == expected_additional_cpu_cycles, "got %d additional cycles != expected %dx", res.additional_cpu_cycles, expected_additional_cpu_cycles);
}
END_TEST

START_TEST(test_bcc_instruction_different_page) {
    // Arrange

    set_status_flag(cpu, C, false);
    int8_t offset = 4;
    bus[0x00EE] = (uint16_t)offset;
    cpu->pc = 0x00ED;

    // Act
    CpuInstructionResult res = BCC_(cpu, RELATIVE);

    // Assert
    uint16_t expected_pc_addr = 0x00F1;
    uint8_t expected_additional_cpu_cycles = 2;

    ck_assert_msg(cpu->pc == expected_pc_addr, "result %#04x != expected %#04x", cpu->pc, expected_pc_addr);
    ck_assert_msg(res.additional_cpu_cycles == expected_additional_cpu_cycles, "got %d additional cycles != expected %dx", res.additional_cpu_cycles, expected_additional_cpu_cycles);
}
END_TEST

Suite* create_instruction_case(Suite* s){
    TCase* tc = tcase_create("Instruction tests");

    tcase_add_checked_fixture(tc, setup, teardown);
    tcase_add_test(tc, test_mem_addresses_on_same_page_check);
    tcase_add_test(tc, test_convert_16_bit_uint_to_8_bit_signed_int);
    tcase_add_test(tc, test_bcc_instruction_same_page);
    tcase_add_test(tc, test_bcc_instruction_different_page);
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
