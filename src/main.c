/**
 * @file main.c
 * @author Argon
 * @brief  BLE Apple 
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <zephyr/types.h>
#include <stddef.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>


//数据段
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_MANUFACTURER_DATA,
				  0x4c, 0x00, 0x07, 0x19, 0x07, 0x0a, 0x20,
				  0x75, 0xaa, 0x30, 0x01, 0x00, 0x00, 0x45,
				  0x12, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00,
				  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				  0x00)};

//扫描响应数据段
static const struct bt_data sd[] = {
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
	BT_DATA_BYTES(BT_DATA_PERIPHERAL_INT_RANGE, 0x20, 0x00, 0x40, 0x00),
	BT_DATA_BYTES(BT_DATA_TX_POWER, 0x09),
};

//设备名
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)


int main(void)
{
	int err;

	printk("BLE Starting\n");

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(NULL);
	if (err)
	{
		printk("Ble init failed (err %d)\n", err);
		return 0;
	}

	printk("Ble initialized\n");

	do
	{
		//开启广播
		printk("Ble adv start\r\n");
		err = bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad),
							  sd, ARRAY_SIZE(sd));
		if (err)
		{
			printk("Advertising failed to start (err %d)\n", err);
			return 0;
		}
		k_sleep(K_SECONDS(4));

		//广播广播
		printk("Ble adv stop\r\n");
		err = bt_le_adv_stop();
		if (err)
		{
			printk("Advertising failed to stop (err %d)\n", err);
			return 0;
		}

		//更新MAC
		bt_addr_le_t local_mac_addr;
		static bt_addr_le_t myaddr = {
			.type = BT_ADDR_LE_RANDOM,
			/* fixed MAC addres */
			.a = {
				.val = {0x01, 0x02, 0x03, 0x04, 0x05, 0xc6},
			},
		};

		printk("Ble update MAC address\r\n");
		err = bt_id_create(&myaddr, NULL);
		if (err < 0)
		{
			printk("bt_id_create err!\r\n");
		}
		k_sleep(K_SECONDS(4));
	} while (1);
	return 0;
}
