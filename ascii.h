void ascii_read_init();
void ascii_write_init();

char *ascii_read();
void ascii_write(char *);

void ascii_read_destroy();
void ascii_write_destroy();

char *ascii_from_jpeg(FILE *);


#define ASCII_DELIMITER '\t'

#define ASCII_SLEEP_NS 50000L
