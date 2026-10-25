#include <stdio.h>

#ifdef KYOPRO_RUN_SOURCE_HASH
__attribute__((constructor)) static void kyopro_run_source_hash_banner() {
    fprintf(stderr, "[source hash] #%.12s\n", KYOPRO_RUN_SOURCE_HASH);
    fflush(stderr);
}
#endif
