/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/
#define _XTAL_FREQ 4000000

__EEPROM_DATA(125,125,125,125,0,60,0,0); // time correction for ventils, Irrigation disable flag

#define TIME_UNIT 60             // time in seconds in one unit
#define DEFAULT_DURATION 5      // in time units for each module

#define VENTIL_1    RB0         // ventil control pin definition
#define VENTIL_2    RB3
#define VENTIL_3    RB4
#define VENTIL_4    RB5

#define INFO_LED RA3
#define DAY_NIGHT_SWITCH_DELAY 60  //in seconds
#define MAX_VENTIL_OPEN_TIME 1800  //in seconds

#define RESET_WIFI_STATE TRISA7    // Reset Out Pin for ESP8266 
#define RESET_WIFI_PIN RA7 

/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */
