// Copyright 2020 Michael Rodriguez
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

#include "emulator.h"
#include "main_window.h"
#include "debug/bios_calls.h"
#include "debug/tty_log.h"

class PSTest : public QObject
{
    Q_OBJECT

public:
    PSTest();
    ~PSTest();

private:
    // Returns the BIOS file to use.
    QString handle_initial_bios_select();

    void emu_report_system_error();

    void open_tty_log();
    void open_bios_calls();

    void bios_call(const uint32_t pc, const uint32_t fn);

    void start_emu();
    void reset_emu();
    void pause_emu();

    BIOSCalls* bios_calls;
    MainWindow* main_window;
    TTYLogger* tty_logger;
    Emulator* emulator;
};