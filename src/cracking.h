//Converted from voron's libcod fork

#ifndef _CRACKING_H_
#define _CRACKING_H_

void cracking_hook_function(int from, int to);
void cracking_hook_call(int from, int to);

typedef struct {
    int from;
    int to;
    unsigned char oldCode[5];
} cHook;

void cHook_init(cHook *hook, int from, int to);
void cHook_hook(cHook *hook);
void cHook_unhook(cHook *hook);

#endif