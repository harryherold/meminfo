# meminfo
Informs about the memory cosumption and IO operations of an application

## Build
```
~> gcc -Wall meminfo.c -o meminfo
```
## Execution
Spawns new process and analyzes it:

```
~> meminfo --spawn <application>
```

Analyzes running process:

```
~> meminfo --pid <pid>
```
## Example

```
~> meminfo --spawn ls
meminfo  meminfo.c

Memory Statistics:
VmPeak Virtual Memory     : 19704 kB
VmSize Virtual Memory     : 19704 kB
VmHWM Peak Physical Memory: 1084 kB
VmRSS Physical Memory     : 1084 kB
VmData Data Segment       : 220 kB
VmStk Stack Segment       : 140 kB
VmExe Text Segment        : 120 kB
VmSwap Swapped Memory     : 0 kB

File I/O Statistics:
Bytes read:    0
Bytes written: 0
```

```
~> meminfo --pid 11489

Memory Statistics:
VmPeak Virtual Memory     : 44996 kB
VmSize Virtual Memory     : 44920 kB
VmHWM Peak Physical Memory: 5208 kB
VmRSS Physical Memory     : 5208 kB
VmData Data Segment       : 428 kB
VmStk Stack Segment       : 140 kB
VmExe Text Segment        : 676 kB
VmSwap Swapped Memory     : 0 kB

File I/O Statistics:
Bytes read:    45056
Bytes written: 8192
```
