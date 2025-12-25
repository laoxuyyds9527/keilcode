#include "w25qxx_drv.h"
#include "spi_drv.h"
#include "stdio.h"
#include "systick.h"

//读取W25QXX的状态寄存器
uint8_t W25QXX_ReadSR(void)
{
    uint8_t byte=0;
    //使能器件
    SPI1_CS_CLR;
    //发送读取状态寄存器命令
    SPI1_ReadWriteByte(W25X_ReadStatusReg);
    //读取一个字节
    byte=SPI1_ReadByte();
    //取消片选 
    SPI1_CS_SET;
    
    return byte;
}

//写W25QXX状态寄存器
void W25QXX_Write_SR(uint8_t sr)
{
    //使能器件
    SPI1_CS_CLR;
    //发送写状态寄存器命令
    SPI1_ReadWriteByte(W25X_WriteStatusReg);
    //写入一个字节 
    SPI1_ReadWriteByte(sr);
    //取消片选 
    SPI1_CS_SET;
}

//等待空闲
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR()&0x01)==0x01);  // 等待BUSY位清空
}

//W25QXX写使能
void W25QXX_Write_Enable(void)
{
    //使能器件
    SPI1_CS_CLR;
    //发送写状态寄存器命令
    SPI1_ReadWriteByte(W25X_WriteEnable);
    //取消片选 
    SPI1_CS_SET;
}

//W25QXX写禁止
void W25QXX_Write_Disable(void) 
{
    //使能器件
    SPI1_CS_CLR;
    //发送写状态寄存器命令
    SPI1_ReadWriteByte(W25X_WriteDisable);
    //取消片选 
    SPI1_CS_SET;
}

//读取芯片ID
uint16_t W25QXX_ReadID(void)
{
    uint16_t temp=0;
    //使能器件
    SPI1_CS_CLR;
    //发送读取ID命令
    SPI1_ReadWriteByte(0x90);
    SPI1_ReadWriteByte(0x00);
    SPI1_ReadWriteByte(0x00);
    SPI1_ReadWriteByte(0x00);
    
    temp |=SPI1_ReadByte()<<8;
    temp |=SPI1_ReadByte();
    SPI1_CS_SET;
    return temp;
}

//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)
{
    uint16_t i;
    
    SPI1_CS_CLR;//拉低片选,开始通信
    //发送读数据指令
    SPI1_ReadWriteByte(W25X_ReadData);
    //发送24bit地址
    SPI1_ReadWriteByte((u8)((ReadAddr)>>16));
    SPI1_ReadWriteByte((u8)((ReadAddr)>>8));
    SPI1_ReadWriteByte((u8)ReadAddr);
    //循环读数据
    for(i=0;i<NumByteToRead;i++)
    {
        pBuffer[i]=SPI1_ReadByte();
    }
    SPI1_CS_SET;//拉高片选,结束通信
}

//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//写入的字节数不应该超过该页的剩余字节数
void W25QXX_WritePage(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
    uint16_t i;
    //SET WEL 写使能
    W25QXX_Write_Enable();
    SPI1_CS_CLR;
    //发送页写命令
    SPI1_ReadWriteByte(W25X_PageProgram);
    //发送24bit地址
    SPI1_ReadWriteByte((u8)((WriteAddr)>>16));
    SPI1_ReadWriteByte((u8)((WriteAddr)>>8));
    SPI1_ReadWriteByte((u8)((WriteAddr)));
    //循环写数据
    for(i=0;i<NumByteToWrite;i++)SPI1_ReadWriteByte(pBuffer[i]);  
	//取消片选
    SPI1_CS_SET;     
    //等待写入结束
	W25QXX_Wait_Busy();					   		
}

//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
    u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数 (WriteAddr%256计算的是这个地址在某一页的位置)		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		W25QXX_WritePage(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	}
}


//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个扇区的最少时间:150ms
void W25QXX_Erase_Sector(u32 Dst_Addr)   
{  
	//监视falsh擦除情况,测试用   
// 	printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096;
    W25QXX_Write_Enable();                  	//SET WEL 	 
    W25QXX_Wait_Busy();   
  	SPI1_CS_CLR;                            	//使能器件   
    SPI1_ReadWriteByte(W25X_SectorErase);      	//发送扇区擦除指令 
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  	//发送24bit地址    
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI1_ReadWriteByte((u8)Dst_Addr);  
	SPI1_CS_SET;                            	//取消片选     	      
    W25QXX_Wait_Busy();   				   		//等待擦除完成
}

//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大65535)   
u8 W25QXX_BUFFER[4096];		 
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			W25QXX_Erase_Sector(secpos);		//擦除这个扇区
			for(i=0;i<secremain;i++)	   		//复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  				//指针偏移
			WriteAddr+=secremain;				//写地址偏移	   
		   	NumByteToWrite-=secremain;			//字节数递减
			if(NumByteToWrite>4096)secremain=4096;//下一个扇区还是写不完
			else secremain=NumByteToWrite;		//下一个扇区可以写完了
		}	 
	}	 
}

//进入掉电模式
void W25QXX_PowerDown(void)   
{ 
  	SPI1_CS_CLR;                           	 	//使能器件   
    SPI1_ReadWriteByte(W25X_PowerDown);        //发送掉电命令  
	SPI1_CS_SET;                            	//取消片选     	      
    delay_us(3);                               //等待TPD  
}   
//唤醒
void W25QXX_WAKEUP(void)   
{  
  	SPI1_CS_CLR;                            	//使能器件   
    SPI1_ReadWriteByte(W25X_ReleasePowerDown);	//  send W25X_PowerDown command 0xAB    
	SPI1_CS_SET;                            	//取消片选     	      
    delay_us(3);                            	//等待TRES1
}

//块擦除 blockNB表示擦除第几块block
void W25QXX_EraseBlock(uint32_t blockNB)
{
    blockNB=blockNB*64*1024;
    W25QXX_Write_Enable();                  	//SET WEL 	 
    W25QXX_Wait_Busy();   
  	SPI1_CS_CLR;                            	//使能器件   
    SPI1_ReadWriteByte(W25X_BlockErase);      	//发送块区擦除指令 
    SPI1_ReadWriteByte((u8)((blockNB)>>16));  	//发送24bit地址    
    SPI1_ReadWriteByte((u8)((blockNB)>>8));   
    SPI1_ReadWriteByte((u8)blockNB);  
	SPI1_CS_SET;                            	//取消片选     	      
    W25QXX_Wait_Busy();   				   		//等待擦除完成
}


