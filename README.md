# STM32_MOD_2

Firmware for a small STM32F1-based audio board: either a LUT-driven oscillator into the onboard DAC, or a short effects chain on whatever comes in on the ADC. The Cube/HAL setup is real; mixer, filter, ADSR, and sampler are mostly placeholders for now (see [Code layout](#code-layout)).

**MCU:** STM32F103RCT6  
**IDE:** STM32CubeIDE (GCC). There is also an IAR project under [`EWARM/`](EWARM/) if you use that instead.

## Requirements

- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) (or IAR + pack for F1)
- ST firmware package **STM32Cube FW_F1 V1.8.6** (matches [`STM32_MOD_2.ioc`](STM32_MOD_2.ioc))

## Build and flash

1. Open [`STM32_MOD_2.ioc`](STM32_MOD_2.ioc) or import the project from this repo in CubeIDE.
2. Build the **Debug** configuration (output under `Debug/`).
3. Flash/debug with your ST-Link (or whatever probe you use in the launch config).

Regenerating code from the `.ioc` is fine; user sections in `main.c` and friends are preserved by Cube’s `USER CODE` blocks.

## Audio modes

Flip these in [`Core/Src/main.c`](Core/Src/main.c) before you build:

- `ACTIVE_AUDIO_MODE` — `SYNTH` or `EFFECTS` (passed into `init_stm32()`).
- `ACTIVE_EFFECT_MODE` — `EFFECT_BYPASS`, `EFFECT_MOVING_AVG`, or `EFFECT_BIT_CRUSHER` ([`Core/Inc/effects.h`](Core/Inc/effects.h)).
- `ACTIVE_MOVING_AVG_TAPS` — tap count for the moving average (capped in [`effects.c`](Core/Src/effects.c)).

**SYNTH:** TIM8 triggers DAC channel 2 via DMA. Half- and full-transfer callbacks call `render_audio_block()`, which fills the DAC buffer from the oscillator ([`Core/Src/stm32_synth.c`](Core/Src/stm32_synth.c), [`Core/Src/oscillator.c`](Core/Src/oscillator.c)). Waveforms are sine, square, triangle, saw, or noise ([`Core/Inc/oscillator.h`](Core/Inc/oscillator.h)). Right now the main loop runs `marry_had_a_little_lamb()` as a demo when in this mode; other tests (UART, frequency sweep, seven-seg) are commented out in the same file.

**EFFECTS:** ADC1 is also triggered off TIM8 TRGO, with DMA into `audio_buffer_adc`. ADC callbacks copy each half into `audio_buffer_sig_path`. DAC callbacks wait until the matching `adc_half_ready[]` flag is set, then run `effects_process_sample()` so output stays aligned with fresh input. Underrun/overrun counters on `struct synth` are there for debugging.

**Sample rate:** System clock is 24 MHz (HSI + PLL in `SystemClock_Config`). TIM8 is set in the IOC to prescaler 3 and period 124, which works out to about **48 kHz** for the DAC/ADC trigger rate. Audio buffers are **256** samples (`AUDIO_BUFFER_SIZE` in [`Core/Inc/stm32_synth.h`](Core/Inc/stm32_synth.h)).

## Front-panel I/O

Pin names live in [`Core/Inc/main.h`](Core/Inc/main.h): two buttons, a pot, an LED, SPI1 to a shift-register style display, plus digit select lines for a **two-digit seven-segment** display. [`Core/Src/seven_seg.c`](Core/Src/seven_seg.c) is refreshed from `HAL_TIM_PeriodElapsedCallback()` on TIM2 in `main.c`.

## Code layout

| Area | Files |
|------|--------|
| Entry, Cube init, DAC/ADC callbacks | [`Core/Src/main.c`](Core/Src/main.c) |
| Mode init, `render_audio_block`, ADC staging | [`Core/Src/stm32_synth.c`](Core/Src/stm32_synth.c), [`Core/Inc/stm32_synth.h`](Core/Inc/stm32_synth.h) |
| Oscillator / LUT | [`Core/Src/oscillator.c`](Core/Src/oscillator.c), [`Core/Inc/oscillator.h`](Core/Inc/oscillator.h) |
| Effects | [`Core/Src/effects.c`](Core/Src/effects.c), [`Core/Inc/effects.h`](Core/Inc/effects.h) |
| Note frequencies | [`Core/Inc/notes.h`](Core/Inc/notes.h) |
| Seven-segment + SPI | [`Core/Src/seven_seg.c`](Core/Src/seven_seg.c), [`Core/Inc/seven_seg.h`](Core/Inc/seven_seg.h) |

**Not wired into the synth audio path yet:** `struct synth` already holds mixer, filter, and ADSR, and [`mixer.c`](Core/Src/mixer.c) implements a simple average of sources, but `render_audio_block()` only comments those stages—no calls. [`filter.c`](Core/Src/filter.c), [`adsr.c`](Core/Src/adsr.c), and [`sampler.c`](Core/Src/sampler.c) are empty or stub implementations.

## Hardware and diagrams

KiCad lives under [`Hardware/`](Hardware/). The project file is [`Hardware/STM_Module.kicad_pro`](Hardware/STM_Module.kicad_pro); schematics include e.g. [`Hardware/STM32.kicad_sch`](Hardware/STM32.kicad_sch) and [`Hardware/power.kicad_sch`](Hardware/power.kicad_sch).

Block diagrams in the repo root: [`signal_path_diagram.drawio`](signal_path_diagram.drawio), [`stm32_structure.drawio`](stm32_structure.drawio).

## License

ST’s HAL/CMSIS drop in with their usual `LICENSE.txt` under `Drivers/`—nothing special added here.
