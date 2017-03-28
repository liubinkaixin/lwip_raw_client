/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/

#ifndef __HTTPD_H__
#define __HTTPD_H__

#include "main.h"
#include "fsdata.h"

struct fs_file {
  char *data;
  int len;
};

int fs_open(char *name, struct fs_file *file);
void httpd_init(void);

#endif /* __HTTPD_H__ */
