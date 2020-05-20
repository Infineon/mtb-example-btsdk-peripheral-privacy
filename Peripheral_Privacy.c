/******************************************************************************
* File Name:   Peripheral_Privacy.c
*
* Version:     1.0.0
*
* Description: This is the source code for the Peripheral_Privacy Example
*              for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* (c) 2020, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

#include "GeneratedSource/cycfg_gatt_db.h"
#include "GeneratedSource/cycfg_pins.h"
#include "app_bt_cfg.h"
#include "sparcommon.h"
#include "wiced_bt_dev.h"
#include "wiced_platform.h"
#include "wiced_bt_trace.h"
#include "wiced_hal_puart.h"
#include "wiced_bt_stack.h"
#include "wiced_timer.h"
#include "wiced_hal_nvram.h"
#include "brcm_fw_types.h"
#include "wiced_rtos.h"

/*******************************************************************
 * Constant Definitions
 ******************************************************************/
/* Enable debug print statements*/
#define ENABLE_DEBUG
/* Max number of bonded devices */
#define BOND_MAX                       (4)

/* LED Thread Priority */
#define LED_THREAD_PRIORITY            (2)

/* Delays for controlling LED Blink rate */
#define DEVICE_CONNECTED_DELAY         (1000)
#define DIRECTED_ADV_DELAY             (100)
#define UNDIRECTED_ADV_DELAY           (500)

#define LED_ON                          (0U)
#define LED_OFF                         (1U)

/* NVSRAM locations available for application data - NVSRAM Volatile Section Identifier */
/* WICED_NVRAM_LOCAL_KEYS holds the local identity keys */
/* WICED_NVRAM_BOND_INFO stores the number of bonded devices and the next one to be over-written when space is full */
/* WICED_NVRAM_HOST_INFO0 stores the BD_ADDR and CCCD values for the first host - the others follow that one */
/* WICED_NVRAM_PAIRED_KEYS0 stores the encryption keys for the first host - the others follow that one in order */
#define WICED_NVRAM_BOND_INFO          ( WICED_NVRAM_VSID_START+ 1 )
#define WICED_NVRAM_HOST_INFO0         ( WICED_NVRAM_BOND_INFO + 1 )
#define WICED_NVRAM_LOCAL_KEYS         ( WICED_NVRAM_HOST_INFO0 + BOND_MAX )
#define WICED_NVRAM_PAIRED_KEYS0       ( WICED_NVRAM_LOCAL_KEYS + 1 )

/*******************************************************************
 * Variable Definitions
 ******************************************************************/
uint16_t connection_id = 0;
wiced_timer_t led_timer;
wiced_bt_ble_advert_mode_t *p_adv_mode = NULL;
wiced_thread_t *led_thread;
uint8_t bondIndex = 0;                 /* This is the index for the VSID for the host we are currently bonded to */
uint8_t bondInfo[] = {0,0};            /* This holds bonding info (number bonded and next free slot) */
wiced_bool_t bond_mode = WICED_TRUE;   /* If true we will go into bonding mode. This will be set false if pre-existing bonding info is available */

/* enum for bondinfo structure */
enum
{
    NUM_BONDED,
    NEXT_FREE
};

/* enum for state machine*/
enum StateMachine
{
    IDLE_NO_DATA,
    IDLE_DATA,
    IDLE_PRIVACY_CHANGE,
    CONNECTED,
    BONDED
} state;

/* Host information saved in  NVRAM */
struct hostinfo_struct
{
    BD_ADDR   bdaddr;                          /* BD address of the bonded host so we know if we reconnected to the same device */
    uint16_t  cccd;                            /* Remember the value of the CCCD (whether notifications were on or off last time we were connected) */
    wiced_bt_ble_privacy_mode_t privacy_mode;  /* Remember the privacy setting of the host */
} __attribute__((packed));
struct hostinfo_struct hostinfo;
struct hostinfo_struct hostinfoTemp;


/*******************************************************************
 * Function Prototypes
 ******************************************************************/
static wiced_bt_dev_status_t    app_bt_management_callback              ( wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data );
static wiced_bt_gatt_status_t   app_connect_callback                    ( wiced_bt_gatt_connection_status_t *p_conn_status );
static wiced_bt_gatt_status_t   app_gatt_callback                       ( wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_data );
static void                     button_cback                            ( void *data, uint8_t port_pin );
static void                     key_button_app_init                     (void);
static wiced_bt_gatt_status_t   app_server_callback                     ( uint16_t conn_id, wiced_bt_gatt_request_type_t type, wiced_bt_gatt_request_data_t *p_data );
static wiced_bt_gatt_status_t   app_read_handler                        ( wiced_bt_gatt_read_t *p_read_req, uint16_t conn_id );
static wiced_bt_gatt_status_t   app_write_handler                       ( wiced_bt_gatt_write_t *p_write_req, uint16_t conn_id );
static wiced_bt_gatt_status_t   app_get_value                           ( uint16_t attr_handle, uint16_t conn_id, uint8_t *p_val, uint16_t max_len, uint16_t *p_len );
static wiced_bt_gatt_status_t   app_set_value                           ( uint16_t attr_handle, uint16_t conn_id, uint8_t *p_val, uint16_t len );
static void                     app_set_advertisement_data              (void);
static void                     app_init                                (void);
static void                     app_led_control                         (uint32_t arg);
static void                     rx_cback                                ( void *data );
static void                     display_menu                            (void);
static void                     directed_adv_handler                    (uint8_t device_index);
static void                     privacy_mode_handler                    (uint8_t device_index);

/*******************************************************************************
 * Function Name: application_start()
 ********************************************************************************
 * Summary: Entry point to the application. Initialize transport configuration
 *          and register BLE management event callback. The actual application
 *          initialization will happen when stack reports that BT device is ready
 *
 * Parameters:
 *   None
 *
 * Return:
 *  None
 *
 ********************************************************************************/
void application_start(void)
{
    /*WICED_BT_TRACE_ENABLE*/
#if defined WICED_BT_TRACE_ENABLE || defined TRACE_TO_WICED_HCI
    wiced_set_debug_uart(WICED_ROUTE_DEBUG_TO_PUART);
#endif

    WICED_BT_TRACE("***************** Peripheral Privacy App Start*************************\r\n");
    display_menu();

#if defined ENABLE_DEBUG
    WICED_BT_TRACE("Size of wiced_bt_device_link_keys_t = %d \r\n",sizeof(wiced_bt_device_link_keys_t));
    WICED_BT_TRACE("Size of hostinfo struct = %d \r\n",sizeof(hostinfo));
    WICED_BT_TRACE("Size of bond info struct = %d \r\n",sizeof(bondInfo));
    WICED_BT_TRACE("Size of local IRK = %d \r\n",sizeof( wiced_bt_local_identity_keys_t ));
#endif

    /* Initialize Stack and Register Management Callback */
    wiced_bt_stack_init(app_bt_management_callback, &wiced_bt_cfg_settings, wiced_bt_cfg_buf_pools);
}


/**************************************************************************************************
 * Function Name: app_init()
 ***************************************************************************************************
 * Summary:
 *   Initialize the Application
 *
 * Parameters:
 *    None.
 *
 * Return:
 *    None.
 ***********************************************************************************************/
