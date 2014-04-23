/*
 *  Oolite board support
 *
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/gpio.h>

#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "eeprom.h"
#include "dev-wmac.h"
#include "machtypes.h"
#include "dev-usb.h"

/*
#define GS_OOLITE_GPIO0			0
#define GS_OOLITE_GPIO1			1
#define GS_OOLITE_GPIO6			6
#define GS_OOLITE_GPIO7			7
#define GS_OOLITE_GPIO8			8
#define GS_OOLITE_GPIO12			12
#define GS_OOLITE_GPIO13			13
#define GS_OOLITE_GPIO14			14
#define GS_OOLITE_GPIO15			15
#define GS_OOLITE_GPIO16			16
#define GS_OOLITE_GPIO17			17
*/
#define GS_OOLITE_GPIO18			18  //for usb switch
/*#define GS_OOLITE_GPIO19			19
#define GS_OOLITE_GPIO20			20
#define GS_OOLITE_GPIO21			21
#define GS_OOLITE_GPIO22			22
#define GS_OOLITE_GPIO26			26

#define GS_OOLITE_GPIO23			23
#define GS_OOLITE_GPIO24			24


#define GS_OOLITE_GPIO_BTN6		6
#define GS_OOLITE_GPIO_BTN7		7
*/

#define GS_OOLITE_GPIO_BTN_RESET		11  

#define GS_OOLITE_GPIO_LED_SYSTEM		27

#define GS_OOLITE_KEYS_POLL_INTERVAL	20	/* msecs */
#define GS_OOLITE_KEYS_DEBOUNCE_INTERVAL (3 * GS_OOLITE_KEYS_POLL_INTERVAL)

static const char *GS_OOLITE_part_probes[] = {
	"tp-link",
	NULL,
};

static struct flash_platform_data GS_OOLITE_flash_data = {
	.part_probes	= GS_OOLITE_part_probes,
};

static struct gpio_led GS_OOLITE_leds_gpio[] __initdata = {
	{

		.name		= "oolite:green:system",
		.gpio		= GS_OOLITE_GPIO_LED_SYSTEM,
		.active_low	= 1,
	/*},{
		.name		= "gpio0",
		.gpio		= GS_OOLITE_GPIO0,
		.active_low	= 1,
	},{
		.name		= "gpio1",
		.gpio		= GS_OOLITE_GPIO1,
		.active_low	= 1,
	},{
		.name		= "gpio6",
		.gpio		= GS_OOLITE_GPIO6,
		.active_low	= 1,
	},{	
		.name		= "gpio7",
		.gpio		= GS_OOLITE_GPIO7,
		.active_low	= 1,
	},{		
		.name		= "gpio8",
		.gpio		= GS_OOLITE_GPIO8,
		.active_low	= 1,
	},{	
		.name		= "gpio12",
		.gpio		= GS_OOLITE_GPIO12,
		.active_low	= 1,
	},{	
		.name		= "gpio13",
		.gpio		= GS_OOLITE_GPIO13,
		.active_low	= 1,
	
	},{	
		.name		= "gpio14",
		.gpio		= GS_OOLITE_GPIO14,
		.active_low	= 1,
	},{	
		.name		= "gpio15",
		.gpio		= GS_OOLITE_GPIO15,
		.active_low	= 1,
	},{	
		.name		= "gpio16",
		.gpio		= GS_OOLITE_GPIO16,
		.active_low	= 1,
	},{	
		.name		= "gpio17",
		.gpio		= GS_OOLITE_GPIO17,
		.active_low	= 1,
*/
	},{

		.name		= "gpio18",
		.gpio		= GS_OOLITE_GPIO18,
		.active_low	= 1,
/*
	},{
		.name		= "gpio19",
		.gpio		= GS_OOLITE_GPIO19,
		.active_low	= 1,
	},{
		.name		= "gpio20",
		.gpio		= GS_OOLITE_GPIO20,
		.active_low	= 1,
	},{
		.name		= "gpio21",
		.gpio		= GS_OOLITE_GPIO21,
		.active_low	= 1,
	},{
		.name		= "gpio22",
		.gpio		= GS_OOLITE_GPIO22,
		.active_low	= 1,
	},{
		.name		= "gpio26",
		.gpio		= GS_OOLITE_GPIO26,
		.active_low	= 1,
	},{
		.name		= "gpio23",
		.gpio		= GS_OOLITE_GPIO23,
		.active_low	= 1,
	},{
		.name		= "gpio24",
		.gpio		= GS_OOLITE_GPIO24,
		.active_low	= 1,
*/	},

};

static struct gpio_keys_button GS_OOLITE_gpio_keys[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = GS_OOLITE_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= GS_OOLITE_GPIO_BTN_RESET,
		.active_low	= 1,
	},
	/*
	{
		.desc		= "BTN_6",
		.type		= EV_KEY,
		.code		= BTN_6,
		.debounce_interval = GS_OOLITE_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= GS_OOLITE_GPIO_BTN6,
		.active_low	= 1,
	},
	{
		.desc		= "BTN_7",
		.type		= EV_KEY,
		.code		= BTN_7,
		.debounce_interval = GS_OOLITE_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= GS_OOLITE_GPIO_BTN7,
		.active_low	= 1,
	},
	*/
};

static void __init GS_OOLITE_setup(void)
{
	u8 *mac = (u8 *) KSEG1ADDR(0x1f01fc00);
	//u8 *ee = ath79_get_eeprom();
	u8 *ee = (u8 *) KSEG1ADDR(0x1fff1000);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(GS_OOLITE_leds_gpio),
				 GS_OOLITE_leds_gpio);

	ath79_register_gpio_keys_polled(-1, GS_OOLITE_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(GS_OOLITE_gpio_keys),
					GS_OOLITE_gpio_keys);

	
	ath79_gpio_function_disable( AR933X_GPIO_FUNC_ETH_SWITCH_LED1_EN |
				     AR933X_GPIO_FUNC_ETH_SWITCH_LED2_EN |
				     AR933X_GPIO_FUNC_ETH_SWITCH_LED3_EN);
	

	ath79_register_usb();

	ath79_register_m25p80(&GS_OOLITE_flash_data);
	ath79_init_mac(ath79_eth0_data.mac_addr, mac, 1);
	ath79_init_mac(ath79_eth1_data.mac_addr, mac, -1);

	ath79_register_mdio(0, 0x0);
	ath79_register_eth(1);
	ath79_register_eth(0);

	ath79_register_wmac(ee, mac);
}

MIPS_MACHINE(ATH79_MACH_GS_OOLITE, "GS-OOLITE",
	     "Oolite V1.0", GS_OOLITE_setup);
