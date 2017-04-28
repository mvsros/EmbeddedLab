
      voltage = AD7790_result() - 32767;
      if(--px > 97) px = 97;
      py = voltage / 512;
      lcd_put_pixel(px, py);