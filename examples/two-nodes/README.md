# Two-endpoint openSAFETY frame exchange

This proof uses the actual openSAFETY 1.4 frame serializer and deserializer
from `src/eplssrc/SN/`. The stack source remains in its upstream paths; this
example adds the application edge and a replaceable SHNF transport boundary.

## What currently works

The executable creates two logical endpoints in one process and exchanges two
SNMT frames through a loopback SHNF adapter:

1. Endpoint A serializes an SNMT request to set endpoint B to PRE-OPERATIONAL.
2. Endpoint B receives and validates the frame, including its paired subframes
   and CRC, then serializes a PRE-OPERATIONAL SNMT response.
3. Endpoint A receives and validates the response.

Both frames are built and parsed by the upstream `SFS_FrmSerialize` and
`SFS_FrmDeSerialize` functions. The adapter also accounts for the upstream
stack's transmit-buffer subframe order. This proves that the protocol frame
format and SHNF boundary can be used for a two-endpoint exchange.

This is a frame-level proof. It does not initialize the full SN state machine
with `SSC_InitAll`, run an SCM cycle, access a configured SOD, or move data
between two independently running processes. The PRE-OPERATIONAL command is
decoded as a valid SNMT frame but is not applied to a running node. Those are
the next integration steps before calling the endpoints fully operational
openSAFETY nodes. The current loopback adapter is deliberately isolated so it
can be replaced by UDP and later by MQTT.

## Build and run on Windows

Use a 32-bit MinGW GCC toolchain. This legacy stack stores pointers in 32-bit
fields in some configurations, so the 64-bit target is not suitable.

```powershell
cmake -S . -B build -G "MinGW Makefiles" `
  -DCMAKE_C_COMPILER=C:/toolchains/mingw32/bin/gcc.exe `
  -DOPENSAFETY_BUILD_FRAME_EXCHANGE=ON
cmake --build build --target opensafety-frame-exchange
./build/examples/two-nodes/opensafety-frame-exchange.exe
```

Expected output includes a valid SNMT request from endpoint A to B and a valid
PRE-OPERATIONAL response from B to A.

## Layout

```text
examples/two-nodes/
  frame_exchange.c   frame-level exchange entry point
  loopback_shnf.c    SHNF memory, loopback queue, CRC adapters
  link_stubs.c       no-SOD callbacks required for the codec-only executable
  CMakeLists.txt     optional demo target
```
