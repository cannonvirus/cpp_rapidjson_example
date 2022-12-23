#include <iostream>

#include "my_json.h"

using namespace std;

int main()
{
    // 주의사항
    // bool은 적용이 안됨. 0,1 Int값으로 받을 것
    // float라서 소수점 6자리까지만 적용됨
    map<string, variant<string, int, float>> param_dict;

    // json reader
    string json_path = "/works/cpp_rapidjson_example/config.json";
    json_reader(json_path, param_dict, false);

    json_check_Count(json_path, param_dict);

    float px = get<float>(param_dict["hallway_width_pixel"]);
    cout << px << endl;
}