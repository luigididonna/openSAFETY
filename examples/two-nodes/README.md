# Two-endpoint openSAFETY SPDO frame exchange

This demonstrator initializes the upstream openSAFETY 1.4 stack (`SSC_InitAll`)
and sends one data-only SPDO frame from logical producer SADR 1 to consumer
SADR 4 through a replaceable SHNF loopback adapter.

The frame uses the stack's `SFS_GetMemBlock`, `SFS_FrmSerialize`, and
`SFS_FrmDeSerialize` APIs. The receiver validates the paired subframes,
addresses, frame type, data byte, and CRC before reporting success. The
demonstration is self-contained in one process, so the loopback adapter can
later be replaced by a network transport.

This proves the exchange and integrity validation of an actual openSAFETY
SPDO-format frame. It does not yet demonstrate two independently operational
Safety Nodes: the application still bypasses the SNMT/SCM state transition and
the `SPDO_ProcessRxSpdo` object-mapping path. That next step requires separate,
role-specific producer and consumer SOD configurations. MQTT integration comes
after that; MQTT transports the frame and does not replace openSAFETY validation.

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

Expected output confirms a 13-byte SPDO frame and the received data byte
`0x5A`.

## Layout

```text
examples/two-nodes/
  spdo_exchange.c          SPDO frame exchange entry point
  demo_sod.c               demo object dictionary used by SSC_InitAll
  demo_sod_spdo_config.h   SPDO communication and mapping parameters
  node-only-config/        stack config override with SCM disabled
  loopback_shnf.c          SHNF memory, loopback queue, CRC adapters
  link_stubs.c             application callbacks for the demo build
  CMakeLists.txt           optional demo target
```
