#define TESTER_IMPLEMENTATION 
#define TESTER_STRIP_PREFIXES 
#include "thirdparty/tester.h"

char test_test_expected_ouput[] = "[0]: 0\n[1]: 1\n[2]: 2\n[3]: 3\n[4]: 4\n[5]: 5\n[6]: 6\n[7]: 7\n[8]: 8\n[9]: 9\n------------------------------\n[0]: 10\n[1]: 9\n[2]: 8\n[3]: 7\n[4]: 6\n[5]: 5\n[6]: 4\n[7]: 3\n[8]: 2\n[9]: 1\n------------------------------\n[0]: 10\n[1]: 9\n[2]: 8\n[3]: 7\n[4]: 6\n[5]: 5\n[6]: 4\n[7]: 3\n[8]: 2\n[9]: 1\n------------------------------\n[0]: 0\n[1]: 1\n[2]: 2\n[3]: 3\n[4]: 4\n[5]: 5\n[6]: 6\n[7]: 7\n[8]: 8\n[9]: 9\n------------------------------\n";

char test_zeroed_expected_output[] = "Mem after alloc_zero():\n[0]: 0\n[1]: 0\n[2]: 0\n[3]: 0\n[4]: 0\n[5]: 0\n[6]: 0\n[7]: 0\n[8]: 0\n[9]: 0\n------------------------------\n"
"Mem after alloc():\n[0]: -842150451\n[1]: -842150451\n[2]: -842150451\n[3]: -842150451\n[4]: -842150451\n[5]: -842150451\n[6]: -842150451\n[7]: -842150451\n[8]: -842150451\n[9]: -842150451\n------------------------------\n"
"alloc_zeroed() after filling with data:\n[0]: 10\n[1]: 9\n[2]: 8\n[3]: 7\n[4]: 6\n[5]: 5\n[6]: 4\n[7]: 3\n[8]: 2\n[9]: 1\n------------------------------\n"
"alloc() after filling with data:\n[0]: 10\n[1]: 9\n[2]: 8\n[3]: 7\n[4]: 6\n[5]: 5\n[6]: 4\n[7]: 3\n[8]: 2\n[9]: 1\n------------------------------\n"
"Mem after arena_reset_zeroed():\n[0]: 0\n[1]: 0\n[2]: 0\n[3]: 0\n[4]: 0\n[5]: 0\n[6]: 0\n[7]: 0\n[8]: 0\n[9]: 0\n[10]: 0\n[11]: 0\n[12]: 0\n[13]: 0\n[14]: 0\n[15]: 0\n[16]: 0\n[17]: 0\n[18]: 0\n[19]: 0\n[20]: 0\n[21]: 0\n[22]: 0\n[23]: 0\n[24]: 0\n[25]: 0\n[26]: 0\n[27]: 0\n[28]: 0\n[29]: 0\n[30]: 0\n[31]: 0\n[32]: 0\n[33]: 0\n[34]: 0\n[35]: 0\n[36]: 0\n[37]: 0\n[38]: 0\n[39]: 0\n[40]: 0\n[41]: 0\n[42]: 0\n[43]: 0\n[44]: 0\n[45]: 0\n[46]: 0\n[47]: 0\n[48]: 0\n[49]: 0\n[50]: 0\n[51]: 0\n[52]: 0\n[53]: 0\n[54]: 0\n[55]: 0\n[56]: 0\n[57]: 0\n[58]: 0\n[59]: 0\n[60]: 0\n[61]: 0\n[62]: 0\n[63]: 0\n[64]: 0\n[65]: 0\n[66]: 0\n[67]: 0\n[68]: 0\n[69]: 0\n[70]: 0\n[71]: 0\n[72]: 0\n[73]: 0\n[74]: 0\n[75]: 0\n[76]: 0\n[77]: 0\n[78]: 0\n[79]: 0\n[80]: 0\n[81]: 0\n[82]: 0\n[83]: 0\n[84]: 0\n[85]: 0\n[86]: 0\n[87]: 0\n[88]: 0\n[89]: 0\n------------------------------\n"
"Memory after alloc():\n[0]: 0\n[1]: 0\n[2]: 0\n[3]: 0\n[4]: 0\n[5]: 0\n[6]: 0\n[7]: 0\n[8]: 0\n[9]: 0\n[10]: 0\n[11]: 0\n[12]: 0\n[13]: 0\n[14]: 0\n[15]: 0\n[16]: 0\n[17]: 0\n[18]: 0\n[19]: 0\n[20]: 0\n[21]: 0\n[22]: 0\n[23]: 0\n[24]: 0\n[25]: 0\n[26]: 0\n[27]: 0\n[28]: 0\n[29]: 0\n[30]: 0\n[31]: 0\n[32]: 0\n[33]: 0\n[34]: 0\n[35]: 0\n[36]: 0\n[37]: 0\n[38]: 0\n[39]: 0\n[40]: 0\n[41]: 0\n[42]: 0\n[43]: 0\n[44]: 0\n[45]: 0\n[46]: 0\n[47]: 0\n[48]: 0\n[49]: 0\n[50]: 0\n[51]: 0\n[52]: 0\n[53]: 0\n[54]: 0\n[55]: 0\n[56]: 0\n[57]: 0\n[58]: 0\n[59]: 0\n[60]: 0\n[61]: 0\n[62]: 0\n[63]: 0\n[64]: 0\n[65]: 0\n[66]: 0\n[67]: 0\n[68]: 0\n[69]: 0\n[70]: 0\n[71]: 0\n[72]: 0\n[73]: 0\n[74]: 0\n[75]: 0\n[76]: 0\n[77]: 0\n[78]: 0\n[79]: 0\n[80]: 0\n[81]: 0\n[82]: 0\n[83]: 0\n[84]: 0\n[85]: 0\n[86]: 0\n[87]: 0\n[88]: 0\n[89]: 0\n[90]: 0\n[91]: 0\n[92]: 0\n[93]: 0\n[94]: 0\n[95]: 0\n[96]: 0\n[97]: 0\n[98]: 0\n[99]: 0\n------------------------------\n";

