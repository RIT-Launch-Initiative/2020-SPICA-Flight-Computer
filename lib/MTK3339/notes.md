[Breakout board datasheet](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-ultimate-gps.pdf)

[MTK3999 command sheet](https://cdn-shop.adafruit.com/datasheets/PMTK_A11.pdf)

# Notes
- Everything is `c` at the moment
- `MTK.c` has `main()`, prints debugging examples
- Output agrees with expectations at the moment
- Send, recieve yet to be implemented

# Python library commands
- [Documentation](https://circuitpython.readthedocs.io/projects/gps/en/latest/)
- `send_command(b'PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0')` turns on GGA and RMC info. We want to parse GGA. The first `1` is RMC info, we may be able to disable that.
- `send_command(b'PMTK220,1000')` sets update rate to 1Hz

# Relevant PMTK Packets
- 314 for enabling outputs
- 220 for setting update rate


