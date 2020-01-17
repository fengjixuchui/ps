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

#include <filesystem>
#include "emulator.h"
#include "../libps/include/ps.h"

Emulator::Emulator(const QString& bios_file)
{
    FILE* handle = fopen(qPrintable(bios_file), "rb");
    bios = new uint8_t[0x80000];
    fread(bios, 1, 0x80000, handle);
    fclose(handle);

    sys = libps_system_create(bios);
}

Emulator::~Emulator()
{
    delete[] bios;
    libps_system_destroy(sys);
}

void Emulator::run()
{
    static QString tty_str;

    while (running)
    {
        QElapsedTimer timer;
        timer.start();

        for (unsigned int cycle = 0;
             cycle != (LIBPS_CPU_CLOCK_RATE / 60);
             ++cycle)
        {
            if (((sys->cpu->pc == 0x000000A0) && sys->cpu->gpr[9] == 0x3C) ||
                ((sys->cpu->pc == 0x000000B0) && sys->cpu->gpr[9] == 0x3D))
            {
                if (sys->cpu->gpr[4] == '\n')
                {
                    emit tty_string(tty_str);
                    tty_str.clear();
                }
                tty_str += sys->cpu->gpr[4];
            }

            if (!sys->cpu->good)
            {
                __debugbreak();
            }

            libps_system_step(sys);
        }

        libps_vblank(sys);
        emit render_frame(sys->bus->gpu->vram);

        const qint64 elapsed = timer.elapsed();
           
        if (elapsed < (1000 / 60))
        {
            QThread::msleep((1000 / 60) - elapsed);
        }
    }
}

void Emulator::begin_run_loop()
{
    if (!running)
    {
        running = true;
    }
}

void Emulator::stop_run_loop()
{
    if (running)
    {
        running = false;
        libps_system_reset(sys);
    }
}

void Emulator::pause_run_loop()
{
    if (running)
    {
        running = false;
    }
}

void Emulator::inject_ps_exe(const QString& file_name)
{
    FILE* test_file = fopen(qPrintable(file_name), "rb");
    const auto test_file_size = std::filesystem::file_size(qPrintable(file_name));
    uint8_t* test_data = static_cast<uint8_t*>(malloc(test_file_size));
    fread(test_data, 1, test_file_size, test_file);
    fclose(test_file);

    running = false;

    uint32_t dest = *(uint32_t *)(test_data + 0x10);

    for (unsigned int ptr = 0x800; ptr != (test_file_size - 0x800); ++ptr)
    {
        *(uint32_t *)(sys->bus->ram + (dest++ & 0x1FFFFFFF)) = test_data[ptr];
    }

    sys->cpu->pc      = *(uint32_t *)(test_data + 0x18);
    sys->cpu->next_pc = *(uint32_t *)(test_data + 0x18);

    sys->cpu->instruction = libps_bus_load_word(sys->bus, sys->cpu->pc);
    free(test_data);

    running = true;
}