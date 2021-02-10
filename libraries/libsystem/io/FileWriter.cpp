#include <libsystem/io/FileWriter.h>

FileWriter::FileWriter(const char *path) : _handle(path, OPEN_WRITE | OPEN_CREATE | OPEN_STREAM)
{
}

FileWriter::FileWriter(Path &path) : _handle(path.string(), OPEN_WRITE | OPEN_CREATE | OPEN_STREAM)
{
}

FileWriter::FileWriter(System::Handle &&handle) : _handle{move(handle)}
{
}

void FileWriter::flush() {}

size_t FileWriter::write(const void *buffer, size_t size)
{
    auto result_or_write = _handle.write(buffer, size);

    if (result_or_write)
    {
        return *result_or_write;
    }
    else
    {
        return 0;
    }
}

size_t FileWriter::length()
{
    auto result_or_stat = _handle.stat();

    if (result_or_stat)
    {
        return result_or_stat->size;
    }
    else
    {
        return 0;
    }
}

size_t FileWriter::position()
{
    auto result_or_tell = _handle.tell(WHENCE_START);

    if (result_or_tell)
    {
        return *result_or_tell;
    }
    else
    {
        return 0;
    }
}