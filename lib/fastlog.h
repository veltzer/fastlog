#ifndef __fastlog_h
#define __fastlog_h

/*
 * Initialize the fastlog system. This will spawn a lower priority thread to do the
 * logging for you.
 */
void fastlog_init(void);

/*
 * Close the fastlog system. This will shut down the thread that does the logging.
 */
void fastlog_close(void);


#endif // __fastlog_h
