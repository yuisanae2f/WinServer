#define ON 1
#define OFF 0

#define True 1
#define False 0

// The bit-vector in order to manage the logger on debugging.
#define Debug ((True) | (True << 1) | (True << 2))

// The URL (a.k.a. endpoint) which is pre-defined
// Defining it will skip the first running the server.
#define URL "192.168.0.2"

// The length of the URL.
// Will be automatically defined as the length of the URL.
#define lURL 11

// The file path for the file logger.
#define Path_Log "log.txt"

// The port number pre-defined.
// Defining it will skip the first input when running the server.
#define PORT "5000"

// The length of the [PORT].
// Will be automatically defined as the length of the URL.
#define lPORT 4
