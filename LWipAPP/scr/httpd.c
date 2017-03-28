/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*                HTTP服务器源文件                 */
/*-------------------------------------------------*/
/*-------------------------------------------------*/

#include "httpd.h"
#include "fsdata.c"
#include "led.h"
#include "buzzer.h" 
#include "jdq.h" 

struct http_state
{
  char *file;
  u32_t left;
};

/*-----------------------------------------------------------------------------------*/
static void
conn_err(void *arg, err_t err)
{
  struct http_state *hs;

  hs = arg;
  mem_free(hs);
}
/*-----------------------------------------------------------------------------------*/
static void
close_conn(struct tcp_pcb *pcb, struct http_state *hs)
{

  tcp_arg(pcb, NULL);
  tcp_sent(pcb, NULL);
  tcp_recv(pcb, NULL);
  mem_free(hs);
  tcp_close(pcb);
}
/*-----------------------------------------------------------------------------------*/
static void
send_data(struct tcp_pcb *pcb, struct http_state *hs)
{
  err_t err;
  u16_t len;

  /* We cannot send more data than space avaliable in the send
     buffer. */
  if (tcp_sndbuf(pcb) < hs->left)
  {
    len = tcp_sndbuf(pcb);
  }
  else
  {
    len = hs->left;
  }

  err = tcp_write(pcb, hs->file, len, 0);

  if (err == ERR_OK)
  {
    hs->file += len;
    hs->left -= len;
  }
}

/*-----------------------------------------------------------------------------------*/
static err_t
http_poll(void *arg, struct tcp_pcb *pcb)
{
  if (arg == NULL)
  {
    /*    printf("Null, close\n");*/
    tcp_close(pcb);
  }
  else
  {
    send_data(pcb, (struct http_state *)arg);
  }

  return ERR_OK;
}
/*-----------------------------------------------------------------------------------*/
static err_t
http_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
  struct http_state *hs;

  hs = arg;

  if (hs->left > 0)
  {
    send_data(pcb, hs);
  }
  else
  {
    close_conn(pcb, hs);
  }

  return ERR_OK;
}

static err_t My_http_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
  int i, j;
  char *data;
  char fname[40];
  struct fs_file file = {0, 0};
  struct http_state *hs;

  hs = arg;

  if (err == ERR_OK && p != NULL)
  {

    /* Inform TCP that we have taken the data. */
    tcp_recved(pcb, p->tot_len);

    if (hs->file == NULL)
    {
      data = p->payload;
      if (strncmp(data, "GET /method=get", 15) == 0)
      {
        i = 15;
        BUZZE=0; 
        JDQ_IO=1; 
		LED_D8=1; 
		LED_D9=1; 
        while(data[i]!=0x20/*?*/)
        {
          i++; 
          if (data[i] == 0x6C /* l */)
          {
            i++;
            if (data[i] ==  0x65 /* e */)
            {
              i++;
              if (data[i] ==  0x64 /* d*/)
              {
                i+=2; 
                if(data[i]==0x31 /* 1 */)
                {
                   BUZZE=1;   
                }
  
                if(data[i]==0x32 /* 2 */)
                {
                  JDQ_IO=0;   
                }
        
                if(data[i]==0x33 /* 3 */)
                {
                  LED_D8=0;    
                }
            
                if(data[i]==0x34 /* 4 */)
                {
                  LED_D9=0; 
                }
              }   
            }
          } 
        }

        pbuf_free(p);

        fs_open("/STM32F107LED.html", &file);

        hs->file = file.data;
        hs->left = file.len;

        send_data(pcb, hs);

        /* Tell TCP that we wish be to informed of data that has been
           successfully sent by a call to the http_sent() function. */
        tcp_sent(pcb, http_sent);
      }    
      else if (strncmp(data, "GET ", 4) == 0) //标题网址
      {
        for (i = 0; i < 40; i++)
        {
          if (((char *)data + 4)[i] == ' ' ||
              ((char *)data + 4)[i] == '\r' ||
              ((char *)data + 4)[i] == '\n')
          {
            ((char *)data + 4)[i] = 0;
          }
        }
        
        i = 0;
        j = 0;
        
        do
        {
          fname[i] = ((char *)data + 4)[j];
          j++;
          i++;
        } while (fname[i - 1] != 0 && i < 40);
        
        pbuf_free(p);

        if (!fs_open(fname, &file))
        {
          fs_open("/STM32F107LED.html", &file);//打开主页
        }
        hs->file = file.data;
        hs->left = file.len;

        send_data(pcb, hs);

        /* Tell TCP that we wish be to informed of data that has been
           successfully sent by a call to the http_sent() function. */
        tcp_sent(pcb, http_sent);
      }
      else
      {
        close_conn(pcb, hs);
      }
    }
    else
    {
      pbuf_free(p);
    }
  }

  if (err == ERR_OK && p == NULL)
  {

    close_conn(pcb, hs);
  }
  
  return ERR_OK;
}
/*-----------------------------------------------------------------------------------*/
static err_t
http_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
  struct http_state *hs;

  /* Allocate memory for the structure that holds the state of the
     connection. */
  hs = mem_malloc(sizeof(struct http_state));

  if (hs == NULL)
  {
    return ERR_MEM;
  }

  /* Initialize the structure. */
  hs->file = NULL;
  hs->left = 0;

  /* Tell TCP that this is the structure we wish to be passed for our
     callbacks. */
  tcp_arg(pcb, hs);

  /* Tell TCP that we wish to be informed of incoming data by a call
     to the http_recv() function. */
  tcp_recv(pcb, My_http_recv);

  tcp_err(pcb, conn_err);

  tcp_poll(pcb, http_poll, 10);
  return ERR_OK;
}
/*-----------------------------------------------------------------------------------*/
void
httpd_init(void)
{
  struct tcp_pcb *pcb;

  pcb = tcp_new();
  tcp_bind(pcb, IP_ADDR_ANY, 80);
  pcb = tcp_listen(pcb);
  tcp_accept(pcb, http_accept);
}
/*-----------------------------------------------------------------------------------*/
int
fs_open(char *name, struct fs_file *file)
{
  struct fsdata_file_noconst *f;

  for (f = (struct fsdata_file_noconst *)FS_ROOT; f != NULL; f = (struct fsdata_file_noconst *)f->next)
  {
    if (!strcmp(name, f->name))
    {
      file->data = f->data;
      file->len = f->len;
      return 1;
    }
  }
  return 0;
}
/*-----------------------------------------------------------------------------------*/

