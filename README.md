# AES-128 algorithm implementation

This is a program that implements the AES-128 encryption algorithm with the CBC mode. With randomly generated Initialization Vectors(IV) and Secret Keys. Implemented with Linux C.

# Usage instructions

## System requirements
- gcc 7.5.0
- GNU Make 4.1
- A linux bash

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
If you are using ```cygwin``` or any Windows-based Linux capabilities, you can compile it into a Windows executable with:
```
make windows
```

Then you can run the program with the following command:
```
./main.out <mode> <text>
```
or (If you are using Windows with ```cygwin```):
```
main.exe <mode> <text>
```
- The ```<mode>``` parameter toggles encryption or decryption, for encryption, use ```-e```, for decryption use ```-d```.
- The ```<text>``` is the text to be processed.
	- For encryption, simply putting the plaintext is enough.
	- For decryption, you need to put the parameters in the following order: ```<text> <init_vector> <secret_key>```
		- Note that all the text must be in HEXADECIMAL FORM

The output would all be in hexadecimal form.

To clean the compilation files , use:
```
make clean
```
