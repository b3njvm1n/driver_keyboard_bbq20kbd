// SPDX-License-Identifier: GPL-2.0-only
/*
 * Keyboard Driver for Blackberry Keyboards BBQ10 from arturo182. Software written by wallComputer.
 * bbqX0kbd_main.c: Main C File.
 */

#include "bbqX0kbd_main.h"

#if (BBQX0KBD_INT == BBQX0KBD_NO_INT)
static atomic_t keepWorking = ATOMIC_INIT(1);
static struct workqueue_struct *workqueue_struct;
#endif

static uint8_t bbqX0kbd_modkeys_to_bits(unsigned short mod_keycode)
{
	uint8_t returnValue;

	switch (mod_keycode) {
	case (KEY_LEFTCTRL):
		returnValue = LEFT_CTRL_BIT;
		break;
	case KEY_RIGHTCTRL:
		returnValue = RIGHT_CTRL_BIT;
		break;
	case KEY_LEFTSHIFT:
		returnValue = LEFT_SHIFT_BIT;
		break;
	case KEY_RIGHTSHIFT:
		returnValue = RIGHT_SHIFT_BIT;
		break;
	case KEY_LEFTALT:
		returnValue = LEFT_ALT_BIT;
		break;
	case KEY_RIGHTALT:
		returnValue = RIGHT_ALT_BIT;
		break;
	}
	return returnValue;
}

static unsigned short bbqX0kbd_get_num_lock_keycode(unsigned short keycode)
{
	unsigned short returnValue;

	switch (keycode) {
	case KEY_W: returnValue = KEY_1; break;
	case KEY_E: returnValue = KEY_2; break;
	case KEY_R: returnValue = KEY_3; break;
	case KEY_S: returnValue = KEY_4; break;
	case KEY_D: returnValue = KEY_5; break;
	case KEY_F: returnValue = KEY_6; break;
	case KEY_Z: returnValue = KEY_7; break;
	case KEY_X: returnValue = KEY_8; break;
	case KEY_C: returnValue = KEY_9; break;
	case KEY_GRAVE: returnValue = KEY_0; break;
	default: returnValue = keycode; break;
	}
	return returnValue;
}

/*
 * FUNCIÓN: Traduce la letra presionada junto a SYM al símbolo exacto del layout US.
 * Retorna el Keycode y modifica force_shift si el símbolo requiere Shift.
 */
static unsigned short bbqX0kbd_get_sym_keycode(unsigned short keycode, int *force_shift)
{
	*force_shift = 0;
	switch (keycode) {
	// Fila 1
	case KEY_Q: *force_shift = 1; return KEY_3;       // #
	case KEY_W: return KEY_1;                         // 1
	case KEY_E: return KEY_2;                         // 2
	case KEY_R: return KEY_3;                         // 3
	case KEY_T: *force_shift = 1; return KEY_9;       // (
	case KEY_Y: *force_shift = 1; return KEY_0;       // )
	case KEY_U: *force_shift = 1; return KEY_MINUS;   // _
	case KEY_I: return KEY_MINUS;                     // -
	case KEY_O: *force_shift = 1; return KEY_EQUAL;   // +
	case KEY_P: *force_shift = 1; return KEY_2;       // @
	// Fila 2
	case KEY_A: *force_shift = 1; return KEY_8;       // *
	case KEY_S: return KEY_4;                         // 4
	case KEY_D: return KEY_5;                         // 5
	case KEY_F: return KEY_6;                         // 6
	case KEY_G: return KEY_SLASH;                     // /
	case KEY_H: *force_shift = 1; return KEY_SEMICOLON; // :
	case KEY_J: return KEY_SEMICOLON;                 // ;
	case KEY_K: return KEY_APOSTROPHE;                // '
	case KEY_L: *force_shift = 1; return KEY_APOSTROPHE; // "
	// Fila 3
	case KEY_Z: return KEY_7;                         // 7
	case KEY_X: return KEY_8;                         // 8
	case KEY_C: return KEY_9;                         // 9
	case KEY_V: *force_shift = 1; return KEY_SLASH;   // ?
	case KEY_B: *force_shift = 1; return KEY_1;       // !
	case KEY_N: return KEY_COMMA;                     // ,
	case KEY_M: return KEY_DOT;                       // .
	// Otros
	case KEY_SPACE: *force_shift = 1; return KEY_7;   // &
	case KEY_GRAVE: return KEY_GRAVE;                 // `
	default: return keycode;
	}
}

