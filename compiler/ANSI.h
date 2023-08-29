// Regular Colors
#define RC_Black "\e[0;30m"
#define RC_Red "\e[0;31m"
#define RC_Green "\e[0;32m"
#define RC_Yellow "\e[0;33m"
#define RC_Blue "\e[0;34m"
#define RC_Purple "\e[0;35m"
#define RC_Cyan "\e[0;36m"
#define RC_White "\e[0;37m"

// Bold
#define B_Black "\e[1;30m"
#define B_Red "\e[1;31m"
#define B_Green "\e[1;32m"
#define B_Yellow "\e[1;33m"
#define B_Blue "\e[1;34m"
#define B_Purple "\e[1;35m"
#define B_Cyan "\e[1;36m"
#define B_White "\e[1;37m"

// Underline
#define UL_Black "\e[4;30m"
#define UL_Red "\e[4;31m"
#define UL_Green "\e[4;32m"
#define UL_Yellow "\e[4;33m"
#define UL_Blue "\e[4;34m"
#define UL_Purple "\e[4;35m"
#define UL_Cyan "\e[4;36m"
#define UL_White "\e[4;37m"

// Background
#define BG_Black "\e[40m"
#define BG_Red "\e[41m"
#define BG_Green "\e[42m"
#define BG_Yellow "\e[43m"
#define BG_Blue "\e[44m"
#define BG_Purple "\e[45m"
#define BG_Cyan "\e[46m"
#define BG_White "\e[47m"

// High Intensty
#define HI_Black "\e[0;90m"
#define HI_Red "\e[0;91m"
#define HI_Green "\e[0;92m"
#define HI_Yellow "\e[0;93m"
#define HI_Blue "\e[0;94m"
#define HI_Purple "\e[0;95m"
#define HI_Cyan "\e[0;96m"
#define HI_White "\e[0;97m"

// Bold High Intensty
#define BHI_Black "\e[1;90m"
#define BHI_Red "\e[1;91m"
#define BHI_Green "\e[1;92m"
#define BHI_Yellow "\e[1;93m"
#define BHI_Blue "\e[1;94m"
#define BHI_Purple "\e[1;95m"
#define BHI_Cyan "\e[1;96m"
#define BHI_White "\e[1;97m"

// High Intensty backgrounds
#define HIBG_Black "\e[0;100m"
#define HIBG_Red "\e[0;101m"
#define HIBG_Green "\e[0;102m"
#define HIBG_Yellow "\e[0;103m"
#define HIBG_Blue "\e[0;104m"
#define HIBG_Purple "\e[0;105m"
#define HIBG_Cyan "\e[0;106m"
#define HIBG_White "\e[0;107m"

// Reset
#define Reset "\e[0m"

void print_lean_ronin()
{
    puts("\n\n" BHI_Green);
    puts("\t\t    _/_/_/                        _/        ");
    puts("\t\t   _/    _/    _/_/    _/_/_/        _/_/_/ ");
    puts("\t\t  _/_/_/    _/    _/  _/    _/  _/  _/    _/");
    puts("\t\t _/    _/  _/    _/  _/    _/  _/  _/    _/ ");
    puts("\t\t_/    _/    _/_/    _/    _/  _/  _/    _/  ");
    puts("\n\n" Reset);
}

void print_universal_ronin()
{
    puts("\n\n" BHI_Green);
    puts("\t\t88888888ba                            88               ");
    puts("\t\t88      \"8b                           \"\"               ");
    puts("\t\t88      ,8P                                            ");
    puts("\t\t88aaaaaa8P'  ,adPPYba,   8b,dPPYba,   88  8b,dPPYba,   ");
    puts("\t\t88\"\"\"\"88'   a8\"     \"8a  88P'   `\"8a  88  88P'   `\"8a  ");
    puts("\t\t88    `8b   8b       d8  88       88  88  88       88  ");
    puts("\t\t88     `8b  \"8a,   ,a8\"  88       88  88  88       88  ");
    puts("\t\t88      `8b  `\"YbbdP\"'   88       88  88  88       88  ");
    puts("\n\n" Reset);
}