#pragma once

#include <abi/Keyboard.h>
#include <libc/skift/Lock.h>
#include <libutils/RingBuffer.h>

#include "kernel/devices/LegacyDevice.h"

#define PS2KBD_ESCAPE 0xE0

class LegacyKeyboard : public LegacyDevice
{
private:
    Lock _events_lock{"legacy-keyboard-event"};
    RingBuffer _events{sizeof(KeyboardPacket) * 1024};
    bool _escaped = false;
    KeyMap *_keymap = nullptr;
    KeyMotion _keystate[__KEY_COUNT] = {};

    Key scancode_to_key(int scancode);

    Codepoint key_to_codepoint(Key key);

    KeyModifier modifiers();

    void handle_key(Key key, KeyMotion motion);

public:
    LegacyKeyboard(DeviceAddress address);

    void handle_interrupt() override;

    bool can_read(FsHandle &handle) override;

    ResultOr<size_t> read(FsHandle &handle, void *buffer, size_t size) override;
};
