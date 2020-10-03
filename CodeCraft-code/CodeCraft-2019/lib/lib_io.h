#ifndef __LIB_IO_H__
#define __LIB_IO_H__

#define MAX_CAR_NUM    20000
#define MAX_CROSS_NUM  2000
#define MAX_ROAD_NUM   2000
//
//
//
extern int read_file(char ** const buff, const unsigned int spec, const char * const filename);

//
extern void write_result(const char * const buff, const char * const filename);

//
extern void release_buff(char ** const buff, const int valid_item_num);

#endif

