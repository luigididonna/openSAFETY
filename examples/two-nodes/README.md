# Two-node openSAFETY SPDO exchange

This demonstrator initializes the upstream openSAFETY 1.4 stack (`SSC_InitAll`)
and runs two logical Safety Nodes through a loopback SHNF transport. The
producer uses SADR 1, the consumer receives SADR 1, and their Safety Object
Dictionaries have role-specific reciprocal SPDO communication settings.

The producer maps object `0x6000/2` into a Tx SPDO. The consumer maps that
payload into `0x6000/1`. The executable activates both mappings, starts the
SPDO state machines, cycles frames in both directions for the stack's time
synchronization, and calls `SPDO_ProcessRxSpdo` and `SPDO_CheckRxTimeout` on
received frames. It verifies the consumer's mapped byte and Rx connection
status through the stack API.

This proves a mapped process-data exchange through the real SPDO stack path,
including frame serialization, subframe and CRC validation, time
synchronization, and receiver mapping. For this focused demo the application
calls `SPDO_SetToOp` directly; the SNMT/SCM node activation lifecycle is not
simulated. MQTT integration is a later transport change and must preserve the
complete openSAFETY frame bytes.

The optional demo config disables the SCM because the two endpoints here are
Safety Nodes. The upstream stack's ordinary configuration is otherwise kept.

## Build and run on Windows

Use a 32-bit MinGW GCC toolchain. This legacy stack stores pointers in 32-bit
fields in some configurations, so the 64-bit target is not suitable.

```powershell
cmake -S . -B build -G "MinGW Makefiles" `
  -DCMAKE_C_COMPILER=C:/toolchains/mingw32/bin/gcc.exe `
  -DOPENSAFETY_BUILD_FRAME_EXCHANGE=ON
cmake --build build --target opensafety-spdo-exchange
./build/examples/two-nodes/opensafety-spdo-exchange.exe
```

Expected output confirms the consumer received `0x5A` at `0x6000/1` and that
the stack reports a valid Rx connection.

## Layout

```text
examples/two-nodes/
  spdo_exchange.c          SPDO frame exchange entry point
  demo_sod.c               producer SOD and 0x6000/2 Tx mapping
  demo_sod_consumer.c      consumer SOD and 0x6000/1 Rx mapping
  demo_sod_index.c         binds role-specific SODs to stack instances
  demo_sod_config.h        shared endpoint addresses and role selection
  demo_sod_spdo_config.h   SPDO communication and mapping parameters
  demo_sod_app.h           demo application and parameter-set definitions
  node-only-config/        stack config override with SCM disabled
  loopback_shnf.c          SHNF memory, loopback queue, CRC adapters
  link_stubs.c             application callbacks for the demo build
  CMakeLists.txt           optional demo target
```
