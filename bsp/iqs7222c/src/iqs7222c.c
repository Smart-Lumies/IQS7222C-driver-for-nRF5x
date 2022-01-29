/**
 **********************************************************************************
 * @file     IQS7222C.cpp
 * @author   Azoteq
 * @version  V1.0.0
 * @date     2021-07-06
 * @brief    This file contains the constructors and methods which allow ease of
 *           use of an IQS7222C capactive touch controller. The IQS7222C is a
 *capacitive touch Integrated Circuit (IC) which provides multiple channel
 *           functionality. This class provides an easy means of initializing
 *           and interacting with the IQS7222C device from an Arduino.
 **********************************************************************************
 * @attention  Makes use of the following standard Arduino libraries:
 * - Arduino.h -> Included in IQS7222C.h, comes standard with Arduino
 * - Wire.h    -> Included in IQS7222C.h, comes standard with Arduino
 *
 **********************************************************************************
 */

// Include Files
#include "iqs7222c.h"
#if IQS7222C_v2_23
//#include "IQS7222C_2.23_init.h"
#elif IQS7222C_v2_6
//#include "IQS7222C_2.6_init.h"
#endif
#if IQS7222C_v1_13
//#include "IQS7222C_1.13_init.h"
#endif

#if IQS7222C_CUSTOM_CONFIG
#include "IQS7222C_init.h"
#endif

#include "i2c_touch.h"
#include "nrf_drv_gpiote.h"
#include "nrf_delay.h"

#include <nrf_log.h>
#include <nrf_log_ctrl.h>
#include <nrf_log_default_backends.h>

/**************************************************************************************************************/
/*                                              STATIC DATA & CONSTANTS */
/**************************************************************************************************************/
static bool iqs7222c_deviceRDY = false;
static uint8_t _deviceAddress;
static uint8_t _readyPin;

//  Device States
static iqs7222c_s iqs7222C_state;
//  Variables
static IQS7222C_MEMORY_MAP IQSMemoryMap;
static bool new_data_available;

static nrf_drv_gpiote_in_config_t _pin_config_in = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);