void app_init(void)
{

    uint8_t count = 0;

    /* These are needed for reading stored keys from NVRAM */
    wiced_result_t              result;
    wiced_bt_device_link_keys_t link_keys;
    uint8_t                     *p = NULL;
    uint8_t                     bytes = 0;

    /* Set current state to IDLE with no data*/
    state= IDLE_NO_DATA;

    /* Allow peer to pair */
    wiced_bt_set_pairable_mode(WICED_TRUE, 0);

    /* Set Advertisement Data */
    app_set_advertisement_data();

    /* Register with stack to receive GATT callback */
    wiced_bt_gatt_register( app_gatt_callback );

    /* Initialize GATT Database */
    wiced_bt_gatt_db_init( gatt_database, gatt_database_len );

    /* Initialize thread that will blink LED during advertising */
    led_thread = wiced_rtos_create_thread();
    wiced_rtos_init_thread(led_thread,
                           LED_THREAD_PRIORITY,
                           "led",
                           app_led_control,
                           500,
                           NULL);

    /* Configure the Button GPIO */
    key_button_app_init();

    /* Setup the UART for input so that we can erase bonding information */
    /* UART transmit is already setup by the WICED_DEBUG_TRACE configuration */
    /* Enable receive and the interrupt */
    wiced_hal_puart_register_interrupt( rx_cback );
    /* Set watermark level to 1 to receive interrupt up on receiving each byte */
    wiced_hal_puart_set_watermark_level( 1 );
    wiced_hal_puart_enable_rx();

    /* Read bonded device information from NVRAM if present */
    wiced_hal_read_nvram(WICED_NVRAM_BOND_INFO, sizeof(bondInfo), bondInfo, &result);
    if(WICED_BT_SUCCESS == result)
    {
        if(bondInfo[NUM_BONDED] > 0)
        {
            /* Load the address resolution DB with each of the keys stored in the NVRAM */
            for(count = 0; count < bondInfo[NUM_BONDED]; count++)
            {
                memset( &link_keys, 0, sizeof(wiced_bt_device_link_keys_t));
                p = (uint8_t*)&link_keys;
                bytes = wiced_hal_read_nvram( WICED_NVRAM_PAIRED_KEYS0 + count, sizeof(wiced_bt_device_link_keys_t), p, &result);
                WICED_BT_TRACE("link key read from NVRAM. Key Size %d:\r\n", bytes);
                result = wiced_bt_dev_add_device_to_address_resolution_db ( &link_keys );
                result = wiced_bt_ble_update_advertising_white_list(WICED_TRUE, link_keys.bd_addr);
                WICED_BT_TRACE("%d",result);
                WICED_BT_TRACE("Host %d: [%B] added to the resolution database \r\n",count+1, link_keys.bd_addr);
                wiced_hal_read_nvram(WICED_NVRAM_HOST_INFO0 + count, sizeof(hostinfo), (uint8_t *)&hostinfoTemp, &result);
                if(WICED_BT_SUCCESS != result)
                {
                    WICED_BT_TRACE(" Reading from NVRAM Failed \r\n");
                }
                /* Load Privacy mode setting for the bonded device*/
                wiced_bt_ble_set_privacy_mode(hostinfoTemp.bdaddr, hostinfoTemp.privacy_mode);
            }
            /* We have bonding information already, so don't go into bonding mode */
            bond_mode = WICED_FALSE;

            /* Change state to IDLE with bond data present*/
            state = IDLE_DATA;

            WICED_BT_TRACE("Number of bonded devices: %d, Next free slot: %d, Number of slots free %d \r\n",bondInfo[NUM_BONDED], bondInfo[NEXT_FREE] + 1, (BOND_MAX - bondInfo[NUM_BONDED]));
            if(1 == bondInfo[NUM_BONDED]){
                WICED_BT_TRACE("\r\nOnly 1 Device [%B] Found,Starting directed Advertisement. \r\n",link_keys.bd_addr);
                WICED_BT_TRACE("Enter 'e' for starting undirected Advertisement to add new device\r\n");
                wiced_bt_start_advertisements(BTM_BLE_ADVERT_DIRECTED_HIGH, link_keys.key_data.ble_addr_type, link_keys.bd_addr);
            }
            else{
                WICED_BT_TRACE("\r\nSelect the bonded Devices Found in below list to Start Directed Advertisement \r\n");
                for(int i=0;i< bondInfo[NUM_BONDED];i++)
                {
                    /* Read device data from the bonded device list */
                    wiced_hal_read_nvram( WICED_NVRAM_HOST_INFO0 + i, sizeof(hostinfoTemp), (uint8_t*)&hostinfoTemp, &result );
                    if(WICED_BT_SUCCESS != result)
                    {
                        WICED_BT_TRACE(" Reading from NVRAM Failed \r\n");
                    }
                    else
                    {
                        WICED_BT_TRACE( "Host %d : [%B] \r\n", i+1, hostinfoTemp.bdaddr);
                    }
                }
                WICED_BT_TRACE("Enter slot number to start directed advertisement for that device \r\n");
                WICED_BT_TRACE("Enter e for Starting undirected Advertisement to add new device \r\n");
                WICED_BT_TRACE("************************** NOTE ***************************************************\r\n");
                WICED_BT_TRACE("*ONCE THE SLOTS ARE FULL THE OLDEST DEVICE DATA WILL BE OVERWRITTEN FOR NEW DEVICE*\r\n");
                WICED_BT_TRACE("***********************************************************************************\r\n");

            }
        }
    }
    else{
        WICED_BT_TRACE("\r\nNo bonded Device Found,Starting Undirected Advertisement \r\n\r\n");
        /* Start Undirected LE Advertisements on device startup. */
        wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL);
    }
}
/**************************************************************************************************
 * Function Name: app_bt_management_callback()
 **************************************************************************************************
 * Summary:
 *   This is a Bluetooth stack management event handler function to receive events from
 *   BLE stack and process as per the application.
 *
 * Parameters:
 *   wiced_bt_management_evt_t event             : BLE event code of one byte length
 *   wiced_bt_management_evt_data_t *p_event_data: Pointer to BLE management event structures
 *
 * Return:
 *  wiced_result_t: Error code from WICED_RESULT_LIST or BT_RESULT_LIST
 *
 ***********************************************************************************************/

