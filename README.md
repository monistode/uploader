# Program Uploader
This is a repo containing code for uploading the binary files for the processsors.


# Installation

Just compile the program on the linux on the de10-nano board.

### !!! -std=c++14 IS VERY IMPORTANT !!!
You should now do
```
make
```
The binary is in `build/bin/upload`

This is deprecated(
```
g++ -std=c++14 upload.cpp -o upload
```

# Usage
Just do
```
./upload.cpp <filename>
```