static void bbqX0kbd_set_brightness(struct bbqX0kbd_data *bbqX0kbd_data, unsigned short keycode, uint8_t *reportKey)
{
	uint8_t swapVar;

	switch (keycode) {
#if (BBQX0KBD_TYPE == BBQ10KBD_FEATHERWING)
	case KEY_Q:
		*reportKey = 1;
		if (bbqX0kbd_data->screenBrightness > 0xFF - BBQ10_BRIGHTNESS_DELTA)
			bbqX0kbd_data->screenBrightness = 0xFF;
		else
			bbqX0kbd_data->screenBrightness = bbqX0kbd_data->screenBrightness + BBQ10_BRIGHTNESS_DELTA;
		break;
	case KEY_W:
		*reportKey = 1;
		if (bbqX0kbd_data->screenBrightness < BBQ10_BRIGHTNESS_DELTA)
			bbqX0kbd_data->screenBrightness = 0;
		else
			bbqX0kbd_data->screenBrightness = bbqX0kbd_data->screenBrightness - BBQ10_BRIGHTNESS_DELTA;
		break;
	case KEY_S:
		*reportKey = 1;
		swapVar = bbqX0kbd_data->screenBrightness;
		bbqX0kbd_data->screenBrightness = (bbqX0kbd_data->screenBrightness == 0) ? bbqX0kbd_data->lastScreenBrightness : 0;
		bbqX0kbd_data->lastScreenBrightness = swapVar;
		break;
#endif
	case KEY_Z:
		*reportKey = 0;
		if (bbqX0kbd_data->keyboardBrightness > 0xFF - BBQ10_BRIGHTNESS_DELTA)
			bbqX0kbd_data->keyboardBrightness = 0xFF;
		else
			bbqX0kbd_data->keyboardBrightness = bbqX0kbd_data->keyboardBrightness + BBQ10_BRIGHTNESS_DELTA;
		break;
	case KEY_X:
		*reportKey = 0;
		if (bbqX0kbd_data->keyboardBrightness < BBQ10_BRIGHTNESS_DELTA)
			bbqX0kbd_data->keyboardBrightness = 0;
		else
			bbqX0kbd_data->keyboardBrightness = bbqX0kbd_data->keyboardBrightness - BBQ10_BRIGHTNESS_DELTA;
		break;
	case KEY_0:
		*reportKey = 0;
		swapVar = bbqX0kbd_data->keyboardBrightness;
		bbqX0kbd_data->keyboardBrightness = (bbqX0kbd_data->keyboardBrightness == 0) ? bbqX0kbd_data->lastKeyboardBrightness : 0;
		bbqX0kbd_data->lastKeyboardBrightness = swapVar;
		break;
	default:
		*reportKey = 2;
		break;
	}
	if (*reportKey == 0)
		bbqX0kbd_write(bbqX0kbd_data->i2c_client, BBQX0KBD_I2C_ADDRESS, REG_BKL, &bbqX0kbd_data->keyboardBrightness, sizeof(uint8_t));

#if (BBQX0KBD_TYPE == BBQ10KBD_FEATHERWING)
	if (*reportKey == 1)
		bbqX0kbd_write(bbqX0kbd_data->i2c_client, BBQX0KBD_I2C_ADDRESS, REG_BK2, &bbqX0kbd_data->screenBrightness, sizeof(uint8_t));
#endif
}

/*
 * FUNCIÓN: Intercepta y traduce los símbolos antes de enviarlos al sistema.
 */