/* Bluetooth Management Event Handler */
wiced_result_t app_bt_management_callback( wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data )
{
    wiced_bt_dev_status_t status = WICED_BT_SUCCESS;
    wiced_bt_dev_status_t result = WICED_BT_SUCCESS;
    wiced_bt_device_address_t bda = { 0 };
    wiced_bt_dev_ble_pairing_info_t *p_ble_info = NULL;
    wiced_bt_device_link_keys_t link_keys;
    uint8_t count = 0;
    uint8_t bytes = 0;

#ifdef ENABLE_DEBUG
    WICED_BT_TRACE("*************** BLE Stack Management Callback Event: %d ***************\r\n", event );
#endif

    switch (event)
    {
    case BTM_ENABLED_EVT:
        /* Bluetooth Controller and Host Stack Enabled */
        WICED_BT_TRACE("Bluetooth Enabled (%s) \r\n",
                ((WICED_BT_SUCCESS == p_event_data->enabled.status) ? "success" : "failure"));

        WICED_BT_TRACE("\r\nDiscover this device with the name: %s \r\n",
                app_gap_device_name);
        wiced_bt_dev_read_local_addr(bda);

        WICED_BT_TRACE("\r\nBluetooth Device Address: [%B] \r\n",bda);

        WICED_BT_TRACE("\r\n");
        WICED_BT_TRACE("Bluetooth Management Event: \t");
        WICED_BT_TRACE("0x%x (%d)",event);
        WICED_BT_TRACE("\r\n");

        app_init();

        break;

    case BTM_DISABLED_EVT:
        /* Bluetooth Controller and Host Stack Disabled */
        WICED_BT_TRACE("Bluetooth Disabled \r\n");
        break;

    case BTM_PASSKEY_NOTIFICATION_EVT: /* Print passkey to the screen so that the user can enter it. */
        WICED_BT_TRACE( "**********************************************************************\r\n");
        WICED_BT_TRACE( "Passkey Notification\r\n");
        WICED_BT_TRACE("PassKey for BDA [%B], Key: %06d \r\n", p_event_data->user_passkey_notification.bd_addr, p_event_data->user_passkey_notification.passkey );
        WICED_BT_TRACE( "**********************************************************************\r\n");
        break;

    case BTM_SECURITY_REQUEST_EVT:
        /* Security Request */
        /* Only grant if we are in bonding mode */
        if(bond_mode == WICED_TRUE)
        {
            WICED_BT_TRACE("Security Request Granted \r\n");
            wiced_bt_ble_security_grant(p_event_data->security_request.bd_addr, WICED_BT_SUCCESS);
        }
        else
        {
            WICED_BT_TRACE("Security Request Denied - not in bonding mode \r\n");
        }
        break;

    case BTM_PAIRING_IO_CAPABILITIES_BLE_REQUEST_EVT:
        /* Request for Pairing IO Capabilities (BLE) */
        WICED_BT_TRACE("BLE Pairing IO Capabilities Request\r\n");
        /* No IO Capabilities on this Platform */
        p_event_data->pairing_io_capabilities_ble_request.local_io_cap = BTM_IO_CAPABILITIES_BLE_DISPLAY_AND_KEYBOARD_INPUT;
        p_event_data->pairing_io_capabilities_ble_request.oob_data = BTM_OOB_NONE;
        p_event_data->pairing_io_capabilities_ble_request.auth_req = BTM_LE_AUTH_REQ_SC_MITM_BOND;
        p_event_data->pairing_io_capabilities_ble_request.max_key_size = 0x10;
        p_event_data->pairing_io_capabilities_ble_request.init_keys = BTM_LE_KEY_PENC|BTM_LE_KEY_PID;
        p_event_data->pairing_io_capabilities_ble_request.resp_keys = BTM_LE_KEY_PENC|BTM_LE_KEY_PID;
        break;

    case BTM_PAIRING_COMPLETE_EVT:

        /* Pairing is Complete */
        p_ble_info = &p_event_data->pairing_complete.pairing_complete_info.ble;
        WICED_BT_TRACE("Pairing Complete %d.\r\n", p_ble_info->reason);

        /* Now that pairing is complete, we will save the BDADDR of the host to NVRAM */
        /* Note that the .bdaddr was captured in the GATT connect callback function */
        if ( WICED_BT_SUCCESS == p_ble_info->reason ) /* Bonding successful */
        {
            /* Write to NVRAM */
            wiced_hal_write_nvram( WICED_NVRAM_HOST_INFO0 + bondInfo[NEXT_FREE], sizeof(hostinfo), (uint8_t*)&hostinfo, &status );
            WICED_BT_TRACE("Bonding info save to NVRAM: [%B]\r\n", &hostinfo.bdaddr);
            WICED_BT_TRACE("Device added in Device privacy mode at slot %d \r\n", bondInfo[NEXT_FREE]+1);
            WICED_BT_TRACE("To add Device in Network privacy mode enter 'p' \r\n");
            /* Remember which slot in the NVRAM the host we just connected to is in */
            bondIndex = bondInfo[NEXT_FREE];
            /*Exit bonding mode*/
            bond_mode = WICED_FALSE;
            /*Move to bonded state*/
            state = BONDED;

            /* Increment number of bonded devices and next free slot and save them in NVRAM */
            bondInfo[NUM_BONDED]++;
            bondInfo[NEXT_FREE] = (bondInfo[NEXT_FREE] + 1) % BOND_MAX;
            wiced_hal_write_nvram( WICED_NVRAM_BOND_INFO, sizeof(bondInfo), bondInfo, &result);
            WICED_BT_TRACE("Number of bonded devices: %d, Next free slot: %d, Number of slots free: %d\r\n",bondInfo[NUM_BONDED], bondInfo[NEXT_FREE]+1, (BOND_MAX - bondInfo[NUM_BONDED]));
        }
        else
        {
            WICED_BT_TRACE("Bonding failed! \r\n");
        }
        break;

    case BTM_ENCRYPTION_STATUS_EVT:
        /* Encryption Status Change */
        WICED_BT_TRACE("Encryption Status event: bd [%B] res %d \r\n", p_event_data->encryption_status.bd_addr, p_event_data->encryption_status.result);

        /* Search for the bd_addr that we just connected to in the NVRAM hostinfo. If it is found then it means we were previously */
        /* connected to this device so we need to restore the values into the hostinfo structure */
        for(count = 0; count < bondInfo[NUM_BONDED]; count++)
        {
            wiced_hal_read_nvram( WICED_NVRAM_HOST_INFO0 + count, sizeof(hostinfoTemp), (uint8_t*)&hostinfoTemp, &result );
            if( memcmp(hostinfo.bdaddr,hostinfoTemp.bdaddr,sizeof(hostinfo.bdaddr) ) == 0) /* Matching address */
            {
                hostinfo.cccd = hostinfoTemp.cccd;  /* Copy in the cccd value from the temporary holding location */
                app_wicedbutton_mb1_client_char_config[0] = hostinfo.cccd; /* Set CCCD value from the value that was previously saved in the NVRAM */
                bondIndex = count; /* Remember which NVRAM slot belongs to the currently connected device */
                WICED_BT_TRACE("Restored existing bonded device info from NVRAM [%B] result: %d \r\n", hostinfo.bdaddr);
                /*Move to bonded state*/
                state = BONDED;
                break; /* Exit out of loop since we found what we need */
            }
        }
        break;

    case BTM_PAIRED_DEVICE_LINK_KEYS_UPDATE_EVT:
        /* save keys to NVRAM if the link is encrypted */
        WICED_BT_TRACE( "Paired Device Key Update\r\n");
        wiced_hal_write_nvram ( WICED_NVRAM_PAIRED_KEYS0 + bondInfo[NEXT_FREE], sizeof( wiced_bt_device_link_keys_t ), (uint8_t*)&(p_event_data->paired_device_link_keys_update), &status );
        WICED_BT_TRACE("Keys save to NVRAM [%B] result: %d\r\n", (uint8_t*)&(p_event_data->paired_device_link_keys_update), status);
        break;

    case BTM_PAIRED_DEVICE_LINK_KEYS_REQUEST_EVT:
        /* Paired Device Link Keys Request */
        WICED_BT_TRACE("Paired Device Link keys Request Event for device [%B]\r\n",&(p_event_data->paired_device_link_keys_request));
        /* Need to search to see if the BD_ADDR we are looking for is in NVRAM. If not, we return WICED_BT_ERROR and the stack */
        /* will generate keys and will then call BTM_PAIRED_DEVICE_LINK_KEYS_UPDATE_EVT so that they can be stored */
        result = WICED_BT_ERROR;  /* Assume the device won't be found. If it is, we will set this back to WICED_BT_SUCCESS */
        for(count = 0; count < bondInfo[NUM_BONDED]; count++)
        {
            wiced_hal_read_nvram( WICED_NVRAM_PAIRED_KEYS0 + count, sizeof(link_keys), (uint8_t*)&link_keys, &status );
            WICED_BT_TRACE("Keys read from NVRAM [%B] result: %d \r\n", &link_keys, status);
            if( memcmp(&(link_keys.bd_addr),&(p_event_data->paired_device_link_keys_request.bd_addr),sizeof(wiced_bt_device_address_t) ) == 0 )
            {
                WICED_BT_TRACE("Matching Device Key Found \r\n");
                /* Copy the key to where the stack wants it */
                memcpy(&(p_event_data->paired_device_link_keys_request),&(link_keys), sizeof(link_keys));
                result = WICED_BT_SUCCESS;
                break; /* Exit the loop since we found what we want */
            }
        }
        status = result; /* The return status will be SUCCESS if the value was found and ERROR if the value wasn't found */
        break;

    case BTM_LOCAL_IDENTITY_KEYS_UPDATE_EVT: /* Update of local privacy keys - save to NVSRAM */
        WICED_BT_TRACE( "Local Identity Key Update\r\n");
        bytes = wiced_hal_write_nvram ( WICED_NVRAM_LOCAL_KEYS, sizeof( wiced_bt_local_identity_keys_t ), (uint8_t*)&(p_event_data->local_identity_keys_update), &status );
        /* Result is the number of bytes written */
        WICED_BT_TRACE("local keys save to NVRAM:\r\n");
        for(count = 0; count < bytes;count++)
        {
            WICED_BT_TRACE("%02X ", p_event_data->local_identity_keys_update.local_key_data[count]);
            if(count % 16 == 0)
            {
                WICED_BT_TRACE("\r\n");
            }
        }
        WICED_BT_TRACE("result: %d\r\n", status);
        break;

    case BTM_LOCAL_IDENTITY_KEYS_REQUEST_EVT: /* Request for local privacy keys - read from NVSRAM */
        WICED_BT_TRACE( "Local Identity Key Request\r\n");
        /* If the status from read_nvram is not SUCCESS, the stack will generate keys and will then call BTM_LOCAL_IDENTITY_KEYS_UPDATE_EVT so that they can be stored */
        bytes = wiced_hal_read_nvram( WICED_NVRAM_LOCAL_KEYS, sizeof(wiced_bt_local_identity_keys_t), (uint8_t *)&(p_event_data->local_identity_keys_request), &status );
        /* Result is the number of bytes read */
        WICED_BT_TRACE("local keys read from NVRAM. Key Size %d:\r\n", bytes);
        for(count = 0; count <bytes;count++)
        {
            WICED_BT_TRACE("%02X ", p_event_data->local_identity_keys_request.local_key_data[count]);
            if(count % 16 == 0)
            {
                WICED_BT_TRACE("\r\n");
            }
        }
        WICED_BT_TRACE("result: %d \r\n", status);
        break;

    case BTM_BLE_ADVERT_STATE_CHANGED_EVT:
        /* Advertisement State Changed */
        p_adv_mode = &p_event_data->ble_advert_state_changed;
        WICED_BT_TRACE("Advertisement State Change: %d\r\n", *p_adv_mode);
        break;

    case BTM_USER_CONFIRMATION_REQUEST_EVT:
        /* Pairing request, handle confirmation of numeric compare here if desired */
        WICED_BT_TRACE("\r\n");
        WICED_BT_TRACE("************************************************************************\r\n");
        WICED_BT_TRACE("******************Pairing Key:[ %d ]******************************\r\n", p_event_data->user_confirmation_request.numeric_value);
        WICED_BT_TRACE("************************************************************************\r\n");
        WICED_BT_TRACE("\r\n");
        wiced_bt_dev_confirm_req_reply( WICED_BT_SUCCESS , p_event_data->user_confirmation_request.bd_addr);
        break;

    case BTM_BLE_CONNECTION_PARAM_UPDATE:
        WICED_BT_TRACE("Connection parameter update status:%d, Connection Interval: %d, Connection Latency: %d, Connection Timeout: %d\r\n", p_event_data->ble_connection_param_update.status, p_event_data->ble_connection_param_update.conn_interval, p_event_data->ble_connection_param_update.conn_latency, p_event_data->ble_connection_param_update.supervision_timeout);
        break;
    default:
        WICED_BT_TRACE("Unhandled Bluetooth Management Event: 0x%x (%d) \r\n", event, event);
        break;
    }

    return status;
}

