[Breakout board datasheet](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-ultimate-gps.pdf)
# Library Objectives
- Implement `HAL_UART_Transmit`: set settings
- Implement `HAL_UART_Recieve`: get lat, longe, sattelite count

# Notes for Yevgeniy

## Misc
- `HAL` is the Hardware Abstraction Layer. Our communications to modules use this library.
- `UART` is the hardware enabling serial data transfer
- `::` in C++ is namespace access

## Python library commands
- [Documentation](https://circuitpython.readthedocs.io/projects/gps/en/latest/)
- GGA format: `GGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh`
- Relevant sections: 2 (Lat), 3 (N/S), 4 (Lon), 5 (E/W), 7 (Sat count). To implement with C++, it is necessary to determine which sections of the binary are relevant.
- need to find `uart` library, `adafruit_gps` does not define.
- 
