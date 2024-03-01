//
// Character classification for shell
//
#ifndef CHARTYPE_H
#define CHARTYPE_H

//
// Character classification table 
//

#define CHAR_BITS  (sizeof(unsigned char) * 8)
#define CHAR_RANGE (1 << CHAR_BITS)

extern const unsigned short ctype[CHAR_RANGE];

//
// Character classes
//

#define C_UNDEF   0x000 // Undefined character class
#define C_SPACE   0x001 // A whitespace character
#define C_DIGIT   0x002 // A numerical digit
#define C_UPPER   0x004 // An uppercase char
#define C_LOWER   0x008 // A lowercase char
#define C_NAME    0x010 // An alphanumeric or underscore
#define C_SPCL    0x020 // Special parameter char
#define C_CTRL    0x040 // Control operator char
#define C_ESC     0x080 // Characters to escape before expansion
#define C_DESC    0x100 // Escapable characters in double quotation mode
#define C_QUOT    0x200 // Quotes

//
// Character class macros 
//

#define is_ctype(c, mask) (ctype[(int)(unsigned char)(c)] & (mask))

// Matches [ \t\n]
#define is_space(c) is_ctype((c), C_SPACE)

// Matches [0-9]
#define is_digit(c) is_ctype((c), C_DIGIT)

// Matches [A-Z]
#define is_upper(c) is_ctype((c), C_UPPER)

// Matches [a-z]
#define is_lower(c) is_ctype((c), C_LOWER)

// Matches [a-zA-Z]
#define is_alpha(c) is_ctype((c), C_LOWER | C_UPPER)

// Matches [a-zA-Z0-9]
#define is_alnum(c) is_ctype((c), C_LOWER | C_UPPER | C_DIGIT)

// Matches [@#?!$0-*] (special parameters)
#define is_spcl(c)  is_ctype((c), C_SPCL)

// Matches [;&|()] (control operator)
#define is_ctrl(c)  is_ctype((c), C_CTRL)

// Matches [*?[]\] (glob expansion escape stuff)
#define is_esc(c)   is_ctype((c), C_ESC)

// Matches [$`"] (double quote escape stuff)
#define is_desc(c)  is_ctype((c), C_DESC)

// Matches alpha, digit or underscore
#define is_name(c)  is_ctype((c), C_LOWER | C_UPPER | C_DIGIT | C_NAME)

// Matches either alpha, digit or underscore or special parameter
#define is_param(c) is_ctype((c), C_LOWER | C_UPPER | C_DIGIT | C_NAME | C_SPCL)

#endif
