/* 
 * File:   state.h
 * Author: user
 *
 * Created on 10 ?????? 2018, 0:09
 */

#ifndef STATE_H
#define	STATE_H

#ifdef	__cplusplus
extern "C" {
#endif

struct States {                     // state flags structure
    unsigned char night_is_on;
    unsigned char irrigation_disabled;
    
    int v1_time;
    int v2_time;
    int v3_time;
    int v4_time;
    
} state;  


#ifdef	__cplusplus
}
#endif

#endif	/* STATE_H */

