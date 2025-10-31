#ifndef NVS_TPP_
#define NVS_TPP_

template<typename T>
esp_err_t NVS::write(const char* key, const T& value) {
    esp_err_t err;
    auto h = handle;

    if constexpr (std::is_same_v<T, int32_t>)
        err = nvs_set_i32(h, key, value);
    else if constexpr (std::is_same_v<T, uint32_t>)
        err = nvs_set_u32(h, key, value);
    else if constexpr (std::is_same_v<T, int64_t>)
        err = nvs_set_i64(h, key, value);
    else if constexpr (std::is_same_v<T, uint64_t>)
        err = nvs_set_u64(h, key, value);
    else if constexpr (std::is_same_v<T, int16_t>)
        err = nvs_set_i16(h, key, value);
    else if constexpr (std::is_same_v<T, uint16_t>)
        err = nvs_set_u16(h, key, value);
    else if constexpr (std::is_same_v<T, int8_t>)
        err = nvs_set_i8(h, key, value);
    else if constexpr (std::is_same_v<T, uint8_t>)
        err = nvs_set_u8(h, key, value);
    else if constexpr (std::is_same_v<T, std::string>)
        err = nvs_set_str(h, key, value.c_str());
    else
        return ESP_ERR_INVALID_ARG;

    if (err == ESP_OK) {
        err = nvs_commit(h);
        ESP_LOGI(NVS_TAG, "Write key '%s' success", key);
    } else {
        ESP_LOGE(NVS_TAG, "Write key '%s' failed (%s)", key, esp_err_to_name(err));
    }

    return err;
}

template<typename T>
T NVS::read(const char* key, const T& default_value) {
    esp_err_t err;
    auto h = handle;
    T value = default_value;

    if constexpr (std::is_same_v<T, int32_t>)
        err = nvs_get_i32(h, key, &value);
    else if constexpr (std::is_same_v<T, uint32_t>)
        err = nvs_get_u32(h, key, &value);
    else if constexpr (std::is_same_v<T, int64_t>)
        err = nvs_get_i64(h, key, &value);
    else if constexpr (std::is_same_v<T, uint64_t>)
        err = nvs_get_u64(h, key, &value);
    else if constexpr (std::is_same_v<T, int16_t>)
        err = nvs_get_i16(h, key, &value);
    else if constexpr (std::is_same_v<T, uint16_t>)
        err = nvs_get_u16(h, key, &value);
    else if constexpr (std::is_same_v<T, int8_t>)
        err = nvs_get_i8(h, key, &value);
    else if constexpr (std::is_same_v<T, uint8_t>)
        err = nvs_get_u8(h, key, &value);
    else if constexpr (std::is_same_v<T, std::string>) {
        size_t size = 0;
        err = nvs_get_str(h, key, nullptr, &size);
        if (err == ESP_OK && size > 0) {
            std::string temp(size, '\0');
            err = nvs_get_str(h, key, temp.data(), &size);
            if (err == ESP_OK) {
                if (!temp.empty() && temp.back() == '\0')
                    temp.pop_back();
                value = temp;
            }
        }
    }
    else
        return default_value;

    if (err == ESP_ERR_NVS_NOT_FOUND)
        ESP_LOGW(NVS_TAG, "Key '%s' not found, using default", key);
    else if (err != ESP_OK)
        ESP_LOGE(NVS_TAG, "Read key '%s' failed (%s)", key, esp_err_to_name(err));
    else
        ESP_LOGI(NVS_TAG, "Read key '%s' success", key);

    return value;
}

#endif // NVS_TPP_