static void bbqX0kbd_report_key(struct bbqX0kbd_data *bbqX0kbd_data, uint8_t scancode, unsigned short keycode, int pressed)
{
	int shift = 0;
	struct input_dev *input_dev = bbqX0kbd_data->input_dev;

	if (bbqX0kbd_data->modifier_keys_status & RIGHT_ALT_BIT) {
		// Se está presionando SYM. Traducimos la letra al símbolo correspondiente.
		keycode = bbqX0kbd_get_sym_keycode(keycode, &shift);
		
		// "Soltamos" la tecla Alt en el sistema para que no interfiera con la escritura del símbolo.
		input_report_key(input_dev, KEY_RIGHTALT, 0);
		
	} else if (bbqX0kbd_data->modifier_keys_status & LEFT_ALT_BIT) {
		// Si el firmware ya envía el ASCII correcto con LFTALT, lo procesamos aquí.
		switch (scancode) {
		case '!': case '@': case '#': case '$': case '%':
		case '^': case '&': case '*': case '(': case ')':
		case '_': case '+': case '{': case '}': case '|':
		case ':': case '"': case '<': case '>': case '?':
		case '~':
			shift = 1;
			break;
		default:
			break;
		}
		if (scancode >= 0x20 && scancode <= 0x7E) {
			input_report_key(input_dev, KEY_LEFTALT, 0);
		}
	}

	// Si el símbolo requiere Shift, lo enviamos
	if (shift) {
		input_report_key(input_dev, KEY_LEFTSHIFT, pressed);
	}
	
	// Enviamos la tecla final
	input_report_key(input_dev, keycode, pressed);
}

#if (BBQX0KBD_INT == BBQX0KBD_NO_INT)
static void bbqX0kbd_work_handler(struct work_struct *work_struct)
{
	struct bbqX0kbd_data *bbqX0kbd_data;
	struct i2c_client *i2c_client;
	uint8_t fifoData[2];
	uint8_t registerValue;
#if (BBQX0KBD_TYPE == BBQ20KBD_PMOD)
	uint8_t registerX, registerY;
#endif
	uint8_t count;
	uint8_t reportKey = 2;
	int returnValue;
	unsigned short keycode;

	if (atomic_read(&keepWorking) == 1) {
		return;
	}

	bbqX0kbd_data = container_of(work_struct, struct bbqX0kbd_data, delayed_work.work);
	i2c_client = bbqX0kbd_data->i2c_client;

	returnValue = bbqX0kbd_read(i2c_client, BBQX0KBD_I2C_ADDRESS, REG_INT, &registerValue, sizeof(uint8_t));
	if (returnValue < 0) return;
	if (registerValue == 0x00) {
		queue_delayed_work(bbqX0kbd_data->workqueue_struct, &bbqX0kbd_data->delayed_work, msecs_to_jiffies(bbqX0kbd_data->work_rate_ms));
		return;
	}

	if (registerValue & REG_INT_OVERFLOW)
		dev_warn(&i2c_client->dev, "%s overflow occurred.\n", __func__);

	if (registerValue & REG_INT_KEY) {
		returnValue = bbqX0kbd_read(i2c_client, BBQX0KBD_I2C_ADDRESS, REG_KEY, &count, sizeof(uint8_t));
		if (returnValue != 0) {
			queue_delayed_work(bbqX0kbd_data->workqueue_struct, &bbqX0kbd_data->delayed_work, msecs_to_jiffies(bbqX0kbd_data->work_rate_ms));
			return;
		}

		count = count & REG_KEY_KEYCOUNT_MASK;
		while (count) {
			returnValue = bbqX0kbd_read(i2c_client, BBQX0KBD_I2C_ADDRESS, REG_FIF, fifoData, 2*sizeof(uint8_t));
			if (returnValue != 0) {
				queue_delayed_work(bbqX0kbd_data->workqueue_struct, &bbqX0kbd_data->delayed_work, msecs_to_jiffies(bbqX0kbd_data->work_rate_ms));
				return;
			}
			if (fifoData[0] == KEY_PRESSED_STATE || fifoData[0] == KEY_RELEASED_STATE) {
				input_event(bbqX0kbd_data->input_dev, EV_MSC, MSC_SCAN, fifoData[1]);

				keycode = bbqX0kbd_data->keycode[fifoData[1]];
				switch (keycode) {
				case KEY_UNKNOWN: break;
				case KEY_RIGHTSHIFT:
					if (bbqX0kbd_data->modifier_keys_status & LEFT_ALT_BIT && fifoData[0] == KEY_PRESSED_STATE)
						bbqX0kbd_data->lockStatus ^= NUMS_LOCK_BIT;
					fallthrough;
				case KEY_LEFTSHIFT:
				case KEY_RIGHTALT:
				case KEY_LEFTALT:
				case KEY_LEFTCTRL:
				case KEY_RIGHTCTRL:
					if (fifoData[0] == KEY_PRESSED_STATE)
						bbqX0kbd_data->modifier_keys_status |= bbqX0kbd_modkeys_to_bits(keycode);
					else
						bbqX0kbd_data->modifier_keys_status &= ~bbqX0kbd_modkeys_to_bits(keycode);
					fallthrough;
				default:
					if (bbqX0kbd_data->lockStatus & NUMS_LOCK_BIT)
						keycode = bbqX0kbd_get_num_lock_keycode(keycode);
					
#if (BBQX0KBD_TYPE == BBQ20KBD_PMOD)
#if (BBQ20KBD_TRACKPAD_USE == BBQ20KBD_TRACKPAD_AS_MOUSE)
					if (bbqX0kbd_data->modifier_keys_status & LEFT_ALT_BIT && fifoData[1] == 0x05)
						keycode = BTN_RIGHT;
#endif
#endif
					if (bbqX0kbd_data->modifier_keys_status & RIGHT_ALT_BIT && fifoData[0] == KEY_PRESSED_STATE)
						bbqX0kbd_set_brightness(bbqX0kbd_data, keycode, &reportKey);
					
					if (reportKey == 2)
						bbqX0kbd_report_key(bbqX0kbd_data, fifoData[1], keycode, fifoData[0] == KEY_PRESSED_STATE);
					break;
				}
			}
			--count;
		}
		input_sync(bbqX0kbd_data->input_dev);
		registerValue = 0x00;
		bbqX0kbd_write(i2c_client, BBQX0KBD_I2C_ADDRESS, REG_INT, &registerValue, sizeof(uint8_t));
	}

#if (BBQX0KBD_TYPE ==  BBQ20KBD_PMOD)
	if (registerValue & REG_INT_TOUCH) {
		returnValue = bbqX0kbd_read(i2c_client, BBQX0KBD_I2C_ADDRESS, REG_TOX, &registerX, sizeof(uint8_t));
		if (returnValue < 0) return;
		returnValue = bbqX0kbd_read(i2c_client, BBQX0KBD_I2C_ADDRESS, REG_TOY, &registerY, sizeof(uint8_t));
		if (returnValue < 0) return;
#if (BBQ20KBD_TRACKPAD_USE == BBQ20KBD_TRACKPAD_AS_MOUSE)
		input_report_rel(bbqX0kbd_data->input_dev, REL_X, (int8_t)registerX);
		input_report_rel(bbqX0kbd_data->input_dev, REL_Y, (int8_t)registerY);
		input_sync(bbqX0kbd_data->input_dev);
		registerValue = 0x00;
		bbqX0kbd_write(i2c_client, BBQX0KBD_I2C_ADDRESS, REG_INT, &registerValue, sizeof(uint8_t));
#endif
	}
#endif
	queue_delayed_work(bbqX0kbd_data->workqueue_struct, &bbqX0kbd_data->delayed_work, msecs_to_jiffies(bbqX0kbd_data->work_rate_ms));
}

