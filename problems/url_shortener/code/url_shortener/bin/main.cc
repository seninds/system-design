#include <iostream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

int main() {
    auto j3 = json::parse("{ \"happy\": true, \"pi\": 3.141 }");
    cout << j3 << endl;
}