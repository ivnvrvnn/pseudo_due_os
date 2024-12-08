// pins configuration

// Video configuraton 
#undef USE_NTSC

// Build configuration
#define ENABLE_SERIAL
#define LOG_OUTPUT_TO_SERIAL

// Cursor settings
#define cursorBlinkDelay 30

// Keyboard keys config
#define OEM_ENTER 40
#define OEM_BACKSPACE 42

// Keybuffer configuration
#define KEYBUFFER_SIZE 256
#undef KEYPOINTER_SUPPORT_LARGER_DATA_TYPE // you can enable it, but its necessary

// Lastcmd size
#define LASTCMD_SIZE 256 // W.I.P. function
