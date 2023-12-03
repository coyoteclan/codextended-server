//Converted from voron's libcod fork

#include "cracking.h"
#include <string.h>

void cracking_hook_function(int from, int to) {
    int relative = to - (from + 5);
    memset((void *)from, 0xE9, 1);
    memcpy((void *)(from + 1), &relative, 4);
}

void cracking_hook_call(int from, int to) {
    int relative = to - (from + 5);
    memcpy((void *)(from + 1), &relative, 4);
}

void cHook_init(cHook *hook, int from, int to)
{
    hook->from = from;
    hook->to = to;
}

void cHook_hook(cHook *hook) {
    memcpy((void *)hook->oldCode, (void *)hook->from, 5);
    cracking_hook_function(hook->from, hook->to);
}

void cHook_unhook(cHook *hook) {
    memcpy((void *)hook->from, (void *)hook->oldCode, 5);
}