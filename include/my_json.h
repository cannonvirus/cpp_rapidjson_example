#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <variant>
#include <map>
#include <string>
#include <vector>
#include <sstream>

// json 불러오기, 정보는 map 형태로 저장됨
void json_reader(std::string json_path, std::map<std::string, std::variant<std::string, int, float>> &notebook, bool print_option);

// json 덮어쓰기
void json_writer(std::string json_path, std::map<std::string, std::variant<std::string, int, float>> &notebook);

// map value값 변경하기
void dict_value_change(std::string key, std::variant<std::string, int, float> value, std::map<std::string, std::variant<std::string, int, float>> &notebook);

void dict_insert(std::string key, std::variant<std::string, int, float> value, std::map<std::string, std::variant<std::string, int, float>> &notebook);

void dict_key_remove(std::string key, std::map<std::string, std::variant<std::string, int, float>> &notebook);

void json_reader_array(std::string json_path, std::map<std::string, std::map<std::string, float>> &notebook);

void json_grow(std::string json_path, std::map<int, std::map<std::string, std::map<std::string, float>>> &notebook);

void export_foodarea(std::string json_path, std::map<std::string, std::map<std::string, float>> &area_box_dict);
// ? ==================================================================================
// ? bool은 적용이 안됨. 0,1 Int값으로 받을 것
// ? float라서 소수점 6자리까지만 적용됨
// map<string, variant<string, int, float>> param_dict;

// ? json reader example
// json_reader("../config.json", param_dict);
// string video_path = get<string>(param_dict["video_path"]);

// ? dict value change example
// dict_value_change("video_path", "OOP", param_dict);
// dict_value_change("epoch", 3500, param_dict);

// ? json writer example
// json_writer("../out.json", param_dict);
// ? ==================================================================================