# Mobiflight FW modifications for multiplexer management
_Replaces closed PR #118_

- _Subject_: implementation of hardware multiplexer management
  - __General multiplexer driver__ - shared by any device that uses multiplexers. 
    Can be readily used to support ___multiplexed routing of existing I/O blocks___ (extending them with very minor modifications)
  - __Digital input device__ (comparable to shift registers in function) using MPX ICs

- _Status_: __Beta stage__ - code modifications complete, firmware + matching Connector PR; Basic functions tested.  
- _Notes_: 
  - The limit number of devices is still provisional and requires evaluation.
  - Due to space constraints, Steppers are temporarily disabled in the ProMicro version.
  
Refers to issue  #119 
  
## Class MFMuxDriver

- _Purpose_: manage 4 selection lines for external multiplexers, on any Arduino pins (defined as type `kTypeMuxDriver`).  
  These lines will be connected in parallel to every hardware multiplexer used (typically 74HCT4067 or -4051).
- One (static) instance only (variable `_MUX`), configured with following command:
 
    `kTypeMuxDriver.Sel0_pin.Sel1_pin.Sel2_pin.Sel3_pin`

  Usual "Name" parameter is missing, since with only one instance the name is pointless.

- A `MuxDriver` can be used:
  - as required component for other devices (_e.g. direct multiplexed digital / analog inputs_), as well 
  - as a device itself, to route inputs or outputs of other "secondary" devices such as display drivers, shift registers etc.  
  
  __Currently, only the first mode above is used, because usage for secondary device routing is not yet supported; therefore, in the Connector, the `MuxDriver` device is inserted/deleted automatically (cannot be done by the user) when one or more `DigInMux`es are defined.__

- A `MuxDriver` can be managed in two ways:
  - devices can freely drive the selector as they require (e.g. doing a complete scan) with no interference with the other mode
  - the MuxDriver is made to automatically increment the selector value at every iteration of the main loop: devices read the current selector value and determine their behavior accordingly (i.e. they "activate" if the current mux channel selection matches their own assigned channel).
  
  _These modes can co-exist, that is, each device can use any of the two modes as required._

## Class MFDigInMux

- _Purpose_: manage 8/16 digital input lines, multiplexed on a single Arduino pins (defined as type `kTypeDigInMux`). 
  These lines will be connected to a hardware multiplexer (typically 74HCT4067 or -4051), whose selector lines will be connected to the pins defined by the `MFMuxDriver` component defined separately.
- Several instances (see  `MAX_DIGIN_MUX` in files `MFBoards.h`), configured with following command:  

    `kTypeDigInMux.dataPin.blocks.mode.name`  

  where:  
  - `dataPin` is the common Arduino input pin
  - `blocks` is the number of 8-byte terminal blocks, i.e. `2` for 16-channel MUXes (x4067) or `1` for 8-channel MUXes (x4051)
  - `mode` is `0` if the block should perform a full scan whenever polled, or `1` if it should read .  
  In both cases, changed inputs immediately generate event callbacks (of type `kMPXDigitalInChange`).
