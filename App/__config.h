#define ON 1
#define OFF 0

#define True 1
#define False 0

// The bit-vector in order to manage the logger on debugging.
#define Debug ((@Log_Console@) | (@Log_Trial@ << 1) | (@Log_Save@ << 2))

// The URL (a.k.a. endpoint) which is pre-defined
// Defining it will skip the first running the server.
#define URL "@Endpoint@"

// The length of the URL.
// Will be automatically defined as the length of the URL.
#define lURL @Endpoint_len@

// The file path for the file logger.
#define Path_Log "@Log_Path@"

// The port number pre-defined.
// Defininig it will skip the first input when running the server.
#define PORT "@Port_Num@"

// The length of the [PORT].
// Will be automatically defined as the length of the URL.
#define lPORT @Port_Num_Len@