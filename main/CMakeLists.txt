
#
# Main component of TF Micro project 'person_detection'.
#

idf_component_register(
    SRCS
        "detection_responder.cc"
        "image_provider.cc"
        "main.cc"
        "main_functions.cc"
        "model_settings.cc"
        "person_detect_model_data.cc"
        "app_camera_esp.c"
        "esp_cli.c"
        "post_request.c"
    
    PRIV_REQUIRES console static_images spi_flash esp_psram esp_wifi nvs_flash esp_http_client
    INCLUDE_DIRS "")
