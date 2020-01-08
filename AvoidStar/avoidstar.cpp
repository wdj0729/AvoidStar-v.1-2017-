#include<stdio.h>
#include<windows.h>
#include<stdbool.h>
#include<conio.h>
#include<stdlib.h>
#include <mmsystem.h> 

#pragma comment(lib,"winmm.lib")

#define SCR_WIDTH 69 //�ܼ�â ����
#define SCR_HEIGHT 30 //�ܼ�â ����
#define Enemy_Num 25 //���� ����
#define Rand_Num 15 //�� y��ġ ����
#define Speed 80 //���� �������� �ӵ�

typedef struct //���� ����ü
{
	int state_; //ĳ������ ����
	int width_; //ĳ������ ����
	int height_; //ĳ������ ����

	int pos_x_; //x�� ��ġ
	int pos_y_; //y�� ��ġ

	int speed; //���� �ӵ�

	char* image_;
}Object, * pObject;

Object player;
Object enemy[Enemy_Num];

int score; //����
int life; //���

//���� ���۸�
char front_buffer[SCR_HEIGHT][SCR_WIDTH]; //���� ȭ�� ��Ȳ
char back_buffer[SCR_HEIGHT][SCR_WIDTH]; //�۾��� ȭ��

void moveCursorTo(const int x, const int y);// ��ǥ�̵��Լ�

void drawToBackBuffer(const int i, const int j, char* image) //������ ��ȭ��
{
	int ix = 0;
	while (1)
	{
		if (image[ix] == '\0')break;

		back_buffer[j][i + ix] = image[ix];

		ix++;
	}
}

void render()//��ǻ�� �׷����� ����� �ܰ�
{
	//�ٲ� �κ��� �ٽ� �׸�
	for (int j = 0; j < SCR_HEIGHT; j++)
		for (int i = 0; i < SCR_WIDTH; i++)
		{
			if (back_buffer[j][i] != front_buffer[j][i])
			{
				moveCursorTo(i, j);

				if (back_buffer[j][i] == '\0')
					printf("%c", ' ');
				else
					printf("%c", back_buffer[j][i]);
			}
		}
	//update frame buffer
	for (int j = 0; j < SCR_HEIGHT; j++)
		for (int i = 0; i < SCR_WIDTH; i++)
		{
			front_buffer[j][i] = back_buffer[j][i];
			back_buffer[j][i] = '\0';
		}
}

