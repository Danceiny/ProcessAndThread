# Pre-requisites
* Unix-like OS, like linux / Mac OS
* gcc
* make 

You can check your environment, maybe like this:
For Debian/Ubuntu
```
gcc --version
make --version
```

If get error, so you need to install them like this:
```
sudo apt-get install gcc
sudo apt-get install make
```

For other OS, you can google it. The Same question~

# Quick start
## Multi-thread
```
$ cd thread
$ make
$ ./multisum
```
Then the executable file `./multisum` will read the file `input.txt` which is under directory `thread`.

You can modify the `input.txt` to calculate different numbers. After modified, just rerun the ./multisum.

For `input.txt`, like this:
```
N=6
M=20
```
It means that the number of thread, which will take part in computation, is 6, and the number to calculate, is 20.

In this case, we'll get result sum(1,2,3,...,20)=210, and you can see that in the `thread/output.txt`.

## Multi-process
For multi-process, see above. 

# Features


# Bugs
* very big number.
* time comsuption.