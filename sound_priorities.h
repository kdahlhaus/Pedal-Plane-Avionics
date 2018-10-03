/*
    Priorities of the differnt types of sounds in the system.
    Multiple sounds will be mixed when possible, but if too many
    sounds are played at once, sounds with a higher numeric priority
    may bump lower priority sounds.
*/

#ifndef _SOUND_PRIORITIES_H_
#define _SOUND_PRIORITIES_H_

#define STARTUP_PRIORITY 0
#define MOTOR_SOUND_PRIORITY 3
#define BOMB_DROP_PRIORITY 6
#define MACHINEGUN_PRIORITY 9
#define ZOOM_PRIORITY 10

#endif
