/*
 * This test is for the functions in Utils.h.
 */

#include "utils/Utils.h"

using namespace Pontilus;
using json = nlohmann::json;

int main() {
    json j = getJSON("assets/tests/a_json_file.json");

    return 0;
}
