/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*           串口IAP源文件,有个函数               */
/*-------------------------------------------------*/

/*-------------------------------------------------*/

#include "iap.h" 
#include "stmflash.h" 
#include "sd.h" 

/*-------------------------------------------------*/
/*函数名：跳转到应用程序段                         */
/*参  数：appxaddr:应用程序地址                    */
/*返回值：无                                       */
/*-------------------------------------------------*/
void IAP_load_app(u32 appxaddr)
{
	iapfun jump2app;
	
	jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
	MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
	jump2app();									//跳转到APP.
}

/*-------------------------------------------------*/
/*函数名：擦除APP用到的扇区5-11                    */
/*参  数：appxaddr:应用程序地址                    */
/*返回值：0：正确  其他：错误                      */
/*-------------------------------------------------*/
u8 Erase_APPSector(void)
{
	if(STMFLASH_Erase(FLASH_Sector_6)) return 2;   //擦除扇区6错误， 返回2
	if(STMFLASH_Erase(FLASH_Sector_7)) return 3;   //擦除扇区7错误， 返回3
	if(STMFLASH_Erase(FLASH_Sector_8)) return 4;   //擦除扇区8错误， 返回4
	if(STMFLASH_Erase(FLASH_Sector_9)) return 5;   //擦除扇区9错误， 返回5
	if(STMFLASH_Erase(FLASH_Sector_10))return 6;   //擦除扇区10错误，返回6
	if(STMFLASH_Erase(FLASH_Sector_11))return 7;   //擦除扇区11错误，返回7
	
	return 0;
}












