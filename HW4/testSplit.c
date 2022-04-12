#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#include "split.h"

static int
joinArgs(int argc, char **argv)
{
    if(argc < 2) {
        fprintf(stderr,
                "Usage: %s separator string1 string2 ...\n",
                argv[0]);
        return 1;
    }

    // argv[1][0] gives separator
    // remaining args give strings to join
    char *s = join(argv+2, argv[1][0]);
    puts(s);
    free(s);

    return 0;
}

static int
splitArg(int argc, char **argv)
{
    if(argc != 3) {
        fprintf(stderr,
                "Usage: %s separator string\n",
                argv[0]);
        return 1;
    }

    // argv[1][0] is separator
    // argv[2] is string to separate
    char **splits = split(argv[2], argv[1][0]);

    for(size_t i = 0; splits[i] != 0; i++) {
        puts(splits[i]);
    }

    freeSplit(splits);

    return 0;
}

// Marsaglia's xorshift PRNG
static uint64_t
xorshift64(uint64_t *seed)
{
	*seed ^= *seed << 13;
	*seed ^= *seed >> 7;
	*seed ^= *seed << 17;
	return *seed;
}


// FNV-1a hash function
#define FNV_PRIME_64 ((1ULL<<40)+(1<<8)+0xb3)
#define FNV_OFFSET_BASIS_64 (14695981039346656037ULL)

static void
hashUpdate(size_t n, const unsigned char *bytes, uint64_t *hash)
{
    uint64_t h = *hash;

    for(size_t i = 0; i < n; i++) {
        h ^= bytes[i];
        h *= FNV_PRIME_64;
    }

    *hash = h;
}

static uint64_t
checksum(size_t count, char **a, int argc, char **argv)
{
    uint64_t hash = FNV_OFFSET_BASIS_64;

    hashUpdate((count+1) * sizeof(char *), (unsigned char *) a, &hash);

    for(int i = 0; i < argc; i++) {
        hashUpdate(strlen(argv[i]), (unsigned char *) argv[i], &hash);
    }

    return hash;
}

static int 
joinMany(int argc, char **argv)
{
    if(argc < 4) {
        fprintf(stderr, 
                "Usage: %s separator count string1 string2 ...\n",
                argv[0]);
        return 1;
    }

    int c = argv[1][0];
    size_t count = strtoull(argv[2], 0, 0);

    int stringCount = argc - 3;
    char **strings = argv + 3;

    // generate a large pseudorandom array
    // sampling from argv
    uint64_t seed = 1337;

    char **a = calloc(count + 1, sizeof(char *));

    for(size_t i = 0; i < count; i++) {
        int sample = xorshift64(&seed) % stringCount;
        a[i] = strings[sample];
    }

    a[count] = 0;

    uint64_t h1 = checksum(count, a, argc, argv);

    char *j = join(a, c);
    puts(j);
    free(j);

    uint64_t h2 = checksum(count, a, argc, argv);

    free(a);

    if(h1 != h2) {
        fprintf(stderr, "%s: join arguments modified\n", argv[0]);
        return 1;
    }
    
    return 0;
}

static int
splitMany(int argc, char **argv)
{
    if(argc != 4) {
        fprintf(stderr, 
                "Usage: %s separator count string\n",
                argv[0]);
        return 1;
    }

    int c = argv[1][0];
    size_t count = strtoull(argv[2], 0, 0);
    char *urn = argv[3];
    size_t urnSize = strlen(urn);

    // generate a large pseudorandom string
    // sampling from urn
    uint64_t seed = 1337;

    char *s = malloc(count+1);

    for(size_t i = 0; i < count; i++) {
        int sample = xorshift64(&seed) % urnSize;
        s[i] = urn[sample];
    }

    s[count] = 0;

    char **a = split(s, c);
    for(size_t i = 0; a[i] != 0; i++) {
        puts(a[i]);
    }
    freeSplit(a);
    free(s);
    
    return 0;
}

struct command {
    // name of command
    // or null to end table
    const char *command;

    // Function to run.
    // argc and argv are shifted to include command
    // name but not program name.
    int (*function)(int argc, char **argv);

    // for command-line documentation
    const char *sampleArguments;
};

static struct command Dispatch[] = {
    { "join", joinArgs, "separator string1 string2 ..." },
    { "split", splitArg, "separator string" },
    { "joinMany", joinMany, "separator count string1 string2 ..." },
    { "splitMany", splitMany, "separator count string" },
    { 0, 0, 0 }
};

int
main(int argc, char **argv)
{
    if(argc == 1) {
        fprintf(stderr, "Usage: %s command args\n\nAvailable commands:\n\n", argv[0]);
        for(size_t i = 0; Dispatch[i].command != 0; i++) {
            fprintf(stderr, "%s %s %s\n", 
                    argv[0],
                    Dispatch[i].command,
                    Dispatch[i].sampleArguments);
        }

        return 1;
    }

    // look for matching command
    for(size_t i = 0; Dispatch[i].command != 0; i++) {
        if(!strcmp(Dispatch[i].command, argv[1])) {
            // got it
            return Dispatch[i].function(argc - 1, argv + 1);
        } 
    }
    
    // didn't get it
    fprintf(stderr,
            "%s: command %s not recognized; run with no arguments for list\n", 
            argv[0], 
            argv[1]);
    return 1;
}