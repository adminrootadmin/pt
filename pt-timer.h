/*
 * Copyright (c) 2004, Swedish Institute of Computer Science.
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 3. Neither the name of the Institute nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE. 
 *
 * This file is part of the protothreads library.
 * 
 * Author: cking
 *
 * $Id: pt-timer.h,v 1.0 2022/08/24 18:06:59 cking Exp $
 */

/**
 * \addtogroup pt
 * @{
 */

/**
 * \defgroup pttimer Protothread timer
 * @{
 *
 * This module implements timer on top of protothreads.
 *
 * The following example shows how to delay.
 *
 \code
#include "pt-timer.h"

PT_THREAD(producer(struct pt *pt))
{
  static struct pt_timer timer;
  
  PT_BEGIN(pt);
  
  PT_TIMER_DELAY(pt, &timer, 1000);
  
  // do something...

  PT_END(pt);
}

 \endcode
 */
   
/**
 * \file
 * delay implemented on protothreads
 * \author
 * cking
 *
 */

#ifndef __PT_TIMER_H__
#define __PT_TIMER_H__

#include "pt.h"

#define PT_TIMER_TYPE unsigned int
    
#define PT_VOLATILE volatile

/**
 * Defining global variable with type "PT_VOLATILE PT_TIMER_TYPE",
 * and PT_TIMER_TICK++ in circles.
 */
#define PT_TIMER_TICK timer_ms

extern PT_VOLATILE PT_TIMER_TYPE PT_TIMER_TICK;


struct pt_timer {
  PT_TIMER_TYPE timer;
};

/**
 * Initialize a timer
 *
 * This macro initializes a timer with a value for the
 * counter. Internally, the timer use the PT_TIMER_TYPE to
 * represent the timer, and therefore the "timer" argument should be
 * within range of the PT_TIMER_TYPE.
 *
 * \param t (struct pt_timer *) A pointer to the pt_timer struct
 * representing the timer
 */
#define PT_TIMER_INIT(t) (t)->timer = PT_TIMER_TICK

/**
 * Check time out
 *
 * If time out,return true.
 *
 * \param record_time (PT_TIMER_TYPE) record time
 * \param setting_time (PT_TIMER_TYPE) setting time
 */
#define PT_TIMER_CHECK(record_time, setting_time) ((PT_TIMER_TYPE)(PT_TIMER_TICK - (PT_TIMER_TYPE)(record_time)) >= (PT_TIMER_TYPE)(setting_time))

/**
 * Wait for time out
 *
 * This macro carries out the "wait" operation on the timer. The
 * wait operation causes the protothread to block while time out.
 *
 * \param pt (struct pt *) A pointer to the protothread (struct pt) in
 * which the operation is executed.
 * \param t (struct pt_timer *) A pointer to the pt_timer struct
 * representing the timer
 * \param time (PT_TIMER_TYPE) delay time
 * \hideinitializer
 */
#define PT_TIMER_DELAY(pt, t, time)	\
  do {					\
    PT_TIMER_INIT(t);					\
    PT_WAIT_UNTIL(pt, PT_TIMER_CHECK((t)->timer, time));		\
  } while(0)

/**
 * Wait for time out
 *
 * This macro carries out the "wait" operation on the timer. The
 * wait operation causes the protothread to block while time out.
 *
 * \param pt (struct pt *) A pointer to the protothread (struct pt) in
 * which the operation is executed.
 * \param cond The condition.
 * \param t (struct pt_timer *) A pointer to the pt_timer struct
 * representing the timer
 * \param time (PT_TIMER_TYPE) delay time
 * \hideinitializer
 */
#define PT_TIMER_WAIT(pt, condition, t, time)	\
  do {					\
    PT_TIMER_INIT(t);					\
    PT_WAIT_UNTIL(pt, (condition) || PT_TIMER_CHECK((t)->timer, time));		\
  } while(0)

#endif /* __PT_TIMER_H__ */

/** @} */
/** @} */
   
