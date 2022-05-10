<h1 align="center">Welcome to cpp_rapidjson_example 👋</h1>
<p>
  <img alt="Version" src="https://img.shields.io/badge/version-1.0-blue.svg?cacheSeconds=2592000" />
  <a href="https://github.com/Tencent/rapidjson/" target="_blank">
    <img alt="readme.md" src="https://img.shields.io/badge/readme.md-yes-brightgreen.svg" />
  </a>
</p>

> 이 프로젝트는 c++에서 dictionary를 파이썬처럼 value값의 type에 상관없이 사용하기 위해 구현한 코드입니다. 

## requirement

- c++17
- rapidjson

## [rapidjson install](https://github.com/Tencent/rapidjson)

```sh
git clone https://github.com/Tencent/rapidjson.git
cd rapidjson

git submodule update --init
mkdir build
cd build
cmake ..
make
make install
```

## Cmake build
```cmake
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")

# build code

find_package(RapidJSON)
message(STATUS "include path: ${RapidJSON_INCLUDE_DIRS}")
```

## json read

```cpp
int main()
{
  // json -> dictionary
  map<string, variant<string, int, float>> param_dict;
  json_reader("../config.json", param_dict);
  
  // epoch : 1000
  // k1 : -5e-06
  // k2 : 8e-06
  // mode_switch : 1
  // video_path : /data/EdgeFarm_pig/00001.mp4

  // dictionary -> value
  string video_path = get<string>(param_dict["video_path"]);
}
```

## json write

```cpp
int main()
{
  // dictionary
  map<string, variant<string, int, float>> param_dict;
  
  // dictionary value change & import

  // json write
  json_writer("../out.json", param_dict);
}
```

## dictionary value change

```cpp
int main()
{
  // dictionary
  map<string, variant<string, int, float>> param_dict;
  
  // int -> int, float -> float, string -> string
  dict_value_change("video_path", "OOP", param_dict);
  dict_value_change("epoch", 3500, param_dict);
}
```

## dictionary key & value insert

```cpp
int main()
{
  // dictionary
  map<string, variant<string, int, float>> param_dict;
  
  dict_insert("image_count", 1000, param_dict);
  dict_insert("image_path", "/data/img", param_dict);
  dict_insert("image_value", 50.0f, param_dict);
}
```

## dictionary key remove

```cpp
int main()
{
  // dictionary
  map<string, variant<string, int, float>> param_dict;
  
  dict_key_remove("image_path", param_dict);
  dict_key_remove("image_count", param_dict);
  dict_key_remove("image_value", param_dict);
}
```

## Author

👤 **cannonvirus**

* Github: [@cannonvirus](https://github.com/cannonvirus)

## Show your support

Give a ⭐️ if this project helped you!

***
_This README was generated with ❤️ by [readme-md-generator](https://github.com/kefranabg/readme-md-generator)_