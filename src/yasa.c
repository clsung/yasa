#include "suffix.h"
#include "mmap.h"
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <time.h>
#include <string.h>

extern FILE * debugfile;
int mmap_len;
static void usage(void);
int main (int argc, char **argv) {
    char *file_name = NULL;
    time_t p_s;
    clock_t s_c; // unsigned long

    char *mmap_ptr = NULL;
    int return_val;
    unsigned int len;

    int dflag, ch, uflag = 1; // default utf8
    char *ep;
    int file_flag;

    int min_length = 1;
    int max_length = 40;

    dflag = file_flag = 0;
    while ((ch = getopt(argc, argv, "dM:m:uUf:P:p:")) != -1) {
        switch (ch) {
        case 'd':
            dflag = 1;
            break;
        case 'u': // utf8 off
            uflag = 0;
            break;
        case 'U': // utf8 on
            uflag = 1;
            break;
        case 'f':
            file_name = optarg;
            file_flag = 1;
            break;
        case 'M':
            max_length = (int)strtol(optarg, &ep, 10);
            if (max_length < 0)
                errno = ERANGE;
            if (errno == EINVAL || errno == ERANGE)
                err(1, "invalid length: %s", optarg);
            if (*ep)
                errx(1, "%s", optarg);
            break;
        case 'm':
            min_length = (int)strtol(optarg, &ep, 10);
            if (min_length < 0)
                errno = ERANGE;
            if (errno == EINVAL || errno == ERANGE)
                err(1, "invalid length: %s", optarg);
            if (*ep)
                errx(1, "%s", optarg);
            break;
        case '?':
        default:
            usage();
        }
    }
    argc -= optind;
    argv += optind;

    if (file_flag == 0)
        file_name = argv[0];
    if (file_name == NULL) {
        usage();
    }
    fprintf (stderr, "Input file is %s\n",file_name);
    fflush(stderr);
    p_s = time(NULL);
    s_c = clock();
    fprintf (stderr, "Input file is hi\n",file_name);
    fflush(stderr);
    debugfile = stderr;
    mmap_ptr = mmap_open_file(file_name);
    fprintf (stderr, "Input file is hi\n",file_name);
    fflush(stderr);
    if (!mmap_ptr) return errno;
    fprintf (stderr, "Input file is %d\n",mmap_ptr);
    fflush(stderr);
    len = mmap_len;
//    len = strlen(mmap_ptr);
    fprintf (stderr, "Input file is %d\n",len);
    fflush(stderr);
    return_val = do_suffix(mmap_ptr, uflag, max_length, min_length, NULL);
    mmap_close (mmap_ptr, len);
    return return_val;
}

static void
usage(void) {
    fprintf(stderr, "usage: yasa [-d] [-u|-U] [-M max_length] [-m min_length] [-f] file\n");
    exit(1);
}
