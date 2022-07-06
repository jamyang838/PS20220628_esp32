
# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.
idf_component_register(SRCS "main.c" "../source/wifi_app.c" "../source/rgb_led.c" "ble.c" "../source/oled.c"
                       INCLUDE_DIRS "../source")
