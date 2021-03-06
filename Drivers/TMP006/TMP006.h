/**
 * @brief       TMP006.h
 * @details     Infrared Thermopile Sensor in Chip-Scale Package.
 *              Header file.
 *
 *
 * @return      N/A
 *
 * @author      Manuel Caballero
 * @date        7/December/2018
 * @version     7/December/2018    The ORIGIN
 * @pre         N/A.
 * @warning     N/A
 * @pre         This code belongs to Nimbus Centre ( http://www.nimbus.cit.ie ). All rights reserved.
 */


#include "stdint.h"
#include "stdbool.h"
#include <math.h>
#include "i2c.h"


#ifndef TMP006_H_
#define TMP006_H_

#ifdef __cplusplus
extern "C" {
#endif



/**
  * @brief   DEFAULT ADDRESSES
  */
typedef enum
{
    TMP006_ADDRESS_ADR1_0_ADR0_0     =   0b1000000,   /*!<   TMP006 I2C Address: ADR1 = 0 | ADR0 = 0             */
    TMP006_ADDRESS_ADR1_0_ADR0_1     =   0b1000001,   /*!<   TMP006 I2C Address: ADR1 = 0 | ADR0 = 1             */
    TMP006_ADDRESS_ADR1_0_ADR0_SDA   =   0b1000010,   /*!<   TMP006 I2C Address: ADR1 = 0 | ADR0 = SDA           */
    TMP006_ADDRESS_ADR1_0_ADR0_SCL   =   0b1000011,   /*!<   TMP006 I2C Address: ADR1 = 0 | ADR0 = SCL           */
    TMP006_ADDRESS_ADR1_1_ADR0_0     =   0b1000100,   /*!<   TMP006 I2C Address: ADR1 = 1 | ADR0 = 0             */
    TMP006_ADDRESS_ADR1_1_ADR0_1     =   0b1000101,   /*!<   TMP006 I2C Address: ADR1 = 1 | ADR0 = 1             */
    TMP006_ADDRESS_ADR1_1_ADR0_SDA   =   0b1000110,   /*!<   TMP006 I2C Address: ADR1 = 1 | ADR0 = SDA           */
    TMP006_ADDRESS_ADR1_1_ADR0_SCL   =   0b1000111    /*!<   TMP006 I2C Address: ADR1 = 1 | ADR0 = SCL           */
} TMP006_addresses_t;



/**
  * @brief   REGISTER MAP
  */
typedef enum
{
    TMP006_SENSOR_VOLTAGE     =   0x00,               /*!<  Sensor voltage      ( Read Only  )                    */
    TMP006_LOCAL_TEMPERATURE  =   0x01,               /*!<  Local temperature   ( Read Only )                     */
    TMP006_CONFIGURATION      =   0x02,               /*!<  Configuration       ( Read/Write )                    */
    TMP006_MANUFACTURER_ID    =   0xFE,               /*!<  Manufacturer ID     ( Read Only )                     */
    TMP006_DEVICE_ID          =   0xFF                /*!<  Device ID           ( Read Only )                     */
} TMP006_register_map_t;



/**
  * @brief   CONFIGURATION REGISTER
  */
/* SOFTWARE RESET BIT <15>
*/
typedef enum
{
    RST_BIT_MASK                      =   ( 1U << 15U ),      /*!<  Software reset bit mask                                                 */
    RST_NORMAL_OPERATION              =   ( 0U << 15U ),      /*!<  Software reset bit: Normal operation                          ( RESET ) */
    RST_SOFTWARE_RESET                =   ( 1U << 15U )       /*!<  Software reset bit: Software reset                                      */
} TMP006_rst_t;


/* MODE OF OPERATION <14:12>
*/
typedef enum
{
    MOD_MASK                           =   ( 0b111 << 12U ),   /*!<  Mode of operation mask                                                  */
    MOD_POWER_DOWN                     =   ( 0b000 << 12U ),   /*!<  Mode of operation: Power-down                                           */
    MOD_SENSOR_AND_DIE_CONT_CONVERSION =   ( 0b111 << 12U )    /*!<  Mode of operation: Sensor and die continuous conversion (MOD) ( RESET ) */
} TMP006_mod_t;


/* ADC CONVERSION RATE <11:9>
*/
typedef enum
{
    CR_MASK                           =   ( 0b111 << 9U ),    /*!<  ADC Conversion rate mask                                                */
    CR_1_AVERAGED_SAMPLES             =   ( 0b000 << 9U ),    /*!<  ADC Conversion rate:  1 number of averaged sample                       */
    CR_2_AVERAGED_SAMPLES             =   ( 0b001 << 9U ),    /*!<  ADC Conversion rate:  2 number of averaged sample                       */
    CR_4_AVERAGED_SAMPLES             =   ( 0b010 << 9U ),    /*!<  ADC Conversion rate:  4 number of averaged sample             ( RESET ) */
    CR_8_AVERAGED_SAMPLES             =   ( 0b011 << 9U ),    /*!<  ADC Conversion rate:  8 number of averaged sample                       */
    CR_16_AVERAGED_SAMPLES            =   ( 0b100 << 9U )     /*!<  ADC Conversion rate: 16 number of averaged sample                       */
} TMP006_cr_t;


/* #DRDY ENABLE BIT <8>
*/
typedef enum
{
    EN_MASK                           =   ( 1U << 8U ),       /*!<  #DRDY enable bit mask                                                   */
    EN_nDRDY_PIN_DISABLED             =   ( 0U << 8U ),       /*!<  #DRDY enable bit: Pin disabled                                ( RESET ) */
    EN_nDRDY_PIN_ENABLED              =   ( 1U << 8U )        /*!<  #DRDY enable bit: Pin enabled                                           */
} TMP006_en_t;


/* DATA READY BIT <7>
*/
typedef enum
{
    nDRDY_MASK                        =   ( 1U << 7U ),       /*!<  Data ready bit mask                                                     */
    nDRDY_CONVERSION_IN_PROGRESS      =   ( 0U << 7U ),       /*!<  Data ready bit: Conversion in progress                        ( RESET ) */
    nDRDY_CONVERSION_COMPLETED        =   ( 1U << 7U )        /*!<  Data ready bit: Conversion completed                                    */
} TMP006_ndrdy_t;




/**
  * @brief   MANUFACTURER ID REGISTER
  */
typedef enum
{
    MANUFACTURER_ID_MASK              =   0xFFFF,             /*!<  Manufacturer ID mask                                                    */
    MANUFACTURER_ID_VALUE             =   0x5449              /*!<  Manufacturer ID value                                         ( RESET ) */
} TMP006_manufacturer_id_register_t;



/**
  * @brief   DEVICE ID REGISTER
  */
typedef enum
{
    DEVICE_ID_MASK                    =   0xFFFF,             /*!<  Device ID mask                                                          */
    DEVICE_ID_VALUE                   =   0x0067              /*!<  Device ID value                                               ( RESET ) */
} TMP006_device_id_register_t;




/**
  * @brief   CONSTANS TO BE USED IN THE FORMULAS
  *           NOTE: User Guide ( sbou107.pdf ) 5.1 Equations for Calculating Target Object Temperatures, p10.
  */
#define A1        0.00175                                         /*!<  A1                                                                */
#define A2        -0.00001678                                     /*!<  A2                                                                */
#define T_REF     298.15                                          /*!<  T_REF, Kelvin                                                     */
#define B0        -0.0000294                                      /*!<  B0                                                                */
#define B1        -0.00000057                                     /*!<  B1                                                                */
#define B2        -0.00000000463                                  /*!<  B2                                                                */
#define C2        13.4                                            /*!<  C2                                                                */
#define S0        ( 0.00000000000005 + 0.00000000000007 ) / 2.0   /*!<  Primary calibration sensitivity factor ( mean of typical values ) */
#define TEMP_1LSB 0.03125                                         /*!<  Temperature: 1 LSB = 1 / 32�C = 0.03125                           */
#define SVOL_1LSB 156.25                                          /*!<  Sensor voltage: 1 LSB = 156.25 nV                                 */



#ifndef TMP006_VECTOR_STRUCT_H
#define TMP006_VECTOR_STRUCT_H
typedef struct
{
    float    ObjectTemperatureC;             /*!<  Temperature of the target object in Celsius degrees                              */
    float    ObjectTemperatureK;             /*!<  Temperature of the target object in Kelvins degrees                              */

    float    TemperatureK;                  /*!<  T_DIE in Kelvins degrees                                                          */
    float    TemperatureC;                  /*!<  T_DIE in Celsius degrees                                                          */
    float    V_Sensor;                      /*!<  Sensor voltage result                                                             */
    float    s0;                            /*!<  Primary calibration sensitivity factor ( typical values: 5�10^�14 and 7�10^�14 )  */

    uint16_t SensorVoltageResultRegister;   /*!<  V_sensor                                                                          */
    uint16_t TemperatureRegister;           /*!<  T_DIE                                                                             */
    uint16_t ConfigurationRegister;         /*!<  Configuration register                                                            */
    
    uint16_t ManufacturerID;                /*!<  Manufacturer ID                                                                   */
    uint16_t DeviceID;                      /*!<  Device ID                                                                         */
} TMP006_data_t;
#endif





/**
  * @brief   INTERNAL CONSTANTS
  */
typedef enum
{
    TMP006_SUCCESS     =       0,
    TMP006_FAILURE     =       1
} TMP006_status_t;




/**
  * @brief   FUNCTION PROTOTYPES
  */
/** It configures the I2C peripheral.
  */
TMP006_status_t  TMP006_Init                      ( I2C_parameters_t myI2Cparameters                                  );

/** It gets the manufacturer ID.
  */
TMP006_status_t  TMP006_GetManufacturerID         ( I2C_parameters_t myI2Cparameters, TMP006_data_t* myManufacturerID );

/** It gets the device ID.
  */
TMP006_status_t  TMP006_GetDeviceID               ( I2C_parameters_t myI2Cparameters, TMP006_data_t* myDeviceID       );

/** It reads the configuration register.
  */
TMP006_status_t  TMP006_ReadConfigurationRegister ( I2C_parameters_t myI2Cparameters, TMP006_data_t* myConfReg        );

/** It performs a software reset.
  */
TMP006_status_t  TMP006_SoftwareReset             ( I2C_parameters_t myI2Cparameters                                  );

/** It sets mode of operation.
  */
TMP006_status_t  TMP006_SetModeOperation          ( I2C_parameters_t myI2Cparameters, TMP006_mod_t myModeOpreation    );

/** It sets conversion rate.
  */
TMP006_status_t  TMP006_SetConversionRate         ( I2C_parameters_t myI2Cparameters, TMP006_cr_t myConversionRate    );

/** It sets #DRDY control.
  */
TMP006_status_t  TMP006_SetnDRDY_EnableBit        ( I2C_parameters_t myI2Cparameters, TMP006_en_t myEnableBit         );

/** It reads raw temperature ( T_DIE ) register.
  */
TMP006_status_t  TMP006_GetRawTemperature         ( I2C_parameters_t myI2Cparameters, TMP006_data_t* myRawTemperature );

/** It reads raw sensor voltage result ( V_SENSOR ) register.
  */
TMP006_status_t  TMP006_GetRawSensorVoltage       ( I2C_parameters_t myI2Cparameters, TMP006_data_t* myRawVoltage     );

/** It calculates the real temperature ( T_DIE ) value.
  */
TMP006_status_t  TMP006_CalculateTemperature      ( TMP006_data_t* myTemperature                                      );

/** It calculates the real sensor voltage ( V_SENSOR ) value.
  */
TMP006_status_t  TMP006_CalculateSensorVoltage    ( TMP006_data_t* myV_sensor                                         );

/** It calculates the object temperature ( T_OBJ ) value.
  */
TMP006_status_t  TMP006_CalculateObjectTemperature( TMP006_data_t* myObjTemperature                                   );



#ifdef __cplusplus
}
#endif

#endif /* TMP006_H */
