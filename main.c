#include "main.h"
#include <avr/sleep.h>
#include <avr/interrupt.h>

// Khai báo biến để lưu số bước đã quay
volatile uint8_t sleep_counter = 0;  // Biến đếm số lần ngắt Timer1

// Khởi tạo Timer1
void timer1_init() {
    TCCR1B |= (1 << WGM12); // Chọn chế độ CTC
    TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler = 1024
    OCR1A = 15624; // Giá trị so sánh cho 1 giây (F_CPU / (1024 * 1Hz) - 1)
    TIMSK1 |= (1 << OCIE1A); // Bật ngắt khi so sánh khớp
}

// Hàm để cho hệ thống ngủ 10 giây
void sleep_for_10s() {
    sleep_counter = 0; // Số giây cần ngủ
    while (sleep_counter < 5) {
        set_sleep_mode(SLEEP_MODE_IDLE); // Chế độ ngủ giữ hoạt động ngoại vi
        sleep_enable(); // Kích hoạt ngủ
        sei(); // Bật ngắt toàn cục
        sleep_cpu(); // Chuyển sang trạng thái ngủ
        sleep_disable(); // Tắt chế độ ngủ sau khi bị đánh thức
    }
}

// Ngắt Timer1
ISR(TIMER1_COMPA_vect) {
  sleep_counter++;
    // Không cần làm gì, chỉ để đánh thức CPU
}

// Khởi tạo hệ thống
void setup() {
    i2c_init();
    ssd1306_init();
    ssd1306_display_begin();
    tsl2561_init();
    timer1_init(); // Khởi tạo Timer1
    init_motor();
}

// Ngắt INT0 để xử lý tín hiệu từ encoder
ISR(INT0_vect) {
    uint8_t current_state = (PIND & (1 << ENC_A)) | ((PIND & (1 << ENC_B)) >> 1); // Đọc trạng thái hiện tại của tín hiệu A và B

    // Giải mã 4 pha
    if (((prev_state == 0) && (current_state == 1)) ||
        ((prev_state == 1) && (current_state == 3)) ||
        ((prev_state == 3) && (current_state == 2)) ||
        ((prev_state == 2) && (current_state == 0))) {
        encoder_pos++;  // Xoay theo chiều thuận
    } else if (((prev_state == 0) && (current_state == 2)) ||
               ((prev_state == 2) && (current_state == 3)) ||
               ((prev_state == 3) && (current_state == 1)) ||
               ((prev_state == 1) && (current_state == 0))) {
        encoder_pos--;  // Xoay theo chiều ngược
    } else {
      return -1;
    }

    prev_state = current_state; // Cập nhật trạng thái trước
}

int main(void)
{
    setup();
    DDRB |= (1 << PB0);  // Bật LED xanh
    DDRB |= (1 << PB1);  // Bật LED đỏ
    DDRB &= ~(1 << PB2); // Đặt PB2 làm đầu vào
    PORTB &= ~(1 << PB2);
    uint16_t step_count = 0;  // Biến lưu số bước đã quay
    uint16_t light_level_max = 0;  // Lưu giá trị ánh sáng tối đa
    uint16_t light_level = 0;

    while (1) {
        light_level = tsl2561_readLight();  // Đọc giá trị cảm biến ánh sáng
        light_level_max = light_level; // Gán giá trị tối đa ban đầu cho light_level_max
        display_light_level(light_level);  // Hiển thị giá trị cảm biến
        display_light_level1(light_level);

        if (light_level > 1000) {
            PORTB |= (1 << PB0);  // Bật LED xanh
            PORTB &= ~(1 << PB1); // Tắt LED đỏ
            motor_forward();  // Quay động cơ về phía trước
            waitForMotorToTurn(1);  // Chờ động cơ quay 1 bước
            motor_stop();  // Dừng động cơ
            step_count++;
            _delay_ms(1000);
            
            while (step_count < 51) { // Quay động cơ tối đa 25 bước
                light_level = tsl2561_readLight();  // Đọc lại giá trị cảm biến ánh sáng
                //light_level_max = light_level;  // Cập nhật giá trị tối đa
                display_light_level(light_level);  // Hiển thị lại giá trị cảm biến//
                display_light_level1(light_level);
                _delay_ms(20);
                if (((light_level - light_level_max) > 1000)  && (light_level > 1000) ) {
                    light_level_max = light_level;  // Cập nhật giá trị tối đa
                    step_count++;  // Tăng số bước đã quay
                    motor_forward();  // Quay động cơ về phía trước
                    waitForMotorToTurn(1);  // Chờ động cơ quay 1 bước
                    motor_stop(); 
                    display_int(step_count);
                    _delay_ms(1000);  // Đợi 1 giây
                    
                } else {
                    motor_reverse();  // Quay lùi động cơ
                    waitForMotorToTurn(1);  // Chờ động cơ quay 1 bước lùi
                    motor_stop();  // Dừng động cơ
                    step_count--;
                    display_int(step_count);
                    break;  // Nếu giá trị nhỏ hơn giá trị tối đa, thoát
                }
            }

            // Kiểm tra nếu số bước vượt quá 18 (hoặc 25), quay ngược lại
            if (step_count >= 51) {
                while (step_count > 0) {
                    motor_reverse();  // Quay lùi động cơ
                    waitForMotorToTurn(55);  // Chờ động cơ quay 1 bước lùi
                    motor_stop();  // Dừng động cơ
                    step_count = 0;
                    _delay_ms(1000);  // Đợi 1 giây
                    
                }
            }
        } else {
            PORTB &= ~(1 << PB0);  // Tắt LED xanh
            PORTB |= (1 << PB1);   // Bật LED đỏ
            motor_stop();  // Dừng động cơ
        }

        _delay_ms(1000);  // Đợi 1 giây
        ssd1306_clear_line(4);  // Xóa dòng hiển thị

        // Đưa hệ thống ngủ 10 giây
        PORTB &= ~(1 << PB0); // Tắt LED trước khi ngủ
        PORTB &= ~(1 << PB1);
        ssd1306_display_off(); // Tắt màn hình OLED
        motor_stop();  // Dừng động cơ sau khi hoàn thành
        sleep_for_10s(); // Gọi hàm ngủ
        setup();  // Khởi động lại hệ thống
    }

    return 0;
}
