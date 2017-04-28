#include "common.h"
#include "twi.h"
#include "BF9864A.h"

// ����� ������ ����������
unsigned char bank0[98];
unsigned char bank1[98];
unsigned char bank2[98];
unsigned char bank3[98];
unsigned char bank4[98];
unsigned char bank5[98];
unsigned char bank6[98];
unsigned char bank7[98];

unsigned char __flash point[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
                                 0x01, 0x02, 0x04, 0x08, 0x10, 0x40, 0x20, 0x10,
                                 0x08, 0x04, 0x02, 0x01, 0x80, 0x40, 0x20, 0x10,
                                 0x08, 0x04, 0x02, 0x01, 0x80, 0x40, 0x20, 0x80,
                                 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
                                 0x01, 0x02, 0x04, 0x08, 0x10, 0x80, 0x40, 0x20,
                                 0x10, 0x08, 0x04, 0x02, 0x01, 0x80, 0x40, 0x20,
                                 0x10, 0x08, 0x04, 0x02, 0x01, 0x80, 0x40, 0x20};

unsigned char __flash bank[] =  {0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
                                 0x06, 0x06, 0x06, 0x06, 0x06, 0x04, 0x04, 0x04,
                                 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05,
                                 0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x04,
                                 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
                                 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
                                 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02};

void lcd_init(void)
{
  CLRBIT(RESPORT, RES);           // ����� �������
  __delay_cycles(8);              // �������� 8 ������
  SETBIT(RESPORT, RES);           // ������� �� ������
  
  twi_start();
  twi_addr(0x78);                 // ����� ���������� (SA0 = 0)
  twi_data(0x00);                 // ������ ����������� � ��������� ����������
  twi_data(0x21);                 // ����������� ����� ���������� (H = 1)
  twi_data(0x12);                 // ���������� ��������
  twi_data(0xCD);                 // ���������� Vop;
  twi_data(0x04);                 // ������������� ����������� TC = 0
  twi_data(0x08);                 // ������� ��� ������, TRS = 0, BRS = 0
  twi_data(0x20);                 // �������� ����� ���������� (H = 0)
  twi_data(0x13);                 // ��������� ���������� HV-�����������
  twi_data(0x05);                 // �������� Vop (PRS = 1)
  twi_data(0x0C);                 // ���������� ����� �������
  twi_stop();
}

void lcd_put_pixel(unsigned char x, unsigned char y)
{
  register unsigned char col;
  if((x < 98) && (y < 64))
    {
      twi_start();
      twi_addr(0x78);             // ����� ����������
      for(col = 0; col < 8; col++)
        {
          twi_data(0x80);         // ����������� ���� � ��������� ����������
          twi_data(0x40 | col);   // ������� ������ �����
          twi_data(0x80);         // ����������� ���� � ��������� ����������
          twi_data(0x80 | x);     // ������� ������ ����� � �����
          twi_data(0xC0);         // ����������� ���� � ��������� ������
          if(col == bank[y])
            twi_data(point[y]);   // �������� ���������� �����
          else twi_data(0x00);    // �������� ���������� �����
        }
      twi_stop();
    }
}

/*void lcd_col_off(unsigned char col)
{
  register unsigned char b;
  if(col < 98)
    {
      twi_start();
      twi_addr(0x78);             // ����� ����������
      for(b = 0; b < 8; b++)
        {
          twi_data(0x80);         // ����������� ���� � ��������� ����������
          twi_data(0x40 | b);     // ������� ������ �����
          twi_data(0x80);         // ����������� ���� � ��������� ����������
          twi_data(0x80 | col);   // ������� ������ ����� � �����
          twi_data(0xC0);         // ����������� ���� � ��������� ������
          twi_data(0x00);         // ��������
          twi_data(0xC0);         // ����������� ���� � ��������� ������
          twi_data(0x00);         // ��������
        }
      twi_stop();
    }
}*/

void lcd_power_down(void)
{
  twi_start();
  twi_addr(0x78);                 // ����� ���������� (SA0 = 0)
  twi_data(0x00);                 // ������ ����������� � ��������� ����������
  twi_data(0x24);                 // ������ �����
  twi_stop();
}

void lcd_power_up(void)
{
  twi_start();
  twi_addr(0x78);                 // ����� ���������� (SA0 = 0)
  twi_data(0x00);                 // ������ ����������� � ��������� ����������
  twi_data(0x20);                 // ����� �� ������� �����
  twi_stop();
}

void lcd_clear(void)
{
  register unsigned int k;
  twi_start();
  twi_addr(0x78);                 // ����� ����������
  twi_data(0x80);                 // ����������� ���� � ��������� ����������
  twi_data(0x40);                 // ����� ������ ������
  twi_data(0x80);                 // ����������� ���� � ��������� ����������
  twi_data(0x80);                 // ����� ������� ��������
  twi_data(0x40);                 // ��������� ����������� ���� � ����� ������
  for(k = 0; k < 918; k++)
    twi_data(0x00);
  twi_stop();
}

