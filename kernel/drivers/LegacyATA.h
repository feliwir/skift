#pragma once

#include <libsystem/thread/Lock.h>

#include "kernel/devices/LegacyDevice.h"

class LegacyATA : public LegacyDevice
{
private:
    Lock _buffer_lock{"legacy-ata"};

    void identify();
    void select();
    void has_failed(uint8_t status);

    void delay(uint16_t io_port);
    void poll(uint16_t io_port);

    // TODO: will have to be uint64_t for LBA48
    void write_lba(uint16_t io_port, uint32_t lba);
    uint8_t read_block(uint8_t *buf, uint32_t lba, size_t size);
    uint8_t write_block(uint8_t *buf, uint32_t lba, size_t size);

    virtual bool did_fail() override
    {
        return !_exists;
    }

    int _bus;
    int _drive;
    uint16_t _ide_buffer[256];
    bool _exists = false;
    String _model;
    bool _supports_48lba;

public:
    LegacyATA(DeviceAddress address);

    bool can_read(FsHandle &handle) override;

    ResultOr<size_t> read(FsHandle &handle, void *buffer, size_t size) override;

    ResultOr<size_t> write(FsHandle &handle, const void *buffer, size_t size) override;
};
