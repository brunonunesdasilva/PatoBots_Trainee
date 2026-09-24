#ifndef BTSTACK_CONFIG_H
#define BTSTACK_CONFIG_H

// BT roles
#define ENABLE_BLE
#define ENABLE_CLASSIC
#define ENABLE_LE_PERIPHERAL
#define ENABLE_LE_CENTRAL
#define ENABLE_PRINTF_HEXDUMP

#define HCI_HOST_ACL_PACKET_NUM     20
#define HCI_HOST_ACL_PACKET_LEN     1024
#define HCI_HOST_SCO_PACKET_NUM     10
#define HCI_HOST_SCO_PACKET_LEN     60

// Buffer sizes
#define HCI_ACL_PAYLOAD_SIZE                 (1691 + 4)
#define HCI_INCOMING_PRE_BUFFER_SIZE         6
#define MAX_NR_BNEP_CHANNELS                 1
#define MAX_NR_BNEP_SERVICES                 1
#define MAX_NR_BTSTACK_LINK_KEY_DB_MEMORY_ENTRIES  2
#define MAX_NR_GATT_CLIENTS                  1
#define MAX_NR_HCI_CONNECTIONS               2
#define MAX_NR_HID_HOST_CONNECTIONS          1
#define MAX_NR_L2CAP_CHANNELS                6
#define MAX_NR_L2CAP_SERVICES                3
#define MAX_NR_RFCOMM_CHANNELS               1
#define MAX_NR_RFCOMM_MULTIPLEXERS           1
#define MAX_NR_RFCOMM_SERVICES               1
#define MAX_NR_SM_LOOKUP_ENTRIES             3
#define MAX_NR_WHITELIST_ENTRIES             16
#define MAX_NR_LE_DEVICE_DB_ENTRIES          16
#define NVM_NUM_DEVICE_DB_ENTRIES            16
#define NVM_NUM_LINK_KEYS   16


// ATT DB
#define HAVE_MALLOC
#define MAX_ATT_DB_SIZE                      512

// GATT
#define ENABLE_GATT_CLIENT_PAIRING
#define MAX_NR_GATT_SUBPROCEDURES            3

// Misc
#define ENABLE_LOG_INFO
#define ENABLE_LOG_ERROR
#define HAVE_ASSERT
#define HAVE_FREERTOS_INCLUDE_PREFIX
#define HAVE_FREERTOS_TASK_NOTIFICATIONS

#endif // BTSTACK_CONFIG_H