void moveCursorTo(const int x, const int y) //��ǥ �̵� �Լ�
{
	const COORD pos = { x,SCR_HEIGHT - y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


void setcolor(int color, int bgcolor) //���� ��ȭ
{
	color &= 0xf;
	bgcolor &= 0xf;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}

void drawBoundary() //���� ����
{
	int i = 0;

	for (int j = 0; j < SCR_WIDTH; j++)
	{
		drawToBackBuffer(i, j, "|");
	}

	i = SCR_WIDTH - 1;
	for (int j = 0; j < SCR_HEIGHT; j++)
	{
		drawToBackBuffer(i, j, "|");
	}
}

void drawAll()//�׸��� �Լ� ����
{
	//draw boundary
	drawBoundary();

	//draw player
	drawToBackBuffer(player.pos_x_, player.pos_y_, player.image_);

	//draw enemy[i]
	for (int i = 0; i < Enemy_Num; i++)
	{
		char* current_state_image = enemy[i].image_ + (enemy[i].width_ + 1) * enemy[i].state_;

		drawToBackBuffer(enemy[i].pos_x_, enemy[i].pos_y_, current_state_image);
	}


	char* current_state_image = player.image_ + (player.width_ + 1) * player.state_;

	drawToBackBuffer(player.pos_x_, player.pos_y_, current_state_image);

	//���� ���
	char scoretext[15];
	sprintf(scoretext, "Score: %d", score);
	drawToBackBuffer(1, 0, scoretext);

	//��� ǥ��
	char lifepoint[15];
	sprintf(lifepoint, "Life: %d", life);
	drawToBackBuffer(15, 0, lifepoint);
}


void GameSpeed() //���� �������� �ӵ�
{
	if (score < 2000)
	{
		Sleep(Speed);
	}
	else if (score >= 2000 && score < 4000)
	{
		Sleep(Speed - 10); //70
	}
	else if (score >= 4000 && score < 6000)
	{
		Sleep(Speed - 20); //60
	}
	else if (score >= 6000 && score < 8000)
	{
		Sleep(Speed - 30); //50
	}
	else if (score >= 8000 && score < 10000)
	{
		Sleep(Speed - 40); //40
	}
	else if (score > 10000 && score < 12000)
	{
		Sleep(Speed - 50); //30
	}
	else if (score >= 12000)
	{
		Sleep(Speed - 60); //20
	}
}

int main()
{
	boolean Loop = true;
	//�������
	PlaySound(TEXT("main.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	//�ܼ�â ũ��
	system("mode con cols=69 lines=30");
	//���� �ʱ�ȭ
	for (int j = 0; j < SCR_HEIGHT; j++)
	{
		for (int i = 0; i < SCR_WIDTH; i++)
		{
			front_buffer[j][i] = '\0';
			back_buffer[j][i] = '\0';
		}
	}

	score = 0; //�ʱ� ����
	life = 5; //���

	player.state_ = 0; //�ʱ� ����
	player.image_ = "P\0X\0X\0X\0X\0X\0X\0X\0X\0X\0X\0X\0X\0X\0X"; //�÷��̾� ����
	player.width_ = 1;
	player.height_ = 1;
	player.pos_x_ = 32;
	player.pos_y_ = 1;

	//�� ����

	for (int i = 0; i < Enemy_Num; i++)
	{
		const int dice = rand() % (SCR_WIDTH - 2); //x��ǥ ����
		const int dice2 = rand() % Rand_Num; //y��ǥ ����

		enemy[i].image_ = "*"; //���� ��ü
		enemy[i].width_ = 1;
		enemy[i].height_ = 1;
		enemy[i].pos_x_ = dice + 1;
		enemy[i].pos_y_ = dice2 + SCR_HEIGHT - (Rand_Num + 1);
	}

	//hide cursor
	CONSOLE_CURSOR_INFO cur_info;
	cur_info.dwSize = 1;
	cur_info.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur_info);

	//���� ������ ȭ��
	moveCursorTo(12, 20);
	printf("���Ӹ�ǥ: �ϴÿ��� �������� ���� ���� ��Ƴ���!");
	moveCursorTo(12, 17);
	printf("���۹��: ����Ű  <-  -> ");
	moveCursorTo(12, 14);
	printf("���ǻ���: ���� ���� ������ ������ �������ϴ�!");
	moveCursorTo(20, 11);
	printf("�ƹ� Ű�� ������ �����մϴ�!");

	_getch();
	moveCursorTo(0, 0);
	system("cls");

	while (Loop) //main game loop
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); //������

		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			if (player.pos_x_ > 1)
				player.pos_x_--;
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			if (player.pos_x_ < SCR_WIDTH - player.width_ - 1)
				player.pos_x_++;
		}

		GameSpeed();

		if (player.state_ > 0)
		{
			player.state_--;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY); //������
		}

		for (int i = 0; i < Enemy_Num; i++)
		{
			//move enemy[i]

			if (enemy[i].pos_y_ < SCR_HEIGHT - player.height_)
				enemy[i].pos_y_--;
			if (enemy[i].pos_y_ < player.height_)
				enemy[i].pos_y_ = player.height_;

			if (enemy[i].pos_y_ == 1) //���� �ٴڿ� ������
			{
				enemy[i].image_ = "";
				score = score + 5;
			}
			if (enemy[i].pos_y_ == player.pos_y_ && player.pos_x_ == enemy[i].pos_x_) //���� �÷��̾� �浹��
			{
				enemy[i].image_ = "";
				if (life > 0)
					life--;
				player.state_ = 15;
				Beep(1000, 234);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY); //������
			}

			if (strcmp(enemy[i].image_, "") == 0) //�������� �ٽ� ����
			{
				const int dice = rand() % (SCR_WIDTH - 2);
				const int dice2 = rand() % Rand_Num;

				enemy[i].image_ = "*"; //���� ��ü
				enemy[i].width_ = 1;
				enemy[i].height_ = 1;
				enemy[i].pos_x_ = dice + 1;
				if (score < 1000)
				{
					enemy[i].pos_y_ = dice2 + SCR_HEIGHT - (Rand_Num + 1);
				}
				else if (score >= 1000 && score < 2000)
				{
					enemy[i].pos_y_ = dice2 + SCR_HEIGHT - (Rand_Num + 2);
				}
				else if (score >= 2000 && score < 4000)
				{
					enemy[i].pos_y_ = dice2 + SCR_HEIGHT - (Rand_Num + 3);
				}
				else if (score >= 4000 && score < 6000)
				{
					enemy[i].pos_y_ = dice2 + SCR_HEIGHT - (Rand_Num + 4);
				}
				else if (score >= 6000 && score < 8000)
				{
					enemy[i].pos_y_ = dice2 + SCR_HEIGHT - (Rand_Num + 5);
				}
				else if (score >= 8000 && score < 10000)
				{
					enemy[i].pos_y_ = dice2 + SCR_HEIGHT - (Rand_Num + 6);
				}
				else if (score >= 10000)
				{
					enemy[i].pos_y_ = dice2 + SCR_HEIGHT - (Rand_Num + 7);
				}
			}

			drawAll();

			render();
		}

		if (life == 0)
		{
			PlaySound(NULL, 0, 0);
			Loop = false;
		}
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); //������
	//����ȭ��
	system("cls");
	moveCursorTo(28, 17);
	printf("Game Over!");
	moveCursorTo(24, 14);
	printf("Your Score : %4d", score);
	_getch();
	moveCursorTo(0, 0);

	return 0;
}