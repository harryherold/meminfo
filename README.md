# meminfo
Informs about the memory cosumption of an application

## Build
```
~> gcc -Wall meminfo.c -o meminfo
```
## Execution
```
~> meminfo <application>
```
## Example
```
~> meminfo ls
```
Output:
```
meminfo  meminfo.c

Memory Statistics:
Virtual Memory : 126844 kB
Physical Memory: 1044 kB
Data Segment   : 240 kB
Stack Segment  : 136 kB
Text Segment   : 120 kB
```
