#ifndef TIMERS_H_
#define TIMERS_H_

/****************************************************************************/
/***        Include Files                                                 ***/
/****************************************************************************/
#include <jendefs.h>
#include <AppHardwareApi_JN5168.h>
#include "Config.h"
#include "robot_functions.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define SLEEP_TIMEOUT	25000

/* Timer data */
#define ROBOT_TICK_FREQUENCY_HZ			100UL//100UL
#define ROBOT_TICK_PERIOD_MS         	1000UL / ROBOT_TICK_FREQUENCY_HZ	/**< Timer period */
#define ROBOT_TICK_PERIOD_COUNT     	(16000UL * ROBOT_TICK_PERIOD_MS)


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void vRobotInitTickTimer(void);
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/


#endif /* TIMERS_H_ */
