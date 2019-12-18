// Copyright 2019 Michael Rodriguez
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
// SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
// OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
// CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#pragma once

#include <stdbool.h>
#include <stdint.h>

struct libps_timer_spec
{
    uint16_t value;
    uint16_t mode;
    uint16_t target;
};

struct libps_timer
{
    struct timer_spec
    {
        uint16_t value;
        uint16_t mode;
        uint16_t target;
    } timers[3];

    bool vblank_or_hblank_occurred;
    bool fire_interrupt;
};

// Allocates memory for a `libps_timer` structure and returns a pointer to it
// if memory allocation was successful, `NULL` otherwise. This function does
// not automatically initialize initial state.
struct libps_timer* libps_timer_create(void);

// Deallocates the memory held by `timer`.
void libps_timer_destroy(struct libps_timer* timer);

// Resets the timers to their initial state.
void libps_timer_reset(struct libps_timer* timer);

// Advances the timers.
void libps_timer_step(struct libps_timer* timer);