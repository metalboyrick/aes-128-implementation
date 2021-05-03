# AES-128 algorithm implementation

This is a program that implements the AES-128 encryption algorithm with the CBC mode. With randomly generated Initialization Vectors(IV) and Secret Keys. Implemented with Linux C.

# Usage instructions

## System requirements
- gcc 7.5.0
- GNU Make 4.1

Both can be installed by using:
```
sudo apt install gcc
sudo apt install make
```

## Compiling the program
The program can be compiled by simply running the following command in the Linux Terminal.
```
make
```
Then you can run the program with the following command:
```
./main.out <mode> <text>
```
- The ```<mode>``` parameter toggles encryption or decryption, for encryption, use ```-e```, for decryption use ```-d```.
- The ```<text>``` is the text to be processed.

To clean the compilation files , use:
```
make clean
```
