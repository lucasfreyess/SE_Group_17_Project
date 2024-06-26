// HTTP Client - FreeRTOS ESP IDF - POST

// archivo extraido y modificado desde https://github.com/SIMS-IOT-Devices/FreeRTOS-ESP-IDF-HTTP-Client/tree/main !!

#include "post_request.h"

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        printf("Attempting connection to WiFi... \n");
        break;
    case WIFI_EVENT_STA_CONNECTED:
        printf("Connected to WiFi! \n");
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("Lost connection to WiFi... \n");
        break;
    case IP_EVENT_STA_GOT_IP:
        printf("Got WiFi's IP! \n\n");
        break;
    default:
        break;
    }
}

void wifi_connection()
{
    // 1 - Wi-Fi/LwIP Init Phase
    esp_netif_init();                    // TCP/IP initiation 					s1.1
    esp_event_loop_create_default();     // event loop 			                s1.2
    esp_netif_create_default_wifi_sta(); // WiFi station 	                    s1.3
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation); // 					                    s1.4
    // 2 - Wi-Fi Configuration Phase
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    
    wifi_config_t wifi_configuration = {
        .sta = 
        {
            .ssid = SSID,
            .password = PASS
        }
    };
    
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    // 3 - Wi-Fi Start Phase
    esp_wifi_start();
    // 4- Wi-Fi Connect Phase
    esp_wifi_connect();
}

esp_err_t client_event_post_handler(esp_http_client_event_handle_t evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        printf("HTTP_EVENT_ON_DATA: %.*s\n", evt->data_len, (char *)evt->data);
        break;
    case HTTP_EVENT_ERROR:
        printf("OCURRIO UN ERROR.... PUEDE QUE SERVIDOR NO HAYA DEVUELTO NADA!!!\n");
        break;
    default:
        //printf("CREOQ EFECTICAMENTE PAGINA NO DEVOLVIO NADA!!!\n");
        break;
    }
    return ESP_OK;
}

esp_err_t client_event_get_handler(esp_http_client_event_handle_t evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        printf("HTTP_EVENT_ON_DATA: %.*s\n", evt->data_len, (char *)evt->data);
        //printf("MODO PULENTO ACTIVADO!!!!!!!!!1\n");
        break;
    case HTTP_EVENT_ERROR:
        printf("OCURRIO UN ERROR.... PUEDE QUE SERVIDOR NO HAYA DEVUELTO NADA!!!\n");
        break;
    default:
        //printf("CREOQ EFECTICAMENTE PAGINA NO DEVOLVIO NADA!!!\n");
        break;
    }
    return ESP_OK;
}

static void rest_get()
{
    esp_http_client_config_t config_get = {
        //.url = "http://worldclockapi.com/api/json/utc/now",
        //.url = "http://157.245.11.33:4000/",//la del ignacio
        .url = "http://64.23.160.52/",
        .method = HTTP_METHOD_GET,
        .cert_pem = NULL,
        .event_handler = client_event_get_handler};
        
    esp_http_client_handle_t client = esp_http_client_init(&config_get);
    
    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}

void post_rest_function(char *label, float score)
{
    //printf("POST REST FUNCTION\npost_data: %s\n\n", post_data);

    esp_http_client_config_t config_post = 
    {
        //.url = "http://127.0.0.1:3000/receive_message",
        //.url = "http://httpbin.org/post",
        //.url = "http://157.245.11.33:4000/",//la del ignacio
        //.url = "http://64.23.160.52/receive_data",
        .url = "http://157.245.11.33:4000/receive_data",
        .method = HTTP_METHOD_POST,
        .cert_pem = NULL,
        .event_handler = client_event_post_handler
    };
        
    esp_http_client_handle_t client = esp_http_client_init(&config_post);

    char json_data[256];
    
    //snprintf(json_data, sizeof(json_data), "{\"message\": \"%s with %f percent accuracy, at %d-%02d-%02d %02d:%02d:%02d\"}", label, score, time_struct.tm_year + 1900, time_struct.tm_mon + 1, time_struct.tm_mday, time_struct.tm_hour, time_struct.tm_min, time_struct.tm_sec);
    snprintf(json_data, sizeof(json_data), "{\"message\": \"%s with %f percent accuracy.\"}", label, score);

    //for (int i = 0; i < 96 * 96; i++) {
    //    snprintf(json_data, sizeof(json_data), "%f, ", image[i]);
    //}

    printf("JSON DATA: \n%s\n\n", json_data);

    esp_http_client_set_post_field(client, json_data, strlen(json_data));
    esp_http_client_set_header(client, "Content-Type", "application/json");

    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}

void wifi_main_function(void)
{
    nvs_flash_init();
    wifi_connection();

    vTaskDelay(2000 / portTICK_PERIOD_MS);
    
}
