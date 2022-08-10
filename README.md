# serv

## Including

To add this library to your project, simply add these two lines to your *CMakeLists.txt*:
```cmake
add_subdirectory(path/to/serv)
target_link_libraries(${PROJECT_NAME} PRIVATE serv::serv)
```

Then include it as:
```cpp
#include <serv/serv.hpp>
```

## TODO

- fix the firewall thing. We don't need to connect to networks other than localhost so this can probably be solved.
