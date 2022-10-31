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

using namespace std;
using namespace rapidjson;

template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

void json_reader(string json_path, map<string, variant<string, int, float>> &notebook)
{
    std::ifstream ifs{json_path};
    if (!ifs.is_open())
    {
        std::cerr << "Could not open file for reading!\n";
    }
    IStreamWrapper isw{ifs};

    Document doc{};
    doc.ParseStream(isw);

    Value::MemberIterator M;

    for (M = doc.MemberBegin(); M != doc.MemberEnd(); M++)
    {
        // key = M->name.GetString();
        string key_str = M->name.GetString();
        if (M->value.IsString())
        {
            string value_str = M->value.GetString();
            notebook[key_str] = value_str;
        }
        else if (M->value.IsFloat())
        {
            float value_float = M->value.GetFloat();
            notebook[key_str] = value_float;
        }
        else if (M->value.IsInt())
        {
            int value_int = M->value.GetInt();
            notebook[key_str] = value_int;
        }
    }

    // cout << "==========================================================" << endl;
    // for (const auto &[k, v] : notebook)
    // {
    //     cout << k << " : ";
    //     visit([](const auto &x)
    //           { cout << x; },
    //           v);
    //     cout << '\n';
    // }
    // cout << "==========================================================" << endl;
}

void json_writer(string json_path, map<string, variant<string, int, float>> &notebook)
{
    string component = "{";
    for (const auto &[k, v] : notebook)
    {
        component.append("\"");
        component.append(k);
        component.append("\":");

        visit(overloaded{[&component](int arg)
                         { component.append(to_string(arg)); },
                         [&component](double arg)
                         { component.append(to_string(arg)); },
                         [&component](const std::string &arg)
                         {  component.append("\"");
                            component.append(arg);
                            component.append("\""); }},
              v);
        component.append(", ");
    };
    component.erase(component.length() - 2, 2);
    component.append("}");

    Document d;
    d.Parse(component.c_str());

    FILE *fp = fopen(json_path.c_str(), "w");

    char writeBuffer[65536];
    FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

    Writer<FileWriteStream> writer(os);
    d.Accept(writer);

    fclose(fp);
}

void dict_value_change(string key, variant<string, int, float> value, map<string, variant<string, int, float>> &notebook)
{
    visit(overloaded{[&notebook, &key](int arg)
                     { notebook[key].emplace<int>(arg); },
                     [&notebook, &key](float arg)
                     { notebook[key].emplace<float>(arg); },
                     [&notebook, &key](const std::string &arg)
                     { notebook[key].emplace<string>(arg); }},
          value);
}

void dict_insert(string key, variant<string, int, float> value, map<string, variant<string, int, float>> &notebook)
{
    visit(overloaded{[&notebook, &key](int arg)
                     {
                         notebook.insert({key, arg});
                     },
                     [&notebook, &key](float arg)
                     {
                         notebook.insert({key, arg});
                     },
                     [&notebook, &key](const std::string &arg)
                     {
                         notebook.insert({key, arg});
                     }},
          value);
}

void dict_key_remove(string key, map<string, variant<string, int, float>> &notebook)
{
    notebook.erase(key);
}

void json_reader_array(string json_path, map<string, map<string, float>> &notebook)
{
    std::ifstream ifs{json_path};
    if (!ifs.is_open())
    {
        std::cerr << "Could not open file for reading!\n";
    }
    IStreamWrapper isw{ifs};

    Document doc{};
    doc.ParseStream(isw);

    Value::MemberIterator M;

    for (M = doc.MemberBegin(); M != doc.MemberEnd(); M++)
    {
        // key = M->name.GetString();
        string key_str = M->name.GetString();

        if (M->value.IsArray()) {
            array<string, 4> box = {"xmin", "ymin", "width", "height"};
            int idx = 0;
            for (auto& v : M->value.GetArray()) {
                notebook[key_str][box[idx]] = v.GetFloat();
                idx++;
            }
        }
    }

}

int main()
{
    // 주의사항
    // bool은 적용이 안됨. 0,1 Int값으로 받을 것
    // float라서 소수점 6자리까지만 적용됨
    map<string, variant<string, int, float>> param_dict;

    // json reader
    json_reader("../config.json", param_dict);
    string video_path = get<string>(param_dict["video_path"]);

    // dict value change
    dict_value_change("video_path", "OOP", param_dict);
    dict_value_change("epoch", 3500, param_dict);

    // dict value insert
    dict_insert("image_count", 1000, param_dict);
    dict_insert("image_path", "/data/img", param_dict);
    dict_insert("image_value", 50.0f, param_dict);

    // dict key remove
    dict_key_remove("image_path", param_dict);
    dict_key_remove("image_count", param_dict);
    dict_key_remove("image_value", param_dict);

    // json writer
    json_writer("../out.json", param_dict);


    // NOTE 1.1 ----------------
    map<string, map<string, float>> area_box_dict;
    json_reader_array("/works/cpp_rapidjson_example/room0.json", area_box_dict);

    for (auto &kv : area_box_dict){
		cout << kv.first << endl;
        cout << "xmin   : " << area_box_dict[kv.first]["xmin"] << endl;
        cout << "ymin   : " << area_box_dict[kv.first]["ymin"] << endl;
        cout << "width  : " << area_box_dict[kv.first]["width"] << endl;
        cout << "height : " << area_box_dict[kv.first]["height"] << endl;
	}

}