# LEA Block Cipher

A C implementation of the LEA block cipher, its common modes of operation, padding methods, and validation-vector processing.

LEA uses a 128-bit block size. This implementation supports 128-, 192-, and 256-bit keys, corresponding to 24, 28, and 32 rounds.

## Implemented Features

### Core Cipher

- key expansion for 128-, 192-, and 256-bit keys;
- 128-bit block encryption;
- 128-bit block decryption;
- encryption and decryption round functions.

### Modes of Operation

- ECB;
- CBC;
- CTR;
- CFB-1;
- CFB-8;
- CFB-64;
- CFB-128;
- OFB.

### Padding

- zero padding;
- one-and-zeros padding and removal;
- PKCS#5-style padding and removal.

### Validation-Vector Processing

- Known Answer Tests (KAT);
- Multi-block Message Tests (MMT);
- Monte Carlo Test routines for ECB, CBC, and CTR.

The `test` directory contains validation data for 128-, 192-, and 256-bit keys. CCM and GCM data files are included in that dataset, but CCM and GCM implementations are not part of the modular source code.

## Files

| File | Purpose |
|---|---|
| `LEAEncDec.[ch]` | Key schedule and block encryption/decryption |
| `LEAMode.[ch]` | ECB, CBC, CTR, CFB, and OFB modes |
| `LEAPad.[ch]` | Padding and unpadding functions |
| `LEAMOVS.[ch]` | Validation-vector parsing and execution |
| `LEAGlobal.[ch]` | Shared constants and macros |
| `LEA.c` | Test driver |
| `LEA_OneFile.c` | Earlier single-file version of the implementation |
| `test/` | KAT, MMT, MCT, and other supplied validation datasets |

## Build and Run

```bash
make
./LEA.exe
```

To clean generated files:

```bash
make clean
```

`LEA.c` contains separate drivers for:

- block encryption/decryption;
- padding;
- modes of operation;
- validation-vector processing.

Select a driver or validation routine by commenting or uncommenting the corresponding function call, then rebuild. The currently selected path runs the CTR Monte Carlo test.

## Validation Notes

The validation routines parse the supplied key, IV, plaintext, and ciphertext fields, execute the selected LEA operation, and print or generate results for comparison with the reference data.

Some validation files are opened using local absolute Windows paths in the current source. Change those paths to the location of the local clone, or convert them to relative paths, before running the tests on another machine.

## Scope

This implementation was created to translate the LEA specification into C and study modes and validation procedures. It has not been audited for constant-time behavior, side-channel resistance, or production security.