char test_flags_expected_output[] = "Arena dynamic page size\ninit_page_size < current_page_size ? true\n------------------------------\n"
"Arena dynamic\nlast_page != start_page ? true\narena_reset()\narena.end == last_page(previous) ? true\n------------------------------\n"
"Arena static\nArena static is full\n";

#define BUILD_DIR "../build/"
#define BUILD_FLAGS "-Wall", "-Wextra", "-g", "-O0"
#define TESTS_DIR "./"

Test tests[] = {
    {
        .cmd = CMD_VEC(BUILD_DIR"zeroed"),
        .judge_by = JUDGE_BY_OUTPUT | JUDGE_BY_EXIT_CODE,
        .expected_exit_code = 0,
        .expected_output = test_zeroed_expected_output,
        .build_cmd = CMD_VEC(CC, BUILD_FLAGS, TESTS_DIR"zeroed.c", "-o", BUILD_DIR"zeroed")
    },
    {
        .cmd = CMD_VEC(BUILD_DIR"flags"),
        .judge_by = JUDGE_BY_OUTPUT | JUDGE_BY_EXIT_CODE,
        .expected_exit_code = 0,
        .expected_output = test_flags_expected_output,
        .build_cmd = CMD_VEC(CC, BUILD_FLAGS, TESTS_DIR"flags.c", "-o", BUILD_DIR"flags")
    },
    {
        .cmd = CMD_VEC(BUILD_DIR"test"),
        .judge_by = JUDGE_BY_OUTPUT | JUDGE_BY_EXIT_CODE,
        .expected_exit_code = 0,
        .expected_output = test_test_expected_ouput,
        .build_cmd = CMD_VEC(CC, BUILD_FLAGS, TESTS_DIR"test.c", "-o", BUILD_DIR"test")
    },
};

int main(int argc, char** argv)
{
    TESTER_REBUILD_YOURSELF(argc, argv);
    size_t tests_cnt = sizeof(tests)/sizeof(Test);

    int build_failed = tests_build(tests, tests_cnt, .async = true);
    printf("Failed %d test builds\n", build_failed);
    if (build_failed > 0) {
        exit(1);
    }

    int tests_failed = tests_run(tests, tests_cnt, .async = true);
    printf("Failed %d tests\n", tests_failed);
    if (tests_failed > 0) {
        exit(1);
    }
    //printf("%*s\n", tests[0]._proc->buff_stdout.capacity, tests[0]._proc->buff_stdout.data);

    return 0;
}

