#define S5eS6_verde    0b00000001	//1
#define S4_verde 	   0b00000010	//2
#define S1_verde 	   0b00000100	//4
#define S1_amarelo 	   0b00001000	//8
#define S1_vermelho    0b00010000	//16
#define S2eS3_verde    0b00100000	//32
#define S2eS3_amarelo  0b01000000	//64
#define S2eS3_vermelho 0b10000000	//128

#define DATA  2
#define LATCH 3
#define CLOCK 4

int estados;
int execucao;
int tempo;
int tempoExtra;

int estado01;
int estado02;
int estado03;
int estado04;

unsigned long inicioCiclo;
unsigned long cicloExtra;

bool botaoPressionado;

void setup()
{
  estados = 0;
  inicioCiclo = 0;
  cicloExtra = 0;
  tempoExtra = 0;
  botaoPressionado = false;
  
  estado01 = S1_verde + S2eS3_vermelho;
  estado02 = S1_amarelo + S2eS3_vermelho;
  estado03 = S1_vermelho + S2eS3_verde + S4_verde;  
  estado04 = S1_vermelho + S2eS3_amarelo + S4_verde;
  
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  
  pinMode(13, OUTPUT);  
  pinMode(5 , INPUT ) ;
}

void loop()
{
  if (inicioCiclo == 0)
  {
    switch (estados)
    {
      // Estado 1
      case 0:
        execucao = estado01;
        tempo = 10000;  
      	cicloExtra = 0;
      	tempoExtra = 0;
      	botaoPressionado = false;
      break;

      // Estado 2    
      case 1:
      	execucao = estado02;
        tempo = 3000;    
      break;

      // Estado 3
      case 2:
        execucao = estado03;
        tempo = 12000;  
      	if (botaoPressionado)
        {
          tempoExtra = 10000;
          cicloExtra = millis();
        }
      break;

      // Estado 4
      case 3:
        execucao = estado04;
        tempo = 3000;  
      	if (botaoPressionado)
        {
          execucao = estado04 + S5eS6_verde - S4_verde;  
        }  
      break;
    }

    digitalWrite(LATCH, LOW);   
    shiftOut(DATA, CLOCK, MSBFIRST, execucao);
    digitalWrite(LATCH, HIGH);  
    
    inicioCiclo = millis();
  }
  
  if(digitalRead(5) == LOW)
  {
    botaoPressionado = true;    
  }
    
  if((estados == 2) && (tempoExtra > 0) && (millis() - cicloExtra >= tempoExtra))
  {
    execucao = estado03 + S5eS6_verde - S4_verde; 
    digitalWrite(LATCH, LOW);   
    shiftOut(DATA, CLOCK, MSBFIRST, execucao);
    digitalWrite(LATCH, HIGH);  
  }
  
  if (millis() - inicioCiclo >= tempo)
  {
    inicioCiclo = 0;
    
    if (estados == 3)
    {
      estados = 0;
    } else {
      estados++;
    }    
  }  
  
  if (botaoPressionado)
  {
    digitalWrite(13, HIGH);  
  } else {
    digitalWrite(13, LOW);  
  }    
}
