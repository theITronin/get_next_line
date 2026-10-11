*This project has been created as part of the 42 curriculum by dbustama*

# Get Next Line

## Description

**Get Next Line** is a 42 project whose goal is to program a function, `get_next_line`, that returns one line at a time from a file descriptor.

```c
char *get_next_line(int fd);
```

Calling the function repeatedly (for example, in a loop) reads the content of the file pointed to by the file descriptor **line by line**, until the end.

**Behavior summary**

- Returns the line that has just been read, **including the terminating `\n`**, except when the end of the file is reached and the file does not end with a `\n`.
- Returns `NULL` if there is nothing else to read or if an error occurred.
- Works both when reading from a regular file and when reading from `stdin`.
- The size of the buffer used by `read()` is defined at compile time with `-D BUFFER_SIZE=n`.
- Allowed external functions: `read`, `malloc`, `free`.
- Forbidden: `libft`, `lseek` and global variables.

**Files**

| Part | Files |
|------|-------|
| Mandatory | `get_next_line.c`, `get_next_line_utils.c`, `get_next_line.h` |
| Bonus | `get_next_line_bonus.c`, `get_next_line_utils_bonus.c`, `get_next_line_bonus.h` |

**Bonus**

- `get_next_line` is implemented with a **single static variable**.
- It can manage **multiple file descriptors at the same time** (e.g. alternating calls on fd 3, 4 and 5) without losing the reading state of each descriptor and without returning a line that belongs to another one.

## Instructions

### Compilation

The project must compile with the flags `-Wall -Werror -Wextra` and with the `-D BUFFER_SIZE=n` flag, using `cc`:

```bash
cc -Wall -Werror -Wextra -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c
```

The project can also be compiled **without** `-D BUFFER_SIZE`; in that case a default value defined in `get_next_line.h` is used.

```bash
cc -Wall -Werror -Wextra get_next_line.c get_next_line_utils.c main.c
```

Bonus part:

```bash
cc -Wall -Werror -Wextra -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main_bonus.c
```

> `main.c` / `main_bonus.c` are your own test programs; they are not part of the delivered work.

### Usage example

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("file.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```

To read from standard input, simply use `fd = 0`.

### Recommended tests

- Different values of `BUFFER_SIZE`: `1`, `42`, `9999`, `10000000`.
- Empty files, files with a single line, files with and without a final `\n`, very long lines and files with only `\n`.
- Reading from `stdin`.
- Invalid file descriptors (e.g. `-1`) and closed descriptors.
- (Bonus) Alternating reads between several file descriptors.
- Memory leaks (e.g. with `valgrind`).

## Algorithm: explanation and justification

### Explanation

The algorithm relies on a **static variable** that keeps, between calls, the characters that have already been read from the file descriptor but not yet returned (the *stash* or *remainder*).

On each call to `get_next_line(fd)`:

1. **Validation.** If `fd` is invalid, `BUFFER_SIZE` is not positive, or an allocation fails, the function returns `NULL`.
2. **Read.** While the stash does not contain a `\n`, a buffer of `BUFFER_SIZE + 1` bytes is allocated and filled with `read(fd, buffer, BUFFER_SIZE)`. The buffer is null-terminated and appended to the stash.
   - If `read` returns `0`, the end of the file has been reached.
   - If `read` returns `-1`, an error occurred: memory is freed, the stash is reset and `NULL` is returned.
3. **Extract the line.** The characters of the stash up to and including the first `\n` (or up to the end if there is no `\n`) are copied into a newly allocated string, which is the value to return.
4. **Update the stash.** The characters after the returned line are kept in a new stash and the old one is freed. If nothing remains, the stash is freed and set to `NULL`.
5. **Return.** The line is returned. If the stash was empty and nothing was read (end of file), `NULL` is returned.

For the **bonus**, the single static variable is an array of pointers indexed by file descriptor (`static char *stash[FD_MAX];`), so every descriptor has its own independent remainder. Reading from one file descriptor never alters the state of another.

### Justification

- **Static variable instead of a global.** A static variable keeps its value between calls while remaining private to the file, which satisfies the "no global variables" rule.
- **Minimal reading.** The function stops reading as soon as a `\n` is found in the stash. It never reads the whole file in advance, as required by the subject.
- **Independent from `BUFFER_SIZE`.** Since the data is accumulated in the stash, the result is the same for any buffer size (from `1` to very large values); only the number of `read` calls changes.
- **No `lseek`.** Because the remainder is stored in memory, there is no need to move the file offset. This also makes the function work with non-seekable descriptors such as `stdin`, pipes or sockets.
- **Memory safety.** Every buffer is freed after use, and the stash is freed as soon as it becomes empty or when an error/end of file occurs, avoiding leaks.
- **Trade-off.** Joining the stash with each new buffer requires copying data, so lines that are much longer than `BUFFER_SIZE` cost more copies. This is accepted in exchange for a simple, robust and easy-to-verify implementation.

> **Note:** adapt this section so that it exactly matches your implementation, and make sure you can explain every step during the evaluation.

## Resources

### References

- `man 2 read`, `man 3 malloc`, `man 3 free`
- Static variables and storage duration in C (C standard / documentation of your choice)
- Articles and tutorials about file descriptors and buffered reading in C
- *The C Programming Language*, Kernighan & Ritchie
- Discussions with other 42 students (peer learning) and peer evaluations

### Use of AI

> Fill this section honestly with what applies to your project.

- AI was used to draft the structure and wording of this `README.md`.
- AI was **not** used to write the project's source code. *(Edit this line if it is not true in your case.)*
- Tasks for which AI was used and the parts of the project concerned: `<list here, e.g. "README.md writing", "explaining a concept", "none in the source code">`.