/**************************************************************************************************
 * Function Name: app_led_control()
 ***************************************************************************************************
 * Summary:
 *Function to toggle led state depending on the state of advertisement.
 *   1. Advertisement ON (Undirected):   slow Blinking led(T = 1 sec)
 *   2. Advertisement ON (Directed):     fast Blinking led(T = 200 msec)
 *   3. Advertisement OFF, Connected:    LED ON
 *   4. Advertisement OFF, Timed out:    LED OFF
 *
 * Parameters:
 *    None.
 *
 * Return:
 *    None.
 ***********************************************************************************************/

void app_led_control(uint32_t arg){

    uint16_t led_status =0;
    uint16_t delay =0;
    while (1)
    {
        if( BTM_BLE_ADVERT_OFF == *p_adv_mode)
        {
            if(0 != connection_id)
            {
                wiced_hal_gpio_set_pin_output(LED1,LED_ON);
            }
            else
            {
                wiced_hal_gpio_set_pin_output(LED1,LED_OFF);
            }
            delay = DEVICE_CONNECTED_DELAY;
        }
        else if((BTM_BLE_ADVERT_DIRECTED_HIGH == *p_adv_mode) || (BTM_BLE_ADVERT_DIRECTED_LOW == *p_adv_mode)){
            delay = DIRECTED_ADV_DELAY;
            led_status = wiced_hal_gpio_get_pin_output(LED1);
            wiced_hal_gpio_set_pin_output(LED1,!led_status);
        }
        else if((BTM_BLE_ADVERT_UNDIRECTED_HIGH == *p_adv_mode) || (BTM_BLE_ADVERT_UNDIRECTED_LOW == *p_adv_mode)){
            delay = UNDIRECTED_ADV_DELAY;
            led_status = wiced_hal_gpio_get_pin_output(LED1);
            wiced_hal_gpio_set_pin_output(LED1,!led_status);
        }

        wiced_rtos_delay_milliseconds(delay, ALLOW_THREAD_TO_SLEEP);
    }
}

/*******************************************************************************
 * Function Name: app_set_advertisement_data()
 ********************************************************************************
 * Summary: Sets up the advertisement data for advertisement packets.
 *
 * Parameters:
 *   None
 *
 * Return:
 *  None
 *
 ********************************************************************************/