/*void lcd_update(void)
{
  register unsigned char byte;    // ���������� �� 72ms �� 4MHz � I2C 125kHz
  twi_start();
  twi_addr(0x78);                 // ����� ����������
  twi_data(0x80);                 // ����������� ����
  twi_data(0x40);                 // ������� (����� ������ ������)
  twi_data(0x80);                 // ����������� ����
  twi_data(0x80);                 // ������� (����� ������� ��������)
  twi_data(0x40);                 // ��������� ����������� ���� � ����� ������
  
  for(byte = 0; byte < 98; byte++)
    {
      twi_data((bank5[byte] << 5) | (bank6[byte] >> 3));
    }
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  
  for(byte = 0; byte < 98; byte++)
    {
      twi_data((bank6[byte] << 5) | (bank7[byte] >> 3));
    }
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  
  for(byte = 0; byte < 98; byte++)
    {
      twi_data((bank7[byte] << 5) | (rotate(bank5[byte] & 0xF8)));
    }
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  
  for(byte = 0; byte < 98; byte++)
    {
      twi_data(rotate(bank4[byte]));
    }
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  
  for(byte = 0; byte < 98; byte++)
    {
      twi_data((bank3[byte] << 7) | (bank2[byte] >> 4) | ((bank1[byte] & 0x07) << 4));
    }
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  
  for(byte = 0; byte < 98; byte++)
    {
      twi_data((bank3[byte] >> 4) | (bank2[byte] << 4));
    }
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  
  for(byte = 0; byte < 98; byte++)
    {
      twi_data(((bank3[byte] & 0xFE) << 4) | (rotate(bank1[byte] & 0xF8)));
    }
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  
   for(byte = 0; byte < 98; byte++)
    {
      twi_data(rotate(bank0[byte]));
    }
  twi_stop();
}

void lcd_update(void)
{
  register unsigned char byte;    // ���������� �� 72ms �� 4MHz � I2C 125kHz
  twi_start();
  twi_addr(0x78);                 // ����� ����������
  twi_data(0x80);                 // ����������� ����
  twi_data(0x40);                 // ������� (����� ������ ������)
  twi_data(0x80);                 // ����������� ����
  twi_data(0x80);                 // ������� (����� ������� ��������)
  twi_data(0x40);                 // ��������� ����������� ���� � ����� ������
  
  for(byte = 0; byte < 98; byte++)
    {
      twi_data(bank0[byte]);
    }
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  
  for(byte = 0; byte < 98; byte++)
    {
      twi_data(bank1[byte]);
    }
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  
  for(byte = 0; byte < 98; byte++)
    {
      twi_data(bank2[byte]);
    }
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  
  for(byte = 0; byte < 98; byte++)
    {
      twi_data(bank3[byte]);
    }
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  
  for(byte = 0; byte < 98; byte++)
    {
      twi_data(bank4[byte]);
    }
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  
  for(byte = 0; byte < 98; byte++)
    {
      twi_data(bank5[byte]);
    }
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  
  for(byte = 0; byte < 98; byte++)
    {
      twi_data(bank6[byte]);
    }
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  twi_data(0x00);
  
   for(byte = 0; byte < 98; byte++)
    {
      twi_data(bank7[byte]);
    }
  twi_stop();
}

void lcd_update(void)
{
  register unsigned char m;
  for(m = 0; m < 64; m++)
    {
      lcd_pixel_on(m, m);
    }
}*/

unsigned char rotate(unsigned char byte)
{
  register unsigned char i;
  for(i = 0; i < 8; i++)          // ����� ����� �� �����
    {
      asm("lsl r16");
      asm("ror r15");             // ����� �� ��������� ������� ����� �������
    }
  asm("mov r16, r15");
  return byte;                    // ������� ������������ ����
}

void lcd_load(void)
{
  register unsigned char byte;
  bank0[0] = 0x7F;
  bank0[1] = 0xA0;
  bank0[2] = 0xE0;
  bank0[3] = 0xF9;

  bank1[5] = 0x7F;
  bank1[6] = 0xA0;
  bank1[7] = 0xE0;
  bank1[8] = 0xF9;

  bank2[10] = 0x7F;
  bank2[11] = 0xA0;
  bank2[12] = 0xE0;
  bank2[13] = 0xF9;
  
  bank3[15] = 0x7F;
  bank3[16] = 0xA0;
  bank3[17] = 0xE0;
  bank3[18] = 0xF9;
  
  bank4[20] = 0x7F;
  bank4[21] = 0xA0;
  bank4[22] = 0xE0;
  bank4[23] = 0xF9;

  bank5[25] = 0x7F;
  bank5[26] = 0xA0;
  bank5[27] = 0xE0;
  bank5[28] = 0xF9;

  bank6[30] = 0x7F;
  bank6[31] = 0xA0;
  bank6[32] = 0xE0;
  bank6[33] = 0xF9;

  bank7[35] = 0x7F;
  bank7[36] = 0xA0;
  bank7[37] = 0xE0;
  bank7[38] = 0xF9;
  
  for(byte = 0; byte < 39; byte++)
    {
      bank0[byte + 40] = bank7[byte];
      bank1[byte + 40] = bank6[byte];
      bank2[byte + 40] = bank5[byte];
      bank3[byte + 40] = bank4[byte];
      bank4[byte + 40] = bank3[byte];
      bank5[byte + 40] = bank2[byte];
      bank6[byte + 40] = bank1[byte];
      bank7[byte + 40] = bank0[byte];
    }
}
