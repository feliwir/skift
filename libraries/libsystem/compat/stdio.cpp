#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <libsystem/Logger.h>
#include <libsystem/io/Filesystem.h>
#include <libsystem/io/Stream.h>

#undef printf
#undef puts

FILE *__stdio_get_stdin()
{
    return in_stream;
}

FILE *__stdio_get_stdout()
{
    return out_stream;
}

FILE *__stdio_get_stderr()
{
    return err_stream;
}

OpenFlag stdio_parse_mode(const char *mode)
{
    OpenFlag flags = 0;

    for (int i = 0; mode[i]; i++)
    {
        char c = mode[i];

        if (c == 'r')
        {
            flags |= OPEN_READ;
        }
        else if (c == 'w')
        {
            flags |= OPEN_WRITE;
            flags |= OPEN_CREATE;
            flags |= OPEN_TRUNC;
        }
        else if (c == 'a')
        {
            flags |= OPEN_WRITE;
            flags |= OPEN_CREATE;
            flags |= OPEN_APPEND;
        }
        else if (c == '+')
        {
            flags |= OPEN_READ;
        }
    }

    return flags;
}

FILE *fdopen(int fd, const char *mode)
{
    Stream *stream = stream_open_handle(fd, stdio_parse_mode(mode));

    if (handle_has_error(stream))
    {
        stream_close(stream);

        return nullptr;
    }

    return stream;
}

FILE *fopen(const char *path, const char *mode)
{
    Stream *stream = stream_open(path, stdio_parse_mode(mode));

    if (handle_has_error(stream))
    {
        stream_close(stream);

        return nullptr;
    }

    return stream;
}

int fclose(FILE *stream)
{
    stream_close((Stream *)stream);

    return 0;
}

int fflush(FILE *stream)
{
    stream_flush((Stream *)stream);

    return 0;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t r = stream_read((Stream *)stream, ptr, size * nmemb);

    return r;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t r = stream_write((Stream *)stream, ptr, size * nmemb);

    return r;
}

int fseek(FILE *stream, long offset, int whence)
{

    int r = -1;

    if (whence == SEEK_SET)
    {
        r = stream_seek((Stream *)stream, offset, WHENCE_START);
    }
    else if (whence == SEEK_CUR)
    {
        r = stream_seek((Stream *)stream, offset, WHENCE_HERE);
    }
    else if (whence == SEEK_END)
    {
        r = stream_seek((Stream *)stream, offset, WHENCE_END);
    }

    return r;
}

long ftell(FILE *stream)
{

    long r = stream_tell((Stream *)stream, WHENCE_START);

    return r;
}

int puts(const char *s)
{

    size_t length = 0;

    for (length = 0; s[length]; length++)
    {
    }

    int r = stream_write(out_stream, s, length);
    r += stream_write(out_stream, "\n", 1);

    return r;
}

int putchar(int c)
{
    int r = stream_putchar(out_stream, c);

    return r;
}

int printf(const char *fmt, ...)
{

    va_list va;
    va_start(va, fmt);

    int result = stream_vprintf(out_stream, fmt, va);

    va_end(va);

    return result;
}

int fprintf(FILE *stream, const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);

    int result = stream_vprintf((Stream *)stream, fmt, va);

    va_end(va);

    return result;
}

int vfprintf(FILE *stream, const char *fmt, va_list va)
{
    int r = stream_vprintf((Stream *)stream, fmt, va);

    return r;
}

int remove(const char *pathname)
{

    int r = filesystem_unlink(pathname);

    return r;
}

int rename(const char *oldpath, const char *newpath)
{
    int r = filesystem_rename(oldpath, newpath);

    return r;
}

int fputs(const char *string, FILE *stream)
{
    return stream_write((Stream *)stream, string, strlen(string));
}

void clearerr(FILE *stream)
{
    handle_clear_error((Stream *)stream);
}

int feof(FILE *stream)
{
    return stream_is_end_file((Stream *)stream);
}

int ferror(FILE *stream)
{
    return handle_has_error((Stream *)stream);
}

char *fgets(char *s, int size, FILE *stream)
{
    Stream *native_stream = (Stream *)stream;

    for (int i = 0; i < size; i++)
    {
        s[i] = stream_getchar(native_stream);

        if (handle_has_error(native_stream) ||
            stream_is_end_file(native_stream))
        {
            return s;
        }
    }

    return s;
}

int fgetc(FILE *stream)
{
    return stream_getchar((Stream *)stream);
}

int fputc(int c, FILE *stream)
{
    return stream_putchar((Stream *)stream, (char)c);
}

int getchar(void)
{
    return fgetc(__stdio_get_stdin());
}

int fscanf(FILE *stream, const char *format, ...)
{
    __unused(stream);
    __unused(format);

    logger_trace("fscanf(\"%s\", ...)", format);

    return 0;
}

int sscanf(const char *str, const char *format, ...)
{
    __unused(str);
    __unused(format);

    logger_trace("sscanf(\"%s\", ...)", format);

    return 0;
}

char *strerror(int e)
{
    const char *s;
    switch (e)
    {
    case EAGAIN:
        s = "Operation would block (EAGAIN)";
        break;
    case EACCES:
        s = "Access denied (EACCESS)";
        break;
    case EBADF:
        s = "Bad file descriptor (EBADF)";
        break;
    case EEXIST:
        s = "File exists already (EEXIST)";
        break;
    case EFAULT:
        s = "Access violation (EFAULT)";
        break;
    case EINTR:
        s = "Operation interrupted (EINTR)";
        break;
    case EINVAL:
        s = "Invalid argument (EINVAL)";
        break;
    case EIO:
        s = "I/O error (EIO)";
        break;
    case EISDIR:
        s = "Resource is directory (EISDIR)";
        break;
    case ENOENT:
        s = "No such file or directory (ENOENT)";
        break;
    case ENOMEM:
        s = "Out of memory (ENOMEM)";
        break;
    case ENOTDIR:
        s = "Expected directory instead of file (ENOTDIR)";
        break;
    case ENOSYS:
        s = "Operation not implemented (ENOSYS)";
        break;
    case EPERM:
        s = "Operation not permitted (EPERM)";
        break;
    case EPIPE:
        s = "Broken pipe (EPIPE)";
        break;
    case ESPIPE:
        s = "Seek not possible (ESPIPE)";
        break;
    case ENXIO:
        s = "No such device or address (ENXIO)";
        break;
    case ENOEXEC:
        s = "Exec format error (ENOEXEC)";
        break;
    default:
        s = "Unknown error code (?)";
    }
    return const_cast<char *>(s);
}

void perror(const char *string)
{
    int error = errno;
    if (string && *string)
    {
        fprintf(stderr, "%s: ", string);
    }
    fprintf(stderr, "%s\n", strerror(error));
}

int setvbuf(FILE *stream, char *buf, int mode, size_t size)
{
    __unused(stream);
    __unused(buf);
    __unused(mode);
    __unused(size);
    // TODO: implement this
    return -1;
}