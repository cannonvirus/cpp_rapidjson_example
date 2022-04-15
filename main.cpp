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

    cout << "==========================================================" << endl;
    for (const auto &[k, v] : notebook)
    {
        cout << k << " : ";
        visit([](const auto &x)
              { cout << x; },
              v);
        cout << '\n';
    }
    cout << "==========================================================" << endl;
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
                     [&notebook, &key](double arg)
                     { notebook[key].emplace<float>(arg); },
                     [&notebook, &key](const std::string &arg)
                     { notebook[key].emplace<string>(arg); }},
          value);
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

    // json writer
    json_writer("../out.json", param_dict);
}