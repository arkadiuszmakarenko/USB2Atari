


#define KEY_PRESSED_MAX 6
typedef struct {
	uint8_t lctrl;
	uint8_t lshift;
	uint8_t lalt;
	uint8_t lgui;
	uint8_t rctrl;
	uint8_t rshift;
	uint8_t ralt;
	uint8_t rgui;
	uint8_t keys[KEY_PRESSED_MAX];
} keyboard_code_t;


extern void initKeyboard(UART_HandleTypeDef *uart_handler);