static int bbqX0kbd_queue_work(struct bbqX0kbd_data *bbqX0kbd_data)
{
	INIT_DELAYED_WORK(&bbqX0kbd_data->delayed_work, bbqX0kbd_work_handler);
	atomic_set(&keepWorking, 0);
	return queue_delayed_work(bbqX0kbd_data->workqueue_struct, &bbqX0kbd_data->delayed_work, msecs_to_jiffies(bbqX0kbd_data->work_rate_ms));
}
#endif


#if (BBQX0KBD_INT == BBQX0KBD_USE_INT)
static void bbqX0kbd_read_fifo(struct bbqX0kbd_data *bbqX0kbd_data)
{
	struct i2c_client *i2c_client = bbqX0kbd_data->i2c_client;
	uint8_t fifo_data[2];
	uint8_t count;
	uint8_t pos;
	int returnValue;

	returnValue = bbqX0kbd_read(i2c_client, BBQX0KBD_I2C_ADDRESS, REG_KEY, &count, sizeof(uint8_t));
	if (returnValue != 0) return;
	count = count & REG_KEY_KEYCOUNT_MASK;
	bbqX0kbd_data->fifoCount = count;
	pos = 0;
	while (count) {
		returnValue = bbqX0kbd_read(i2c_client, BBQX0KBD_I2C_ADDRESS, REG_FIF, fifo_data, 2*sizeof(uint8_t));
		if (returnValue != 0) return;
		bbqX0kbd_data->fifoData[pos][0] = fifo_data[0];
		bbqX0kbd_data->fifoData[pos][1] = fifo_data[1];
		++pos;
		--count;
	}
}

