#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Arena {
    long size;
    void *memory;
    long currentUsage;
};

struct Arena* Arena_New(long bytes) {
    struct Arena *arena = malloc(sizeof(struct Arena));
    arena->currentUsage = 0;
    arena->memory = malloc(bytes);
    arena->size = bytes;
    return arena;
}

void Arena_Reset(struct Arena *arena, int setToZeroes) {
    arena->currentUsage = 0;
    if (setToZeroes) memset(arena->memory, 0, arena->size);
}

void Arena_DeInit(struct Arena *arena) {
    free(arena->memory);
    free(arena);
}

void Arena_DumpInfo(struct Arena *arena) {
    printf("Size: %lu, currentUsage: %lu\n", arena->size, arena->currentUsage);
}

void* Arena_Allocate(struct Arena *arena, long bytes) {
    void *ptr = &arena->memory[arena->currentUsage];
    arena->currentUsage += bytes;
    if (arena->currentUsage > arena->size) return NULL;
    return ptr;
}

#define TEST 0
#if TEST
char* textString(struct Arena *arena, char *text) {
    char *textResult = Arena_Allocate(arena, strlen(text));
    printf("Did overflow: %i\n", textResult == NULL);
    strcpy(textResult, text);
    return textResult;
}

int main() {
    struct Arena *arena = Arena_New(1024);
    char *text = textString(arena, "Hello");
    printf("%s\n", text);
    Arena_DumpInfo(arena);
    Arena_Reset(arena, 0);
    Arena_DumpInfo(arena);
    char *text2 = textString(arena, "Finally");
    // They use the same memory, so they should match.
    printf("%s, %s\n", text, text2);
    Arena_DumpInfo(arena);
    Arena_Reset(arena, 1);
    // Memset to 0, so text, text 2 should be 0'ed.
    printf("%s, %s\n", text, text2);
    return 0;
}
#endif
