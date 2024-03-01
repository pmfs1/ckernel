//
// Utility for generating import definitions for DLLs
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <os/pe.h>

void *get_rva(char *dll, struct image_header *imghdr, unsigned long rva) {
    int i;

    // Find section containing the relative virtual address
    for (i = 0; i < imghdr->header.number_of_sections; i++) {
        struct image_section_header *sect = &imghdr->sections[i];
        if (sect->virtual_address <= rva && sect->virtual_address + sect->size_of_raw_data > rva) {
            return dll + sect->pointer_to_raw_data + (rva - sect->virtual_address);
        }
    }
    return NULL;
}

void *get_image_directory(char *dll, struct image_header *imghdr, int dir) {
    return get_rva(dll, imghdr, imghdr->optional.data_directory[dir].virtual_address);
}

int main(int argc, char *argv[]) {
    char *dll_filename;
    char *def_filename;
    char *dll;
    int fd;
    unsigned int size, i;
    struct stat st;
    struct dos_header *doshdr;
    struct image_header *imghdr;
    struct image_export_directory *expdir;
    unsigned long *names;
    unsigned short *ordinals;
    char *module_name;
    FILE *output;

    // Parse command line
    if (argc == 2) {
        dll_filename = argv[1];
        def_filename = NULL;
    } else if (argc == 3) {
        dll_filename = argv[1];
        def_filename = argv[2];
    } else {
        fprintf(stderr, "usage: impdef <dll> [<def>]\n");
        return 1;
    }

    // Load DLL image into memory
    fd = open(dll_filename, O_RDONLY | O_BINARY);
    if (fd < 0 || fstat(fd, &st) < 0) {
        perror(dll_filename);
        return 1;
    }
    size = st.st_size;
    dll = (char *) malloc(size);
    if (read(fd, dll, size) != size) {
        perror(dll_filename);
        free(dll);
        return 1;
    }
    close(fd);

    // Check PE file signature
    doshdr = (struct dos_header *) dll;
    imghdr = (struct image_header *) (dll + doshdr->e_lfanew);
    if (doshdr->e_lfanew > size || imghdr->signature != IMAGE_PE_SIGNATURE) {
        fprintf(stderr, "%s: Not a PE file\n", dll_filename);
        free(dll);
        return 1;
    }

    // Open output file.
    if (def_filename) {
        output = fopen(def_filename, "wb");
        if (output == NULL) {
            perror(def_filename);
            free(dll);
            return 1;
        }
    } else {
        output = stdout;
    }

    // Output exported functions.
    expdir = get_image_directory(dll, imghdr, IMAGE_DIRECTORY_ENTRY_EXPORT);
    module_name = get_rva(dll, imghdr, expdir->name);
    fprintf(output, "LIBRARY %s\r\n\r\nEXPORTS\r\n", module_name);
    names = get_rva(dll, imghdr, expdir->address_of_names);
    ordinals = get_rva(dll, imghdr, expdir->address_of_name_ordinals);
    for (i = 0; i < expdir->number_of_names; i++) {
        char *name = get_rva(dll, imghdr, names[i]);
        fprintf(output, "%s@%d\r\n", name, ordinals[i]);
    }

    if (def_filename) fclose(output);
    free(dll);
    return 0;
}