static void bbqX0kbd_work_fnc(struct work_struct *work_struct_ptr)
{
	struct bbqX0kbd_data *bbqX0kbd_data;
	struct input_dev *input_dev;
	struct i2c_client *i2c_client;
	unsigned short keycode;

	uint8_t pos = 0;
	uint8_t reportKey = 2;
	uint8_t registerValue = 0x00;

	bbqX0kbd_data = container_of(work_struct_ptr, struct bbqX0kbd_data, work_struct);
	input_dev = bbqX0kbd_data->input_dev;
	i2c_client = bbqX0kbd_data->i2c_client;

	while (bbqX0kbd_data->fifoCount > 0) {
		if (bbqX0kbd_data->fifoData[pos][0] == KEY_PRESSED_STATE || bbqX0kbd_data->fifoData[pos][0] == KEY_RELEASED_STATE) {
			input_event(input_dev, EV_MSC, MSC_SCAN, bbqX0kbd_data->fifoData[pos][1]);

			keycode = bbqX0kbd_data->keycode[bbqX0kbd_data->fifoData[pos][1]];
			switch (keycode) {
			case KEY_UNKNOWN: break;
			case KEY_RIGHTSHIFT:
				if (bbqX0kbd_data->modifier_keys_status & LEFT_ALT_BIT && bbqX0kbd_data->fifoData[pos][0] == KEY_PRESSED_STATE)
					bbqX0kbd_data->lockStatus ^= NUMS_LOCK_BIT;
				fallthrough;
			case KEY_LEFTSHIFT:
			case KEY_RIGHTALT:
			case KEY_LEFTALT:
			case KEY_LEFTCTRL:
			case KEY_RIGHTCTRL:
				if (bbqX0kbd_data->fifoData[pos][0] == KEY_PRESSED_STATE)
					bbqX0kbd_data->modifier_keys_status |= bbqX0kbd_modkeys_to_bits(keycode);
				else
					bbqX0kbd_data->modifier_keys_status &= ~bbqX0kbd_modkeys_to_bits(keycode);
				fallthrough;
			default:
				if (bbqX0kbd_data->lockStatus & NUMS_LOCK_BIT)
					keycode = bbqX0kbd_get_num_lock_keycode(keycode);
				
#if (BBQX0KBD_TYPE == BBQ20KBD_PMOD)
#if (BBQ20KBD_TRACKPAD_USE == BBQ20KBD_TRACKPAD_AS_MOUSE)
				if (bbqX0kbd_data->modifier_keys_status & LEFT_ALT_BIT && bbqX0kbd_data->fifoData[pos][1] == 0x05)
					keycode = BTN_RIGHT;
#endif
#endif
				if (bbqX0kbd_data->modifier_keys_status & RIGHT_ALT_BIT && bbqX0kbd_data->fifoData[pos][0] == KEY_PRESSED_STATE)
					bbqX0kbd_set_brightness(bbqX0kbd_data, keycode, &reportKey);
				
				if (reportKey == 2)
					bbqX0kbd_report_key(bbqX0kbd_data, bbqX0kbd_data->fifoData[pos][1], keycode, bbqX0kbd_data->fifoData[pos][0] == KEY_PRESSED_STATE);
				break;
			}
		}
		++pos;
		--bbqX0kbd_data->fifoCount;
	}

#if (BBQX0KBD_TYPE ==  BBQ20KBD_PMOD)
	if (bbqX0kbd_data->touchInt) {
#if (BBQ20KBD_TRACKPAD_USE == BBQ20KBD_TRACKPAD_AS_MOUSE)
		input_report_rel(input_dev, REL_X, bbqX0kbd_data->rel_x);
		input_report_rel(input_dev, REL_Y, bbqX0kbd_data->rel_y);
		bbqX0kbd_data->touchInt = 0;
#endif
	}
#endif

	input_sync(input_dev);
	registerValue = 0x00;
	bbqX0kbd_write(i2c_client, BBQX0KBD_I2C_ADDRESS, REG_INT, &registerValue, sizeof(uint8_t));
}

