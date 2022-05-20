[![Codacy Badge](https://app.codacy.com/project/badge/Grade/36aaa6b1f5e94ca6a2e3f865d706dee6)](https://www.codacy.com/gh/DanBrezeanu/R502-fingerprint/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=DanBrezeanu/R502-fingerprint&amp;utm_campaign=Badge_Grade)

# R502-fingerprint

Lightweight C library for controlling the HZGROW R502 capacitive fingerprint sensor.

It does not support all the available commands, but rather the necessary ones to use the device as a fingerprint module.

## Commands supported
*   Device status reading
*   Password authentication and modification
*   Enrolling and deleting fingerprints
*   Verification against saved fingerprints
*   Leds control

For more information see [wiki](https://github.com/DanBrezeanu/R502-fingerprint/wiki/R502) or [examples](examples).

## Build

    sudo apt install libserialport0 libserialport-dev
    sudo make install

For building examples: &nbsp; `make examples `

## About
This program uses the libserialport Library, which is licensed under the GNU Lesser General Public Library, version 3.0.

## Contributing guidelines
If you consider contributing to the project, please make sure that your changes have been tested on a R502 fingerprint module.
