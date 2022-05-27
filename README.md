# SSE string functions for x86-64 Linux

[![Test](https://github.com/aklomp/sse-strings/actions/workflows/test.yml/badge.svg)](https://github.com/aklomp/sse-strings/actions/workflows/test.yml)

November 5, 2014

The story of this page is that I was [nerd sniped](https://xkcd.com/356/).
During the course of optimizing a `strcasecmp()` bottleneck at work, I
discovered the
[assembly language versions of basic C string routines](http://www.strchr.com/strcmp_and_strlen_using_sse_4.2)
at strchr.com, and they tickled me to get back into assembly language.
Something about a problem just difficult enough to be interesting, and just
easy enough to be tractable as a side project.

I've programmed in x86 assembly language before as a teenager, but that was on
a 486 that ran 16-bit DOS (with segments and offsets), with only a 16-bit
assembler available to me, where I had to splice in my own 32-bit opcodes with
`db` directives. Still, I have many fond memories of small moments of magic.
Like finally getting my screen13 scrolling routines to run properly inside of
my PowerBasic demo programs. I later went on to do some assembly work on the
Z80 processor of my TI-83 calculator (a weird platform), and even did some
Win32 assembly with MASM, but then my interest faded. I found assembly language
to make for fantastic rainy afternoon puzzles, but lacking in power to make
actual complex, useful things.

Now that the '90's are over and open-source tooling is easily available, and we
have fast 64-bit multicore CPUs that run everything in a single clock cycle,
playing around with assembly is a lot more fun. Let's get to work! I present on
this page a selection of small string routines written for 64-bit Linux
platforms. I doubt that these are competitive in terms of speed to what common
libraries use (they unroll their loops, are concerned with things like data
dependencies and cache performance, and actually benchmark their code), but
it's been a fun puzzle.

All routines shown are written for [NASM](http://www.nasm.us) in Intel assembly
syntax (the One True Form) and assume 64-bit (System V AMD64 ABI) calling
conventions. These are so much nicer than the 32-bit calling conventions. The
first two arguments (that's all we need here) are passed in `rdi` and `rsi`,
return values go in `rax`, and the only registers the callee needs to save are
`rbp`, `rbx` and `r12–r15`
([source](https://en.wikipedia.org/wiki/X86_calling_conventions)). The routines
use SSE vector instructions where applicable, and consist of
position-independent code, so that they can be included in shared libraries.
Because this is the bright new x64 universe for which NASM has good defaults,
we don't need to set up segments, but can get away with declaring our functions
and our data `global`.

## Assembling and linking

Let's say you have `strlen.asm`. Assemble it into an object file named
`strlen.o` like so:

```sh
nasm -f elf64 strlen.asm
```

Include the appropriate header file in your C program so the compiler knows how
to call the function. In the linker stage, simply add the object file to the
list of object files on the compiler's command line to link them in to the
final binary.

## Caveat

These functions are all unaware of how long a string is, and will read 16 bytes
of memory at a time until a zero byte is found. This might mean that they read
a few bytes past the end of the string, which could cause a memory access
violation.

All these functions are only lightly tested. In production code, use the
functions provided by your regular libc, they'll probably be faster and
battle-hardened. This is a hobby project, no guarantees given or implied, etc.

## License

Because code must have a license, I declare these functions to be licensed
under the [2-clause BSD](http://opensource.org/licenses/BSD-2-Clause) license.

## strlen\_sse2

Determine the length of a zero-terminated string using only SSE2-level
instructions. This code looks a lot like the code at strchr.com, but that's
because this really is the most condensed, simplified way to write it. When
you're working at this level, there is not much opportunity for stylistic
flourish.

## strlen\_sse4

Determine the length of a zero-terminated string using the SSE4.2 `pcmpistri`
string instruction. Thanks to Lord Sméagol for suggesting the improvement of
using `lea` to perform an addition without touching the flags.

## strcmp\_x64

Compare zero-terminated string `a` against string `b`. Returns zero if the
strings are equal, less than zero if `a < b`, and greater than zero if `a > b`.

Actually, this function makes stronger guarantees about its return value, so
that it is identical (and thus comparable) to glibc's `strcmp`. It will return
the *signed difference* between the first differing character between `a` and
`b`. For example, if `a` is `0xFF` and `b` is `0`, it returns `-255`.

## strcmpeq\_x64

Comparing strings for equality is one of the most popular uses for `strcmp`,
but is slightly inefficient because of the extra arithmetic performed on the
differing bytes. If we're only interested in equality, we can take some
shortcuts. The family of `strcmpeq` functions are a nonstandard variation of
`strcmp` that only check whether two strings are equal or not, and return `0`
or `1` (which can be typed to a compiler as a C99 `bool`).

## strcmpeq\_sse2

Return `1` if zero-terminated strings `a` and `b` are equal, `0` if not. Uses
SSE2-level vector instructions to do the least amount of work per byte. Bails
out at the first sign that the strings must differ.

## strcmpeq\_sse4

Compare two strings for equality using SSE4.2's `pcmpistri` instruction, which
was virtually made for the job. It handles zero-termination and sets all the
right flags for us. Compared to the SSE2 code above, this is extremely compact.

## strcasecmpeq\_sse2

Compare zero-terminated strings `s1` and `s2` in case-insensitive fashion.
Return `1` if they are equal and `0` if they are not. The case-insensitive part
is a 1970's-style hack: it does no actual locale-dependent or language-aware
conversion, all characters in the `A–Z` range are simply arithmetically
transposed to characters in the `a–z` range. It's fast if you just care about
comparing plain ASCII strings.
