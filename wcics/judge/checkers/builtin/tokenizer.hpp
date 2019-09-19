#pragma once

// checker that checks by tokens
// calls func with the tokens and the arg

typedef int (*check_token_sig) (const char*, const char*, const char*);

int check_tokens(const char* expected, const char* user, check_token_sig, const char* arg);