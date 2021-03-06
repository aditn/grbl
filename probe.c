/*
  probe.c - code pertaining to probing methods
  Part of Grbl

  Copyright (c) 2014 Sungeun K. Jeon, Adam Shelly

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/
  
#include "system.h"
#include "probe.h"
#include "counters.h"



// Probe pin initialization routine.
void probe_init() 
{
  PROBE_DDR &= ~(PROBE_MASK); // Configure as input pins
  PROBE_PORT |= PROBE_MASK;   // Enable internal pull-up resistors. Normal high operation.
}


// Monitors probe pin state and records the system position when detected. Called by the
// stepper ISR per ISR tick.
// NOTE: This function must be extremely efficient as to not bog down the stepper ISR.
void probe_state_monitor()
{
  uint8_t probe_on = probe_get_state();
  if (sysflags.probe_state == PROBE_ACTIVE && probe_on) {
    sysflags.probe_state = PROBE_OFF;
    memcpy(sys.probe_position, sys.position, sizeof(float)*N_AXIS);
    SYS_EXEC |= EXEC_FEED_HOLD;
  }
  if (ESTOP_PIN & ESTOP_MASK) {
    sys.alarm |= ALARM_ESTOP;
    SYS_EXEC |= (EXEC_FEED_HOLD|EXEC_ALARM|EXEC_CRIT_EVENT);
  }
}




