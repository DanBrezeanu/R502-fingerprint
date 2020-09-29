# R502-fingerprint

Lightweight C library for controlling the HZGROW R502 capacitive fingerprint sensor.

It does not support all the available commands, but rather the necessary ones to use the device as a fingerprint module.

## Commands supported
* Device status reading
* Password authentication
* Enrolling and deleting fingerprints
* Verification against saved fingerprints

For more information see [wiki](https://github.com/DanBrezeanu/R502-fingerprint/wiki/R502) or [examples](examples).

## Build

    sudo apt install libserialport0 libserialport-dev
    sudo make install
    
For building examples: &nbsp; `make examples `

## Contributing guidelines
If you consider contributing to the project, please make sure that your changes have been tested on a R502 fingerprint module.
