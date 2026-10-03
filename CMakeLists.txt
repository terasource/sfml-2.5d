cmake_minimum_required(VERSION 3.10)

project(sfml-2.5d)

# C++ Standart Ayarı
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
file(GLOB SOURCES "*.cpp")
list(REMOVE_ITEM SOURCES 
"${CMAKE_CURRENT_SOURCE_DIR}/client.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/server.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/server_exec.cpp"
)

# --- Hedef (Executable) ---
add_executable(game ${SOURCES}
    collisionmanager.cpp
    collisionmanager.hpp
    texturemanager.cpp
    texture.hpp
    texture.cpp
    collider.cpp
    collider.hpp
    playerState.cpp
    playerState.hpp
    inputHandler.hpp
    inputHandler.cpp
    networkl/serializationlayer/message.cpp
    networkl/serializationlayer/message.hpp
    networkl/serializationlayer/packetreader.cpp
    networkl/serializationlayer/packetreader.hpp
    networkl/serializationlayer/packetwriter.cpp
    networkl/serializationlayer/packetwriter.hpp
    input_types.hpp
    server.cpp
    server.hpp
    client.cpp
    client.hpp
)

# --- Kütüphaneleri Bağla (Linkle) ---
target_link_libraries(game PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio 
    nlohmann_json::nlohmann_json
)

add_executable(server_exec 
    server_exec.cpp
    server.hpp
    server.cpp
    networkl/serializationlayer/message.cpp
    networkl/serializationlayer/message.hpp
    networkl/serializationlayer/packetreader.cpp
    networkl/serializationlayer/packetreader.hpp
    networkl/serializationlayer/packetwriter.cpp
    networkl/serializationlayer/packetwriter.hpp
    playerState.hpp
    playerState.cpp
    )

# --- Asset Kopyalama (Opsiyonel ama Tavsiye Edilir) ---
# Derleme sonrası asset klasörünü build içine kopyalar ki oyun çalışınca resimleri bulabilsin.
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:game>/assets"
)