/*
 * tsl2561.c
 *
 * Created: 10/11/2024 3:11:33 PM
 *  Author: hop
 */ 
 #include "tsl2561.h"

 void tsl2561_init(void) {
    // Bật nguồn cho TSL2561
    i2c_writeToAddress(TSL2561_ADDR, TSL2561_COMMAND_BIT | TSL2561_CONTROL_REG, TSL2561_POWER_ON);
    // Cấu hình thời gian tích hợp, chế độ tích hợp ngắn, dải đo 13.7ms
    i2c_writeToAddress(TSL2561_ADDR, TSL2561_COMMAND_BIT | TSL2561_TIMING_REG, 0x01);
}


uint16_t tsl2561_readLight(void) {
    uint8_t data0_low = i2c_readFromAddress(TSL2561_ADDR, TSL2561_COMMAND_BIT | TSL2561_DATA0_LOW);
    uint8_t data0_high = i2c_readFromAddress(TSL2561_ADDR, TSL2561_COMMAND_BIT | TSL2561_DATA0_HIGH);
    uint8_t data1_low = i2c_readFromAddress(TSL2561_ADDR, TSL2561_COMMAND_BIT | TSL2561_DATA1_LOW);
    uint8_t data1_high = i2c_readFromAddress(TSL2561_ADDR, TSL2561_COMMAND_BIT | TSL2561_DATA1_HIGH);

    // Kết hợp byte thấp và byte cao để tạo thành giá trị 16-bit
    uint16_t data0 = (data0_high << 8) | data0_low;
    uint16_t data1 = (data1_high << 8) | data1_low;

    // Có thể thêm công thức tính toán lux dựa trên data0 và data1

    return data0; // Tạm thời trả về giá trị kênh 0
}

void tsl2561_powerOff(void) {
    i2c_writeToAddress(TSL2561_ADDR, TSL2561_COMMAND_BIT | TSL2561_CONTROL_REG, TSL2561_POWER_OFF);
}


void display_light_level(uint16_t light_level) {
    char buffer[16];  // Tạo một buffer để chứa chuỗi
    char previous_buffer[16] = "";  // Biến lưu giá trị hiển thị trước đó
    snprintf(buffer, sizeof(buffer), "LUX: %u  ", light_level); // Tạo chuỗi cho giá trị mới
    // So sánh độ dài chuỗi cũ và mới
    if (strlen(previous_buffer) > strlen(buffer)) {
        // Nếu chuỗi cũ dài hơn, xóa phần dư thừa của chuỗi cũ
        ssd1306_display_string(strlen(buffer), 2, "                "); // Xóa phần ký tự dư
    }

    // Hiển thị chuỗi mới
    ssd1306_display_string(0, 2, buffer);
  
    // Cập nhật chuỗi trước đó
    strcpy(previous_buffer, buffer);



}

void display_int(uint16_t int_value) {
    char buffer[16];  // Tạo một buffer để chứa chuỗi
    char previous_buffer[16] = "";  // Biến lưu giá trị hiển thị trước đó
    //float angle_value = 7.2*int_value;
    snprintf(buffer, sizeof(buffer), "Angle: %d", 7*int_value); // Tạo chuỗi cho giá trị mới
    // So sánh độ dài chuỗi cũ và mới
    if (strlen(previous_buffer) > strlen(buffer)) {
      //     // Nếu chuỗi cũ dài hơn, xóa phần dư thừa của chuỗi cũ
        ssd1306_display_string(strlen(buffer), 6, "          "); // Xóa phần ký tự dư
    }

    // Hiển thị chuỗi mới
    ssd1306_display_string(0, 6, buffer);
    // Cập nhật chuỗi trước đó
    strcpy(previous_buffer, buffer);

}

void display_light_level1(uint16_t light_level) {
    char buffer[16];  // Tạo một buffer để chứa chuỗi
    char previous_buffer[16] = "";  // Biến lưu giá trị hiển thị trước đó
    snprintf(buffer, sizeof(buffer), "HSuat: %d", light_level/500); // Tạo chuỗi cho giá trị mới

    // So sánh độ dài chuỗi cũ và mới
    if (strlen(previous_buffer) > strlen(buffer)) {
        // Nếu chuỗi cũ dài hơn, xóa phần dư thừa của chuỗi cũ
        ssd1306_display_string(strlen(buffer), 4, "             "); // Xóa phần ký tự dư
    }

    // Hiển thị chuỗi mới
    ssd1306_display_string(0, 4, buffer);

    // Cập nhật chuỗi trước đó
    strcpy(previous_buffer, buffer);

}