void app_set_advertisement_data(void)
{
    wiced_bt_ble_advert_elem_t adv_elem[3] = { 0 };
    uint8_t adv_flag = BTM_BLE_GENERAL_DISCOVERABLE_FLAG | BTM_BLE_BREDR_NOT_SUPPORTED;
    uint8_t num_elem = 0;
    uint8_t wiced101_service_uuid[LEN_UUID_128] = { __UUID_SERVICE_WICEDBUTTON };

    /* Advertisement Element for Flags */
    adv_elem[num_elem].advert_type             = BTM_BLE_ADVERT_TYPE_FLAG;
    adv_elem[num_elem].len                     = sizeof(uint8_t);
    adv_elem[num_elem].p_data                 = &adv_flag;
    num_elem++;

    /* Advertisement Element for Name */
    adv_elem[num_elem].advert_type          = BTM_BLE_ADVERT_TYPE_NAME_COMPLETE;
    adv_elem[num_elem].len                  = strlen((const char *) wiced_bt_cfg_settings.device_name);
    adv_elem[num_elem].p_data               = (uint8_t *) wiced_bt_cfg_settings.device_name;
    num_elem++;

    /* Advertisement Element for Wiced101 Service */
    adv_elem[num_elem].advert_type             = BTM_BLE_ADVERT_TYPE_128SRV_COMPLETE;
    adv_elem[num_elem].len                     = LEN_UUID_128;
    adv_elem[num_elem].p_data                 = wiced101_service_uuid;
    num_elem++;

    /* Set Raw Advertisement Data */
    wiced_bt_ble_set_raw_advertisement_data(num_elem, adv_elem);
}

/*******************************************************************************
 * Function Name: app_gatt_callback()
 ********************************************************************************
 * Summary: Bluetooth Management Event callback function registered by
 *          application_start() This function acts like a Finite State
 *          Machine(FSM) for the application.
 *
 * Parameters:
 *   None
 *
 * Return:
 *  None
 *
 ********************************************************************************/
wiced_bt_gatt_status_t app_gatt_callback( wiced_bt_gatt_evt_t event,
                                          wiced_bt_gatt_event_data_t *p_data )
{
    wiced_bt_gatt_status_t result = WICED_SUCCESS;
    wiced_bt_gatt_attribute_request_t *p_attr_req = NULL;

    switch( event )
    {
    case GATT_CONNECTION_STATUS_EVT:
        result = app_connect_callback( &p_data->connection_status );
        break;

    case GATT_ATTRIBUTE_REQUEST_EVT:
        p_attr_req = &p_data->attribute_request;
        result = app_server_callback( p_attr_req->conn_id, p_attr_req->request_type, &p_attr_req->data );
        break;

    default:
        result = WICED_BT_GATT_SUCCESS;
        break;
    }

    return result;
}

/*******************************************************************************
 * Function Name: key_button_app_init()
 ********************************************************************************
 * Summary: This function configures the button for the interrupts.
 *
 * Parameters:
 *   None
 *
 * Return:
 *  None
 *
 ********************************************************************************/
void key_button_app_init(void)
{
    /* Configure the Button GPIO as an input with a resistive pull up and falling edge interrupt */
    wiced_hal_gpio_register_pin_for_interrupt( SW3, button_cback, NULL );
    wiced_hal_gpio_configure_pin( SW3,
                                ( GPIO_INPUT_ENABLE | GPIO_PULL_UP | GPIO_EN_INT_FALLING_EDGE ),
                                GPIO_PIN_OUTPUT_HIGH );

}

/*******************************************************************************
 * Function Name: button_cback()
 ********************************************************************************
 * Summary: This function handles the interrupt generated by the button press.
 *
 * Parameters:
 *   data  data related to the interrupt, populated internally.
 *   port_pin   pin number on which interrupt was received.
 *
 * Return:
 *  None
 *
 ********************************************************************************/
void button_cback( void *data, uint8_t port_pin )
{
    /* Increment the button value to register the button press */
    app_wicedbutton_mb1[0]++;
    /* If the connection is up and if the client wants notifications, send updated button press value */
    if ( connection_id != 0)
    {
        if(app_wicedbutton_mb1_client_char_config[0] & GATT_CLIENT_CONFIG_NOTIFICATION)
        {
            wiced_bt_gatt_send_notification(connection_id, HDLC_WICEDBUTTON_MB1_VALUE,
                                            app_wicedbutton_mb1_len, app_wicedbutton_mb1);
            WICED_BT_TRACE("Send Notification: sending Button value\r\n");
        }
        else{
            WICED_BT_TRACE("Notifications are Disabled\r\n");
        }
    }
    else{
        WICED_BT_TRACE("Connection is not Up \r\n");
    }
    /* Clear the GPIO interrupt */
    wiced_hal_gpio_clear_pin_interrupt_status( port_pin );
}

/*******************************************************************************
 * Function Name: app_connect_callback()
 ********************************************************************************
 * Summary: This function handles the connection and disconnection events. It also
 *          stores the currently connected device information in hostinfo structure.
 *
 * Parameters:
 *   p_conn_status  contains information related to the connection/disconnection
 *                  event.
 *
 * Return:
 * wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e
 *                         in wiced_bt_gatt.h
 *
 ********************************************************************************/
wiced_bt_gatt_status_t app_connect_callback(
                               wiced_bt_gatt_connection_status_t *p_conn_status )
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_ERROR;
    wiced_result_t                  result;
    wiced_bt_device_link_keys_t     link_keys;

    if ( NULL != p_conn_status )
    {
        if ( p_conn_status->connected )
        {
            // Device has connected
            WICED_BT_TRACE("Connected : BDA [%B], Connection ID '%d'\r\n", p_conn_status->bd_addr, p_conn_status->conn_id );

            /* Handling the connection by updating connection ID */
            connection_id = p_conn_status->conn_id;
            state = CONNECTED;

            /* Copy address of connected device to the hostinfo structure to be saved in NVRAM when pairing is complete */
            memcpy(hostinfo.bdaddr, p_conn_status->bd_addr, sizeof(BD_ADDR));
        }
        else
        {
            // Device has disconnected
            WICED_BT_TRACE("Disconnected : BDA [%B], Connection ID '%d', Reason '%d'\r\n", p_conn_status->bd_addr, p_conn_status->conn_id, p_conn_status->reason );

            /* Handling the disconnection */
            connection_id = 0;
            hostinfo.cccd = app_wicedbutton_mb1_client_char_config[0];
            wiced_hal_write_nvram(WICED_NVRAM_HOST_INFO0 + bondIndex, sizeof(hostinfo), (uint8_t *)&hostinfo, &result);
            memset( hostinfo.bdaddr, 0, sizeof(BD_ADDR));
            /* Reset the CCCD value so that on a reconnect CCCD will be off */
            app_wicedbutton_mb1_client_char_config[0] = 0;
            bond_mode= WICED_FALSE;

            wiced_hal_read_nvram( WICED_NVRAM_BOND_INFO, sizeof(bondInfo), bondInfo, &result);

            if (bondInfo[NUM_BONDED]> 0)
            {
                state = IDLE_DATA;
                WICED_BT_TRACE("Select the bonded Devices Found in below list to Start Directed Advertisement \r\n");
                for(int i=0;i< bondInfo[NUM_BONDED];i++)
                {
                    /* Read device data from the bonded device list */
                    wiced_hal_read_nvram( WICED_NVRAM_HOST_INFO0 + i, sizeof(hostinfoTemp), (uint8_t*)&hostinfoTemp, &result );
                    WICED_BT_TRACE( "Host %d: [%B] \r\n", i+1, hostinfoTemp.bdaddr);
                }
                WICED_BT_TRACE("Enter slot number to start directed advertisement for that device \r\n");
                WICED_BT_TRACE("Enter e for Starting undirected Advertisement to add new device \r\n");
            }
            else
            {
                state = IDLE_NO_DATA;
                wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0,NULL);
            }
        }
        status = WICED_BT_GATT_SUCCESS;
    }

    return status;
}

