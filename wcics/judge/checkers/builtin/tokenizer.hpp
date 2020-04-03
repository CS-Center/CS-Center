#pragma once

// checker that checks by tokens
// calls func with the tokens and the arg

typedef int (*check_token_sig) (const char*, const char*, const char*);

int check_tokens(int jfd, int ufd, check_token_sig, const char* arg);