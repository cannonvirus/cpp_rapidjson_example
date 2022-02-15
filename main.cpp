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

using namespace std;
using namespace rapidjson;

void read_json()
{
    string json_path = "/works/cpp_rapidjson_example/config.json";

    //? read json
    std::ifstream ifs;
    ifs.open(json_path);

    if (!ifs.is_open())
    {
        std::cerr << "Could not open file for reading!\n";
    }
    IStreamWrapper isw{ifs};

    Document doc{};
    doc.ParseStream(isw);

    Value &Val_video_path = doc["video_path"];
    Value &Val_mode_switch = doc["mode_switch"];
    Value &Val_k1 = doc["k1"];
    Value &Val_k2 = doc["k2"];
    Value &Val_epoch = doc["epoch"];

    string video_path = Val_video_path.GetString();
    bool mode_switch = Val_mode_switch.GetBool();
    float k1 = Val_k1.GetFloat();
    double k2 = Val_k2.GetDouble();
    int epoch = Val_epoch.GetInt();

    cout << "video_path : " << video_path << endl;
    cout << "mode_switch : " << mode_switch << endl;
    cout << "k1 : " << k1 << endl;
    cout << "k2 : " << k2 << endl;
    cout << "epoch : " << epoch << endl;
}

void write_json()
{
    string json_path = "/works/cpp_rapidjson_example/write.json";
    const char *json_cont = "{\"total_obj_count\":0, \"total_frame\":0, \"total_activity\":0, \"total_weights\":0}";
    Document d;
    d.Parse(json_cont);

    FILE *fp = fopen(json_path.c_str(), "w");

    char writeBuffer[65536];
    FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

    Writer<FileWriteStream> writer(os);
    d.Accept(writer);

    fclose(fp);
}

void read_and_write_json()
{
    string json_path = "/works/cpp_rapidjson_example/config.json";

    //? read json
    std::ifstream ifs;
    ifs.open(json_path);

    if (!ifs.is_open())
    {
        std::cerr << "Could not open file for reading!\n";
    }
    IStreamWrapper isw{ifs};

    Document doc{};
    doc.ParseStream(isw);

    StringBuffer buffer{};
    Writer<StringBuffer> writer{buffer};
    doc.Accept(writer);

    if (doc.HasParseError())
    {
        std::cout << "Error  : " << doc.GetParseError() << '\n'
                  << "Offset : " << doc.GetErrorOffset() << '\n';
    }

    doc["epoch"] = 500;

    std::ofstream ofs{R"(/works/cpp_rapidjson_example/output.json)"};
    if (!ofs.is_open())
    {
        std::cerr << "Could not open file for writing!\n";
    }

    OStreamWrapper osw{ofs};
    Writer<OStreamWrapper> writer2{osw};
    doc.Accept(writer2);
}

int main()
{
    cout << "rapid json example" << endl;
    read_json();
    write_json();
    read_and_write_json();
}