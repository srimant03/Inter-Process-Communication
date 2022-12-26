# Inter-Process-Communication
IPC between 2 programs, sending and receiving strings, through 3 mechanisms FIFOs, Unix Sockets & Shared Memory.
Each mechanism uses 2 programs P1 & P2. 

P1 generates 50 random strings of length 5 characters using `srand()` and `(rand() % 26) + 97`. 
P1 then sends a group of 5 strings to P2 along with their corresponding indices. P2 reads the strings and sends an acknowledgement of the receipt of the strings by returning the highest index received to P1. After receiving an acknowledgement the next 5 strings are sent.

Thus, after receiving the highest possible index ie. 50; the program stops.

## How to run the programs?

Clone the entire repository & go to the respective folders.

Use `make` to compile the 2 programs P1 & P2 in each case.

The 2 programs must be run simultaneosly in 2 different terminals to see the resulting Inter Process Communication.

Run the following commands in 2 different terminals in the same order.
```
./p2
./p1
```



