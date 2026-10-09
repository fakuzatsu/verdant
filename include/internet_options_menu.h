#ifndef GUARD_InternetOptions_MENU_H
#define GUARD_InternetOptions_MENU_H

#define NO_PID 0xFFFFFFFF

// Selects this game's schema on the PokeMobile server.
#define INTERNET_GAME_IDENTIFIER "1VERDANT"

#define INTERNET_HMAC_SALT "POKEMOBILE-SESSION!!"
#define INTERNET_HMAC_SALT_SIZE 20

void CB2_InitInternetOptions(void);

#endif // GUARD_InternetOptions_MENU_H