/**************************************************************************************************
* Function Name: app_server_callback()
***************************************************************************************************
* Summary:
*   This function handles GATT server events from the BT stack.
*
* Parameters:
*   uint16_t conn_id                            : Connection ID
*   wiced_bt_gatt_request_type_t type           : Type of GATT server event
*   wiced_bt_gatt_request_data_t *p_data        : Pointer to GATT server event data
*
* Return:
*  wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e in wiced_bt_gatt.h
*
**************************************************************************************************/
wiced_bt_gatt_status_t app_server_callback( uint16_t conn_id, wiced_bt_gatt_request_type_t type, wiced_bt_gatt_request_data_t *p_data )
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_ERROR;

    switch ( type )
    {
    case GATTS_REQ_TYPE_READ:
        status = app_read_handler( &p_data->read_req, conn_id );
        break;
    case GATTS_REQ_TYPE_WRITE:
        status = app_write_handler( &p_data->write_req, conn_id );
        break;
    }

    return status;
}

/**************************************************************************************************
* Function Name: app_write_handler()
***************************************************************************************************
* Summary:
*   This function handles Write Requests received from the client device
*
* Parameters:
*   wiced_bt_gatt_write_t *p_write_req          : Pointer that contains details of Write Request
*                                                 including the attribute handle
*   uint16_t conn_id                            : Connection ID
*
* Return:
*  wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e in wiced_bt_gatt.h
*
**************************************************************************************************/
wiced_bt_gatt_status_t app_write_handler( wiced_bt_gatt_write_t *p_write_req, uint16_t conn_id )
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_INVALID_HANDLE;

    /* Attempt to perform the Write Request */
    status = app_set_value(p_write_req->handle, conn_id, p_write_req->p_val, p_write_req->val_len);

    return status;
}

/**************************************************************************************************
* Function Name: app_read_handler()
***************************************************************************************************
* Summary:
*   This function handles Read Requests received from the client device
*
* Parameters:
*   wiced_bt_gatt_write_t *p_read_req           : Pointer that contains details of Read Request
*                                                 including the attribute handle
*   uint16_t conn_id                            : Connection ID
*
* Return:
*  wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e in wiced_bt_gatt.h
*
**************************************************************************************************/
wiced_bt_gatt_status_t app_read_handler( wiced_bt_gatt_read_t *p_read_req, uint16_t conn_id )
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_INVALID_HANDLE;

    /* Attempt to perform the Read Request */
    status = app_get_value(p_read_req->handle, conn_id, p_read_req->p_val, *p_read_req->p_val_len, p_read_req->p_val_len);

    return status;
}

/**************************************************************************************************
* Function Name: app_get_value()
***************************************************************************************************
* Summary:
*   This function handles reading of the attribute value from the GATT database and passing the
*   data to the BT stack. The value read from the GATT database is stored in a buffer whose
*   starting address is passed as one of the function parameters
*
* Parameters:
*   uint16_t attr_handle                    : Attribute handle for read operation
*   uint16_t conn_id                        : Connection ID
*   uint8_t *p_val                          : Pointer to the buffer to store read data
*   uint16_t max_len                        : Maximum buffer length available to store the read data
*   uint16_t *p_len                         : Actual length of data copied to the buffer
*
* Return:
*   wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e in wiced_bt_gatt.h
*
**************************************************************************************************/
wiced_bt_gatt_status_t app_get_value( uint16_t attr_handle, uint16_t conn_id, uint8_t *p_val, uint16_t max_len, uint16_t *p_len )
{
    int i = 0;
    wiced_bool_t isHandleInTable = WICED_FALSE;
    wiced_bt_gatt_status_t res = WICED_BT_GATT_INVALID_HANDLE;

    // Check for a matching handle entry
    for (i = 0; i < app_gatt_db_ext_attr_tbl_size; i++)
    {
        if (app_gatt_db_ext_attr_tbl[i].handle == attr_handle)
        {
            // Detected a matching handle in external lookup table
            isHandleInTable = WICED_TRUE;
            // Detected a matching handle in the external lookup table
            if (app_gatt_db_ext_attr_tbl[i].cur_len <= max_len)
            {
                // Value fits within the supplied buffer; copy over the value
                *p_len = app_gatt_db_ext_attr_tbl[i].cur_len;
                memcpy(p_val, app_gatt_db_ext_attr_tbl[i].p_data, app_gatt_db_ext_attr_tbl[i].cur_len);
                res = WICED_BT_GATT_SUCCESS;
            }
            else
            {
                // Value to read will not fit within the buffer
                res = WICED_BT_GATT_INVALID_ATTR_LEN;
            }
            break;
        }
    }

    if (!isHandleInTable)
    {
        // Add code to read value using handles not contained within external lookup table
        // This can apply when the option is enabled to not generate initial value arrays.
        // If the value for the current handle is successfully read then set the result using:
        // res = WICED_BT_GATT_SUCCESS;
        switch ( attr_handle )
        {
        default:
            // The read operation was not performed for the indicated handle
            WICED_BT_TRACE("Read Request to Invalid Handle: 0x%x\r\n", attr_handle);
            res = WICED_BT_GATT_READ_NOT_PERMIT;
            break;
        }
    }

    return res;
}

