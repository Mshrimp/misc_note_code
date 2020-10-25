extern int test_1(void);
extern int test_2(void);

typedef struct {
    unsigned char *name;
    unsigned char *addr;
} symbol_t;

symbol_t symbol_tbl[] = {
    {"test_1", &(char *)test_1()},
    {"test_2", &(char *)test_2()},
};
