# Cryptographic Arithmetic and Algorithm Implementations

C implementations of cryptographic arithmetic, elliptic-curve operations, and the LEA block cipher, developed during an undergraduate research internship at the CSE Lab, Kookmin University.

The repository follows a bottom-up implementation path: 256-bit integer and prime-field arithmetic, elliptic-curve operations built on that arithmetic, and a separate implementation of the LEA block cipher and its modes of operation. The implementations were written for study and experimental validation rather than production use.

## Components

| Component | Main contents |
|---|---|
| [`primeALU`](./primeALU) | Multi-precision integer arithmetic and arithmetic over the NIST P-256 prime field |
| [`ECC`](./ECC) | P-256 point operations, coordinate conversion, scalar multiplication, and multi-scalar multiplication |
| [`LEA`](./LEA) | LEA-128/192/256, block-cipher modes, padding methods, and validation-vector processing |

## Implementation Overview

### Prime-Field Arithmetic

The [`primeALU`](./primeALU) implementation represents a 256-bit integer as eight 32-bit words and provides:

- multi-precision addition, subtraction, multiplication, and squaring;
- operand-scanning, product-scanning, and Karatsuba multiplication;
- addition, subtraction, multiplication, and reduction modulo the P-256 prime;
- modular inversion using binary extended-GCD and Fermat-based methods;
- test-vector comparison and Windows-based time/cycle measurements.

### Elliptic-Curve Arithmetic

The [`ECC`](./ECC) implementation builds on the same 256-bit arithmetic and provides:

- point addition and doubling in affine and Jacobian coordinates;
- Jacobian-to-affine coordinate conversion;
- binary, NAF, wNAF, sliding-window, fixed-window, and fixed-comb scalar multiplication;
- Shamir's trick and interleaving for multi-scalar multiplication;
- output comparison using 10,000 scalar inputs and reference results.

### LEA Block Cipher

The [`LEA`](./LEA) implementation provides:

- key schedules and encryption/decryption for 128-, 192-, and 256-bit keys;
- ECB, CBC, CTR, CFB-1/8/64/128, and OFB modes;
- zero, one-and-zeros, and PKCS#5-style padding;
- parsers and test routines for KAT, MMT, and selected MCT validation data.

## Repository Structure

```text
CSE/
├── primeALU/   # 256-bit integer and P-256 prime-field arithmetic
├── ECC/        # P-256 elliptic-curve operations
├── LEA/        # LEA cipher, modes, padding, and validation data
└── *.pptx      # Internship presentation materials
```

Detailed implementation and execution notes are available in each component directory:

- [primeALU README](./primeALU/README.md)
- [ECC README](./ECC/README.md)
- [LEA README](./LEA/README.md)

## Development Environment

- Language: C
- Operating system: Windows
- Build tool: GNU Make
- Compiler: GCC via MSYS2/MinGW-w64
- Debugger: GDB

## Build

Each component has its own `Makefile`.

```bash
cd primeALU   # or ECC / LEA
make
./main.exe    # LEA uses ./LEA.exe
```

The current arithmetic and ECC drivers include Windows-specific headers or APIs. Some test drivers also contain local absolute paths and select a test by commenting or uncommenting code, so those paths and selections may need to be adjusted before execution.

## Validation Approach

The implementations were checked by:

1. reading supplied input vectors or standard validation datasets;
2. running the corresponding arithmetic or cryptographic operation;
3. writing the computed output;
4. comparing it with the reference output;
5. measuring elapsed time or CPU cycles for selected arithmetic routines.

## Disclaimer

These implementations were created for educational and research purposes. They have not been reviewed for constant-time behavior, side-channel resistance, or production security. Do not use them to protect real-world sensitive data.
