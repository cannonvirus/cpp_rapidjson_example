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

void json_check_Count(const string& json_path, map<string, variant<string, int, float>> &json_dict) {
    vector<string> edgefarm_key_list = {"device_id", 
							"end_interval", 
							"hallway_width_cm", 
							"hallway_width_pixel", 
							"limit_min_weight", 
							"limit_max_weight", 
							"weight_bias", 
							"age", 
							"linegap", "linegap_position", 
							"vpi_k1", "vpi_k2", 
							"x_scale", "y_scale", 
							"x_rotate", "y_rotate", 
							"zx_perspect", "zy_perspect", 
							"x_pad", "y_pad", 
							"x_focus", "y_focus",
                            "cam_id", "ship_direction",
                            "upload_time", "reboot_time",
                            "direction"
							};

    for (const auto& edgefarm_key : edgefarm_key_list) {
		bool bKeyInsert = false;
		for (const auto& [k, v] : json_dict) {
			if (edgefarm_key == k) {
				bKeyInsert = true;
				break;
			}
		}
		if (!bKeyInsert) {
			cout << edgefarm_key << " : [Not Find]" << endl;
            if (edgefarm_key == "linegap") dict_insert(edgefarm_key, 0.02f, json_dict);
            else if (edgefarm_key == "linegap_position") dict_insert(edgefarm_key, 0.5f, json_dict);
            else if (edgefarm_key == "age") dict_insert(edgefarm_key, 50, json_dict);
            else if (edgefarm_key == "weight_bias") dict_insert(edgefarm_key, 0.0f, json_dict);
            else if (edgefarm_key == "ship_direction") dict_insert(edgefarm_key, "r", json_dict);
            else if (edgefarm_key == "hallway_width_cm") dict_insert(edgefarm_key, 10.0f, json_dict);
            else if (edgefarm_key == "hallway_width_pixel") dict_insert(edgefarm_key, 10.0f, json_dict); // 앞으로 버전은 무조건 float
            // dev_v1.1.0.0 부터 추가됨
            else if (edgefarm_key == "cam_id") dict_insert(edgefarm_key, -1, json_dict);
            else if (edgefarm_key == "upload_time") dict_insert(edgefarm_key, "01:00:00", json_dict);
            else if (edgefarm_key == "reboot_time") dict_insert(edgefarm_key, "00:00:00", json_dict);
            else if (edgefarm_key == "direction") dict_insert(edgefarm_key, 0, json_dict);
			dict_insert(edgefarm_key, 0, json_dict);
		}
	}

    // hallway_width_pixel (int -> float)

    if (holds_alternative<int>(json_dict["hallway_width_pixel"])) {
        int tmp_px =  get<int>(json_dict["hallway_width_pixel"]);
        dict_value_change("hallway_width_pixel", static_cast<float>(tmp_px), json_dict);
    }

	json_writer(json_path, json_dict);
}