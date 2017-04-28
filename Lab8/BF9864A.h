// назначение портов
#define		RESPORT   PORTC

// назначение выводов
#define		RES       PC3

void lcd_init(void);
void lcd_power_down(void);
void lcd_power_up(void);
void lcd_clear(void);
void lcd_load(void);
void lcd_update(void);
void lcd_put_pixel(unsigned char x, unsigned char y);
unsigned char rotate(unsigned char byte);
