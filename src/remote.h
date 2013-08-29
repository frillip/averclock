void remote_init(void);
void remote_command(void);
void remote_feedback(void);

boolean led_status = 0;

#INT_RDA
void remote_command(void)
{
	char command = 0x00;
	command = fgetc(COM1);
	if(command!='R') fprintf(COM1, "%c",command);
	switch (command) {
		case 'm':
			wallclock_inc_min();
		break;

		case 'M':
			wallclock_dec_min();
		break;

		case 'h':
			wallclock_inc_hour();
		break;

		case 'H':
			wallclock_dec_hour();
		break;

		case 'l':
			led_status = !led_status;
			output_bit(PIN_A1,led_status);
		break;

		case 'b':
			alarm=TRUE;
			manual_alarm=TRUE;
			alarm_count=0;
		break;

		case 'R':
			write_eeprom(EEPROM_RESET,0x42);			// Write reset flag
			write_eeprom(EEPROM_HOURS,time.hours);
			write_eeprom(EEPROM_MINUTES,time.minutes);
			write_eeprom(EEPROM_SECONDS,time.seconds);	// Write current time to EEPROM
			reset_cpu();
		break;
	}
	remote_feedback();
	update_display();
}

void remote_feedback(void)
{
	fprintf(COM1,"\r\n%02u:%02u:%02u - %u\r\n",time.hours,time.minutes,time.seconds,light);
}

uint8_t offset=0;
char command[16];
boolean incoming=FALSE;
unsigned char reset_cmd[] = {"AT+RESET\0"}; 

uint8_t strcmp(unsigned char *s1, unsigned char const *s2)
{
   for (; *s1 == *s2; s1++, s2++)
      if (*s1 == '\0')
         return(0);
   return((*s1 < *s2) ? -1: 1);
}

/*
#INT_RDA
void remote_command(void)
{
	command[offset]=fgetc(COM1);
	if(command[offset]==0xff)
	{
		delay_ms(10);
		reset_cpu();
	}
	fprintf(COM1,"%c", command[offset]);
	if(command[offset]==0xd) fprintf(COM1,"\n");
	if(incoming==FALSE)
	{
		if(command[0]=='A')
		{
			if((offset)&&(command[1]=='T'))
			{
				if ((offset==2)&&(command[2]=='+'))
				{
					incoming=TRUE;
				}
				else offset=0;
				offset++;
			}
			else offset=0;
			offset++;
		}
		else offset=0;
	}
	else offset++;
	if(((command[offset-1]==0x00)||(command[offset-1]==0xd))&&(incoming))
	{
		incoming=FALSE;
		if(strcmp(command,reset_cmd))
		{
			fprintf(COM1, "Resetting...\r\n");
			delay_ms(10);
			reset_cpu();
		}
	}
	if(offset==16)
	{
		offset=0;
		fprintf(COM1, "\r\n");
		incoming=FALSE;
	}
}
*/
