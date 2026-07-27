# P-256 Prime-Field Arithmetic

A C implementation of multi-precision integer arithmetic and prime-field arithmetic used by the NIST P-256 elliptic curve.

Values are represented as little-endian arrays of eight 32-bit words. The implementation does not rely on an external big-number library.

## Implemented Operations

### Multi-Precision Integer Arithmetic

- addition and subtraction with carry/borrow;
- integer comparison;
- operand-scanning multiplication;
- product-scanning multiplication;
- recursive Karatsuba multiplication;
- multi-precision squaring.

### Prime-Field Arithmetic

- modular addition and subtraction;
- multiplication followed by P-256 reduction;
- reduction modulo
  `FFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF`;
- binary extended-GCD inversion;
- Fermat-based inversion.

## Files

| File | Purpose |
|---|---|
| `bigIntGlobal.[ch]` | Shared constants and global definitions |
| `bigIntIO.[ch]` | Reading and writing multi-precision values |
| `bigIntPlusMinus.[ch]` | Addition, subtraction, comparison, and field add/subtract |
| `bigIntMulSqrRed.[ch]` | Multiplication, squaring, and P-256 reduction |
| `bigIntInv.[ch]` | Binary and Fermat-based modular inversion |
| `bigIntTest.[ch]` | Test-vector drivers for each operation |
| `main.c` | Selected test execution and time/cycle measurement |
| `timeMeasure.c` | Separate performance-measurement experiments |
| `bigIntOperation_OneFile.c` | Earlier single-file version of the arithmetic implementation |

The `add&sub`, `mul&squ`, `reduction`, and `inv` directories contain input, reference, and generated test-vector files.

## Build and Run

The supplied `Makefile` targets GCC on Windows.

```bash
make
./main.exe
```

To remove generated object files and the executable:

```bash
make clean
```

`main.c` currently runs `bigIntInvTest()`. Other test calls are present but commented out; select the operation to test by changing those calls and rebuilding.

## Validation and Measurement

Each test routine reads operand vectors, executes the selected implementation, and writes the result for comparison with its reference vector. The driver measures:

- elapsed wall-clock time with `QueryPerformanceCounter`;
- CPU cycles with `__rdtsc`.

Because the current source includes `Windows.h` and Windows timing APIs, it is intended for an MSYS2/MinGW-w64 environment. Some test-vector paths are local absolute paths and must be changed to match the repository location on another machine.

## Scope

This directory demonstrates the implementation and comparison of arithmetic algorithms. It is not a constant-time or production-ready big-number library.
