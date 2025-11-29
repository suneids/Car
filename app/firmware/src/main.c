#include "myhal/myhal.h"
#include "myhal/pwm.h"

typedef struct {
 unsigned char deg;
 signed char speed;
 unsigned char xor;
}Packet;

typedef struct {
 uint32_t deg[2];
 uint32_t speed;
}PacketNormalized;

const uint32_t PWMMax = 1999,
   EnginePWMMax = PWMMax,
   servoMax = PWMMax/10,
   servoMin = PWMMax/20,
   servoRange = servoMax - servoMin;

PacketNormalized np;
uint8_t forward = 1, packet_start = 0, nextByte = 0;
uint32_t lWheelforward = 78, rWheelforward = 102;
char currentPacket[4] = { 0 };

Packet packet = {0, 0, 0};

PacketNormalized PreparePacket(Packet p);
PacketNormalized FixTurn(PacketNormalized p);
void Normalize(PacketNormalized *p);



int main(void) {
	enableGPIOClock(GPIO_PORT_A);
	usartInit(USART_1, 9600);
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	timerInit(TIM_PORT_2, 71, PWMMax);
	//Настройка портов для ШИМ
	for(int i = 0; i < 4; i++){
	pinMode(GPIO_PORT_A, i, GPIO_MODE_OUTPUT_10MHz, GPIO_CNF_PUSH_PULL_ALT, 0);
	pwmInit(TIM_PORT_2, i+1);
	}

	pwmWrite(TIM_PORT_2, 1, rWheelforward);
	pwmWrite(TIM_PORT_2, 2, lWheelforward);
	pwmWrite(TIM_PORT_2, 3, 0);
	pwmWrite(TIM_PORT_2, 4, 0);

    while(1){
    	if(usartAvailable(USART_1) >= 4){
			uint8_t start = usartReadByte(USART_1);
			if(start != 0xFE) continue;
			// Читаем остальные 3 байта
			packet.deg = usartReadByte(USART_1);
			packet.speed = (int8_t)usartReadByte(USART_1);
			packet.xor = usartReadByte(USART_1);

			// Проверка XOR
			if(((uint8_t)packet.deg ^ (uint8_t)packet.speed) != packet.xor) continue;
			 if(packet.speed < 0){
				 packet.speed = -packet.speed;
				 forward = 0;
			 }
			 else{
				 forward = 1;
			 }
			 np = PreparePacket(packet);
			 np = FixTurn(np);
			 Normalize(&np);

			 // PWM серв
			 pwmWrite(TIM_PORT_2, 1, np.deg[0]);
			 pwmWrite(TIM_PORT_2, 2, np.deg[1]);
			 // PWM мотор
			 if(forward){
				 pwmWrite(TIM_PORT_2, 3, np.speed);
				 pwmWrite(TIM_PORT_2, 4, 0);
			 }
			 else {
				 pwmWrite(TIM_PORT_2, 3, 0);
				 pwmWrite(TIM_PORT_2, 4, np.speed);
			 }
		}
	}
}


PacketNormalized PreparePacket(Packet p){
	 PacketNormalized result;
	 result.speed = p.speed;
	 result.deg[0] = p.deg;
	 result.deg[1] = p.deg;
	 return result;
}


PacketNormalized FixTurn(PacketNormalized p){
	PacketNormalized result;
	result.speed = p.speed;
	uint32_t rightWheel = p.deg[1]+12;
	uint32_t leftWheel = p.deg[0]-12;
	if(rightWheel > rWheelforward){
		for(int i = 0; i < 20 && rightWheel <= 180; i++)
			rightWheel++;
	}
	else if(leftWheel < lWheelforward){
		for(int i = 0; i < 20 && leftWheel >= 0; i++)
			leftWheel--;
	}

	result.deg[0] = rightWheel;
	result.deg[1] = leftWheel;
	return result;
}


void Normalize(PacketNormalized *p){
	p->speed = (EnginePWMMax / 100 ) * p->speed;
	p->deg[0] = servoMin + servoRange * p->deg[0] / 180;
	p->deg[1] = servoMin + servoRange * p->deg[1] / 180;
}

