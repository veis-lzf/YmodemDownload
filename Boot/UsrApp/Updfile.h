#ifndef __UPDFILE_H__
#define __UPDFILE_H__

#include <stdint.h>

#define MAX_FILENAME_SIZE	256

typedef enum
{
	UNKONW_FILE, // 未知
	BINARY_FILE, // bin格式文件
	HEX_FILE, // hex格式文件
	S19_FILE, // s19格式文件
} eFileType;

typedef struct 
{
	uint8_t file_name[MAX_FILENAME_SIZE]; // 文件名
	eFileType file_type; // 文件类型
	uint32_t file_size; // 文件大小
	uint32_t tm_modify; // 修改时间秒
} sFileType;


#endif /* __UPDFILE_H__ */
