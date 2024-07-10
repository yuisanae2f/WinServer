# WinServer
> JSON throwing socket server only works on Windows.  
> GET for reading, POST for writing

# GET Method
## Path
> The path of the JSON to read.

> Thinks only one: path  
> Reads all data from path and toss it

# POST Method
## Path
> The path of the JSON to write.

## add method
0. Set
1. ListAppend
2. Remove

## Parameters
> You must start with a letter `!`.

> The first "arg" must be the length of the parameter vector.  
> Then the parameter vector to parse would come.

> The each element of the paramter may look like this:
```c
struct el_Parameter_ {
	unsigned std::string Key;
}
```

> The leftover bytes will be parsed as JSON to write.