/*******************************************************************************
PAW3395 / PAW3950 光位移传感器驱动（软件 SPI 版）
基于逐飞 PMW3901 驱动框架 + gitee stm32_paw3395_mouse 初始化序列改写
软件 SPI：SCLK=P14_4, MOSI=P12_5, MISO=P12_4, NCS=P14_5
*******************************************************************************/

#include "zf_common_headfile.h"
#include "driver_paw3395.h"

static soft_spi_info_struct paw3395_spi;                    // 软件 SPI 对象

int16 paw3395_delta_x = 0,  paw3395_delta_y = 0;            // 每周期位移增量
int32 paw3395_delta_x_i = 0, paw3395_delta_y_i = 0;         // 累计位移

//-------------------------------------------------------------------------------------------------------------------
// 写寄存器（PixArt 协议：地址字节 bit7 置 1）
//-------------------------------------------------------------------------------------------------------------------
static void paw3395_write_register(uint8 reg, uint8 data)
{
    PAW3395_NCS(0);
    soft_spi_write_8bit_register(&paw3395_spi, (uint8)(reg | 0x80u), data);
    PAW3395_NCS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// 读寄存器（PixArt 协议：地址字节 bit7 清 0）
//-------------------------------------------------------------------------------------------------------------------
static uint8 paw3395_read_register(uint8 reg)
{
    uint8 data = 0;
    PAW3395_NCS(0);
    soft_spi_read_8bit_registers(&paw3395_spi, (uint8)(reg & ~0x80u), &data, 1);
    PAW3395_NCS(1);
    return data;
}

//-------------------------------------------------------------------------------------------------------------------
// 上电初始化寄存器配置（完整序列，来自 PixArt PAW3395 参考驱动）
//-------------------------------------------------------------------------------------------------------------------
static void paw3395_power_up_init(void)
{
    uint8 read_tmp, i;

    paw3395_write_register(0x7F, 0x07);
    paw3395_write_register(0x40, 0x41);
    paw3395_write_register(0x7F, 0x00);
    paw3395_write_register(0x40, 0x80);
    paw3395_write_register(0x7F, 0x0E);
    paw3395_write_register(0x55, 0x0D);
    paw3395_write_register(0x56, 0x1B);
    paw3395_write_register(0x57, 0xE8);
    paw3395_write_register(0x58, 0xD5);
    paw3395_write_register(0x7F, 0x14);
    paw3395_write_register(0x42, 0xBC);
    paw3395_write_register(0x43, 0x74);
    paw3395_write_register(0x4B, 0x20);
    paw3395_write_register(0x4D, 0x00);
    paw3395_write_register(0x53, 0x0E);
    paw3395_write_register(0x7F, 0x05);
    paw3395_write_register(0x44, 0x04);
    paw3395_write_register(0x4D, 0x06);
    paw3395_write_register(0x51, 0x40);
    paw3395_write_register(0x53, 0x40);
    paw3395_write_register(0x55, 0xCA);
    paw3395_write_register(0x5A, 0xE8);
    paw3395_write_register(0x5B, 0xEA);
    paw3395_write_register(0x61, 0x31);
    paw3395_write_register(0x62, 0x64);
    paw3395_write_register(0x6D, 0xB8);
    paw3395_write_register(0x6E, 0x0F);
    paw3395_write_register(0x70, 0x02);
    paw3395_write_register(0x4A, 0x2A);
    paw3395_write_register(0x60, 0x26);
    paw3395_write_register(0x7F, 0x06);
    paw3395_write_register(0x6D, 0x70);
    paw3395_write_register(0x6E, 0x60);
    paw3395_write_register(0x6F, 0x04);
    paw3395_write_register(0x53, 0x02);
    paw3395_write_register(0x55, 0x11);
    paw3395_write_register(0x7A, 0x01);
    paw3395_write_register(0x7D, 0x51);
    paw3395_write_register(0x7F, 0x07);
    paw3395_write_register(0x41, 0x10);
    paw3395_write_register(0x42, 0x32);
    paw3395_write_register(0x43, 0x00);
    paw3395_write_register(0x7F, 0x08);
    paw3395_write_register(0x71, 0x4F);
    paw3395_write_register(0x7F, 0x09);
    paw3395_write_register(0x62, 0x1F);
    paw3395_write_register(0x63, 0x1F);
    paw3395_write_register(0x65, 0x03);
    paw3395_write_register(0x66, 0x03);
    paw3395_write_register(0x67, 0x1F);
    paw3395_write_register(0x68, 0x1F);
    paw3395_write_register(0x69, 0x03);
    paw3395_write_register(0x6A, 0x03);
    paw3395_write_register(0x6C, 0x1F);
    paw3395_write_register(0x6D, 0x1F);
    paw3395_write_register(0x51, 0x04);
    paw3395_write_register(0x53, 0x20);
    paw3395_write_register(0x54, 0x20);
    paw3395_write_register(0x71, 0x0C);
    paw3395_write_register(0x72, 0x07);
    paw3395_write_register(0x73, 0x07);
    paw3395_write_register(0x7F, 0x0A);
    paw3395_write_register(0x4A, 0x14);
    paw3395_write_register(0x4C, 0x14);
    paw3395_write_register(0x55, 0x19);
    paw3395_write_register(0x7F, 0x14);
    paw3395_write_register(0x4B, 0x30);
    paw3395_write_register(0x4C, 0x03);
    paw3395_write_register(0x61, 0x0B);
    paw3395_write_register(0x62, 0x0A);
    paw3395_write_register(0x63, 0x02);
    paw3395_write_register(0x7F, 0x15);
    paw3395_write_register(0x4C, 0x02);
    paw3395_write_register(0x56, 0x02);
    paw3395_write_register(0x41, 0x91);
    paw3395_write_register(0x4D, 0x0A);
    paw3395_write_register(0x7F, 0x0C);
    paw3395_write_register(0x4A, 0x10);
    paw3395_write_register(0x4B, 0x0C);
    paw3395_write_register(0x4C, 0x40);
    paw3395_write_register(0x41, 0x25);
    paw3395_write_register(0x55, 0x18);
    paw3395_write_register(0x56, 0x14);
    paw3395_write_register(0x49, 0x0A);
    paw3395_write_register(0x42, 0x00);
    paw3395_write_register(0x43, 0x2D);
    paw3395_write_register(0x44, 0x0C);
    paw3395_write_register(0x54, 0x1A);
    paw3395_write_register(0x5A, 0x0D);
    paw3395_write_register(0x5F, 0x1E);
    paw3395_write_register(0x5B, 0x05);
    paw3395_write_register(0x5E, 0x0F);
    paw3395_write_register(0x7F, 0x0D);
    paw3395_write_register(0x48, 0xDD);
    paw3395_write_register(0x4F, 0x03);
    paw3395_write_register(0x52, 0x49);
    paw3395_write_register(0x51, 0x00);
    paw3395_write_register(0x54, 0x5B);
    paw3395_write_register(0x53, 0x00);
    paw3395_write_register(0x56, 0x64);
    paw3395_write_register(0x55, 0x00);
    paw3395_write_register(0x58, 0xA5);
    paw3395_write_register(0x57, 0x02);
    paw3395_write_register(0x5A, 0x29);
    paw3395_write_register(0x5B, 0x47);
    paw3395_write_register(0x5C, 0x81);
    paw3395_write_register(0x5D, 0x40);
    paw3395_write_register(0x71, 0xDC);
    paw3395_write_register(0x70, 0x07);
    paw3395_write_register(0x73, 0x00);
    paw3395_write_register(0x72, 0x08);
    paw3395_write_register(0x75, 0xDC);
    paw3395_write_register(0x74, 0x07);
    paw3395_write_register(0x77, 0x00);
    paw3395_write_register(0x76, 0x08);
    paw3395_write_register(0x7F, 0x10);
    paw3395_write_register(0x4C, 0xD0);
    paw3395_write_register(0x7F, 0x00);
    paw3395_write_register(0x4F, 0x63);
    paw3395_write_register(0x4E, 0x00);
    paw3395_write_register(0x52, 0x63);
    paw3395_write_register(0x51, 0x00);
    paw3395_write_register(0x54, 0x54);
    paw3395_write_register(0x5A, 0x10);
    paw3395_write_register(0x77, 0x4F);
    paw3395_write_register(0x47, 0x01);
    paw3395_write_register(0x5B, 0x40);
    paw3395_write_register(0x64, 0x60);
    paw3395_write_register(0x65, 0x06);
    paw3395_write_register(0x66, 0x13);
    paw3395_write_register(0x67, 0x0F);
    paw3395_write_register(0x78, 0x01);
    paw3395_write_register(0x79, 0x9C);
    paw3395_write_register(0x40, 0x00);
    paw3395_write_register(0x55, 0x02);
    paw3395_write_register(0x23, 0x70);
    paw3395_write_register(0x22, 0x01);

    system_delay_ms(1);

    for(i = 0; i < 60; i++)
    {
        read_tmp = paw3395_read_register(0x6C);
        if(read_tmp == 0x80) break;
        system_delay_ms(1);
    }
    if(i == 60)
    {
        paw3395_write_register(0x7F, 0x14);
        paw3395_write_register(0x6C, 0x00);
        paw3395_write_register(0x7F, 0x00);
    }
    paw3395_write_register(0x22, 0x00);
    paw3395_write_register(0x55, 0x00);
    paw3395_write_register(0x7F, 0x07);
    paw3395_write_register(0x40, 0x40);
    paw3395_write_register(0x7F, 0x00);
}

//-------------------------------------------------------------------------------------------------------------------
// 读取运动数据（Motion Burst，12 字节，16-bit delta）
//-------------------------------------------------------------------------------------------------------------------
void paw3395_get_motion(void)
{
    uint8 buf[12];
    PAW3395_NCS(0);
    soft_spi_read_8bit_registers(&paw3395_spi, PAW3395_MOTION_BURST, buf, 12);
    PAW3395_NCS(1);

    paw3395_delta_x = (int16)((buf[3] << 8) | buf[2]);      // 解析 X 位移（16-bit 有符号）
    paw3395_delta_y = (int16)((buf[5] << 8) | buf[4]);      // 解析 Y 位移（16-bit 有符号）

    paw3395_delta_x_i += paw3395_delta_x;                   // 积分累计位移
    paw3395_delta_y_i += paw3395_delta_y;
}

//-------------------------------------------------------------------------------------------------------------------
// 设置分辨率（CPI，需为 50 的整数倍）
//-------------------------------------------------------------------------------------------------------------------
void paw3395_set_cpi(uint16 cpi)
{
    uint16 v = cpi / 50;
    PAW3395_NCS(0);
    soft_spi_write_8bit_register(&paw3395_spi, PAW3395_MOTION_CTRL | 0x80u, 0x00);
    PAW3395_NCS(1);

    paw3395_write_register(PAW3395_RESOLUTION_X_L, (uint8)(v & 0xFF));
    paw3395_write_register(PAW3395_RESOLUTION_X_H, (uint8)(v >> 8));
    paw3395_write_register(PAW3395_SET_RESOLUTION, 0x01);
}

//-------------------------------------------------------------------------------------------------------------------
// 清零累计位移
//-------------------------------------------------------------------------------------------------------------------
void paw3395_clear_count(void)
{
    paw3395_delta_x_i = 0;
    paw3395_delta_y_i = 0;
}

//-------------------------------------------------------------------------------------------------------------------
// 初始化 PAW3395（上电序列 + 完整寄存器配置）
//-------------------------------------------------------------------------------------------------------------------
uint8 paw3395_init(void)
{
    // 初始化软件 SPI（SPI_MODE0，手动 CS）
    soft_spi_init(&paw3395_spi, SPI_MODE0, 20,
                  PAW3395_SCLK_PIN, PAW3395_MOSI_PIN, PAW3395_MISO_PIN, SOFT_SPI_PIN_NULL);
    gpio_init(PAW3395_NCS_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);   // 手动片选，初始拉高

    system_delay_ms(50);                                        // 上电等待

    // 复位 SPI 端口（NCS 高-低-高）
    PAW3395_NCS(0);
    PAW3395_NCS(1);
    PAW3395_NCS(0);

    // 写 0x5A 到 Power_Up_Reset
    paw3395_write_register(PAW3395_POWER_UP_RESET, 0x5A);
    system_delay_ms(5);

    // 上电初始化寄存器配置
    paw3395_power_up_init();

    // 读一次运动寄存器（0x02~0x06）
    paw3395_read_register(0x02);
    paw3395_read_register(0x03);
    paw3395_read_register(0x04);
    paw3395_read_register(0x05);
    paw3395_read_register(0x06);

    PAW3395_NCS(1);

    // 读 Product_ID 验证通信（PAW3395 = 0x51）
    uint8 id = paw3395_read_register(PAW3395_PRODUCT_ID);
    printf("[PAW3395] product_id = 0x%02X\n", id);

    return (id == 0x51) ? 0 : 1;                                // 0x51 为 PAW3395 的 ID
}
