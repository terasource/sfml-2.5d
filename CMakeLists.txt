cmake_minimum_required(VERSION 3.10)

project(sfml-2.5d)

# C++ Standart Ayarı

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Windows Özel Ayarları ---
# Eğer Windows'taysan kütüphanelerin yerini buraya yazmalısın.
if(WIN32)
    # ÖRNEK: C:/SFML-3.1/lib/cmake/SFML gibi kendi yolunu yaz
    # set(SFML_DIR "C:/Kutuphaneler/SFML-3.1/lib/cmake/SFML")
    # set(nlohmann_json_DIR "C:/Kutuphaneler/nlohmann_json/lib/cmake/nlohmann_json")
endif()

if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "opt/homebrew")
endif()

# --- Kütüphaneleri Bul ---
# SFML 3.x
find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Window System Audio)

# nlohmann_json (paru -S nlohmann-json ile kurduğun paket)
find_package(nlohmann_json REQUIRED)

# --- Kaynak Dosyalar ---
file(GLOB_RECURSE SOURCES CMAKE_CONFIGURE_DEPENDS "*.cpp")

list(REMOVE_ITEM SOURCES 
"${CMAKE_CURRENT_SOURCE_DIR}/networkl/client/client.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/networkl/server/server.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/networkl/server/server_exec.cpp"
)

list(FILTER SOURCES EXCLUDE REGEX "/(build|debug)/")


# --- Hedef (Executable) ---
add_executable(game ${SOURCES}
    player/collisionmanager.cpp
    player/collisionmanager.hpp
    texture/texturemanager.cpp
    texture/texture.hpp
    texture/texture.cpp
    player/collider.cpp
    player/collider.hpp
    player/playerState.cpp
    player/playerState.hpp
    networkl/client/inputHandler.hpp
    networkl/client/inputHandler.cpp
    networkl/serializationlayer/message.cpp
    networkl/serializationlayer/message.hpp
    networkl/serializationlayer/packetreader.cpp
    networkl/serializationlayer/packetreader.hpp
    networkl/serializationlayer/packetwriter.cpp
    networkl/serializationlayer/packetwriter.hpp
    networkl/client/input_types.hpp
    networkl/server/server.cpp
    networkl/server/server.hpp
    networkl/client/client.cpp
    networkl/client/client.hpp
    texture/spritemanager.cpp
    texture/spritemanager.hpp
    texture/sprite.cpp
    texture/sprite.hpp
)

if(MSVC)
    target_compile_options(game PRIVATE /W4 /WX)
else()
    target_compile_options(game PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# --- Kütüphaneleri Bağla (Linkle) ---
target_link_libraries(game PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio 
    nlohmann_json::nlohmann_json
)

add_executable(server_exec
    networkl/server/server_exec.cpp
    networkl/server/server.hpp
    networkl/server/server.cpp
    networkl/serializationlayer/message.cpp
    networkl/serializationlayer/message.hpp
    networkl/serializationlayer/packetreader.cpp
    networkl/serializationlayer/packetreader.hpp
    networkl/serializationlayer/packetwriter.cpp
    networkl/serializationlayer/packetwriter.hpp
    player/playerState.hpp
    player/playerState.cpp
)

target_include_directories(game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(server_exec PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# --- Asset Kopyalama (Opsiyonel ama Tavsiye Edilir) ---
# Derleme sonrası asset klasörünü build içine kopyalar ki oyun çalışınca resimleri bulabilsin.
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:game>/assets"
)