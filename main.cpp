#include <iostream>
#include <sstream>

#include "my_json.h"

using namespace std;

int main() 
{
    string json_path = "/works/cpp_rapidjson_example/example.json";
    map<string, map<string, float>> area_box_dict;

    export_foodarea(json_path, area_box_dict);

    // Print the resulting map
    for (auto const& [key, val] : area_box_dict) {
        std::cout << key << ": {";
        for (auto const& [k, v] : val) {
            std::cout << k << ": " << v << ", ";
        }
        std::cout << "}" << std::endl;
    }

    for (auto &kv : area_box_dict){
		cout << kv.first << endl;
		cout << area_box_dict[kv.first]["xmin"] << endl;
		cout << area_box_dict[kv.first]["ymin"] << endl;
		cout << area_box_dict[kv.first]["width"] << endl;
		cout << area_box_dict[kv.first]["height"] << endl;
	}
}