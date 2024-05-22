// Grupp 14: sammanställning av trevliga funktionaliteter ;)
#include <14.h> //bibliotek och definitioner

//statiska grejer
static nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);
static uint8_t uarte_buffer; //lagrar inputs

//startup funktioner
void nRF5340setup() {
	//anger standardkonfigurationer & pins för TX & RX
	const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);
	//initierar UARTE-driver
	nrfx_err_t errr = nrfx_uarte_init(&instance, &config, NULL);
	if (errr != 0) {
		//aktiverar huvudvärk
	}
    uarte_write_clear();
}
void nRF5340LED() {
	//definierar pins
	nrf_gpio_cfg_output(LED1);
	nrf_gpio_cfg_output(LED2);
	nrf_gpio_cfg_output(LED3);
	nrf_gpio_cfg_output(LED4);
	//ger pins startvärden
	nrf_gpio_pin_write(LED1, 1);
	nrf_gpio_pin_write(LED2, 1);
	nrf_gpio_pin_write(LED3, 1);
	nrf_gpio_pin_write(LED4, 1);
}
void nRF5340Buttons() {
	nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(BUTTON3, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(BUTTON4, NRF_GPIO_PIN_PULLUP);
}
void nRF5340RTC() {
	nrfx_rtc_config_t config = NRFX_RTC_DEFAULT_CONFIG;
	//sätter RTCn till 32768 Hz
	nrfx_rtc_enable(&instance);

	//denna funktion är inte klar, jag vettefan hur denhär ska användas.
}

//skriv/läs funktioner
void uarte_write(uint8_t* data, uint8_t length) {
	nrfx_uarte_tx(&instance, data, length, 0);
	while(nrfx_uarte_tx_in_progress(&instance));
}
void uarte_write_clear() {
	uint8_t clearscreen[] = CLEAR_SCREEN;
	nrfx_uarte_tx(&instance, clearscreen, sizeof(clearscreen), 0);
}
void uarte_write_new_line() {
	uint8_t data[] = "\n\r"; 
	uint8_t length = sizeof(data);
	nrfx_uarte_tx(&instance, data, length, 0);
}
int read_int(uint8_t* numberArr) {
	int numberInt = atoi(numberArr);
	return numberInt;
}
void make_string(uint8_t charArray[], int tal) {
	sprintf(charArray, "%d", tal);
}

//skriv-funktioner
int scan_visible_int(int size) {
	uint8_t inputString[size];
	memset(inputString, '\0', sizeof(inputString));
	for (int n = 0; n < sizeof(inputString); n++) {
		nrfx_uarte_rx(&instance, &uarte_buffer, sizeof(uarte_buffer));
		uarte_write(&uarte_buffer, sizeof(uarte_buffer));
		if (uarte_buffer == 13) {
			inputString[n] = '\0';
			break;
		}
		else {
			inputString[n] = uarte_buffer;
		}
	}
	return atoi(inputString);
}
int scan_int(int size) {
	uint8_t inputString[size];
	memset(inputString, '\0', sizeof(inputString));
	for (int n = 0; n < sizeof(inputString); n++) {
		nrfx_uarte_rx(&instance, &uarte_buffer, sizeof(uarte_buffer));
		if (uarte_buffer == 13) {
			inputString[n] = '\0';
			break;
		}
		else {
			inputString[n] = uarte_buffer;
		}
	}
	return atoi(inputString);
}
void scan_visible_string(uint8_t* str, uint8_t lenght) {
	memset(str, '\0', lenght);
	for (int n = 0; n < lenght; n++) {
		nrfx_uarte_rx(&instance, &uarte_buffer, sizeof(uarte_buffer));
		uarte_write(&uarte_buffer, sizeof(uarte_buffer));
		if (uarte_buffer == 13) {
			str[n] = '\0';
			break;
		}
		else {
			str[n] = uarte_buffer;
		}
	}
}
void scan_string(uint8_t* str, uint8_t lenght) {
	memset(str, '\0', lenght);
	for (int n = 0; n < lenght; n++) {
		nrfx_uarte_rx(&instance, &uarte_buffer, sizeof(uarte_buffer));
		if (uarte_buffer == 13) {
			str[n] = '\0';
			break;
		}
		else {
			str[n] = uarte_buffer;
		}
	}
}
