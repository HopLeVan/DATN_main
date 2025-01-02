#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define IN3 PD3
#define IN4 PD4
#define ENB PD5
#define ENC_A PD2
#define ENC_B PD7

int v = 255;                  // Tốc độ quay (giá trị từ 0 đến 255)
volatile int encoder_pos = 0; // Vị trí hiện tại của encoder
volatile uint8_t prev_state = 0;  // Trạng thái trước đó của tín hiệu AB

void init_motor() {
  
    // Thiết lập các chân IN3, IN4, ENB thành đầu ra
    DDRD |= (1 << IN3) | (1 << IN4) | (1 << ENB);  // Thiết lập các chân PD3, PD4, PD5 là OUTPUT
    DDRD &= ~((1 << ENC_A) | (1 << ENC_B));        // Thiết lập ENC_A, ENC_B là đầu vào

    // Thiết lập PWM cho ENB (sử dụng Timer0, chân PD5 - OC0B)
    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0B1);  // Fast PWM, non-inverted mode on OCR0B
    TCCR0B = (1 << CS01);  // Chọn prescaler = 8, bắt đầu Timer0

    // Thiết lập ngắt ngoài cho ENC_A (PD2 - INT0)
    EICRA |= (1 << ISC00);  // Ngắt khi có thay đổi cạnh
    EIMSK |= (1 << INT0);   // Cho phép ngắt INT0
    
    prev_state = (PIND & (1 << ENC_A)) | ((PIND & (1 << ENC_B)) >> 1); // Lưu trạng thái ban đầu

    sei(); // Kích hoạt ngắt toàn cục
}


void motor_forward() {
    PORTD &= ~(1 << IN4);  // IN4 = LOW (PD4)
    PORTD |= (1 << IN3);   // IN3 = HIGH (PD3)
    OCR0B = v;             // Gán giá trị cho OCR0B để điều chỉnh độ rộng xung PWM
}


void motor_reverse() {
    PORTD &= ~(1 << IN3);  // IN3 = LOW (PD3)
    PORTD |= (1 << IN4);   // IN4 = HIGH (PD4)
    OCR0B = v;             // Gán giá trị cho OCR0B để điều chỉnh độ rộng xung PWM
}

void motor_stop() {
    PORTD &= ~((1 << IN3) | (1 << IN4)); // Dừng cả hai chân IN3 và IN4
    OCR0B = 0;                           // Tắt PWM cho ENB
}

void waitForMotorToTurn(int steps) {
    int target_pos = encoder_pos + steps; // Mục tiêu dựa trên số bước cần quay

    while (encoder_pos < target_pos) {
        encoder_pos++;  // Cập nhật vị trí (giả lập, nếu dùng encoder thực thì không cần)
        _delay_ms(100); // Nghỉ 1 giây giữa các bước
    }
}
#endif // MOTOR_CONTROL_H