/**************************************************************************************************************/
/*                                             PRIVATE METHODS */
/**************************************************************************************************************/
void ready_interupt(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
int readRandomBytes(uint8_t memoryAddress, uint8_t numBytes,
                    uint8_t bytesArray[], bool stopOrRestart);
int writeRandomBytes(uint8_t memoryAddress, uint8_t numBytes,
                     uint8_t bytesArray[], bool stopOrRestart);
int writeRandomBytes16(uint16_t memoryAddress, uint8_t numBytes,
                       uint8_t bytesArray[], bool stopOrRestart);

/**************************************************************************************************************/
/*                                              PUBLIC METHODS */
/**************************************************************************************************************/

/**
 * @name   begin
 * @brief  A method to initialize the IQS7222C device with the device address
 * and ready pin specified by the user.
 * @param  deviceAddress -> The address of the IQS7222C device.
 *         readyPin      -> The Arduino pin which is connected to the ready pin
 * of the IQS7222C device.
 * @retval Returns true if communication has been successfully established,
 * returns false if not.
 * @notes  Receiving a true return value does not mean that initialization was
 * successful. Receiving a true return value only means that the IQS device
 * responded to the request for communication. Receiving a false return value
 * means that initialization did not take place at all. If communication is
 * successfully established then it is unlikely that initialization will fail.
 */
bool iqs7222c_begin(uint8_t deviceAddressIn, uint8_t readyPinIn, const nrf_drv_twi_t *m_twi)
{
    // Initialize I2C communication here, since this library can't function
    // without it.
    ret_code_t retCode = NRF_SUCCESS;
    i2c_touch_init(m_twi);

    bool response = false; // The return value. Is set to the return value of the requestComms function.
    _deviceAddress = deviceAddressIn;
    _readyPin = readyPinIn;

    // initalize GPIO module
    if (false == nrf_drv_gpiote_is_init())
    {
        retCode = nrf_drv_gpiote_init();
    }

    if (retCode != NRF_SUCCESS)
    {
        return false;
    }

    retCode = nrf_drv_gpiote_in_init(_readyPin, &_pin_config_in, NULL);

    if (retCode != NRF_SUCCESS)
    {
        return false;
    }

    // Request communication and run ATI routine.
    response = iqs7222c_waitForReady();

    if (response)
    {
        iqs7222c_acknowledgeReset(STOP);
    }

    return response;
}

/**
 * @name   inIt
 * @brief  A method that runs through a normal start-up routine to set up the
 * IQS7222C with the desired settings from the IQS7222C_init.h file.
 * @retval Returns true if the full start-up routine has been completed, returns
 * false if not.
 * @notes  No false return will be given, the program will thus be stuck when
 * one of the cases is not able to finish. See serial communication to find the
 * ERROR case
 */
bool iqs7222c_init(void)
{
    uint16_t prod_num;
    uint8_t ver_maj, ver_min;

    switch (iqs7222C_state.init_state)
    {
    case IQS7222C_INIT_READ_RESET:
        //NRF_LOG_INFO("IQS7222C_INIT_READ_RESET");

        /* Read Info Flags */
        iqs7222c_updateInfoFlags(RESTART);
        if (iqs7222c_checkReset())
        {
            //NRF_LOG_INFO("Software Reset event occured.");
            // If the Show Reset bit is set, acknowledge the reset event
            iqs7222c_acknowledgeReset(RESTART);
            //NRF_LOG_INFO("Acknowledge reset.");
            iqs7222C_state.init_state = IQS7222C_INIT_VERIFY_PRODUCT;
        }
        else
        {
            iqs7222C_state.init_state = IQS7222C_INIT_CHIP_RESET;
        }
        break;

    case IQS7222C_INIT_CHIP_RESET:
        //NRF_LOG_INFO("IQS7222C_INIT_CHIP_RESET");

        // Perform SW Reset
        iqs7222c_SW_Reset(RESTART);
        //NRF_LOG_INFO("Software Reset Bit set.");
        nrf_delay_ms(100);

        while (!iqs7222c_waitForReady())
            ;

        // Read the Info flags
        iqs7222c_updateInfoFlags(RESTART);
        // Check if Show Reset bit is set
        if (iqs7222c_checkReset())
        {
            //NRF_LOG_INFO("Software Reset event occured.");
            // If the Show Reset bit is set, acknowledge the reset event
            iqs7222c_acknowledgeReset(RESTART);
            //NRF_LOG_INFO("Acknowledge reset.");
            iqs7222C_state.init_state = IQS7222C_INIT_VERIFY_PRODUCT;
        }
        break;

    /* Verifies product number to determine if correct device is connected to
   * Arduino */
    case IQS7222C_INIT_VERIFY_PRODUCT:
        //NRF_LOG_INFO("IQS7222C_INIT_VERIFY_PRODUCT");
        iqs7222C_state.init_state = IQS7222C_INIT_UPDATE_SETTINGS;
#if false 
        prod_num = iqs7222c_getProductNum(RESTART);
        ver_maj = iqs7222c_getmajorVersion(RESTART);
        ver_min = iqs7222c_getminorVersion(RESTART);
        //NRF_LOG_INFO("Product number is: %d", prod_num);
        //NRF_LOG_INFO(" v %d.%d", ver_maj, ver_min);
        if (prod_num == IQS7222C_PRODUCT_NUM)
        {
            //NRF_LOG_INFO("\t\tIQS7222C Confirmed!");
#if IQS7222C_v2_23
            if (ver_maj == 2 && ver_min == 23)
            {
                //NRF_LOG_INFO("\t\tv");
                //NRF_LOG_INFO(ver_maj);
                //NRF_LOG_INFO(".");
                //NRF_LOG_INFO(ver_min);
                //NRF_LOG_INFO(" is compatible");
                iqs7222C_state.init_state = IQS7222C_INIT_UPDATE_SETTINGS;
            }
            else
            {
                //NRF_LOG_INFO("\t\tv");
                //NRF_LOG_INFO(ver_maj);
                //NRF_LOG_INFO(".");
                //NRF_LOG_INFO(ver_min);
                //NRF_LOG_INFO(" not compatible, current selection is v2.23!");
                iqs7222C_state.init_state = IQS7222C_INIT_NONE;
            }
#elif IQS7222C_v2_6
            if (ver_maj == 2 && ver_min == 6)
            {
                //NRF_LOG_INFO("\t\tv");
                //NRF_LOG_INFO(ver_maj);
                //NRF_LOG_INFO(".");
                //NRF_LOG_INFO(ver_min);
                //NRF_LOG_INFO(" is compatible");
                iqs7222C_state.init_state = IQS7222C_INIT_UPDATE_SETTINGS;
            }
            else
            {
                //NRF_LOG_INFO("\t\tv");
                //NRF_LOG_INFO(ver_maj);
                //NRF_LOG_INFO(".");
                //NRF_LOG_INFO(ver_min);
                //NRF_LOG_INFO(" not compatible, current selection is v2.6!");
                iqs7222C_state.init_state = IQS7222C_INIT_NONE;
            }
#elif IQS7222C_v1_13
            if (ver_maj == 1 && ver_min == 13)
            {
                //NRF_LOG_INFO("\t\tv");
                //NRF_LOG_INFO(ver_maj);
                //NRF_LOG_INFO(".");
                //NRF_LOG_INFO(ver_min);
                //NRF_LOG_INFO(" is compatible");
                iqs7222C_state.init_state = IQS7222C_INIT_UPDATE_SETTINGS;
            }
            else
            {
                //NRF_LOG_INFO("\t\tv");
                //NRF_LOG_INFO(ver_maj);
                //NRF_LOG_INFO(".");
                //NRF_LOG_INFO(ver_min);
                //NRF_LOG_INFO(" not compatible, current selection is v1.13!");
                iqs7222C_state.init_state = IQS7222C_INIT_NONE;
            }
#endif
        }
        else
        {
            //NRF_LOG_INFO("\t\tDevice is not a IQS7222C!");
            iqs7222C_state.init_state = IQS7222C_INIT_NONE;
        }
#endif
        break;

    /* Write all settings to IQS7222A from .h file */
    case IQS7222C_INIT_UPDATE_SETTINGS:
        //NRF_LOG_INFO("IQS7222C_INIT_UPDATE_SETTINGS");
        iqs7222c_writeMM(RESTART);
        iqs7222C_state.init_state = IQS7222C_INIT_ACK_RESET;
        break;

    /* Acknowledge that the device went through a reset */
    case IQS7222C_INIT_ACK_RESET:
        //NRF_LOG_INFO("IQS7222C_INIT_ACK_RESET");
        iqs7222c_acknowledgeReset(RESTART);
        iqs7222C_state.init_state = IQS7222C_INIT_ATI;

    /* Run the ATI algorithm to recalibrate the device with newly added settings
   */
    case IQS7222C_INIT_ATI:
        //NRF_LOG_INFO("IQS7222C_INIT_ATI");
        iqs7222c_TP_ReATI(STOP);
        iqs7222C_state.init_state = IQS7222C_ACTIVATE_EVENT_MODE;
        break;

    /* Turn on I2C event mode */
    case IQS7222C_ACTIVATE_EVENT_MODE:
        //NRF_LOG_INFO("IQS7222C_ACTIVATE_EVENT_MODE");
        while (!iqs7222c_waitForReady())
            ;
        iqs7222c_setEventMode(STOP);
        iqs7222C_state.init_state = IQS7222C_INIT_DONE;
        break;

    /* Turn on I2C stream in touch mode */
    case IQS7222C_ACTIVATE_STREAM_IN_TOUCH:
        //NRF_LOG_INFO("IQS7222C_ACTIVATE_STREAM_IN_TOUCH");
        while (!iqs7222c_waitForReady())
            ;
        iqs7222c_setStreamInTouch(STOP);
        iqs7222C_state.init_state = IQS7222C_INIT_DONE;
        break;

    /* If all operations has been completed correctly, RDY pin can be set up as
   * intterupt to indicate when new data is available */
    case IQS7222C_INIT_DONE:
        //NRF_LOG_INFO("IQS7222C_INIT_DONE");

        /* Setup interrupt pin for ready windows */
        nrf_drv_gpiote_in_uninit(_readyPin);
        nrf_drv_gpiote_in_config_t rdy_pin_interrupt_cfg = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
        nrf_drv_gpiote_in_init(_readyPin, &rdy_pin_interrupt_cfg, ready_interupt);

        //NRF_LOG_INFO("IQS7222C Ready!");
        iqs7222c_deviceRDY = false;
        new_data_available = false;
        return true;
        break;

    default:
        break;
    }
    return false;
}

/**
 * @name   run
 * @brief  Run the device ready check and read the neccessary bytes when the
 * IQS7222c has pulled low the RDY line. The new_data_available flag will be set
 * when a ready low is received from the IQS7222c.
 * @param  None
 * @retval None.
 * @notes  iqs7222c_queueValueUpdates can be edited by the user if other data should be
 * read everytime a RDY window is received.
 */
void iqs7222c_run(void)
{
    if (iqs7222c_deviceRDY)
    {
        iqs7222c_queueValueUpdates();
        new_data_available = true;
        iqs7222c_deviceRDY = false;
    }
}

/**
 * @name   ready_interupt
 * @brief  A method used as intterupt function. Only activated when a HL
 * innterupt is seen on the correct Arduino interrupt pin.
 * @param  None.
 * @retval None.
 * @notes  Keep this function as simple as possible to prevent stuck states and
 * slow operations.
 */
void ready_interupt(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    iqs7222c_deviceRDY = true;
}

/**
 * @name   iqs7222c_queueValueUpdates
 * @brief   All I2C read operations in the iqs7222c_queueValueUpdates method will be
 * performed each time the IQS7222A opens a RDY window.
 * @param  None.
 * @retval None.
 * @notes  Any Address in memory map can be read from here.
 */
void iqs7222c_queueValueUpdates(void)
{
    uint8_t transferBytes[10]; // The array which will hold the bytes to be
                               // transferred.

    // Read the info flags.
    readRandomBytes(IQS7222C_MM_INFOFLAGS, 10, transferBytes, STOP);
    // Assign the info flags to the info flags union.
    IQSMemoryMap.iqs7222c_info_flags.iqs7222c_infoflags_lsb = transferBytes[0];
    IQSMemoryMap.iqs7222c_info_flags.iqs7222c_infoflags_msb = transferBytes[1];

    IQSMemoryMap.iqs7222c_events.iqs7222c_events_lsb = transferBytes[2];
    IQSMemoryMap.iqs7222c_events.iqs7222c_events_msb = transferBytes[3];

    IQSMemoryMap.iqs7222c_prox_event_states.iqs7222c_prox_event_states_lsb =
        transferBytes[4];
    IQSMemoryMap.iqs7222c_prox_event_states.iqs7222c_prox_event_states_msb =
        transferBytes[5];

    IQSMemoryMap.iqs7222c_touch_event_states.iqs7222c_touch_event_states_lsb =
        transferBytes[6];
    IQSMemoryMap.iqs7222c_touch_event_states.iqs7222c_touch_event_states_msb =
        transferBytes[7];

    IQSMemoryMap.iqs7222c_slider_wheel_out.iqs7222c_slider_0_out_lsb =
        transferBytes[8];
    IQSMemoryMap.iqs7222c_slider_wheel_out.iqs7222c_slider_0_out_msb =
        transferBytes[9];
}

uint8_t iqs7222c_getTouchByte(bool stopOrRestart)
{
    uint8_t transferBytes[2];
    IQS7222C_TOUCH_EVENT_STATES touchData;
    int retVal = readRandomBytes(IQS7222C_MM_TOUCH_EVENT_STATES, 2, transferBytes, stopOrRestart);
    touchData.iqs7222c_touch_event_states_lsb = transferBytes[0];
    touchData.iqs7222c_touch_event_states_msb = transferBytes[1];
#if NRF_LOG_ENABLED
    NRF_LOG_INFO("Touch data:   [ %4d, %4d, %4d, %4d, %4d, %4d] ",
                 touchData.ch0_touch_event,
                 touchData.ch1_touch_event,
                 touchData.ch2_touch_event,
                 touchData.ch3_touch_event,
                 touchData.ch4_touch_event,
                 touchData.ch5_touch_event);
#endif
    return touchData.iqs7222c_touch_event_states_lsb;
}

int iqs7222c_getCounts(bool stopOrRestart)
{
    uint8_t transferBytes[20];
    int retVal = readRandomBytes(IQS7222C_MM_CHANNEL_0_COUNTS, 20, transferBytes, stopOrRestart);
    uint16_t channelCounts[10];
    channelCounts[0] = (uint16_t)(transferBytes[0]);
    channelCounts[0] |= (uint16_t)(transferBytes[1] << 8);
    channelCounts[1] = (uint16_t)(transferBytes[2]);
    channelCounts[1] |= (uint16_t)(transferBytes[3] << 8);
    channelCounts[2] = (uint16_t)(transferBytes[4]);
    channelCounts[2] |= (uint16_t)(transferBytes[5] << 8);
    channelCounts[3] = (uint16_t)(transferBytes[6]);
    channelCounts[3] |= (uint16_t)(transferBytes[7] << 8);
    channelCounts[4] = (uint16_t)(transferBytes[8]);
    channelCounts[4] |= (uint16_t)(transferBytes[9] << 8);
    channelCounts[5] = (uint16_t)(transferBytes[10]);
    channelCounts[5] |= (uint16_t)(transferBytes[11] << 8);
    channelCounts[6] = (uint16_t)(transferBytes[12]);
    channelCounts[6] |= (uint16_t)(transferBytes[13] << 8);
    channelCounts[7] = (uint16_t)(transferBytes[14]);
    channelCounts[7] |= (uint16_t)(transferBytes[15] << 8);

    NRF_LOG_INFO("Channel count [ %4d, %4d, %4d, %4d, %4d, %4d]", channelCounts[0], channelCounts[1], channelCounts[2], channelCounts[3], channelCounts[4], channelCounts[5]);
    return retVal;
}

int iqs7222c_getLta(bool stopOrRestart)
{
    uint8_t transferBytes[20];
    int retVal = readRandomBytes(IQS7222C_MM_CHANNEL_0_LTA, 20, transferBytes, stopOrRestart);
    uint16_t channelCounts[10];
    channelCounts[0] = (uint16_t)(transferBytes[0]);
    channelCounts[0] |= (uint16_t)(transferBytes[1] << 8);
    channelCounts[1] = (uint16_t)(transferBytes[2]);
    channelCounts[1] |= (uint16_t)(transferBytes[3] << 8);
    channelCounts[2] = (uint16_t)(transferBytes[4]);
    channelCounts[2] |= (uint16_t)(transferBytes[5] << 8);
    channelCounts[3] = (uint16_t)(transferBytes[6]);
    channelCounts[3] |= (uint16_t)(transferBytes[7] << 8);
    channelCounts[4] = (uint16_t)(transferBytes[8]);
    channelCounts[4] |= (uint16_t)(transferBytes[9] << 8);
    channelCounts[5] = (uint16_t)(transferBytes[10]);
    channelCounts[5] |= (uint16_t)(transferBytes[11] << 8);
    channelCounts[6] = (uint16_t)(transferBytes[12]);
    channelCounts[6] |= (uint16_t)(transferBytes[13] << 8);
    channelCounts[7] = (uint16_t)(transferBytes[14]);
    channelCounts[7] |= (uint16_t)(transferBytes[15] << 8);

    NRF_LOG_INFO("Channel LTA   [ %4d, %4d, %4d, %4d, %4d, %4d]", channelCounts[0], channelCounts[1], channelCounts[2], channelCounts[3], channelCounts[4], channelCounts[5]);
    return retVal;
}

/**
 * @name  iqs7222c_waitForReady
 * @brief  A method which waits for the IQS7222C device to pull the ready pin
 * low. If there is no response for 100ms a false value is returned and the user
 * can then either force communication by using the method described in
 * section 11.9.2 in the datasheet or just wait longer by placing this method in
 * a while loop.
 * @param  None.
 * @retval Boolean: True if a response is received within 100ms, false if not.
 * @notes  Use this function when the device is in stream mode and you want the
 * master to await communication from the IQS7222C device. If you want the
 * master to initiate communication use the Force Communication method. For
 * optimal program flow, it is suggested that RDY is used to sync on new data.
 * The forced/polling method is only recommended if the master must perform I2C
 * and Event Mode is active.
 */
bool iqs7222c_waitForReady(void)
{
    bool readyLow = false;      // The return value. Set to true if the ready pin is
                                // pulled low by the IQS7222C within 100ms.
    uint16_t notReadyCount = 0; // Increments every time the loop executes to keep
                                // track of how long the request is going on.

    // Wait for communication from IQS7222C device. Timeout after 100ms.
    while (nrf_drv_gpiote_in_is_set(_readyPin))
    {
        notReadyCount++;
        nrf_delay_us(100);

        if ((notReadyCount % 1000) == 0)
            return readyLow;
    }
    // If the processing gets here then a response has been received.
    readyLow = true;
    return readyLow;
}

/**
 * @name	checkReset
 * @brief  A method which checks if the device has reset and returns the reset
 * status.
 * @param  None.
 * @retval Returns true if a reset has occurred, false if no reset has occurred.
 * @notes  If a reset has occurred the device settings should be reloaded using
 * the begin function. After new device settings have been reloaded the
 * acknowledge reset function can be used to clear the reset flag.
 */
bool iqs7222c_checkReset(void)
{

    IQSMemoryMap.iqs7222c_info_flags.iqs7222c_infoflags_lsb &= SHOW_RESET_BIT;
    // Return the reset status.
    if (IQSMemoryMap.iqs7222c_info_flags.iqs7222c_infoflags_lsb != 0)
        return true;
    else
        return false;
}

/**
 * @name	checkProductNum
 * @brief  A method which checks the device product number and compare the
 * result to the defined value to return a boolean result.
 * @param  stopOrRestart -> Specifies whether the communications window must be
 * kept open or must be closed after this action. Use the STOP and RESTART
 * definitions.
 * @retval Returns true if product number is a match, false if numbers do not
 * match.
 * @notes  If the product is not correctly identified an appropriate messages
 * should be displayed.
 */
uint16_t iqs7222c_getProductNum(bool stopOrRestart)
{
    uint8_t
        transferBytes[2];    // A temporary array to hold the byte to be transferred.
    uint8_t prodNumLow = 0;  // Temporary storage for the counts low byte.
    uint8_t prodNumHigh = 0; // Temporary storage for the counts high byte.
    uint16_t prodNumReturn =
        0; // The 16bit return value.
           // Use an array to be consistent with other methods of the library.
    // Read the Device info from the IQS7222C.
    readRandomBytes(IQS7222C_MM_PROD_NUM, 2, transferBytes, stopOrRestart);

    // Construct the 16bit return value.
    prodNumLow = transferBytes[0];
    prodNumHigh = transferBytes[1];
    prodNumReturn = (uint16_t)(prodNumLow);
    prodNumReturn |= (uint16_t)(prodNumHigh << 8);
    // Return the counts value.
    return prodNumReturn;
}

/**
 * @name	getmajorVersion
 * @brief  A method which checks the device firmware version number, major
 * value.
 * @param  stopOrRestart -> Specifies whether the communications window must be
 * kept open or must be closed after this action. Use the STOP and RESTART
 * definitions.
 * @retval Returns major version number value.
 */
uint8_t iqs7222c_getmajorVersion(bool stopOrRestart)
{
    uint8_t
        transferBytes[2]; // A temporary array to hold the byte to be transferred.
    uint8_t ver_maj =
        0; // Temporary storage for the firmware version major number.

    // Read the Device info from the IQS7222C.
    readRandomBytes(IQS7222C_MM_MAJOR_VERSION_NUM, 2, transferBytes,
                    stopOrRestart);

    // get major value from correct byte
    ver_maj = transferBytes[0];
    // Return the major firmware version number value.
    return ver_maj;
}

/**
 * @name	getminorVersion
 * @brief  A method which checks the device firmware version number, minor
 * value.
 * @param  stopOrRestart -> Specifies whether the communications window must be
 * kept open or must be closed after this action. Use the STOP and RESTART
 * definitions.
 * @retval Returns minor version number value.
 */
uint8_t iqs7222c_getminorVersion(bool stopOrRestart)
{
    uint8_t
        transferBytes[2]; // A temporary array to hold the byte to be transferred.
    uint8_t ver_min =
        0; // Temporary storage for the firmware version minor number.

    // Read the Device info from the IQS7222C.
    readRandomBytes(IQS7222C_MM_MINOR_VERSION_NUM, 2, transferBytes,
                    stopOrRestart);

    // get major value from correct byte
    ver_min = transferBytes[0];
    // Return the minor firmware version number value.
    return ver_min;
}

/**
 * @name	acknowledgeReset
 * @brief  A method which clears the Show Reset bit by writing it to a 0.
 * @param  stopOrRestart -> Specifies whether the communications window must be
 * kept open or must be closed after this action. Use the STOP and RESTART
 * definitions.
 * @retval None.
 * @notes  If a reset has occurred the device settings should be reloaded using
 * the begin function. After new device settings have been reloaded this method
 * should be used to clear the reset bit.
 */
void iqs7222c_acknowledgeReset(bool stopOrRestart)
{
    uint8_t transferBytes[2]; // A temporary array to hold the bytes to be
                              // transferred.
    // Read the System Flags from the IQS7222C, these must be read first in order
    // not to change any settings. We are interested in the 2nd byte at the
    // address location, therefore, we must read and write both bytes.
    readRandomBytes(IQS7222C_MM_CONTROL_SETTINGS, 2, transferBytes, RESTART);
    // SWrite the AAck Reset bit to 1 to clear the Show Reset Flag.
    transferBytes[0] |= ACK_RESET_BIT;
    // Write the new byte to the System Flags address.
    writeRandomBytes(IQS7222C_MM_CONTROL_SETTINGS, 2, transferBytes,
                     stopOrRestart);
}

/**
 * @name   TP_ReATI
 * @brief  A method which sets the REDO_ATI_BIT in order to force the IQS7222C
 * device to run the Automatic Tuning Implementation (ATI) routine.
 * @param  stopOrRestart -> Specifies whether the communications window must be
 * kept open or must be closed after this action. Use the STOP and RESTART
 * definitions.
 * @retval None.
 * @notes  To force ATI, bit 4 in PROX_SETTINGS_0 is set. PROX_SETTINGS_0 is
 * byte 0 in the PROXSETTINGS_0_1 address.
 */
void iqs7222c_TP_ReATI(bool stopOrRestart)
{
    uint8_t transferByte[2]; // Array to store the bytes transferred.
                             // Use an array to be consistent with other methods
                             // in this class.
    readRandomBytes(IQS7222C_MM_CONTROL_SETTINGS, 2, transferByte, RESTART);
    // Mask the settings with the REDO_ATI_BIT.
    transferByte[0] |=
        TP_REATI_BIT; // This is the bit required to start an ATI routine.
    // Write the new byte to the required device.
    writeRandomBytes(IQS7222C_MM_CONTROL_SETTINGS, 2, transferByte,
                     stopOrRestart);
}

void iqs7222c_reSeed(bool stopOrRestart)
{
    uint8_t transferByte[2]; // Array to store the bytes transferred.
                             // Use an array to be consistent with other methods
                             // in this class.
    readRandomBytes(IQS7222C_MM_CONTROL_SETTINGS, 2, transferByte, RESTART);
    transferByte[0] |= TP_RESEED_BIT;
    writeRandomBytes(IQS7222C_MM_CONTROL_SETTINGS, 2, transferByte,
                     stopOrRestart);
}

/**
 * @name   SW_Reset
 * @brief  A method which sets the SW RESET bit in order to force the IQS7222C
 * device to do a SW reset.
 * @param  stopOrRestart -> Specifies whether the communications window must be
 * kept open or must be closed after this action. Use the STOP and RESTART
 * definitions.
 * @retval None.
 * @notes  To perform SW Reset, bit 9 in SYSTEM_CONTROL is set.
 */
void iqs7222c_SW_Reset(bool stopOrRestart)
{
    uint8_t transferByte[2]; // Array to store the bytes transferred.
                             // Use an array to be consistent with other methods
                             // in this class.
    readRandomBytes(IQS7222C_MM_CONTROL_SETTINGS, 2, transferByte, RESTART);
    // Mask the settings with the SW_RESET_BIT.
    transferByte[0] |= SW_RESET_BIT; // This is the bit required to perform SW Reset.
    // Write the new byte to the required device.
    writeRandomBytes(IQS7222C_MM_CONTROL_SETTINGS, 2, transferByte,
                     stopOrRestart);
}

/**
 * @name   setEventMode
 * @brief  A method to set the IQS7222C device into event mode.
 * @param  stopOrRestart -> Specifies whether the communications window must be
 * kept open or must be closed after this action. Use the STOP and RESTART
 * definitions.
 * @retval None.
 * @notes  All other bits at the register address are preserved.
 */
void iqs7222c_setEventMode(bool stopOrRestart)
{
    uint8_t transferBytes[2]; // The array which will hold the bytes which are
                              // transferred.

    // First read the bytes at the memory address so that they can be preserved.
    readRandomBytes(IQS7222C_MM_CONTROL_SETTINGS, 2, transferBytes, RESTART);
    // Clear the EVENT_MODE_BIT in PROX_SETTINGS_1
    transferBytes[0] |= EVENT_MODE_BIT;
    // Write the bytes back to the device
    writeRandomBytes(IQS7222C_MM_CONTROL_SETTINGS, 2, transferBytes,
                     stopOrRestart);
}

/**
 * @name   setStreamInTouch
 * @brief  A method to set the IQS7222C device into streaming when in touch
 * mode.
 * @param  stopOrRestart -> Specifies whether the communications window must be
 * kept open or must be closed after this action. Use the STOP and RESTART
 * definitions.
 * @retval None.
 * @notes  All other bits at the register address are preserved.
 */
void iqs7222c_setStreamInTouch(bool stopOrRestart)
{
    uint8_t transferBytes[2]; // The array which will hold the bytes which are
                              // transferred.

    // First read the bytes at the memory address so that they can be preserved.
    readRandomBytes(IQS7222C_MM_CONTROL_SETTINGS, 2, transferBytes, RESTART);
    // Set the STREAM_IN_TOUCH_BIT in CONTROL_SETTINGS
    transferBytes[0] |= STREAM_IN_TOUCH_BIT;
    // Write the bytes back to the device
    writeRandomBytes(IQS7222C_MM_CONTROL_SETTINGS, 2, transferBytes,
                     stopOrRestart);
}

/**
 * @name   updateInfoFlags
 * @brief  A method which reads the IQS7222C info flags and assigns them to the
 * infoFlags union.
 * @param  stopOrRestart -> Specifies whether the communications window must be
 * kept open or must be closed after this action. Use the STOP and RESTART
 * definitions.
 * @retval None.
 * @notes  The infoFlags union is altered with the new value of the info flags
 * register.
 */
void iqs7222c_updateInfoFlags(bool stopOrRestart)
{
    uint8_t transferBytes[8]; // The array which will hold the bytes to be
                              // transferred.

    // Read the info flags.
    readRandomBytes(IQS7222C_MM_INFOFLAGS, 8, transferBytes, stopOrRestart);
    // Assign the info flags to the info flags union.
    IQSMemoryMap.iqs7222c_info_flags.iqs7222c_infoflags_lsb = transferBytes[0];
    IQSMemoryMap.iqs7222c_info_flags.iqs7222c_infoflags_msb = transferBytes[1];
}

/**
 * @name   get_PowerMode
 * @brief  A method which reads the system flags unions and returns the current
 * power mode.
 * @param  void
 * @retval None
 * @notes  See Datasheet on power mode options and timeouts. Normal Power, Low
 * Power and Ultra Low Power (ULP).
 */
IQS7222C_power_modes iqs7222c_get_PowerMode(void)
{
    uint8_t buffer = IQSMemoryMap.iqs7222c_info_flags.power_mode;

    if (buffer == NORMAL_POWER_BIT)
    {
        return NormalPower;
    }
    else if (buffer == LOW_POWER_BIT)
    {
        return LowPower;
    }
    else if (buffer == ULP_BIT)
    {
        return ULP;
    }

    return NormalPower;
}

/**
 * @name   channel_touchState
 * @brief  A method which reads the unions for a given channel ad determines if
 * the channel is in a touch state.
 * @param  channel -> The channel name on the IQS7222C (CH0-CH10) for which a
 * touch state needs to be determined.
 * @retval Returns true is a touch is avtive and false if there is no touch.
 * @notes  See the IQS7222C_Channel_e typedef for all possible channel names.
 */
bool iqs7222c_channel_touchState(IQS7222C_Channel_e channel)
{
    switch (channel)
    {
    case IQS7222C_CH0:
        if (IQSMemoryMap.iqs7222c_touch_event_states.ch0_touch_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH1:
        if (IQSMemoryMap.iqs7222c_touch_event_states.ch1_touch_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH2:
        if (IQSMemoryMap.iqs7222c_touch_event_states.ch2_touch_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH3:
        if (IQSMemoryMap.iqs7222c_touch_event_states.ch3_touch_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH4:
        if (IQSMemoryMap.iqs7222c_touch_event_states.ch4_touch_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH5:
        if (IQSMemoryMap.iqs7222c_touch_event_states.ch5_touch_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH6:
        if (IQSMemoryMap.iqs7222c_touch_event_states.ch6_touch_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH7:
        if (IQSMemoryMap.iqs7222c_touch_event_states.ch7_touch_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH8:
        if (IQSMemoryMap.iqs7222c_touch_event_states.ch8_touch_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH9:
        if (IQSMemoryMap.iqs7222c_touch_event_states.ch9_touch_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    default:
        return false;
        break;
    }
}

/**
 * @name   channel_proxState
 * @brief  A method which reads the unions for a given channel and determines if
 * the channel is in a proximity state.
 * @param  channel -> The channel name on the IQS7222C (CH0-CH10) for which a
 * proximity state needs to be determined.
 * @retval Returns true is proximity is avtive and false if there is no
 * proximity.
 * @notes  See the IQS7222C_Channel_e typedef for all possible channel names.
 */
bool iqs7222c_channel_proxState(IQS7222C_Channel_e channel)
{
    switch (channel)
    {
    case IQS7222C_CH0:
        if (IQSMemoryMap.iqs7222c_prox_event_states.ch0_prox_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH1:
        if (IQSMemoryMap.iqs7222c_prox_event_states.ch1_prox_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH2:
        if (IQSMemoryMap.iqs7222c_prox_event_states.ch2_prox_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH3:
        if (IQSMemoryMap.iqs7222c_prox_event_states.ch3_prox_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH4:
        if (IQSMemoryMap.iqs7222c_prox_event_states.ch4_prox_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH5:
        if (IQSMemoryMap.iqs7222c_prox_event_states.ch5_prox_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH6:
        if (IQSMemoryMap.iqs7222c_prox_event_states.ch6_prox_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH7:
        if (IQSMemoryMap.iqs7222c_prox_event_states.ch7_prox_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH8:
        if (IQSMemoryMap.iqs7222c_prox_event_states.ch8_prox_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    case IQS7222C_CH9:
        if (IQSMemoryMap.iqs7222c_prox_event_states.ch9_prox_event == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
        break;

    default:
        return false;
        break;
    }
}

/**
 * @name   silderCoordinate
 * @brief  A method which reads the coordinates slider output union and
 * calculated the current slider position.
 * @param  slider -> The slider name on the IQS7222C (Slider 0 or Slider 1).
 * @retval Returns a 16 bit value which contains the slider coordinates from 0
 * to the resulution maximum.
 * @notes  See the IQS7222C_slider_e typedef for all possible slider names.
 */
uint16_t iqs7222c_silderCoordinate(IQS7222C_slider_e slider)
{
    uint16_t buffer;
    if (slider == IQS7222C_SLIDER0)
    {
        buffer = IQSMemoryMap.iqs7222c_slider_wheel_out.iqs7222c_slider_0_out_lsb;
        buffer += IQSMemoryMap.iqs7222c_slider_wheel_out.iqs7222c_slider_0_out_msb
                  << 8;
        return buffer;
    }
    else
    {
        buffer = IQSMemoryMap.iqs7222c_slider_wheel_out.iqs7222c_slider_1_out_lsb;
        buffer += IQSMemoryMap.iqs7222c_slider_wheel_out.iqs7222c_slider_1_out_msb
                  << 8;
        return buffer;
    }
}
/**************************************************************************************************************/
/*											ADVANCED
 * PUBLIC METHODS
 */
/**************************************************************************************************************/

/**
 * @name   writeMM
 * @brief  Function to write the whole memory map to the device (writable)
 * registers
 * @param  IQS7222CA_init.h -> exported GUI init.h file
 * @retval None.
 * @notes
 */
int iqs7222c_writeMM(bool stopOrRestart)
{
    int returnValue = 0;
    uint8_t transferBytes[30]; // Temporary array which holds the bytes to be
                               // transferred.

    /* Change the Cycle Setup */
    /* Memory Map Position 0x8000 - 0x8403 */
    transferBytes[0] = CYCLE_0_CONV_FREQ_FRAC;
    transferBytes[1] = CYCLE_0_CONV_FREQ_PERIOD;
    transferBytes[2] = CYCLE_0_SETTINGS;
    transferBytes[3] = CYCLE_0_CTX_SELECT;
    transferBytes[4] = CYCLE_0_IREF_0;
    transferBytes[5] = CYCLE_0_IREF_1;
    transferBytes[6] = CYCLE_1_CONV_FREQ_FRAC;
    transferBytes[7] = CYCLE_1_CONV_FREQ_PERIOD;
    transferBytes[8] = CYCLE_1_SETTINGS;
    transferBytes[9] = CYCLE_1_CTX_SELECT;
    transferBytes[10] = CYCLE_1_IREF_0;
    transferBytes[11] = CYCLE_1_IREF_1;
    transferBytes[12] = CYCLE_2_CONV_FREQ_FRAC;
    transferBytes[13] = CYCLE_2_CONV_FREQ_PERIOD;
    transferBytes[14] = CYCLE_2_SETTINGS;
    transferBytes[15] = CYCLE_2_CTX_SELECT;
    transferBytes[16] = CYCLE_2_IREF_0;
    transferBytes[17] = CYCLE_2_IREF_1;
    transferBytes[18] = CYCLE_3_CONV_FREQ_FRAC;
    transferBytes[19] = CYCLE_3_CONV_FREQ_PERIOD;
    transferBytes[20] = CYCLE_3_SETTINGS;
    transferBytes[21] = CYCLE_3_CTX_SELECT;
    transferBytes[22] = CYCLE_3_IREF_0;
    transferBytes[23] = CYCLE_3_IREF_1;
    transferBytes[24] = CYCLE_4_CONV_FREQ_FRAC;
    transferBytes[25] = CYCLE_4_CONV_FREQ_PERIOD;
    transferBytes[26] = CYCLE_4_SETTINGS;
    transferBytes[27] = CYCLE_4_CTX_SELECT;
    transferBytes[28] = CYCLE_4_IREF_0;
    transferBytes[29] = CYCLE_4_IREF_1;
    returnValue += writeRandomBytes16(IQS7222C_MM_CYCLE_SETUP_0, 30, transferBytes, RESTART);
    //NRF_LOG_INFO("\t\t1. Write Cycle Settings");

    /* Change the Global Cycle Setup */
    /* Memory Map Position 0x8500 - 0x8502 */
    transferBytes[0] = GLOBAL_CYCLE_SETUP_0;
    transferBytes[1] = GLOBAL_CYCLE_SETUP_1;
    transferBytes[2] = COARSE_DIVIDER_PRELOAD;
    transferBytes[3] = FINE_DIVIDER_PRELOAD;
    transferBytes[4] = COMPENSATION_PRELOAD_0;
    transferBytes[5] = COMPENSATION_PRELOAD_1;
    returnValue += writeRandomBytes16(IQS7222C_MM_GLOBAL_CYCLE_SETUP, 6, transferBytes, RESTART);
    //NRF_LOG_INFO("\t\t2. Write Global Cycle Settings");

    /* Change the Button Setup 0 - 4 */
    /* Memory Map Position 0x9000 - 0x9502 */
    transferBytes[0] = BUTTON_0_PROX_THRESHOLD;
    transferBytes[1] = BUTTON_0_ENTER_EXIT;
    transferBytes[2] = BUTTON_0_TOUCH_THRESHOLD;
    transferBytes[3] = BUTTON_0_TOUCH_HYSTERESIS;
    transferBytes[4] = BUTTON_0_PROX_EVENT_TIMEOUT;
    transferBytes[5] = BUTTON_0_TOUCH_EVENT_TIMEOUT;
    transferBytes[6] = BUTTON_1_PROX_THRESHOLD;
    transferBytes[7] = BUTTON_1_ENTER_EXIT;
    transferBytes[8] = BUTTON_1_TOUCH_THRESHOLD;
    transferBytes[9] = BUTTON_1_TOUCH_HYSTERESIS;
    transferBytes[10] = BUTTON_1_PROX_EVENT_TIMEOUT;
    transferBytes[11] = BUTTON_1_TOUCH_EVENT_TIMEOUT;
    transferBytes[12] = BUTTON_2_PROX_THRESHOLD;
    transferBytes[13] = BUTTON_2_ENTER_EXIT;
    transferBytes[14] = BUTTON_2_TOUCH_THRESHOLD;
    transferBytes[15] = BUTTON_2_TOUCH_HYSTERESIS;
    transferBytes[16] = BUTTON_2_PROX_EVENT_TIMEOUT;
    transferBytes[17] = BUTTON_2_TOUCH_EVENT_TIMEOUT;
    transferBytes[18] = BUTTON_3_PROX_THRESHOLD;
    transferBytes[19] = BUTTON_3_ENTER_EXIT;
    transferBytes[20] = BUTTON_3_TOUCH_THRESHOLD;
    transferBytes[21] = BUTTON_3_TOUCH_HYSTERESIS;
    transferBytes[22] = BUTTON_3_PROX_EVENT_TIMEOUT;
    transferBytes[23] = BUTTON_3_TOUCH_EVENT_TIMEOUT;
    transferBytes[24] = BUTTON_4_PROX_THRESHOLD;
    transferBytes[25] = BUTTON_4_ENTER_EXIT;
    transferBytes[26] = BUTTON_4_TOUCH_THRESHOLD;
    transferBytes[27] = BUTTON_4_TOUCH_HYSTERESIS;
    transferBytes[28] = BUTTON_4_PROX_EVENT_TIMEOUT;
    transferBytes[29] = BUTTON_4_TOUCH_EVENT_TIMEOUT;
    returnValue += writeRandomBytes16(IQS7222C_MM_BUTTON_SETUP_0, 30, transferBytes, RESTART);
    //NRF_LOG_INFO("\t\t3. Write Button Settings 0 - 4");

    /* Change the Button Setup 5 - 9 */
    /* Memory Map Position 0x9500 - 0x9902 */
    transferBytes[0] = BUTTON_5_PROX_THRESHOLD;
    transferBytes[1] = BUTTON_5_ENTER_EXIT;
    transferBytes[2] = BUTTON_5_TOUCH_THRESHOLD;
    transferBytes[3] = BUTTON_5_TOUCH_HYSTERESIS;
    transferBytes[4] = BUTTON_5_PROX_EVENT_TIMEOUT;
    transferBytes[5] = BUTTON_5_TOUCH_EVENT_TIMEOUT;
    transferBytes[6] = BUTTON_6_PROX_THRESHOLD;
    transferBytes[7] = BUTTON_6_ENTER_EXIT;
    transferBytes[8] = BUTTON_6_TOUCH_THRESHOLD;
    transferBytes[9] = BUTTON_6_TOUCH_HYSTERESIS;
    transferBytes[10] = BUTTON_6_PROX_EVENT_TIMEOUT;
    transferBytes[11] = BUTTON_6_TOUCH_EVENT_TIMEOUT;
    transferBytes[12] = BUTTON_7_PROX_THRESHOLD;
    transferBytes[13] = BUTTON_7_ENTER_EXIT;
    transferBytes[14] = BUTTON_7_TOUCH_THRESHOLD;
    transferBytes[15] = BUTTON_7_TOUCH_HYSTERESIS;
    transferBytes[16] = BUTTON_7_PROX_EVENT_TIMEOUT;
    transferBytes[17] = BUTTON_7_TOUCH_EVENT_TIMEOUT;
    transferBytes[18] = BUTTON_8_PROX_THRESHOLD;
    transferBytes[19] = BUTTON_8_ENTER_EXIT;
    transferBytes[20] = BUTTON_8_TOUCH_THRESHOLD;
    transferBytes[21] = BUTTON_8_TOUCH_HYSTERESIS;
    transferBytes[22] = BUTTON_8_PROX_EVENT_TIMEOUT;
    transferBytes[23] = BUTTON_8_TOUCH_EVENT_TIMEOUT;
    transferBytes[24] = BUTTON_9_PROX_THRESHOLD;
    transferBytes[25] = BUTTON_9_ENTER_EXIT;
    transferBytes[26] = BUTTON_9_TOUCH_THRESHOLD;
    transferBytes[27] = BUTTON_9_TOUCH_HYSTERESIS;
    transferBytes[28] = BUTTON_9_PROX_EVENT_TIMEOUT;
    transferBytes[29] = BUTTON_9_TOUCH_EVENT_TIMEOUT;
    returnValue += writeRandomBytes16(IQS7222C_MM_BUTTON_SETUP_5, 30, transferBytes, RESTART);
    //NRF_LOG_INFO("\t\t4. Write Button Settings 5 - 9");

    /* Change the CH0 Setup */
    /* Memory Map Position 0xA000 - 0xA005 */
    transferBytes[0] = CH0_SETUP_0;
    transferBytes[1] = CH0_SETUP_1;
    transferBytes[2] = CH0_ATI_SETTINGS_0;
    transferBytes[3] = CH0_ATI_SETTINGS_1;
    transferBytes[4] = CH0_MULTIPLIERS_0;
    transferBytes[5] = CH0_MULTIPLIERS_1;
    transferBytes[6] = CH0_ATI_COMPENSATION_0;
    transferBytes[7] = CH0_ATI_COMPENSATION_1;
    transferBytes[8] = CH0_REF_PTR_0;
    transferBytes[9] = CH0_REF_PTR_1;
    transferBytes[10] = CH0_REFMASK_0;
    transferBytes[11] = CH0_REFMASK_1;
    returnValue += writeRandomBytes16(IQS7222C_MM_CHANNEL_SETUP_0, 12, transferBytes, RESTART);
    //NRF_LOG_INFO("\t\t5. Write Channel 0 Settings");

    /* Change the CH1 Setup */
    /* Memory Map Position 0xA100 - 0xA105 */
    transferBytes[0] = CH1_SETUP_0;
    transferBytes[1] = CH1_SETUP_1;
    transferBytes[2] = CH1_ATI_SETTINGS_0;
    transferBytes[3] = CH1_ATI_SETTINGS_1;
    transferBytes[4] = CH1_MULTIPLIERS_0;
    transferBytes[5] = CH1_MULTIPLIERS_1;
    transferBytes[6] = CH1_ATI_COMPENSATION_0;
    transferBytes[7] = CH1_ATI_COMPENSATION_1;
    transferBytes[8] = CH1_REF_PTR_0;
    transferBytes[9] = CH1_REF_PTR_1;
    transferBytes[10] = CH1_REFMASK_0;
    transferBytes[11] = CH1_REFMASK_1;
    returnValue += writeRandomBytes16(IQS7222C_MM_CHANNEL_SETUP_1, 12, transferBytes, RESTART);
    //NRF_LOG_INFO("\t\t6. Write Channel 1 Settings");

    /* Change the CH2 Setup */
    /* Memory Map Position 0xA200 - 0xA205 */
    transferBytes[0] = CH2_SETUP_0;
    transferBytes[1] = CH2_SETUP_1;
    transferBytes[2] = CH2_ATI_SETTINGS_0;
    transferBytes[3] = CH2_ATI_SETTINGS_1;
    transferBytes[4] = CH2_MULTIPLIERS_0;
    transferBytes[5] = CH2_MULTIPLIERS_1;
    transferBytes[6] = CH2_ATI_COMPENSATION_0;
    transferBytes[7] = CH2_ATI_COMPENSATION_1;
    transferBytes[8] = CH2_REF_PTR_0;
    transferBytes[9] = CH2_REF_PTR_1;
    transferBytes[10] = CH2_REFMASK_0;
    transferBytes[11] = CH2_REFMASK_1;
    returnValue += writeRandomBytes16(IQS7222C_MM_CHANNEL_SETUP_2, 12, transferBytes, RESTART);
    //NRF_LOG_INFO("\t\t7. Write Channel 2 Settings");

    /* Change the CH3 Setup */
    /* Memory Map Position 0xA300 - 0xA305 */
    transferBytes[0] = CH3_SETUP_0;
    transferBytes[1] = CH3_SETUP_1;
    transferBytes[2] = CH3_ATI_SETTINGS_0;
    transferBytes[3] = CH3_ATI_SETTINGS_1;
    transferBytes[4] = CH3_MULTIPLIERS_0;
    transferBytes[5] = CH3_MULTIPLIERS_1;
    transferBytes[6] = CH3_ATI_COMPENSATION_0;
    transferBytes[7] = CH3_ATI_COMPENSATION_1;
    transferBytes[8] = CH3_REF_PTR_0;
    transferBytes[9] = CH3_REF_PTR_1;
    transferBytes[10] = CH3_REFMASK_0;
    transferBytes[11] = CH3_REFMASK_1;
    returnValue += writeRandomBytes16(IQS7222C_MM_CHANNEL_SETUP_3, 12, transferBytes, RESTART);
    //NRF_LOG_INFO("\t\t8. Write Channel 3 Settings");

    /* Change the CH4 Setup */
    /* Memory Map Position 0xA400 - 0xA405 */
    transferBytes[0] = CH4_SETUP_0;
    transferBytes[1] = CH4_SETUP_1;
    transferBytes[2] = CH4_ATI_SETTINGS_0;
    transferBytes[3] = CH4_ATI_SETTINGS_1;
    transferBytes[4] = CH4_MULTIPLIERS_0;
    transferBytes[5] = CH4_MULTIPLIERS_1;
    transferBytes[6] = CH4_ATI_COMPENSATION_0;
    transferBytes[7] = CH4_ATI_COMPENSATION_1;
    transferBytes[8] = CH4_REF_PTR_0;
    transferBytes[9] = CH4_REF_PTR_1;
    transferBytes[10] = CH4_REFMASK_0;
    transferBytes[11] = CH4_REFMASK_1;
    returnValue += writeRandomBytes16(IQS7222C_MM_CHANNEL_SETUP_4, 12, transferBytes, RESTART);
    //NRF_LOG_INFO("\t\t9. Write Channel 4 Settings");

    /* Change the CH5 Setup */
    /* Memory Map Position 0xA500 - 0xA505 */
    transferBytes[0] = CH5_SETUP_0;
    transferBytes[1] = CH5_SETUP_1;
    transferBytes[2] = CH5_ATI_SETTINGS_0;
    transferBytes[3] = CH5_ATI_SETTINGS_1;
    transferBytes[4] = CH5_MULTIPLIERS_0;
    transferBytes[5] = CH5_MULTIPLIERS_1;
    transferBytes[6] = CH5_ATI_COMPENSATION_0;
    transferBytes[7] = CH5_ATI_COMPENSATION_1;
    transferBytes[8] = CH5_REF_PTR_0;
    transferBytes[9] = CH5_REF_PTR_1;
    transferBytes[10] = CH5_REFMASK_0;
    transferBytes[11] = CH5_REFMASK_1;
    returnValue += writeRandomBytes16(IQS7222C_MM_CHANNEL_SETUP_5, 12, transferBytes, RESTART);
    //NRF_LOG_INFO("\t\t10. Write Channel 5 Settings");

    /* Change the CH6 Setup */
    /* Memory Map Position 0xA600 - 0xA605 */
    transferBytes[0] = CH6_SETUP_0;
    transferBytes[1] = CH6_SETUP_1;
    transferBytes[2] = CH6_ATI_SETTINGS_0;
    transferBytes[3] = CH6_ATI_SETTINGS_1;
    transferBytes[4] = CH6_MULTIPLIERS_0;
    transferBytes[5] = CH6_MULTIPLIERS_1;
    transferBytes[6] = CH6_ATI_COMPENSATION_0;
    transferBytes[7] = CH6_ATI_COMPENSATION_1;
    transferBytes[8] = CH6_REF_PTR_0;
    transferBytes[9] = CH6_REF_PTR_1;
    transferBytes[10] = CH6_REFMASK_0;
    transferBytes[11] = CH6_REFMASK_1;
    returnValue += writeRandomBytes16(IQS7222C_MM_CHANNEL_SETUP_6, 12, transferBytes, RESTART);
    //NRF_LOG_INFO("\t\t11. Write Channel 6 Settings");

    /* Change the CH7 Setup */
    /* Memory Map Position 0xA700 - 0xA705 */
    transferBytes[0] = CH7_SETUP_0;
    transferBytes[1] = CH7_SETUP_1;
    transferBytes[2] = CH7_ATI_SETTINGS_0;
    transferBytes[3] = CH7_ATI_SETTINGS_1;
    transferBytes[4] = CH7_MULTIPLIERS_0;
    transferBytes[5] = CH7_MULTIPLIERS_1;
    transferBytes[6] = CH7_ATI_COMPENSATION_0;
    transferBytes[7] = CH7_ATI_COMPENSATION_1;
    transferBytes[8] = CH7_REF_PTR_0;
    transferBytes[9] = CH7_REF_PTR_1;
    transferBytes[10] = CH7_REFMASK_0;
    transferBytes[11] = CH7_REFMASK_1;
    returnValue += writeRandomBytes16(IQS7222C_MM_CHANNEL_SETUP_7, 12, transferBytes, RESTART);
    //NRF_LOG_INFO("\t\t12. Write Channel 7 Settings");

    /* Change the CH8 Setup */
    /* Memory Map Position 0xA800 - 0xA805 */
    transferBytes[0] = CH8_SETUP_0;
    transferBytes[1] = CH8_SETUP_1;
    transferBytes[2] = CH8_ATI_SETTINGS_0;
    transferBytes[3] = CH8_ATI_SETTINGS_1;
    transferBytes[4] = CH8_MULTIPLIERS_0;
    transferBytes[5] = CH8_MULTIPLIERS_1;
    transferBytes[6] = CH8_ATI_COMPENSATION_0;
    transferBytes[7] = CH8_ATI_COMPENSATION_1;
    transferBytes[8] = CH8_REF_PTR_0;
    transferBytes[9] = CH8_REF_PTR_1;
    transferBytes[10] = CH8_REFMASK_0;
    transferBytes[11] = CH8_REFMASK_1;
    returnValue += writeRandomBytes16(IQS7222C_MM_CHANNEL_SETUP_8, 12, transferBytes, RESTART);
    //NRF_LOG_INFO("\t\t13. Write Channel 8 Settings");

    /* Change the CH9 Setup */
    /* Memory Map Position 0xA900 - 0xA905 */
    transferBytes[0] = CH9_SETUP_0;
    transferBytes[1] = CH9_SETUP_1;
    transferBytes[2] = CH9_ATI_SETTINGS_0;
    transferBytes[3] = CH9_ATI_SETTINGS_1;
    transferBytes[4] = CH9_MULTIPLIERS_0;
    transferBytes[5] = CH9_MULTIPLIERS_1;
    transferBytes[6] = CH9_ATI_COMPENSATION_0;
    transferBytes[7] = CH9_ATI_COMPENSATION_1;
    transferBytes[8] = CH9_REF_PTR_0;
    transferBytes[9] = CH9_REF_PTR_1;
    transferBytes[10] = CH9_REFMASK_0;
    transferBytes[11] = CH9_REFMASK_1;
    returnValue += writeRandomBytes16(IQS7222C_MM_CHANNEL_SETUP_9, 12, transferBytes, RESTART);
    //NRF_LOG_INFO("\t\t14. Write Channel 9 Settings");

    /* Change the Filter Betas */
    /* Memory Map Position 0xAA00 - 0xAA01 */
    transferBytes[0] = COUNTS_BETA_FILTER;
    transferBytes[1] = LTA_BETA_FILTER;
    transferBytes[2] = LTA_FAST_BETA_FILTER;
    transferBytes[3] = RESERVED_FILTER_0;
    returnValue += writeRandomBytes16(IQS7222C_MM_FILTER_BETAS, 4, transferBytes, RESTART);
    //NRF_LOG_INFO("\t\t15. Write Filter Betas");

    /* Change the Slider/Wheel 0 Setup 0 & Delta Link */
    /* Memory Map Position 0xB000 - 0xB009 */
    transferBytes[0] = SLIDER0SETUP_GENERAL;
    transferBytes[1] = SLIDER0_LOWER_CAL;
    transferBytes[2] = SLIDER0_UPPER_CAL;
    transferBytes[3] = SLIDER0_BOTTOM_SPEED;
    transferBytes[4] = SLIDER0_TOPSPEED_0;
    transferBytes[5] = SLIDER0_TOPSPEED_1;
    transferBytes[6] = SLIDER0_RESOLUTION_0;
    transferBytes[7] = SLIDER0_RESOLUTION_1;
    transferBytes[8] = SLIDER0_ENABLE_MASK_0_7;
    transferBytes[9] = SLIDER0_ENABLE_MASK_8_9;
    transferBytes[10] = SLIDER0_ENABLESTATUSLINK_0;
    transferBytes[11] = SLIDER0_ENABLESTATUSLINK_1;
    transferBytes[12] = SLIDER0_DELTA0_0;
    transferBytes[13] = SLIDER0_DELTA0_1;
    transferBytes[14] = SLIDER0_DELTA1_0;
    transferBytes[15] = SLIDER0_DELTA1_1;
    transferBytes[16] = SLIDER0_DELTA2_0;
    transferBytes[17] = SLIDER0_DELTA2_1;
    transferBytes[18] = SLIDER0_DELTA3_0;
    transferBytes[19] = SLIDER0_DELTA3_1;
    returnValue += writeRandomBytes16(IQS7222C_MM_SLIDER_SETUP_0, 20, transferBytes, RESTART);
    //NRF_LOG_INFO("\t\t16. Slider/Wheel 0 Settings");

    /* Change the Slider/Wheel 1 Setup 0 */
    /* Memory Map Position 0xB100 - 0xB105 */
    transferBytes[0] = SLIDER1SETUP_GENERAL;
    transferBytes[1] = SLIDER1_LOWER_CAL;
    transferBytes[2] = SLIDER1_UPPER_CAL;
    transferBytes[3] = SLIDER1_BOTTOM_SPEED;
    transferBytes[4] = SLIDER1_TOPSPEED_0;
    transferBytes[5] = SLIDER1_TOPSPEED_1;
    transferBytes[6] = SLIDER1_RESOLUTION_0;
    transferBytes[7] = SLIDER1_RESOLUTION_1;
    transferBytes[8] = SLIDER1_ENABLE_MASK_0_7;
    transferBytes[9] = SLIDER1_ENABLE_MASK_8_9;
    transferBytes[10] = SLIDER1_ENABLESTATUSLINK_0;
    transferBytes[11] = SLIDER1_ENABLESTATUSLINK_1;
    transferBytes[12] = SLIDER1_DELTA0_0;
    transferBytes[13] = SLIDER1_DELTA0_1;
    transferBytes[14] = SLIDER1_DELTA1_0;
    transferBytes[15] = SLIDER1_DELTA1_1;
    transferBytes[16] = SLIDER1_DELTA2_0;
    transferBytes[17] = SLIDER1_DELTA2_1;
    transferBytes[18] = SLIDER1_DELTA3_0;
    transferBytes[19] = SLIDER1_DELTA3_1;
    returnValue += writeRandomBytes16(IQS7222C_MM_SLIDER_SETUP_1, 20, transferBytes, RESTART);
    //NRF_LOG_INFO("\t\t17. Slider/Wheel 1 Settings");

    /* Change the GPIO Settings */
    /* Memory Map Position 0xC000 - 0xC202 */
    transferBytes[0] = GPIO0_SETUP_0;
    transferBytes[1] = GPIO0_SETUP_1;
#if IQS7222C_v1_13
    transferBytes[2] = ENABLE_MASK_0_7;
    transferBytes[3] = ENABLE_MASK_8_9;
    transferBytes[4] = ENABLESTATUSLINK_0;
    transferBytes[5] = ENABLESTATUSLINK_1;
    uint8_t m_bytes = 6;
#elif IQS7222C_v2_6 || IQS7222C_v2_23
    transferBytes[2] = GPIO0_ENABLE_MASK_0_7;
    transferBytes[3] = GPIO0_ENABLE_MASK_8_9;
    transferBytes[4] = GPIO0_ENABLESTATUSLINK_0;
    transferBytes[5] = GPIO0_ENABLESTATUSLINK_1;
    transferBytes[6] = GPIO1_SETUP_0;
    transferBytes[7] = GPIO1_SETUP_1;
    transferBytes[8] = GPIO1_ENABLE_MASK_0_7;
    transferBytes[9] = GPIO1_ENABLE_MASK_8_9;
    transferBytes[10] = GPIO1_ENABLESTATUSLINK_0;
    transferBytes[11] = GPIO1_ENABLESTATUSLINK_1;
    transferBytes[12] = GPIO2_SETUP_0;
    transferBytes[13] = GPIO2_SETUP_1;
    transferBytes[14] = GPIO2_ENABLE_MASK_0_7;
    transferBytes[15] = GPIO2_ENABLE_MASK_8_9;
    transferBytes[16] = GPIO2_ENABLESTATUSLINK_0;
    transferBytes[17] = GPIO2_ENABLESTATUSLINK_1;
    uint8_t m_bytes = 18;
#endif
    returnValue += writeRandomBytes16(IQS7222C_MM_GPIO_0_SETTINGS, m_bytes, transferBytes,
                                      RESTART);
    //NRF_LOG_INFO("\t\t18. GPIO 0 Settings");

    /* Change the System Settings */
    /* Memory Map Position 0xD0 - 0xD9 */
    transferBytes[0] = SYSTEM_CONTROL_0;
    transferBytes[1] = SYSTEM_CONTROL_1;
    transferBytes[2] = ATI_ERROR_TIMEOUT_0;
    transferBytes[3] = ATI_ERROR_TIMEOUT_1;
    transferBytes[4] = ATI_REPORT_RATE_0;
    transferBytes[5] = ATI_REPORT_RATE_1;
    transferBytes[6] = NORMAL_MODE_TIMEOUT_0;
    transferBytes[7] = NORMAL_MODE_TIMEOUT_1;
    transferBytes[8] = NORMAL_MODE_REPORT_RATE_0;
    transferBytes[9] = NORMAL_MODE_REPORT_RATE_1;
    transferBytes[10] = LP_MODE_TIMEOUT_0;
    transferBytes[11] = LP_MODE_TIMEOUT_1;
    transferBytes[12] = LP_MODE_REPORT_RATE_0;
    transferBytes[13] = LP_MODE_REPORT_RATE_1;
    transferBytes[14] = ULP_MODE_TIMEOUT_0;
    transferBytes[15] = ULP_MODE_TIMEOUT_1;
    transferBytes[16] = ULP_MODE_REPORT_RATE_0;
    transferBytes[17] = ULP_MODE_REPORT_RATE_1;
    transferBytes[18] = TOUCH_PROX_EVENT_MASK;
    transferBytes[19] = POWER_ATI_EVENT_MASK;
    transferBytes[20] = I2CCOMMS_0;
    returnValue += writeRandomBytes(IQS7222C_MM_CONTROL_SETTINGS, 21, transferBytes,
                                    RESTART);
    //NRF_LOG_INFO("\t\t19. System Settings");

/* Change the GPIO Override */
/* Memory Map Position 0xDB - 0xDB */
#if IQS7222C_v2_6 || IQS7222C_v2_23
    transferBytes[0] = GPIO_OVERRIDE;
    returnValue += writeRandomBytes(IQS7222C_MM_GPIO_OVERRIDE, 1, transferBytes, stopOrRestart);
    //NRF_LOG_INFO("\t\t20. GPIO Override");
#endif

/* Change the Comms timeout setting */
/* Memory Map Position 0xDC - 0xDC */
#if IQS7222C_v2_23
    transferBytes[0] = COMMS_TIMEOUT_0;
    transferBytes[1] = COMMS_TIMEOUT_1;
    returnValue += writeRandomBytes(IQS7222C_MM_COMMS_TIMEOUT, 2, transferBytes, stopOrRestart);
    //NRF_LOG_INFO("\t\t21. Communication Timeout");
#endif
    return returnValue;
}

// /**************************************************************************************************************/
// /*                                              PRIVATE METHODS */
// /**************************************************************************************************************/

/**
 * @name    readRandomBytes
 * @brief   A methods which reads a specified number of bytes from a specified
 * address and saves it into a user supplied array. This method is used by all
 * other methods in this class which read data drom the IQS7222C device.
 * @param   memoryAddress -> The memory address at which to start reading bytes
 * from.  See the "iqs7222c_addresses.h" file. numBytes      -> The number of
 * bytes that must be read. bytesArray    -> The array which will store the
 * bytes to be read, this array will be overwritten. stopOrRestart -> A boolean
 * which specifies whether the communication window should remain open or be
 * closed after transfer. False keeps it open, true closes it. Use the STOP and
 * RESTART definitions.
 * @retval  No value is returend, however, the user supplied array is
 * overwritten.
 * @notes   Uses standard arduino "Wire" library which is for I2C communication.
 *          Take note that C++ cannot return an array, therefore, the array
 * which is passed as an argument is overwritten with the required values. Pass
 * an array to the method by using only its name, e.g. "bytesArray", without the
 * brackets, this basically passes a pointer to the array.
 */
int readRandomBytes(uint8_t memoryAddress, uint8_t numBytes,
                    uint8_t bytesArray[], bool stopOrRestart)
{
    return (i2c_touch_read_register(_deviceAddress, memoryAddress, numBytes, &bytesArray[0], stopOrRestart));
}

/**
 * @name   writeRandomBytes
 * @brief  A mthod which writes a specified number of bytes to a specified
 * address, the bytes to write are supplied by means of an array pointer. This
 * method is used by the all other methods of this class which write data to the
 * IQS7222C device.
 * @param  memoryAddress -> The memory address at which to start writing the
 * bytes to. See the "iqs7222c_addresses.h" file. numBytes      -> The number of
 * bytes that must be written. bytesArray    -> The array which stores the bytes
 * which will be written to the memory location. stopOrRestart -> A boolean
 * which sepcifies whether the communication window should remain open or be
 * closed of transfer. False keeps it open, true closes it. Use the STOP and
 * RESTART definitions.
 * @retval No value is returend, only the IQS device registers are altered.
 * @notes  Uses standard arduino "Wire" library which is for I2C communication.
 *         Take note that a full array cannot be passed to a function in C++.
 *         Pass an array to the function by using only its name, e.g.
 * "bytesArray", without the square brackets, this basically passes a pointer to
 * the array. The values to be written must be loaded into the array prior to
 * passing it to the function.
 */
int writeRandomBytes(uint8_t memoryAddress, uint8_t numBytes,
                     uint8_t bytesArray[], bool stopOrRestart)
{
    return (i2c_touch_write_register(_deviceAddress, memoryAddress, numBytes, bytesArray, stopOrRestart));
}

/**
 * @name   writeRandomBytes
 * @brief  A mthod which writes a specified number of bytes to a specified
 * address, the bytes to write are supplied by means of an array pointer. This
 * method is used by the all other methods of this class which write data to the
 * IQS7222C device.
 * @param  memoryAddress -> The memory address at which to start writing the
 * bytes to. See the "iqs7222c_addresses.h" file. numBytes      -> The number of
 * bytes that must be written. bytesArray    -> The array which stores the bytes
 * which will be written to the memory location. stopOrRestart -> A boolean
 * which sepcifies whether the communication window should remain open or be
 * closed of transfer. False keeps it open, true closes it. Use the STOP and
 * RESTART definitions.
 * @retval No value is returend, only the IQS device registers are altered.
 * @notes  Uses standard arduino "Wire" library which is for I2C communication.
 *         Take note that a full array cannot be passed to a function in C++.
 *         Pass an array to the function by using only its name, e.g.
 * "bytesArray", without the square brackets, this basically passes a pointer to
 * the array. The values to be written must be loaded into the array prior to
 * passing it to the function.
 */
int writeRandomBytes16(uint16_t memoryAddress, uint8_t numBytes,
                       uint8_t bytesArray[], bool stopOrRestart)
{
    return (i2c_touch_write_register_16(_deviceAddress, memoryAddress, numBytes, bytesArray, stopOrRestart));
}

/**
  * @name   force_I2C_communication
  * @brief  A method which writes data 0x00 to memory address 0xFF to open a
  communication window on the IQS7222C.
  * @param  None
  * @retval None
  * @notes  Uses standard arduino "Wire" library which is for I2C communication.

  */
void iqs7222c_force_I2C_communication(void)
{
    uint8_t force_comm_byte[1] = {0xFF};
    i2c_touch_write_register(_deviceAddress, 0x00, 1, &force_comm_byte[0], STOP);
}

/**
 * @brief Get touch state byte. Only LSB, Channels [0-8]
 * 
 * @return uint8_t representation of touch detected on channels [0-8]
 */
uint8_t iqs7222c_getTouchStateByte(void)
{
    return IQSMemoryMap.iqs7222c_touch_event_states.iqs7222c_touch_event_states_lsb;
}

bool iqs7222c_isNewDataAvailable(void)
{
    return new_data_available;
}