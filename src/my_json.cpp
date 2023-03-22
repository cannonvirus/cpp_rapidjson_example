#include "my_json.h"

using namespace std;
using namespace rapidjson;

template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

void json_reader(string json_path, map<string, variant<string, int, float>> &notebook, bool print_option)
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

    if (print_option)
    {
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

void json_grow(string json_path, map<int, map<string, map<string, float>>> &notebook) {

    std::ifstream ifs{json_path};
    if (!ifs.is_open())
    {
        std::cerr << "Could not open file for reading!\n";
    }
    IStreamWrapper isw{ifs};

    Document doc{};
    doc.ParseStream(isw);

    const Value& info = doc["info"];

    for (SizeType i = 0; i < info.Size(); i++) {
        int id = info[i]["id"].GetInt();
        const char* detection_area = info[i]["detection_area"].GetString();
        const char* food_area = info[i]["food_area"].GetString();
        cout << id << endl;
        cout << detection_area << endl;
        cout << food_area << endl;
        
    }
}

void export_foodarea(string json_path, map<string, map<string, float>> &area_box_dict) {

    ifstream ifs{json_path};
    if (!ifs.is_open())
    {
        cerr << "Could not open file for reading!\n";
    }
    IStreamWrapper isw{ifs};

    Document doc{};
    doc.ParseStream(isw);

    const Value& info = doc["info"];

    vector<float> values;
    
    for (SizeType i = 0; i < info.Size(); i++) {
        // int id = info[i]["id"].GetInt();
        const char* food_area = info[i]["food_area"].GetString();
        std::string str = food_area;
        // cout << id << " " << str << endl;

        stringstream ss(str);
        string token;

        while (getline(ss, token, ',')) {
            values.push_back(stof(token));
        }

        // map<int, map<string, float>> area_box_dict;
        // {0 : {xmin : 0.0, ymin : 1.0 ...}}

        for (int i = 0; i < static_cast<int>(values.size()); i += 4) {
            string obj_index = "EAT" + to_string(i / 4);
            std::map<std::string, float> obj_dict;
            obj_dict["xmin"] = values[i];
            obj_dict["ymin"] = values[i+1];
            obj_dict["width"] = values[i+2];
            obj_dict["height"] = values[i+3];
            area_box_dict[obj_index] = obj_dict;
        }
        
    }

}