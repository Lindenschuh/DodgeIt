/*
 * LEDs.h
 *
 * Created: 22.03.2017 13:55:27
 *  Author: RasuD
 */ 
#define LEDs0 0b11111111
#define LEDs1 0b01111111
#define LEDs2 0b00111111
#define LEDs3 0b00011111
#define LEDs4 0b00001111
#define LEDs5 0b00000111
#define LEDs6 0b00000011
#define LEDs7 0b00000001
#define LEDs8 0b00000000

#define DirectionPort DDRC
#define LEDPort PORTC //TODO: Port for LEDS

void ShowHealth(int health)
{
	DirectionPort = 0xff;
	
	switch(health)
	{
		case 0:
		LEDPort = LEDs0;
		break;
		
		case 1:
		LEDPort = LEDs1;
		break;
		
		case 2:
		LEDPort = LEDs2;
		break;
		
		case 3:
		LEDPort = LEDs3;
		break;
		
		case 4:
		LEDPort = LEDs4;
		break;
		
		case 5:
		LEDPort = LEDs5;
		break;
		
		case 6:
		LEDPort = LEDs6;
		break;
		
		case 7:
		LEDPort = LEDs7;
		break;
		
		case 8:
		LEDPort = LEDs8;
		break;
		
		default:
		PORTB = LEDs0;
		break;
		
	}
	
}