/**************************************************************************************************
* Function Name: app_set_value()
***************************************************************************************************
* Summary:
*   This function handles writing to the attribute handle in the GATT database using the
*   data passed from the BT stack. The value to write is stored in a buffer
*   whose starting address is passed as one of the function parameters
*
* Parameters:
*   uint16_t attr_handle                    : Attribute handle for write operation
*   uint16_t conn_id                        : Connection ID
*   uint8_t *p_val                          : Pointer to the buffer that stores the data to be written
*   uint16_t len                            : Length of data to be written
*
* Return:
*   wiced_bt_gatt_status_t: See possible status codes in wiced_bt_gatt_status_e in wiced_bt_gatt.h
*
**************************************************************************************************/
wiced_bt_gatt_status_t app_set_value( uint16_t attr_handle, uint16_t conn_id, uint8_t *p_val, uint16_t len )
{
    int i = 0;
    wiced_bool_t isHandleInTable = WICED_FALSE;
    wiced_bool_t validLen = WICED_FALSE;
    wiced_bt_gatt_status_t res = WICED_BT_GATT_INVALID_HANDLE;

    // Check for a matching handle entry
    for (i = 0; i < app_gatt_db_ext_attr_tbl_size; i++)
    {
        if (app_gatt_db_ext_attr_tbl[i].handle == attr_handle)
        {
            // Detected a matching handle in external lookup table
            isHandleInTable = WICED_TRUE;
            // Verify that size constraints have been met
            validLen = (app_gatt_db_ext_attr_tbl[i].max_len >= len);
            if (validLen)
            {
                // Value fits within the supplied buffer; copy over the value
                app_gatt_db_ext_attr_tbl[i].cur_len = len;
                memcpy(app_gatt_db_ext_attr_tbl[i].p_data, p_val, len);
                res = WICED_BT_GATT_SUCCESS;

                switch ( attr_handle )
                {
                case HDLD_WICEDBUTTON_MB1_CLIENT_CHAR_CONFIG:
                    if ( len != 2 )
                    {
                        return WICED_BT_GATT_INVALID_ATTR_LEN;
                    }
                    hostinfo.cccd = p_val[0] | ( p_val[1] << 8 );

                    /* Save value to NVRAM */
                    wiced_result_t temp_result;
                    wiced_hal_write_nvram( WICED_NVRAM_VSID_START, sizeof(hostinfo), (uint8_t*)&hostinfo, &temp_result );
                    WICED_BT_TRACE("Writing CCCD value to NVRAM\r\n");
                    break;
                default:
                    WICED_BT_TRACE("Write is not supported \r\n");
                }
            }
            else
            {
                // Value to write does not meet size constraints
                res = WICED_BT_GATT_INVALID_ATTR_LEN;
            }
            break;
        }
    }

    if (!isHandleInTable)
    {
        switch ( attr_handle )
        {
        default:
            // The write operation was not performed for the indicated handle
            WICED_BT_TRACE("Write Request to Invalid Handle: 0x%x\r\n", attr_handle);
            res = WICED_BT_GATT_WRITE_NOT_PERMIT;
            break;
        }
    }

    return res;
}

/*******************************************************************************
 * Function Name: rx_cback()
 ********************************************************************************
 * Summary: Interrupt callback function for UART. Receives the user commands and
 *          performs the related tasks.
 *
 * Parameters:
 *  data   pointer to the data received over UART.
 *
 * Return:
 *  None
 *
 ********************************************************************************/
void rx_cback( void *data )
{
    uint8_t  readbyte = 0;
    wiced_result_t                  result;
    wiced_bt_device_link_keys_t     link_keys;
    wiced_bt_local_identity_keys_t  local_keys;
    BD_ADDR                         bonded_address;
    uint8_t                         count = 0;
    wiced_bt_dev_status_t           status;
    uint8_t                         device_index = 0;

    /* Read one byte from the buffer and (unlike GPIO) reset the interrupt */
    wiced_hal_puart_read( &readbyte );
    wiced_hal_puart_reset_puart_interrupt();
    device_index = readbyte - '0';

    switch( readbyte)
    {
    case '1':
        if ((IDLE_DATA == state) && (1 <= bondInfo[NUM_BONDED]))
        {
            directed_adv_handler(device_index);
        }
        else if((IDLE_PRIVACY_CHANGE == state) && (1 <= bondInfo[NUM_BONDED]))
        {
            privacy_mode_handler(device_index);
        }
        else
            WICED_BT_TRACE("Invalid Operation\r\n");
        break;

    case '2':
        if ((IDLE_DATA == state) && (2 <= bondInfo[NUM_BONDED]))
        {
            directed_adv_handler(device_index);
        }
        else if((IDLE_PRIVACY_CHANGE == state) && (2 <= bondInfo[NUM_BONDED]))
        {
            privacy_mode_handler(device_index);
        }
        else
            WICED_BT_TRACE("Invalid Operation\r\n");
        break;

    case '3':
        if ((IDLE_DATA == state) && (3 <= bondInfo[NUM_BONDED]))
        {
            directed_adv_handler(device_index);
        }
        else if((IDLE_PRIVACY_CHANGE == state) && (3 <= bondInfo[NUM_BONDED]))
        {
            privacy_mode_handler(device_index);
        }
        else
            WICED_BT_TRACE("Invalid Operation\r\n");
        break;

    case '4':
        if ((IDLE_DATA == state) && (4 == bondInfo[NUM_BONDED]))
        {
            directed_adv_handler(device_index);
        }
        else if((IDLE_PRIVACY_CHANGE == state) && (4 == bondInfo[NUM_BONDED]))
        {
            privacy_mode_handler(device_index);
        }
        else
            WICED_BT_TRACE("Invalid Operation\r\n");
        break;

    case 'r':
        if (IDLE_DATA == state)
        {
            for(int i=0;i< bondInfo[NUM_BONDED];i++)
            {
                /* Read device data from the bonded device list */
                wiced_hal_read_nvram( WICED_NVRAM_HOST_INFO0 + i, sizeof(bonded_address), (uint8_t*)&bonded_address, &result );
                wiced_bt_dev_delete_bonded_device(bonded_address);
                WICED_BT_TRACE( "Removed host [%B] from bonded device list \r\n", bonded_address );

                /* Remove device keys from address resolution database */
                wiced_hal_read_nvram( WICED_NVRAM_PAIRED_KEYS0 + i, sizeof(wiced_bt_device_link_keys_t), (uint8_t*)&link_keys, &result);
                wiced_bt_dev_remove_device_from_address_resolution_db ( &link_keys );

                /* Remove device bonding information from NVRAM */
                wiced_hal_delete_nvram( WICED_NVRAM_HOST_INFO0 + i, &result );
                wiced_hal_delete_nvram ( WICED_NVRAM_PAIRED_KEYS0 + i, &result );
            }
            /* Update number of bonded devices and next free slot to zero */
            bondInfo[NUM_BONDED]=0;
            bondInfo[NEXT_FREE]=0;
            bond_mode = WICED_TRUE;
            wiced_hal_delete_nvram( WICED_NVRAM_BOND_INFO,&result);
            wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL);

            /* Change state to Idle and no data */
            state = IDLE_NO_DATA;
        }
        else if(IDLE_NO_DATA == state)
            WICED_BT_TRACE("No bond data present \r\n");
        else
            WICED_BT_TRACE("This option is not available when device is in connected or bonded state!!\r\n");

        break;

    case 'e':
        WICED_BT_TRACE("************************** NOTE ***************************************************\r\n");
        WICED_BT_TRACE("*ONCE THE SLOTS ARE FULL THE OLDEST DEVICE DATA WILL BE OVERWRITTEN FOR NEW DEVICE*\r\n");
        WICED_BT_TRACE("***********************************************************************************\r\n");
        if (!((CONNECTED == state) || (BONDED == state)))
        {
            if(bond_mode == WICED_FALSE) /* Enter bond mode */
            {
                /* Check to see if we need to erase one of the existing devices */
                if(bondInfo[NUM_BONDED] == BOND_MAX)
                {
                    /* Remove oldest device from the bonded device list */
                    wiced_hal_read_nvram( WICED_NVRAM_HOST_INFO0 + bondInfo[NEXT_FREE], sizeof(bonded_address), (uint8_t*)&bonded_address, &result );
                    wiced_bt_dev_delete_bonded_device(bonded_address);
                    WICED_BT_TRACE( "Bonding slots full removing the oldest device \r\n");
                    WICED_BT_TRACE( "Removed host [%B] from bonded device list \r\n", bonded_address );

                    /* Remove oldest device from address resolution database */
                    wiced_hal_read_nvram( WICED_NVRAM_PAIRED_KEYS0 + bondInfo[NEXT_FREE], sizeof(wiced_bt_device_link_keys_t), (uint8_t*)&link_keys, &result);
                    wiced_bt_dev_remove_device_from_address_resolution_db ( &link_keys );

                    /* Remove oldest device bonding information from NVRAM */
                    memset( &hostinfoTemp, 0, sizeof(hostinfoTemp));
                    memset( &link_keys, 0, sizeof(wiced_bt_device_link_keys_t));
                    wiced_hal_write_nvram( WICED_NVRAM_HOST_INFO0 + bondInfo[NEXT_FREE], sizeof(hostinfoTemp), (uint8_t*)&hostinfoTemp, &result );
                    wiced_hal_write_nvram ( WICED_NVRAM_PAIRED_KEYS0 + bondInfo[NEXT_FREE], sizeof( wiced_bt_device_link_keys_t ), (uint8_t*)&link_keys, &result );

                    /* Reduce number of bonded devices by one */
                    bondInfo[NUM_BONDED]--;
                    wiced_hal_write_nvram( WICED_NVRAM_BOND_INFO, sizeof(bondInfo), bondInfo, &result);
                }

                /* Put into bonding mode  */
                bond_mode = WICED_TRUE;
                WICED_BT_TRACE( "Bonding Mode Entered\r\n");
                /* restart the advertisements in Bonding Mode */
                wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL);
            }
            else /* Exit bonding mode */
            {
                bond_mode = WICED_FALSE;
                WICED_BT_TRACE( "Bonding Mode Exited\r\n");
            }
        }
        else
            WICED_BT_TRACE("This option is not available when device is in connected or bonded state!!");
        break;

    case 'h':
        display_menu();
        break;

    case 'l' :
        WICED_BT_TRACE("Number of bonded devices: %d, Next free slot: %d, Number of free slot: %d \r\n",bondInfo[NUM_BONDED], bondInfo[NEXT_FREE]+1, (BOND_MAX - bondInfo[NUM_BONDED]));
        for(count = 0; count < bondInfo[NUM_BONDED]; count++)
        {
            wiced_hal_read_nvram( WICED_NVRAM_PAIRED_KEYS0 + count, sizeof(link_keys), (uint8_t*)&link_keys, &status );
            WICED_BT_TRACE("BD_ADDR: [%B]\r\n", &link_keys);
        }
        break;
    case 'p':
        /* If current state is bonded toggle current device privacy mode  else
        * print all devices and ask user for device to toggle Privacy mode*/
        if ( BONDED == state)
        {
            wiced_bt_ble_set_privacy_mode(hostinfo.bdaddr, BTM_BLE_PRIVACY_MODE_NETWORK);
            hostinfo.privacy_mode ^= 1;
            WICED_BT_TRACE( "Connected device Privacy Mode changed to (0 for Network, 1 for Device) :  %d \r\n", hostinfo.privacy_mode );
            wiced_hal_write_nvram(WICED_NVRAM_HOST_INFO0 + bondIndex, sizeof(hostinfo), (uint8_t *)&hostinfo, &result);
        }
        else
        {
            state = IDLE_PRIVACY_CHANGE;
            WICED_BT_TRACE("Select the bonded Devices Found in below list to toggle current privacy mode \r\n");
            for(int i=0;i< bondInfo[NUM_BONDED];i++)
            {
                /* Read device data from the bonded device list */
                wiced_hal_read_nvram(WICED_NVRAM_HOST_INFO0 + i, sizeof(hostinfoTemp), (uint8_t *)&hostinfoTemp, &result);
                WICED_BT_TRACE( "Host %d: [%B] Current Privacy Mode (0 for Network, 1 for Device): %d \r\n", hostinfoTemp.bdaddr, i+1, hostinfoTemp.privacy_mode);
            }
            WICED_BT_TRACE( "\r\nEnter the slot number of the device to change privacy mode: \r\n");
        }
    break;

    default:
    WICED_BT_TRACE("Invalid Input\r\n");
    }
}


