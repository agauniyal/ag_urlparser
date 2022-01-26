#include "urlparser.hpp"
#include <string>

/**
 * AFL CMDLINE
 * ===========
 * AFL_HARDEN=1 afl-clang-fast++ -fsanitize=fuzzer -Ofast -std=c++20 fuzzer_main.cpp ../urlparser.cpp
 * ./urlparser_tlds.cpp -I.. -o f.app
 *
 * afl-fuzz -D -x xml.dict -m none -i input -o output/ ./f.app
 *
 * LibFuzzer
 * =========
 * clang++ -std=c++20 -g -O1 -fsanitize=fuzzer,address,undefined fuzzer_main.cpp ../urlparser.cpp ../urlparser_tlds.cpp
 * -I../ -o f.app
 *
 * ./f.app -jobs=8
 *
 */

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* Data, size_t Size) {
    try {
        UrlParser p(std::string(Data, Data + Size));
        auto i = UrlParser::getDomainForURL(p.url(), false).size();
    } catch (const std::exception& e) {
    }

    return 0;
}