static irqreturn_t bbqX0kbd_irq_handler(int irq, void *dev_id)
{
	struct bbqX0kbd_data *bbqX0kbd_data = dev_id;
	struct i2c_client *client = bbqX0kbd_data->i2c_client;
	int returnValue;
	uint8_t registerValue;

	returnValue = bbqX0kbd_read(client, BBQX0KBD_I2C_ADDRESS, REG_INT, &registerValue, sizeof(uint8_t));
	if (returnValue < 0) return IRQ_NONE;
	if (registerValue == 0x00) return IRQ_NONE;
	if (registerValue & REG_INT_OVERFLOW) dev_warn(&client->dev, "%s overflow occurred.\n", __func__);

	if (registerValue & REG_INT_KEY) {
		bbqX0kbd_read_fifo(bbqX0kbd_data);
		schedule_work(&bbqX0kbd_data->work_struct);
	}
#if (BBQX0KBD_TYPE == BBQ20KBD_PMOD)
	if (registerValue & REG_INT_TOUCH) {
		returnValue = bbqX0kbd_read(client, BBQX0KBD_I2C_ADDRESS, REG_TOX, &registerValue, sizeof(uint8_t));
		if (returnValue < 0) return IRQ_NONE;
		bbqX0kbd_data->rel_x = (int8_t)registerValue;
		returnValue = bbqX0kbd_read(client, BBQX0KBD_I2C_ADDRESS, REG_TOY, &registerValue, sizeof(uint8_t));
		if (returnValue < 0) return IRQ_NONE;
		bbqX0kbd_data->rel_y = (int8_t)registerValue;
		bbqX0kbd_data->touchInt = 1;
		schedule_work(&bbqX0kbd_data->work_struct);
	} else
		bbqX0kbd_data->touchInt = 0;
#endif
	return IRQ_HANDLED;
}
#endif

static int bbqX0kbd_probe(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	struct bbqX0kbd_data *bbqX0kbd_data;
	struct input_dev *input;
	int returnValue;
	int i;
	uint8_t registerValue = 0x00;

	bbqX0kbd_data = devm_kzalloc(dev, sizeof(*bbqX0kbd_data), GFP_KERNEL);
	if (!bbqX0kbd_data) return -ENOMEM;
	bbqX0kbd_data->i2c_client = client;
	memcpy(bbqX0kbd_data->keycode, keycodes, sizeof(bbqX0kbd_data->keycode));

	returnValue = bbqX0kbd_write(client, BBQX0KBD_I2C_ADDRESS, REG_RST, &registerValue, sizeof(uint8_t));
	if (returnValue) return -ENODEV;
	msleep(400);

	returnValue = bbqX0kbd_read(client, BBQX0KBD_I2C_ADDRESS, REG_VER, &registerValue, sizeof(uint8_t));
	if (returnValue != 0) return -ENODEV;
	bbqX0kbd_data->version_number = registerValue;

	registerValue = REG_CFG_DEFAULT_SETTING;
	returnValue = bbqX0kbd_write(client, BBQX0KBD_I2C_ADDRESS, REG_CFG, &registerValue, sizeof(uint8_t));
	if (returnValue != 0) return -ENODEV;

#if (BBQX0KBD_TYPE == BBQ20KBD_PMOD)
	registerValue = REG_CFG2_DEFAULT_SETTING;
	returnValue = bbqX0kbd_write(client, BBQX0KBD_I2C_ADDRESS, REG_CF2, &registerValue, sizeof(uint8_t));
	if (returnValue != 0) return -ENODEV;
#endif

#if (BBQX0KBD_INT == BBQX0KBD_NO_INT)
	if (BBQX0KBD_POLL_PERIOD < BBQX0KBD_MINIMUM_WORK_RATE || BBQX0KBD_POLL_PERIOD > BBQX0KBD_MAXIMUM_WORK_RATE)
		bbqX0kbd_data->work_rate_ms = BBQX0KBD_DEFAULT_WORK_RATE;
	else
		bbqX0kbd_data->work_rate_ms = BBQX0KBD_POLL_PERIOD;
#endif

	input = devm_input_allocate_device(dev);
	if (!input) return -ENOMEM;
	bbqX0kbd_data->input_dev = input;

	input->name = client->name;
	input->id.bustype = BBQX0KBD_BUS_TYPE;
	input->id.vendor  = BBQX0KBD_VENDOR_ID;
	input->id.product = BBQX0KBD_PRODUCT_ID;
	input->id.version = BBQX0KBD_VERSION_ID;
	input->keycode = bbqX0kbd_data->keycode;
	input->keycodesize = sizeof(bbqX0kbd_data->keycode[0]);
	input->keycodemax = ARRAY_SIZE(bbqX0kbd_data->keycode);

	for (i = 0; i < NUM_KEYCODES; i++)
		__set_bit(bbqX0kbd_data->keycode[i], input->keybit);

	__clear_bit(KEY_RESERVED, input->keybit);
	__set_bit(EV_REP, input->evbit);
	__set_bit(EV_KEY, input->evbit);
	input_set_capability(input, EV_MSC, MSC_SCAN);
#if (BBQX0KBD_TYPE == BBQ20KBD_PMOD && BBQ20KBD_TRACKPAD_USE == BBQ20KBD_TRACKPAD_AS_MOUSE)
	input_set_capability(input, EV_REL, REL_X);
	input_set_capability(input, EV_REL, REL_Y);
#endif

	bbqX0kbd_data->modifier_keys_status = 0x00;
	bbqX0kbd_data->lockStatus = 0x00;
#if (BBQX0KBD_TYPE == BBQ10KBD_FEATHERWING)
	bbqX0kbd_data->screenBrightness = 0xFF;
	bbqX0kbd_data->lastScreenBrightness = bbqX0kbd_data->screenBrightness;
	bbqX0kbd_write(client, BBQX0KBD_I2C_ADDRESS, REG_BK2, &bbqX0kbd_data->screenBrightness, sizeof(uint8_t));
#endif
	bbqX0kbd_data->keyboardBrightness = 0xFF;
	bbqX0kbd_data->lastKeyboardBrightness = bbqX0kbd_data->keyboardBrightness;
	bbqX0kbd_write(client, BBQX0KBD_I2C_ADDRESS, REG_BKL, &bbqX0kbd_data->keyboardBrightness, sizeof(uint8_t));

#if (BBQX0KBD_INT == BBQX0KBD_USE_INT)
	returnValue = devm_request_threaded_irq(dev, client->irq, NULL, bbqX0kbd_irq_handler, IRQF_SHARED | IRQF_ONESHOT, client->name, bbqX0kbd_data);
	if (returnValue != 0) return returnValue;
	INIT_WORK(&bbqX0kbd_data->work_struct, bbqX0kbd_work_fnc);
#endif

#if (BBQX0KBD_INT == BBQX0KBD_NO_INT)
	bbqX0kbd_data->workqueue_struct = create_singlethread_workqueue("bbqX0kbd_workqueue");
	if (bbqX0kbd_data->workqueue_struct == NULL) return -ENOMEM;
	workqueue_struct = bbqX0kbd_data->workqueue_struct;
	bbqX0kbd_queue_work(bbqX0kbd_data);
#endif
	returnValue = input_register_device(input);
	if (returnValue != 0) return returnValue;

	return 0;
}

