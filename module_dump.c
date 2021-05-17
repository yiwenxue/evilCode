#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdint.h>

struct hw_module_t;
struct hw_device_t;

typedef struct hw_device_t {
    uint32_t tag;
    uint32_t version;
    struct hw_module_t *module;
    uint32_t reserved[12];
    int (*close)(struct hw_device_t *device);
} hw_device_t __attribute__((aligned(4)));

typedef struct hw_module_methods_t {
    int (*open)(struct hw_module_t const *module, char const *id, struct hw_device_t **device);
} hw_module_methods_t __attribute__((aligned(4)));

typedef struct hw_module_t {
    uint32_t tag;
    uint16_t mod_api_version;
    uint16_t hal_api_version;
    char const *id;
    char const *name;
    char const *author;
    struct hw_module_methods_t *methods;
    void *dso;
    uint32_t reserved[32-7];
} hw_module_t __attribute__((aligned(4)));

typedef struct app_args_t {
    char const *module;
} app_args_t;

static int parse_args(app_args_t *args, int argc, char **argv);
static void show_usage();
static int dump_module(hw_module_t const *mod);
static void mem_dump(void const *addr, size_t length);

int main(int argc, char **argv)
{
    app_args_t args = {
        .module = NULL,
    };

    int err = 0;
    void *dlh = NULL;

    err = parse_args(&args, argc, argv);
    if (0 != err || !args.module) {
        err = EINVAL;
        show_usage();
        goto cleanup;
    }

    dlh = dlopen(args.module, RTLD_LAZY);
    if (NULL == dlh) {
        err = errno;
        fprintf(stderr, "cannot load module (%s).\n", strerror(errno));
        goto cleanup;
    }

    dlerror();
    hw_module_t *hm = (hw_module_t*)dlsym(dlh, "HMI");
    if (NULL == hm) {
        err = errno;
        fprintf(stderr, "cannot find symbol \"HMI\" (%s).\n", dlerror());
        goto cleanup;
    }

    hm->dso = dlh;

    err = dump_module(hm);

cleanup:
    if (NULL != dlh) {
        dlclose(dlh);
    }
    return err;
}

static int parse_args(app_args_t *args, int argc, char **argv)
{
    int opt;
    while (-1 != (opt = getopt(argc, argv, "m:"))) {
        switch (opt) {
        case 'm':
            args->module = optarg;
            break;
        default:
            return EINVAL;
        }
    }
    return 0;
}

static void show_usage()
{
    printf("Usage: hmidump\n");
}

typedef struct module_registry_t {
    char const *id;
    int (*dump)(hw_module_t const *mod);
} module_registry_t;

static int mod_gralloc_dump(hw_module_t const *mod);

static module_registry_t MODULES[] = {
    { "gralloc", mod_gralloc_dump, },
    { NULL, NULL },
};

static int dump_module(hw_module_t const *mod)
{
    int i;
    union tag_t {
        uint32_t tag32;
        char tag8[4];
    } tag = {
        mod->tag,
    };

    printf("tag                = %c%c%c%c\n", tag.tag8[3], tag.tag8[2], tag.tag8[1], tag.tag8[0]);
    printf("module_api_version = %d.%d\n", (mod->mod_api_version >> 8) & 0xff, mod->mod_api_version & 0xff);
    printf("hal_api_version    = %d.%d\n", (mod->hal_api_version >> 8) & 0xff, mod->hal_api_version & 0xff);
    printf("id                 = %s\n", mod->id ? mod->id : "(null)");
    printf("name               = %s\n", mod->name ? mod->name : "(null)");
    printf("author             = %s\n", mod->author ? mod->author : "(null)");
    printf("methods            = %p\n", mod->methods);
    if (mod->methods) {
    printf("  .open            = %p\n", mod->methods->open);
    }
    printf("dso                = %p\n", mod->dso);

    if (!mod->id) {
        return 0;
    }

    for (i = 0; NULL != MODULES[i].id; ++i) {
        if (0 == strcmp(MODULES[i].id, mod->id)) {
            return MODULES[i].dump(mod);
        }
    }

    return 0;
}

typedef struct gralloc_module_t {
    struct hw_module_t common;
    int (*registerBuffer)();
    int (*unregisterBuffer)();
    int (*lock)();
    int (*unlock)();
    int (*perform)();
    void *reserved_proc[7];
} gralloc_module_t __attribute__((aligned(8)));

typedef struct gralloc_module_public_t {
    gralloc_module_t base;
    void *pFrameBufferDevice;
    int (*GetPhyAddrs)();
    int (*Blit)();
    int (*Blit2)();
} gralloc_module_public_t;

static int mod_gralloc_dump(hw_module_t const *mod)
{
    gralloc_module_t const *gralloc = (gralloc_module_t const *)mod;

    printf("registerBuffer     = %p\n", gralloc->registerBuffer);
    printf("unregisterBuffer   = %p\n", gralloc->unregisterBuffer);
    printf("lock               = %p\n", gralloc->lock);
    printf("unlock             = %p\n", gralloc->unlock);
    printf("perform            = %p\n", gralloc->perform);

    if (0 == strcmp(mod->author, "Imagination Technologies")) {
    gralloc_module_public_t const *gmp = (gralloc_module_public_t const *)gralloc;
    printf("pFrameBufferDevice = %p\n", gmp->pFrameBufferDevice);
    printf("GetPhyAddrs        = %p\n", gmp->GetPhyAddrs);
    printf("Blit               = %p\n", gmp->Blit);
    printf("Blit2              = %p\n", gmp->Blit2);
    mem_dump(mod, sizeof(gralloc_module_public_t) + 128);
    } else {
    mem_dump(mod, sizeof(gralloc_module_t) + 128);
    }

    return 0;
}

static void mem_dump(void const *addr, size_t length)
{
    uintptr_t ui_addr = (uintptr_t)addr;
    uintptr_t const ui_begin = ui_addr;
    uintptr_t const ui_end = ui_addr + length;
    int i;

    if (0 != ui_addr & 0x1f) {
        ui_addr -= ui_addr & 0x1f;
    }

    printf("---- DUMP ----------------------\n");
    printf("address: %p\n", addr);
    printf("size   : %u\n", length);
    while(ui_addr < ui_end) {
        printf("%p: ", ui_addr);
        for (i = 0; i < 32; ++i) {
            if (ui_addr >= ui_end) {
                break;
            }
            if (ui_begin > ui_addr) {
                printf("   ");
            } else {
                printf("%02X ", *(uint8_t*)ui_addr);
            }
            ++ui_addr;
        }
        printf("\n");
    }
    printf("\n");
}

