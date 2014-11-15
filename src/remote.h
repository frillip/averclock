void remote_init(void);
void remote_command(void);
void remote_feedback(void);
uint8_t offset=0;
char command[16];
char command_buffer[13];
boolean command_incoming=FALSE;
boolean command_waiting=FALSE;
boolean command_complete=FALSE;
boolean led_status = 0;

void remote_feedback(void)
{
	fprintf(COM1,"\r\n%02u:%02u:%02u\r\n",time.hours,time.minutes,time.seconds);
}

uint8_t strcmp(unsigned char *s1, unsigned char *s2)
{
   for (; *s1 == *s2; s1++, s2++)
      if (*s1 == '\0')
         return(1);
   return(0);
}

uint8_t strncmp(unsigned char *s1, unsigned char *s2, uint8_t n)
{
   for (; n > 0; s1++, s2++, n--)
      if (*s1 != *s2)
         return(0);
      else if (*s1 == '\0')
         return(1);
   return(1);
}


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
	if(command_incoming)
	{
		command_buffer[offset-3]=command[offset];
		if((command[offset]==0x00)||(command[offset]==0x0d))
		{
			command_incoming=FALSE;
			command_waiting=TRUE;
			command[offset]=0x00;
			command_buffer[offset-3]=command[offset];
			memset(command, 0, sizeof(command));
			offset=0;
			return;
		}
		offset++;
		if(offset==16)
		{
			command_incoming=FALSE;
			memset(command, 0, sizeof(command_buffer));
			memset(command, 0, sizeof(command));
			offset=0;
			fprintf(COM1, "Overflow!\r\n");
		}
	}
	else
	{
		if((offset==0)&&(command[0]=='A'))
		{
			offset++;
		}
		else if((offset==1)&&(command[1]=='T'))
		{
			offset++;
		}
		else if ((offset==2)&&(command[2]=='+'))
		{
			command_incoming=TRUE;
			offset++;
		}
		else offset=0;
	}
}

void process_command(void)
{
	command_waiting=FALSE;
	command_complete=FALSE;
	if(strcmp(command_buffer,"RESET"))
	{
		fprintf(COM1, "Resetting...\r\n");
		write_eeprom(EEPROM_RESET,0x42);			// Write reset flag
		write_eeprom(EEPROM_HOURS,time.hours);
		write_eeprom(EEPROM_MINUTES,time.minutes);
		write_eeprom(EEPROM_SECONDS,time.seconds);	// Write current time to EEPROM
#IFDEF DRINKING_GAME
		write_eeprom(EEPROM_SHOTS,shot_count);
#ENDIF
		delay_ms(10);
		reset_cpu();
	}
	else if(strcmp(command_buffer,"SECONDS"))
	{
		set_timer1(-32768);
		time.seconds=0;
		remote_feedback();
		update_display();
		command_complete=TRUE;
	}
	else if(strcmp(command_buffer,"MINUTES+"))
	{
		wallclock_inc_min();
		remote_feedback();
		update_display();
		command_complete=TRUE;
	}
	else if(strcmp(command_buffer,"MINUTES-"))
	{
		wallclock_dec_min();
		remote_feedback();
		update_display();
		command_complete=TRUE;
	}
	else if(strcmp(command_buffer,"HOURS+"))
	{
		wallclock_inc_hour();
		remote_feedback();
		update_display();
		command_complete=TRUE;
	}
	else if(strcmp(command_buffer,"HOURS-"))
	{
		wallclock_dec_hour();
		remote_feedback();
		update_display();
		command_complete=TRUE;
	}
	else if(strncmp(command_buffer,"TIME",4))
	{
		if(command_buffer[9])
		{
			time.seconds=(((uint8_t)command_buffer[8]-48)*10)+((uint8_t)command_buffer[9]-48);
			set_timer1(-32768);
		}
		if(command_buffer[7])
		{
			time.minutes=(((uint8_t)command_buffer[6]-48)*10)+((uint8_t)command_buffer[7]-48);
			time.hours=(((uint8_t)command_buffer[4]-48)*10)+((uint8_t)command_buffer[5]-48);
			update_display();
		}
		remote_feedback();
		command_complete=TRUE;
	}
	else if(strcmp(command_buffer,"ALARM"))
	{
		alarm=TRUE;
		manual_alarm=TRUE;
		alarm_count=0;
		command_complete=TRUE;
	}
	else if(strncmp(command_buffer,"LED",3))
	{
		if(command_buffer[3]=='0') led_status=0;
		else if (command_buffer[3]=='1') led_status=1;
		else led_status=!led_status;
		output_bit(PIN_A1,led_status);
		command_complete=TRUE;
	}
	#IFDEF DRINKING_GAME
	else if(strncmp(command_buffer,"SHOTS",5))
	{
		if(command_buffer[6]) shot_count=(((uint8_t)command_buffer[5]-48)*10)+((uint8_t)command_buffer[6]-48);
		fprintf(COM1,"%u\r\n", shot_count);
		command_complete=TRUE;
	}
	#ENDIF
	if(command_complete) fprintf(COM1,"OK\r\n");
	memset(command_buffer, 0, sizeof(command_buffer));
}