static void bbqX0kbd_shutdown(struct i2c_client *client)
{
	int returnValue;
	uint8_t registerValue = 0x00;
	returnValue = bbqX0kbd_write(client, BBQX0KBD_I2C_ADDRESS, REG_BKL, &registerValue, sizeof(uint8_t));
	if (returnValue != 0) return;
#if (BBQX0KBD_TYPE == BBQ10KBD_FEATHERWING)
	returnValue = bbqX0kbd_write(client, BBQX0KBD_I2C_ADDRESS, REG_BK2, &registerValue, sizeof(uint8_t));
#endif
}

static struct i2c_driver bbqX0kbd_driver = {
	.driver = {
		.name = DEVICE_NAME,
		.of_match_table = bbqX0kbd_of_device_id,
	},
	.probe		= bbqX0kbd_probe,
	.shutdown	= bbqX0kbd_shutdown,
	.id_table	= bbqX0kbd_i2c_device_id,
};

static int __init bbqX0kbd_init(void)
{
	int returnValue;
	returnValue = i2c_add_driver(&bbqX0kbd_driver);
	if (returnValue != 0) {
		pr_err("%s Could not initialise BBQX0KBD driver! Error: %d\n", __func__, returnValue);
		return returnValue;
	}
	return returnValue;
}
module_init(bbqX0kbd_init);

static void __exit bbqX0kbd_exit(void)
{
#if (BBQX0KBD_INT == BBQX0KBD_NO_INT)
	atomic_set(&keepWorking, 1);
	flush_workqueue(workqueue_struct);
	msleep(500);
	destroy_workqueue(workqueue_struct);
#endif
	i2c_del_driver(&bbqX0kbd_driver);
}
module_exit(bbqX0kbd_exit);
