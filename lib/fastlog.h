#ifndef __fastlog_h
#define __fastlog_h

/*
 * Copyright Mark Veltzer <mark.veltzer@gmail.com>, 2011
 */

/*
 * Initialize the fastlog system. This will spawn a lower priority thread to do the
 * logging for you. It is an expensive call and should be used once at the begining
 * of the application.
 */
void fastlog_init(void);

/*
 * Close the fastlog system. This will shut down the thread that does the logging.
 * This is a blocking syscall and may delay you until the thread that does the logging
 * shuts down.
 */
void fastlog_close(void);


#endif // __fastlog_h