/*******************************************************************************
 * Function Name: display_menu()
 ********************************************************************************
 * Summary: Function to print the Menu.
 *
 * Parameters:
 *  None
 *
 * Return:
 *  None
 *
 ********************************************************************************/
void display_menu(void){
    WICED_BT_TRACE("************************** MENU ****************************************\r\n");
    WICED_BT_TRACE("**1) Press 'l' to check for no of bonded devices and next empty slot  **\r\n");
    WICED_BT_TRACE("**2) Press 'r' to erase all the bond data present in NVRAM            **\r\n");
    WICED_BT_TRACE("**3) Press 'e' to enter the bonding mode and add devices to bond list **\r\n");
    WICED_BT_TRACE("**4) Enter slot number to start directed advertisement for that device**\r\n");
    WICED_BT_TRACE("**5) Press 'p' to change privacy mode of bonded device                **\r\n");
    WICED_BT_TRACE("**6) Press 'h' any time in application to print the menu              **\r\n");
    WICED_BT_TRACE("***********************************************************************\r\n");
}


/*******************************************************************************
 * Function Name: directed_adv_handler()
 ********************************************************************************
 * Summary: Directed advertisement Handler.
 *
 * Parameters:
 *  device_index   Index of the device stored in the device list to start directed
 *                 advertisement to.
 *
 * Return:
 *  None
 *
 ********************************************************************************/
void directed_adv_handler(uint8_t device_index)
{
    wiced_result_t                  result;
    wiced_bt_device_link_keys_t     link_keys;

    wiced_bt_start_advertisements(BTM_BLE_ADVERT_OFF, 0, NULL);
    wiced_hal_read_nvram( WICED_NVRAM_PAIRED_KEYS0 + device_index -1, sizeof(wiced_bt_device_link_keys_t), (uint8_t*)&link_keys, &result);
    WICED_BT_TRACE("Starting directed Advertisement for [%B] \r\n",link_keys.bd_addr);
    WICED_BT_TRACE("Enter e for Starting undirected Advertisement to add new device\r\n");
    wiced_bt_start_advertisements(BTM_BLE_ADVERT_DIRECTED_HIGH, link_keys.key_data.ble_addr_type, link_keys.bd_addr);
}

/*******************************************************************************
 * Function Name: void privacy_mode_handler()
 ********************************************************************************
 * Summary: Handles request for change of privacy mode of bonded devices.
 *
 * Parameters:
 *  device_index   Index of the device stored in the device list to change privacy
 *                 mode.
 *
 * Return:
 *  None
 *
 ********************************************************************************/
void privacy_mode_handler(uint8_t device_index)
{
    wiced_result_t                  result;
    wiced_hal_read_nvram(WICED_NVRAM_HOST_INFO0 + device_index - 1, sizeof(hostinfoTemp), (uint8_t *)&hostinfoTemp, &result);
    hostinfoTemp.privacy_mode ^= 1;
    WICED_BT_TRACE( "Privacy Mode for device %d changed to (0 for Network, 1 for Device) :  %d \r\n",device_index, hostinfoTemp.privacy_mode );
    wiced_hal_write_nvram(WICED_NVRAM_HOST_INFO0 + device_index - 1, sizeof(hostinfoTemp), (uint8_t *)&hostinfoTemp, &result);
}